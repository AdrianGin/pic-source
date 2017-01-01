#include "PetitFS/pff.h"
#include "hardwareSpecific.h"
#include "waveplayer328p.h"
#include <string.h>

uint8_t Buff[WAVE_OUTBUFFER_SIZE];		/* Audio output FIFO */
FATFS filesys;

volatile uint8_t audioReadptr;
volatile uint8_t audioWriteptr;
volatile uint8_t audioState;
uint8_t fastMode;

uint8_t len;

uint8_t is16Bit;
uint8_t isStereo;

enum
{
   MAX_RES = 181,
   CENTRE_LEVEL = MAX_RES / 2,
};

uint8_t waveIsPlaying(void)
{
   return audioState;
}

/* Using Timer1 */
void waveAudioOn(void)
{
   TIMSK1 |= (1 << TOIE1);
   audioState = WAVE_OUTPUT_ON;
}

void waveAudioOff(void)
{
   /* Simply turn off the timer */
   TIMSK1 &= ~(1 << TOIE1);
   OCR1A = CENTRE_LEVEL;
   OCR1B = CENTRE_LEVEL;
   audioState = WAVE_OUTPUT_OFF;
}

/* AudioSetup must be called at the start of a new wave file */
void waveAudioSetup(void)
{

   TCCR0A = 0x00;
   TCCR0B = 0x00;

   DDRD |= (1 << 6);
   DDRD |= (1 << 5);

   DDRD |= (1 << 3);


   ICR1 = MAX_RES;
#if 0
   TCCR1A |= ((1 << WGM11)  | (1 << COM1A0) | (1 << COM1B0));
   TCCR1A &= ~((1 << WGM10) | (1 << COM1A1) | (1 << COM1B1));
   TCCR1B |= (1 << CS10) | (1 << WGM12) | (1<<WGM13);
   TCCR1B &= ~((1 << CS11) | (1 << CS12));
#endif

#if 1
   TCCR0A |= ((1 << WGM01) | (1 << WGM00) | (1 << COM0A1) | (1 << COM0B1));
   TCCR0A &= ~((1 << COM0A0) | (1 << COM0B0) );
   TCCR0B |= ((1 << CS00) );
   TCCR0B &= ~((1 << CS01) | (1 << CS02) | (1 << WGM02) );
   TCNT0 = 0x00;
#endif

#if 1
   TCCR2A |= ((1 << WGM21) | (1 << WGM20)  | (1 << COM2B1));
   TCCR2A &= ~((1 << COM2B0) );
   TCCR2B |= ((1 << CS20) );
   TCCR2B &= ~((1 << CS21) | (1 << CS22) | (1 << WGM22) );
   TCNT2 = 0x00;
#endif



#if WAVE_STEREO_ENABLED
   DDRD |= (1 << 5);
   //TCCR1A |= (1 << COM1B0) | (1 << COM1B1);
#endif

   DDRD |= (1 << 6);
   /* Set levels back to centre */
   OCR0A = 50;
   OCR0B = 128;

   OCR2B = 50;

   audioReadptr = 0;
   audioWriteptr = 0;
}


/* This assumes the filesystem has been mounted, 
 * waveContinuePlaying needs to be constantly called in the main loop thereafter
 */
uint8_t wavePlayFile(waveHeader_t* wavefile, uint8_t* filename)
{
   uint32_t ret;
   uint8_t retries = WAVE_RETRY_COUNT;

   while( retries-- )
   {
      ret = waveParseHeader(wavefile, filename);
      if( ret > WAVE_MINIMUM_SAMPLES )
      {

         uartTxString_P(&PrimaryUART,  PSTR("Wave Parsed"));

         waveAudioSetup();
         waveAudioOn();
         return WAVE_SUCCESS;
      }
      else
      {
         uartTx(&PrimaryUART, ret);
         uartTxString_P(&PrimaryUART,  PSTR("File too small!"));
      }
   }
   
   return ret;
}


void wavePutByte(uint8_t byte)
{
   static uint8_t highByte = 1;

   /* Throw away every other byte, due to the Little Endianness */
   if( is16Bit && highByte)
   {
      highByte = 0;
      return;
   }

   highByte = 1;

   /* Wait for a bit */
   /* Forward data to Audio Fifo */
   while( (( (uint8_t)(audioWriteptr + 1)) == audioReadptr) && (audioState) )
   {
      PORTD ^= (1<<7);
      //PORTC ^= (1<<4);
   }

   /* Do the signed 16bit -128 -> 127 to unsigned 8bit 0 - >255here */
   Buff[audioWriteptr] = byte + (is16Bit << 7);

   //Scale to max res
   uint16_t temp = (Buff[audioWriteptr] * MAX_RES) / 255;
   Buff[audioWriteptr] = (uint8_t)temp;
   audioWriteptr++;

   //audioWriteptr &= WAVE_OUTMASK;
}


/* If this returns false, then the song has finished */
uint8_t waveContinuePlaying(waveHeader_t* wavefile)
{
   WORD bytesWritten;
   WORD bytesToPlay;

   if( !waveIsPlaying() )
   {
      return 0;
   }

   if( wavefile->dataSize > WAVE_OUTBLOCK_SIZE)
   {
      bytesToPlay = WAVE_OUTBLOCK_SIZE;
   }
   else
   {
      bytesToPlay = wavefile->dataSize;
   }   

   pf_read(0, bytesToPlay, &bytesWritten);
   wavefile->dataSize = wavefile->dataSize - bytesToPlay;

   if( bytesWritten != bytesToPlay )
   {
      //uartTxString_P( PSTR("IO Error") );
      return 0;
   }
   
   if( wavefile->dataSize )
   {
      return 1;
   }
   else
   {
      return 0;
   }


}


void waveProcessBuffer(waveHeader_t* wavefile)
{
   /* Left is first */
   OCR1A = Buff[(audioReadptr)];
   /* Right is second */
   /* This will not do anything if WAVE_STEREO_ENABLED is not set to 1 */
   OCR1B = Buff[(audioReadptr + isStereo)];
   audioReadptr = (audioReadptr + 1 + isStereo) & WAVE_OUTMASK;
   //audioReadptr = (audioReadptr + 1 + isStereo);
}


/* Returns the number of data bytes otherwise returns an error code */
uint32_t waveParseHeader(waveHeader_t* wavefile, uint8_t* filename)
{
    
   WORD bytesRead;
   uint32_t chunkSize;

   memset(wavefile, 0, sizeof(waveHeader_t));
   is16Bit = 0;
   isStereo = 0;



   if(pf_open((const char*)filename) != FR_OK) return WAVE_IO_ERROR+1;
   if(pf_lseek(0) != FR_OK) return WAVE_IO_ERROR+2;
        
   /* Check RIFF-WAVE file header */
	if (pf_read(Buff, 12, &bytesRead)) return WAVE_IO_ERROR+3;

    /* Make sure it is a WAVE file */
	if (bytesRead != 12 || LD_DWORD(Buff+8) != FCC('W','A','V','E')) return WAVE_INVALID_FILE;

   for (;;) 
   {
        /* Get Chunk ID and size */
		if (pf_read(Buff, 8, &bytesRead)) return WAVE_IO_ERROR+4;		
		if (bytesRead != 8) return WAVE_INVALID_FILE;
		chunkSize = LD_DWORD(Buff+4);		/* Chunk size */
    
        /* Switch by chunk type */
		switch (LD_DWORD(Buff)) 
      {	
		   case FCC('f','m','t',' ') :		/* 'fmt ' chunk */
		      if (chunkSize & 1) chunkSize++;
			   if (chunkSize > 128 || chunkSize < 16) return WAVE_INVALID_FILE;		/* Check chunk size */
			   if (pf_read(Buff, chunkSize, &bytesRead)) return WAVE_IO_ERROR+5;	/* Get the chunk content */
			   if (bytesRead != chunkSize) return WAVE_INVALID_FILE;
			   if (Buff[FMT_FORMAT] != 1) return WAVE_INVALID_FILE;				/* Check coding type (1: LPCM) */
			   if (Buff[FMT_NUM_CHANNELS] < 1 && Buff[FMT_NUM_CHANNELS] > 2) return WAVE_INVALID_FILE; 			/* Check channels (1/2: Mono/Stereo) */
			   
            wavefile->channelCount = Buff[FMT_NUM_CHANNELS];						/* Save channel flag */
            /* Setup the offsets for stereo */
            if( wavefile->channelCount == 2 )
            {
               isStereo = 1;
            }

			   if (Buff[FMT_RESOLUTION] != 8 && Buff[FMT_RESOLUTION] != 16) return WAVE_INVALID_FILE;		/* Check resolution (8/16 bit) */
			   wavefile->resolution = Buff[FMT_RESOLUTION];							/* Save resolution flag */
			   /* Setup the resolution byte offsets */
            if( wavefile->resolution == 16 )
            {
               is16Bit = 1;
            }
          
            wavefile->sampleRate = LD_DWORD(&Buff[FMT_SAMPLERATE]);					/* Check sampling freqency (8k-48k) */
			   if (wavefile->sampleRate < WAVE_MINSAMPLE_RATE || wavefile->sampleRate > WAVE_MAXSAMPLE_RATE) return WAVE_INVALID_FILE;
			   
            if( isStereo && is16Bit && wavefile->sampleRate >= 44100 )
            {
               fastMode = 1;
            }
            else
            {
               fastMode = 0;
            }


            /* Set interval timer (sampling period) */
            /* Use OCR2 */
            OCR2A = (F_CPU/32/wavefile->sampleRate) - 1;
			   break;

		   case FCC('d','a','t','a') :		/* 'data' chunk (start to play) */
			   if (chunkSize < WAVE_MINIMUM_SAMPLES) return WAVE_INVALID_FILE;	/* Check size */
			   wavefile->dataSize = chunkSize;
            return chunkSize;

		   case FCC('D','I','S','P') :		/* 'DISP' chunk (skip) */
		   case FCC('f','a','c','t') :		/* 'fact' chunk (skip) */
		   case FCC('L','I','S','T') :		/* 'LIST' chunk (skip) */
			   if (chunkSize & 1) chunkSize++;
			   if (pf_lseek(filesys.fptr + chunkSize)) return WAVE_IO_ERROR+6;
			   break;

		   default:						/* Unknown chunk */
			   return WAVE_INVALID_FILE;
      }
   }
}
