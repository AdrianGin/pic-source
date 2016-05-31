/*
   MSP430F16x Electronic Drum Kit
   
*/
#include "hardwareSpecific.h"
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>



#include "main.h"
#include "Delay/delay.h"
#include "SoftI2C/softi2c.h"

#include "Sample/sample.h"
#include "Sensor/sensor.h"
#include "ADC/adc12.h"
#include "mmculib/uint16toa.h"
#include "UI_LCD/UI_LCD.h"
#include "UI/UI.h"
#include "UI_KP/UI_KP.h"
#include "Softtimer/Softtimer.h"
#include "Menu/Menu.h"
#include "MIDI/midi.h"
#include "Profiles/profiles.h"
#include "MIDI/SysEx/SysEx.h"
#include "VUMeter/vumeter.h"
#include "UserFunctions/userFunctions.h"
#include "ControllerMode/ControllerMode.h"
#include "version.h"
#include "LCDSettings.h"
#include "MenuSettings.h"

#if VERSION_CODE == VERSION_WITH_PE
const char VersionId[] = "1.0W 4/1/08";
#endif

#if VERSION_CODE == VERSION_WITHOUT_PE
/* Initial Release: 
   const char VersionId[] = "1.0 4/1/08";
*/
const char VersionId[] = "1.3a 23/10/10";

// Version 1.3a 23/10/10 - Optimised Profile ROM usage, no need for IMAGES now.

/* 19/10/2010 
   Update: USB MIDI to be incorporated using an external ATMEGA chip. 
   The next version of the LAD drum will be based off an ATMEGA32. 
   (Maybe single chip, USB->MIDI conversion takes up quite a bit of CPU time.
   
   Also going to improve the ROM usage of the profiles as well as make Profile1 
   start up by default so your changes are kept upon start up.

*/

/* Change Log: 1/05/10
 * To Do: Set the analogue input port to low to clear the input
 * signal buffer.
 * X1 Crystal installed and set to 16MHz.
 *
 * We could save each sample so we know what the voltage waveform looks like
 *
 */


/* Change Log 23/03/09
 * Note: Benchmarks performed and speed has been increase 10 fold.
 * Added Controller Mode process and Keypad inputs as extra digital channels.
 * Controller mode allows analogue channels to have potentiometer and
 * variable voltage input. MIDI is only output if the value detected is different.
 *
 * Hardware Change: USB Powered.
 *
 */

/* Change Log: 26/07/08
 * Optimised Menu System for Sub menus. Added SysEx Input & Output Capability 
 * Added ability to change MIDI output command code 
 *    Hence support for an Analogue Foot controller in the place of a
 *    piezo input has been added.
 * Better mutlitasking support, added the use of 'Processes' 
 * LCD Device and Menu structures have been optimised 
 * Smaller code size 
 *
 * Next revision: General clean up
 *                Port to Atmega series (MEGA32)
 *                Improve Multiplexing of signals to improve sample rates 
 */
 
 

#endif

uint16_t BenchMarkCount = 0;

uint8_t ActiveProcess = 0;

/**
Main function with some blinking leds
*/
int main(void) 
{

   WDTCTL = WDTPW + WDTHOLD;
   
   /* Setup the clocks */
   BCSCTL1 &= ~XT2OFF;
   BCSCTL1 |= XTS;
   
   /* 8-16MHz operation */
   BCSCTL3 = (XT2S_2) | (LFXT1S_2);
   _BIS_SR(OSCOFF);

   while( IFG1 & OFIFG )
   {
      IFG1 &= ~OFIFG;
      _delay_us(50);
   }

   /* SMCLK is XT2 and so is the MCLK */
   BCSCTL2 |= (SELM_2 | SELS);


   ProfileInit();
   
   /* Make profile 1 the default profile on start up */
   Profile_Read(0);
   
   
   SensorInit();
   DigitalInputInit();
   /* Setup the communications module */   
   UART_Select(0);
   UART_Init();

   /* Implement the changes */
   MIDI_SetRate(MIDI_GetRate());
   MIDI_SetBaud(MIDI_GetBaud());
   MIDI_SetChannelCode( MIDI_GetChannelCode() );

   /* Update Activated Analogue Channels */
   UpdateActiveChannels();

   /* Update the Retrigger periods */
   UpdateChannelRetriggers();


   
   /* ADC Module Init */
   ADC12_Init();
   ADC12_SetStartAddress(CSTARTADD_0);
   ADC12_SetupAddress(0, INCH_A0);
   ADC12_Enable();   
   
   TimerInit();
   


   /* Enable Interrupt detection on INTP1.3 for a Low to High only for
	 * MAX7300 */
#if VERSION_CODE == VERSION_WITH_PE  
   P1DIR &= ~(UI_INT_PIN);
   P1IES &= ~(UI_INT_PIN);
   P1IE  |=  (UI_INT_PIN);

   I2CInit();   
#endif

   /* Enable Keypad */
   UI_KP_Init();   
   UI_Activate();

   /* Enable LCD */
   UI_LCD_HWInit();
   UI_LCD_Init(&PrimaryDisplay);
   UI_LCD_LoadDefaultChars();
	
   LCD_BL_DDR |= (1 << LCD_BL_PIN);
   LCD_BL_PORT &= ~(1 << LCD_BL_PIN);

   MenuSetDisplay(&primaryMenu, MENU_LCD);
   MenuSetDisplay(&analogueMenu, MENU_LCD);
   MenuSetDisplay(&digitalMenu, MENU_LCD);      
   /* Menu Setup */
   MenuSetInput(&primaryMenu, 0); 
	
	/* Menu must be Initialised first */
	/* Backlight 'off' is at 5% */
   UI_LCD_BLInit(5);
   UI_LCD_BL_On();
	
   aboutScroll(MAIN_SCREEN);
   SoftTimerStart( SoftTimer2[SC_LCD_BL_Period] );	
	

	
   _delay_ms(900);	
   UI_LCD_LoadDefaultChars();					  
   /* Reprint Menu */  
   MenuUpdate(&primaryMenu, RESET_MENU);   
   
   /* Enable interrupts */
   eint();  
   
   while(1)
   {     
      switch( ActiveProcess )
      {
         case PLAY_MODE:
            Play();
            /*Benchmark();
            BenchMarkCount++;*/
         break;
         
         case CONTROLLER_MODE:
				MidiControllerMode();
			break;
         
         case RECEIVE_SYSEX:
            ReceiveSysEx();
         break;
         

         
      }
   }
   
   return 0;

}

void Benchmark(void)
{
   uint8_t i = 0;
   uint8_t SelectedChannel = 0;
   uint16_t sample;
   
	while( ActiveChannels[i] != LAST_CHANNEL_INDEX)
	{
      
      SelectedChannel = ActiveChannels[i++];
      SensorChannel(SelectedChannel);
   	_delay_us(SensorSettings->CrosstalkDelay);

      /* Take a sample */
      sample = ADC12_Sample();
                           
         /* Obtain Peak */
      ObtainPeak(SelectedChannel, sample);
   }   
}

void MidiControllerMode(void)
{
   uint8_t i = 0;
   uint16_t sample;
   uint16_t conditionedSignal;
	   
	for( i = 0; i <= ANALOGUE_INPUTS; i++ )
	{
         SensorChannel(i);
         _delay_us(SensorSettings->CrosstalkDelay);

         /* Take a sample */
         sample = ADC12_Sample();
			
			/* If the difference is not due to noise */
			if( (sample > GetChannelThresh(i)) &&
				 ((sample > (GetLastSampleValue(i) + GetChannelThresh(i))) ||
				  (sample < (GetLastSampleValue(i) - GetChannelThresh(i)))))
			{
				         
				/* Determine the resulting MIDI code */
				conditionedSignal = GainFunction(i, sample - GetChannelThresh(i));
			
				if( conditionedSignal > 127 )
				{
					conditionedSignal = 127;       
				}
				
				

				if( conditionedSignal != GetLastMIDIValue(i))
				{				
					/* Ensure the MIDI code is sent */			
					ObtainPeak(i, sample);   
					SetLastSampleValue(i, sample); 
				}
			}
   }   
}


void Play(void)
{
   uint8_t i = 0;
   uint8_t SelectedChannel = 0;
   uint16_t sample;
   
	while( ActiveChannels[i] != LAST_CHANNEL_INDEX)
	{
      
      SelectedChannel = ActiveChannels[i++];
      
      if( !(RetriggerPeriod[ SelectedChannel ].timerEnable) )
      {
         SensorChannel(SelectedChannel);
         _delay_us(SensorSettings->CrosstalkDelay);
         /* Take a sample */
         sample = ADC12_Sample();
                           
         /* Obtain Peak */
         ObtainPeak(SelectedChannel, sample);
      } 
   }   
}

#ifdef __MSP430_HAS_UART0__
interrupt (USART0RX_VECTOR) usart0_rx(void)
{
   uint8_t buffer = U0RXBUF;

#else
interrupt (USCIAB0RX_VECTOR) usart0_rx(void)
{
   uint8_t buffer = UCA0RXBUF;
#endif

   static uint8_t channel = 0;
   
   /* Start the Aux Timer for any key press or UART input */
   TBCCTL2 |= (CCIE);
   IFG2 &= ~(UCA0RXIFG);
   
   UART_Tx(buffer);

   switch( ActiveProcess )
   {
      case PLAY_MODE:
         if( buffer == '+' )
         {
            SoftTimer2[SC_AutoMenuUpdate].timeCompare++;
            UART_Tx(SoftTimer2[SC_AutoMenuUpdate].timeCompare);
         }
         
         if( buffer == '-' )
         {
            SoftTimer2[SC_AutoMenuUpdate].timeCompare--;
            UART_Tx(SoftTimer2[SC_AutoMenuUpdate].timeCompare);
         }
        
        
         if( buffer == 'D' )
         {
            UART_TxDump((uint8_t*)PROFILE_FLASH_ADDRESS(0), 512 );
            UART_TxDump((uint8_t*)PROFILE_FLASH_ADDRESS(1), 512 );
            UART_TxDump((uint8_t*)PROFILE_FLASH_ADDRESS(2), 512 );		      
         }
      
      	if( buffer == 'E' )
         {
            /*UART_TxDump((uint8_t*)PROFILE_IMAGE_ADDRESS(0), 512 );
            UART_TxDump((uint8_t*)PROFILE_IMAGE_ADDRESS(1), 512 ); 
            UART_TxDump((uint8_t*)PROFILE_IMAGE_ADDRESS(2), 512 );*/		     
         }
      
         if( buffer == 'b' )
         {
      		SoftTimerStart(SoftTimer2[SC_VUMeterUpdate]);
            //MAX7300_SetRegister(UI_MAX7300_ADDRESS, MAX7300_P27, 0x01);
         }
         
         if( buffer == 'c' )
         {
      		UI_LCD_Pos(&PrimaryDisplay, 3, 10);
      		UI_LCD_Char(&PrimaryDisplay, 0);
      		UI_LCD_Char(&PrimaryDisplay, 1);
      		UI_LCD_Char(&PrimaryDisplay, 2);
      		UI_LCD_Char(&PrimaryDisplay, 3);		
      		UI_LCD_Char(&PrimaryDisplay, 4);
      		UI_LCD_Char(&PrimaryDisplay, 5);
      		UI_LCD_Char(&PrimaryDisplay, 6);
      		UI_LCD_Char(&PrimaryDisplay, 7);		
      	}
      break;
      
      case RECEIVE_SYSEX:
         ringbuffer_put( (RINGBUFFER_T*)&ReceiveBuffer, buffer);
      break;
   }
   
}

/* Handle a key press from a MAX7300 interrupt / or directly  */
interrupt (PORT1_VECTOR)   port1_int(void)
{  
   dint(); 
#if VERSION_CODE == VERSION_WITH_PE     
   if( (UI_INT_PORT & UI_INT_PIN) )
   {
		/* Reset Interrupt */
      UI_INT_IFG &= ~(UI_INT_PIN);  
#endif
      
#if VERSION_CODE == VERSION_WITHOUT_PE
   if( (UI_COL_IN & UI_COLS) )
   {
#endif      
      uint8_t IntResult;
      static uint8_t LCD_BL_Mode = LCD_BL_TIMED;
      /* Reset Interrupt on UI */
      IntResult = UI_KP_GetPress();      

      if( IntResult != KP_INVALID)
      {
         switch( ActiveProcess )
         {
            case PLAY_MODE:
               
            break;
            
            case CONTROLLER_MODE:
					/* KEY ABC to return to the main menu 
					 * and exit Controller Mode */
					if( IntResult == 0x78 )
					{
						IntResult = KP_BACK;
						ActiveProcess = PLAY_MODE;
						break;
					}
					/* KEY* to return to the main menu 
					 * but stay in Controller Mode */
					if( IntResult == KP_STAR && (CM_GetMenuMode() == CM_SETTINGS_MODE) )
					{
						CM_SetMenuMode(CM_MENU_MODE);
						IntResult = KP_BACK;
						break;
					}
					
					if( CM_GetMenuMode() == CM_SETTINGS_MODE )
					{
						CM_ReceiveInput(UI_KP_ReturnID(IntResult));
	
						/* Reset Button States */
						UI_INT_IFG &= ~(UI_COLS);
		            UI_Activate();
						eint();
						return;
					}
				break;
				
            /* Any key will cancel SysEx reception */
            case RECEIVE_SYSEX:
               ActiveProcess = PLAY_MODE;
            break;
         }
         
			UI_LCD_BL_On();
			/* Start the Aux Timer for any key press or UART input */
			TBCCTL2 |= (CCIE);
         MenuSetInput(ActiveMenu, IntResult);   
         MenuUpdate(ActiveMenu, RESET_MENU);  
			
	      /* Toggles the permanent state of the LCD BL */
	      if( IntResult == KP_5 )
	      {
	         LCD_BL_Mode ^= 1;
	         
	         /* Turn it off, but enable the timer */
	         if( LCD_BL_Mode == LCD_BL_TIMED )
	         {
	            UI_LCD_BL_Off();    
	         }
	         else
	         {
               /* Permanently ON */
					SoftTimerReset( SoftTimer2[SC_LCD_BL_Period] );
					SoftTimerStop( SoftTimer2[SC_LCD_BL_Period] );
					
					/* If no SoftTimer2's are enabled, then turn off the Timer2 module */
					if( !SoftTimer_IsTimer2Active() )
					{
                  /* Stop the Auxuliary Timer */
	               TBCCTL2 &= ~(CCIE);
               }
					
				}
	      }   
			
			if( LCD_BL_Mode == LCD_BL_TIMED )
			{
	         SoftTimerReset( SoftTimer2[SC_LCD_BL_Period] );
			   SoftTimerStart( SoftTimer2[SC_LCD_BL_Period] );
	      }						     
      }
      
    
      
      
      
          
#if VERSION_CODE == VERSION_WITH_PE    
   	UI_SetRegister(UI_INTERRUPT, 0);
#endif   

#if VERSION_CODE == VERSION_WITHOUT_PE 
		/* Reset Interrupt */
      UI_INT_IFG &= ~(UI_COLS);
#endif      
	
      UI_Activate();
   }
	eint();
}

