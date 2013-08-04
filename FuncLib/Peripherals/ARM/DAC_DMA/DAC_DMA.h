/*
 * DAC_DMA.h
 *
 *  Created on: 7/05/2013
 *      Author: Adrian
 */

#ifndef DAC_DMA_H_
#define DAC_DMA_H_



#include <stdint.h>

#define AUDIO_FREQ	(48000)

//in kHz
#define MAX_AUDIO_FREQ	(AUDIO_FREQ/1000)
#define MAX_AUDIO_BIT_RESOLUTION (16)
#define MAX_AUDIO_CHANNELS (2)
#define MAX_AUDIO_PACKET_SIZE	(MAX_AUDIO_FREQ*MAX_AUDIO_BIT_RESOLUTION*MAX_AUDIO_CHANNELS / 8)


typedef enum { FULL=0,LOW_EMPTY=1,HIGH_EMPTY=2 }                AUDIO_PlaybackBuffer_Status ;
typedef enum { MONO, STEREO}                                    AUDIO_Format_enum;
typedef enum { NO_SOUND, IS_PLAYING }                           AUDIO_Playback_status_enum;
typedef enum { LG_8_BITS=8, LG_16_BITS=16}  					AUDIO_Length_enum;

#define DAC_DMA_CH					(DMA2_Channel3)
#define DAC_DMA_IRQn				(DMA2_Channel3_IRQn)
#define DAC_DMA_HT					(DMA2_IT_HT3)
#define DAC_DMA_TC					(DMA2_IT_TC3)
#define DAC_DMA_GL					(DMA2_IT_GL3)
#define DAC_DMA_RCC				(RCC_AHBPeriph_DMA2)

#define WAVE_OUTBUFFER_SIZE   (1152)
#define WAVE_OUTMASK         (WAVE_OUTBUFFER_SIZE-1)

extern volatile AUDIO_PlaybackBuffer_Status Audio_buffer_fill;

void DAC_DMA_Init(void);
void DAC_DMA_NVIC_Init(void);
void DAC_DMA_Stop(void);


void DAC_DMA_Configuration(uint8_t * addr, int size);
uint16_t DAC_DMA_SendToDMABuffer(uint8_t* readBuf, uint16_t count, uint8_t bufIndex);
uint16_t WavePlayer_ConvertPCM(uint8_t nChans, uint8_t bitlength, void* buff, uint16_t* outBuff, uint16_t nBytes);

void DMA2_Channel3_IRQHandler(void);

uint8_t DAC_DMA_GetStatus(void);

#endif /* DAC_DMA_H_ */
