/****************************************Copyright (c)**************************************************                         
**
**                                 http://www.powermcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			Audio.h
** Descriptions:		None
**
**------------------------------------------------------------------------------------------------------
** Created by:			AVRman
** Created date:		2010-11-18
** Version:				1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef __AUDIO_H
#define __AUDIO_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "mp3common.h"
#include "MP3Header.h"
#include <string.h>
#include <stdio.h>

/* Private define ------------------------------------------------------------*/
#define READBUF_SIZE   ( 7 * 1024 )      /* Value must min be 2xMAINBUF_SIZE = 2x1940 = 3880bytes */

/* Private variables ---------------------------------------------------------*/
/* file system*/
extern FATFS    fs;                      /* Work area (file system object) for logical drive */
extern FIL      mp3FileObject;           /* file objects */
extern FRESULT  res;                     /* FatFs function common result code	*/
extern UINT     n_Read;                  /* File R/W count */
extern uint8_t  readBuf[READBUF_SIZE];   /* Read buffer where data from SD card is read to */
extern uint32_t MP3_Data_Index;			 /* MP3已播放大小 */
extern volatile uint32_t outOfData; 	 

/* Private typedef -----------------------------------------------------------*/
typedef enum { FULL=0,LOW_EMPTY=1,HIGH_EMPTY=2 }                AUDIO_PlaybackBuffer_Status ;
typedef enum { MONO, STEREO}                                    AUDIO_Format_enum;
typedef enum { NO_SOUND, IS_PLAYING }                           AUDIO_Playback_status_enum;
typedef enum { LG_8_BITS=8, LG_16_BITS=16}  					AUDIO_Length_enum;


/* Private function prototypes -----------------------------------------------*/
void AUDIO_Playback_Stop(void);
void PlayAudioFile(FIL *FileObject,char *path);
const char* MP3_GetStrResult(int err);
void AUDIO_Play( s8 * buffer, int size ); 
int FillBuffer(FIL *FileObject , uint8_t start)  ;
void DMA_Configuration( s8 * addr, int size);
void I2S_Configuration(uint32_t I2S_AudioFreq);
AUDIO_PlaybackBuffer_Status AUDIO_PlaybackBuffer_GetStatus(AUDIO_PlaybackBuffer_Status value);
void AUDIO_Init_audio_mode(AUDIO_Length_enum length, uint16_t frequency, AUDIO_Format_enum format);


#endif 
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
