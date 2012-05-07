#ifndef _WAVE_PLAYER_FS_H
#define _WAVE_PLAYER_FS_H

#include "FatFS/ff.h"

/* Four Bytes to a Long */
#define FCC(b1,b2,b3,b4)	(((uint32_t)b4<<24)+((uint32_t)b3<<16)+((uint16_t)b2<<8)+(uint8_t)b1)	/* FourCC */
//#define GET32(x)            (uint32_t)(*(uint32_t*)x)

#define WAVE_SUCCESS        (0)
#define WAVE_IO_ERROR       (2)
#define WAVE_INVALID_FILE   (4)
#define WAVE_MINIMUM_SAMPLES   (32768)

#define WAVE_STEREO_ENABLED (1)
#define WAVE_OUTPUT_ON     (1)
#define WAVE_OUTPUT_OFF    (0)
#define WAVE_RETRY_COUNT   (10)

/* 2:I/O error, 4:Invalid file, >=1024:Ok(number of samples) */
#define FMT_FORMAT    (0)
#define FMT_NUM_CHANNELS (2)
#define FMT_SAMPLERATE  (4)
#define FMT_RESOLUTION  (14)

#define WAVE_MINSAMPLE_RATE (8000)
#define WAVE_MAXSAMPLE_RATE (48000)

#define WAVE_OUTBLOCK_SIZE   (WAVE_MINIMUM_SAMPLES)

#define WAVE_OUTBUFFER_SIZE   (1024)
#define WAVE_OUTMASK         (WAVE_OUTBUFFER_SIZE-1)

#define WAVE_TRANSFER_BYTE_SIZE (512)

#define WAVE_BYTE_COMPLETE    (0)
#define WAVE_BUFFER_INCOMPLETE  (WAVE_AUDIO_STATUS_REENTRY)

#define WAVE_AUDIO_STATUS_PLAYING (1<<0)
#define WAVE_AUDIO_STATUS_HIGHBYTE (1<<1)
#define WAVE_AUDIO_STATUS_REENTRY (1<<2)

typedef struct waveHeader
{
   uint8_t channelCount;
   uint8_t resolution;
   uint16_t sampleRate;
   uint32_t dataSize;
} waveHeader_t;

typedef struct wavefile
{
   uint8_t Buffer[WAVE_OUTBUFFER_SIZE];
   volatile uint16_t audioReadptr;
   volatile uint16_t audioWriteptr;
   uint8_t audioStatus;
    
   waveHeader_t waveHeader;
   FIL fileptr;
} waveFile_t;

uint16_t waveBufferedBytes(waveFile_t* waveFile);

uint8_t waveIsPlaying(waveFile_t* waveFile);
/* Puts the byte on the buffer */
uint8_t wavePutByte(waveFile_t* waveFile, uint8_t byte);

uint8_t wavePlayFile(waveFile_t* wavefile, uint8_t* filename);
uint32_t waveParseHeader(waveFile_t* wavefile, uint8_t* filename);
uint8_t waveContinuePlaying(waveFile_t* waveFile);

#endif
