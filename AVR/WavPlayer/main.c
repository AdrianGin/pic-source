 
#include "hardwareSpecific.h"
#include "SD_MMC/sd.h"
#include "PetitFS/pff.h"
#include "SPI/spi.h"
#include "mmculib/uint16toa.h"
#include "waveplayer328p.h"



volatile waveHeader_t wavefile;
volatile uint8_t newSongFlag = 1;

volatile uint8_t ChangeBufferFlagServiceCount = 0;
volatile uint8_t ChangeBufferFlag = 0;

volatile uint8_t GetNextBufferFlagCount = 0;
volatile uint8_t GetNextBufferFlag = 1;

uint8_t outputString[20] = "A228m.wav";


AVR_USART_t PrimaryUART = {
   &UCSR0A,
   &UCSR0B,
   &UCSR0C,
   &UBRR0H,
   &UBRR0L,
   &UDR0,

   &ReceiveBuffer,
   &TransmitBuffer
};


void SD_Startup(void)
{
   SD_CS_PORT |= (1 << SD_CS_PIN);
   _delay_ms(100);
}

void SD_SetMaxSpeed(void)
{
   SPSR |= (1<<SPI2X);
}

int main(void) 
{  
   uint8_t ret;
   clock_prescale_set(clock_div_1);

   uartInit(&PrimaryUART, 1);
   uartSetBaud(&PrimaryUART, 0, BAUD38400);
   sei(); 

   SPI_Init();

   DDRC |= (1 << 4);

   DDRD |= (1 << PD7);

   DDRB |= (1 << 1);
   DDRB |= (1 << 2);


   SD_CS_DDR |= (1 << SD_CS_PIN);

   uartTxString_P(&PrimaryUART,  PSTR("Welcome"));

   ret = pf_mount(&filesys);

   if( ret )
   {
      uartTx(&PrimaryUART,  ret + '0');
      uartTxString_P(&PrimaryUART,  PSTR("PFS failed"));
      while(1);
   }

   uartTxString_P(&PrimaryUART,  PSTR("PFS Success!"));

   for( ;; )
   {
      
      //uartTx(OCR2);
      /* Is a mutliple of WAVE_OUTBLOCK_SIZE */
      /* If we are ready to receive the next bytes then do it */
      if( newSongFlag )
      {
         uartTxString_P(&PrimaryUART,  PSTR("Playing\n"));
         wavePlayFile( (waveHeader_t*)&wavefile, outputString);

         uartNewLine(&PrimaryUART);

         uint16toa(wavefile.channelCount, outputString, 0);
         uartTxString_P(&PrimaryUART,  PSTR("Channel Count: ") );
         uartTxString(&PrimaryUART, outputString);
         uartNewLine(&PrimaryUART);

         uint16toa(wavefile.resolution, outputString, 0);
         uartTxString_P(&PrimaryUART,  PSTR("Res: ") );
         uartTxString(&PrimaryUART, outputString);
         uartNewLine(&PrimaryUART);

         uint16toa(wavefile.sampleRate, outputString, 0);
         uartTxString_P(&PrimaryUART,  PSTR("SampleRate: ") );
         uartTxString(&PrimaryUART, outputString);
         uartNewLine(&PrimaryUART);

         uartTxString_P(&PrimaryUART,  PSTR("DataSize: ") );
         uint16toa(((uint32_t)(wavefile.dataSize)>>16), outputString, 0);
         uartTxString(&PrimaryUART, outputString);
         uartNewLine(&PrimaryUART);
         uint16toa(wavefile.dataSize, outputString, 0);
         uartTxString(&PrimaryUART, outputString);
         uartNewLine(&PrimaryUART);

         newSongFlag = 0;
      }




      if(waveIsPlaying())
      {
         if( CurrentBuffer != NextBuffer )
         {
            if(waveContinuePlaying((waveHeader_t*)&wavefile) == 0)
            {
               waveAudioOff();
               uartTxString_P(&PrimaryUART,  PSTR("Wave Finished!"));
            }
            //GetNextBufferFlagCount++;
         }
      }

      /*
      if( ChangeBufferFlag != ChangeBufferFlagServiceCount)
      {
         //CurrentBuffer ^= 1;
         ChangeBufferFlagServiceCount++;
      }*/

   }
   
   if( pf_mount(0) == FR_OK )
   {
      uartTxString_P(&PrimaryUART,  PSTR("Unmounted!") );
   }

   return 0;
}




ISR(TIMER1_OVF_vect)
{

   static uint8_t oversampling = 0;

   //PORTC ^= (1 << 4);
   /* We need to put this here to increase the speed */
   /* Left is first */
   OCR1A = AudioBuffer[CurrentBuffer][(audioReadptr)];
   /* Right is second */
   /* This will not do anything if WAVE_STEREO_ENABLED is not set to 1 */
   OCR1B = AudioBuffer[CurrentBuffer][(audioReadptr + isStereo)];

   oversampling++;

   if( oversampling == 2 )
   {
      audioReadptr = (audioReadptr + 1 + isStereo) & WAVE_OUTMASK;
      /*if( audioReadptr == (2) )
      {
         GetNextBufferFlag++;
      }*/

      if( audioReadptr == 0 )
      {
         CurrentBuffer ^= 1;
         //ChangeBufferFlag++;
      }

      oversampling = 0;

   }

   //OCR1A = 121;
   //OCR1B = 121;
   //PORTB ^= (1<<1);


   /*if( fastMode )
   {
      OCR1A = Buff[(audioReadptr)];
      OCR1B = Buff[(audioReadptr + 1)];
      audioReadptr = (audioReadptr + 2);
   }
   else
   {
      waveProcessBuffer((waveHeader_t*)&wavefile);
   }*/
}



ISR(USART_TX_vect)
{
   // Tx the next byte if there are still bytes in the buffer
   if( !ringbuffer_isEmpty((RINGBUFFER_T*)PrimaryUART.TransmitBuffer) )
   {
      *PrimaryUART.UDRx = ringbuffer_get((RINGBUFFER_T*)PrimaryUART.TransmitBuffer);
   }
}

ISR(USART_RX_vect)
{
   uint8_t buffer = UDR0;
   static uint8_t readPtr = 0;
   sei();

   uartTx(&PrimaryUART, buffer);


   if( buffer == 13 || buffer == 10 )
   {
      waveAudioOff();
      strcpy_P( (char*)&outputString[readPtr] , PSTR(".wav") );
      readPtr = 0;
      uartNewLine(&PrimaryUART);
      //uartTxString_P( PSTR("Now playing: ") );
      uartTxString(&PrimaryUART, outputString);
      newSongFlag = 1;
      return;
   }

   outputString[readPtr++] = buffer;

}
