/*

Copyright (c) 2011 Adrian Gin (adrian.gin[at]gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include "EDrum.h"
#include "hardwareSpecific.h" 
#include "MIDI/SysEx/SysEx.h"
#include "UI_LCD/UI_LCD.h"
#include "UI_KP/UI_KP.h"
#include "LCDSettings.h"
#include "MenuSettings.h"
#include "Profiles/profiles.h"
#include "UserFunctions/userfunctions.h"

#include "SPI/spi.h"
#include "VUmeter/vumeter.h"
#include "Sample/sample.h"
#include "Sensor/sensor.h"
#include "TimerCallbacks/TimerCallbacks.h"

#include "hardUart/hardUart.h"
#include "avrADC/adc.h"

#include "mmculib/uint8toa.h"
#include "edrumAVRsharedfunctions.h"
#include "USBMIDI/USBMIDI.h"

#include <stdlib.h>
#include <util/crc16.h>


const prog_char VersionId[] = "1.6 22/03/11";

uint16_t BenchMarkCount = 0;

volatile uint8_t ActiveProcess = DEFAULT_PROCESS;

volatile uint8_t PrintChannel;

/* 5/03/2011
   After benchmarking, we have decided to use DIV64 ADC clock
   Crosstalk delay of 13us
   This produces a noise floor of ADC:5-9
   We'll set the threshold to 1% (ADC:10) of 1024
   We get good results, however over 13 channels activated, we only get about 7-8 samples per 10ms.

   Lets test it, and it works, we'll keep it.

*/

/* 20/02/2011
   Welcome to 2011 Edrum AVR Mega

   Flash Memory usage is nearing 100%, consider moving the profiles to EEPROM.
   Completed initial version, which somewhat works.

   Make Profile1 load up by default.
   Try to reduce the amount of Flash used.

   Incorporate a bootloader via SYS-EX.

*/



/* 19/10/2010 
   Update: USB MIDI to be incorporated using an external ATMEGA chip. 
   The next version of the LAD drum will be based off an ATMEGA32. 
   (Maybe single chip, USB->MIDI conversion takes up quite a bit of CPU time.
   
   Also going to improve the ROM usage of the profiles as well as make Profile1 
   start up by default so your changes are kept upon start up.

*/



int main(void)
{

   DDRD &= ~(1 << 3);
   PORTD &= ~(1<<3);

   MCUCSR = (1 << JTD);
   MCUCSR = (1 << JTD);

   /* Setup the USB */
   sei();

   /* Setup the communications module */   
   UART_Init(0);
   UART_SetBaud(0, 39);

   usbInit();
   SoftTimer_TimerInit();
   SoftTimerStart( SoftTimer1[SC_usbPoll] );
   

   
   
   SPI_Init();
   /* Enable LCD */
   _delay_ms(10);
   UI_LCD_HWInit();
   _delay_ms(20);
   /* Enable Keypad */
   UI_KP_Init();   
	
	/* Menu must be Initialised first */
	/* Backlight 'off' is at 5% */
   //UI_LCD_BLInit(5);
   //UI_LCD_BL_On();

   UI_LCD_Init(&PrimaryDisplay);
   LCD_BL_DDR |= (1 << LCD_BL_PIN);
   LCD_BL_PORT |= (1 << LCD_BL_PIN);

   /* Menu Display Init */
   MenuSetDisplay(&primaryMenu, MENU_LCD);
   MenuSetDisplay(&analogueMenu, MENU_LCD);
   MenuSetDisplay(&digitalMenu, MENU_LCD);      
   MenuSetDisplay(&dualTrigMenu, MENU_LCD);

   if( VerifyDownload() == 0)
   {  
      Shutdown();
      cli();
      bootloader_enter();
   }
   
   ProfileInit();   
   /* Make profile 1 the default profile on start up */
   Profile_Read(1);

   SensorInit();
   SensorInputSelect(GetSensorInput());
   DigitalInputInit();

   /* Implement the changes */
   MIDI_SetBaud(MIDI_GetBaud());
   MIDI_SetChannelCode( MIDI_GetChannelCode() );

   /* Update Activated Analogue Channels */
   UpdateActiveChannels();
   ResetValues();

   /* Update the Retrigger periods */
   UpdateChannelRetriggers();

   /* ADC Module Init */
   ADC_Init();
   
   /* Menu Setup */
   MenuSetInput(&primaryMenu, 0); 

   aboutScroll(MAIN_SCREEN);

   _delay_ms(1000);

   UI_LCD_LoadDefaultChars();					  
   /* Reprint Menu */
   MenuUpdate(&primaryMenu, RESET_MENU);   

   /*Activate Interrupt */
   MCUCR |= ((1 << ISC11) | (1 << ISC10));
   GICR |= (1 << INT1);

   /* Flush the buffer */
   Callback_Keypress();

   SoftTimerStop(SoftTimer1[SC_usbPoll]);
   SoftTimerStart(SoftTimer1[SC_MIDIScan]);

   sei();

   uint8_t inByte;
   uint16_t sysExCount = 0;
   while (1)
   {   
      usbPoll();
      USBMIDI_OutputData();

      switch( ActiveProcess )
      {

         /* Note that there is no break here, since
          * We do not wish to disable the PlayMode during
          * USB_MIDI_THRU mode */
         case USB_MIDI_THRU:
            
            while( USBMIDI_GetByte(&inByte, EXTERNAL_MIDI_CABLE) != NO_DATA_BYTE )
            {
               UART_Tx(inByte);
            }

         case PLAY_MODE:
#if SET_BENCHMARK
            Benchmark();
            BenchMarkCount++;
#else
            if( SoftTimerIsEnabled(SoftTimer1[SC_MIDIScan]) )
            {  
               Play();
            }
            //UART_Tx(SignalPeak[0]>>8);
#endif
         break;

         case SENDING_SYSEX:
            if(sysExCount == sizeof(Profile_t)+3 )
            {
               sysExCount = SEND_SYSEX_STOP;
            }
            SysExSendNextByte(&CurrentProfile, sysExCount++);
            /* Send the header and sysex end bytes */
            if( sysExCount == SEND_SYSEX_STOP+1)
            {
               sysExCount = 0;
               SoftTimerStart(SoftTimer1[SC_MIDIScan]);
               UF_MenuPrint_P( PSTR("Profile sucessfully"));
               UF_MenuNewLine();
               UF_MenuPrint_P( PSTR("uploaded!") );
               delayWithUSBPoll(8, 0);
               UF_MenuUpOneLevel(&primaryMenu);
               MenuUpdate(ActiveMenu, NO_EXECUTE);
               ActiveProcess = DEFAULT_PROCESS;
            }

         break;


         case RECEIVE_SYSEX:
            
            if(USBMIDI_GetByte(&inByte, EDRUM_MIDI_CABLE) != NO_DATA_BYTE)
            {
               ringbuffer_put( (RINGBUFFER_T*)&ReceiveBuffer, inByte);
            }

            if( ringbuffer_len((RINGBUFFER_T*)&ReceiveBuffer)  )
            {
               ParseSysExData(ringbuffer_get((RINGBUFFER_T*)&ReceiveBuffer));
            }
         break;

         case FIRMWARE_UPGRADE:
            Shutdown();
            bootloader_enter();
         break;


      }      
   }

   return 0;

}

void StartUp(void)
{
   ENABLE_KEYPAD();
   ENABLE_PRIMARY_TIMER();
   ENABLE_AUXILIARY_TIMER();
}

void Shutdown(void)
{
   DISABLE_KEYPAD();
   DISABLE_PRIMARY_TIMER();
   DISABLE_AUXILIARY_TIMER();
   /* Reset the timer flags */
   TIFR = 0xFF;
}

void Play(void)
{
   uint8_t i = 0;
   uint8_t SelectedChannel;
   uint16_t sample;

	while( ActiveChannels[i] != LAST_CHANNEL_INDEX)
	{
      SelectedChannel = ActiveChannels[i++];
      if( !SoftTimerIsEnabled(RetriggerPeriod[SelectedChannel]) )
      {
         SensorChannel(SelectedChannel);
         _delay_loop_1(SensorSettings->CrosstalkDelay << 1);
         _delay_loop_1(SensorSettings->CrosstalkDelay << 1);
         _delay_loop_1(SensorSettings->CrosstalkDelay << 1);
         /* Take a sample, doing it twice improves the reading accuracy */
         sample = ADC_Sample();
                        
         /* Obtain Peak */
         if( ObtainPeak(SelectedChannel, sample) == SAMPLE_IS_FALLING)
         {
            MIDI_OutputAnalogueChannel(SelectedChannel);
         }
      } 
   }
}

#if SET_BENCHMARK
void Benchmark(void)
{
   uint8_t i;
   uint8_t SelectedChannel;
   uint16_t sample;
   
	while( ActiveChannels[i] != LAST_CHANNEL_INDEX)
	{
      SelectedChannel = ActiveChannels[i++];
      if( !(RetriggerPeriod[ SelectedChannel ].timerEnable) )
      {
         SensorChannel(SelectedChannel);
         _delay_loop_1(SensorSettings->CrosstalkDelay << 1);
         _delay_loop_1(SensorSettings->CrosstalkDelay << 1);
         _delay_loop_1(SensorSettings->CrosstalkDelay << 1);
         /* Take a sample */
         sample = ADC_Sample();

#if 0
         if( SelectedChannel == PrintChannel )
         {
            UART_Tx( (uint8_t)(sample>>8) );
            UART_Tx( (uint8_t)(sample) );
         }
#endif         
         //SoftTimerStart(RetriggerPeriod[SelectedChannel]);               
         /* Obtain Peak */
         ObtainPeak(SelectedChannel, sample);
      } 
   }

   //SoftTimerStop(SoftTimer1[SC_MIDIScan]);
     
}
#endif


/* Performs a CRC16 check */
uint8_t VerifyDownload(void)
{
   uint16_t crc = 0, appCRC;
   uint32_t i;

   for (i = 0; i < (APP_END - 1); i++)
   {
      crc = _crc_xmodem_update(crc, FLASH_GET_PGM_BYTE(i) );
   }

   appCRC = FLASH_GET_PGM_WORD(i);
   

   if( crc != appCRC )
   {
      //UI_LCD_String_P(&PrimaryDisplay, PSTR("  DOWNLOAD FAILED!") );
      //UI_LCD_String_P(&PrimaryDisplay, PSTR("IF DOWNLOAD FAILS:") );
   	//UI_LCD_String_P(&PrimaryDisplay, PSTR("  **HOLD ANY KEY**") );
   	//UI_LCD_String_P(&PrimaryDisplay, PSTR("CYCLE POWER & RETRY") );        
      //sei();
      FirmwareInstructions(DOWNLOAD_FAILED);
      return 0;
   }

   return 1; // must be 0
}


ISR(USART_RXC_vect)
{
   uint8_t buffer;
   buffer = UDR;
   sei();

/* Echo the data back out */
   //USBMIDI_PutByte(buffer);
   //uartTx(buffer);

   switch( ActiveProcess )
   {
      case PLAY_MODE:
         //UART_Tx(buffer);
         //PrintChannel = buffer -'0';
      break;
      
      case RECEIVE_SYSEX:
         ringbuffer_put( (RINGBUFFER_T*)&ReceiveBuffer, buffer);
      break;

      case FIRMWARE_UPGRADE:
//         ParseFirmwareData(buffer);
      break;

      case USB_MIDI_THRU:
         USBMIDI_PutByte(buffer, EXTERNAL_MIDI_CABLE);
      break;
   }
}



/* Once a tx has completed, this is called */
ISR(USART_TXC_vect, ISR_NOBLOCK)
{
   // Tx the next byte if there are still bytes in the buffer
   if( !ringbuffer_isEmpty((RINGBUFFER_T*)PrimaryUART.TransmitBuffer) )
   {
      UDR = ringbuffer_get((RINGBUFFER_T*)PrimaryUART.TransmitBuffer);
   }
}


// This must be declared "naked" because we want to let the
// bootloader function handle all of the register push/pops
// and do the RETI to end the handler.
void INT0_vect(void) __attribute__((naked));
ISR(INT0_vect, ISR_NOBLOCK)
{
    asm("jmp 0x7004");
}



ISR(INT1_vect, ISR_NOBLOCK)
{
   /* Flag a MENU Update request */
   UI_KP_SetState(KP_NO_REPEAT);
   SoftTimer2[SC_Keypress].timeCompare = KP_WAIT;
   SoftTimerStart(SoftTimer2[SC_Keypress]);
   ENABLE_AUXILIARY_TIMER();
   DISABLE_KEYPAD();
}


ISR(TIMER1_COMPA_vect, ISR_NOBLOCK)
{
   OCR1A += (SAMPLE_10MS_CRITICAL);
   RunCriticalTimer();
}

ISR(BADISR_vect, ISR_NOBLOCK)
{
    // user code here
}


ISR(TIMER0_COMP_vect, ISR_NOBLOCK)
{  
   OCR0 += (SAMPLE_10MS);
   RunAuxTimers();
}


