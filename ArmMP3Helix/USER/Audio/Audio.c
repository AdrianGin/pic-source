/****************************************Copyright (c)**************************************************                         
 **
 **                                 http://www.powermcu.com
 **
 **--------------File Info-------------------------------------------------------------------------------
 ** File name:			Audio.C
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

/* Includes ------------------------------------------------------------------*/
#include <includes.h>
#include "Audio.h"
#include "waveplayer.h"
#include "hw_config.h"

/* Private variables ---------------------------------------------------------*/
AUDIO_PlaybackBuffer_Status BufferStatus; /* Status of buffer */
s16 AudioBuffer[4608]; /* Playback buffer - Value must be 4608 to hold 2xMp3decoded frames */
static HMP3Decoder hMP3Decoder; /* Content is the pointers to all buffers and information for the MP3 Library */
MP3FrameInfo mp3FrameInfo; /* Content is the output from MP3GetLastFrameInfo,
 we only read this once, and conclude it will be the same in all frames
 Maybe this needs to be changed, for different requirements */
volatile uint32_t bytesLeft; /* Saves how many bytes left in readbuf	*/
volatile uint32_t outOfData; /* Used to set when out of data to quit playback */
volatile AUDIO_PlaybackBuffer_Status audio_buffer_fill;
AUDIO_Playback_status_enum AUDIO_Playback_status;

uint8_t Audio_Type;

AUDIO_Format_enum AUDIO_Format;
s8 *Audio_buffer;
uint8_t readBuf[READBUF_SIZE]; /* Read buffer where data from SD card is read to */
uint8_t *readPtr; /* Pointer to the next new data */
/* file system*/
FATFS fs; /* Work area (file system object) for logical drive */
FIL mp3FileObject; /* file objects */
FRESULT res; /* FatFs function common result code	*/
UINT n_Read; /* File R/W count */
/* MP3已播放字节 */
uint32_t MP3_Data_Index;
/* OS计数信号量 */
extern OS_EVENT *DMAComplete;
extern OS_EVENT *StopMP3Decode;

static waveHeader_t Wavefile;
volatile uint8_t TimeOut;

static void WavNVIC_Configuration(void);

/*******************************************************************************
 * Function Name  : AUDIO_PlaybackBuffer_Status
 * Description    : Gets the status of Playback buffer
 * Input          : - value: 0 = get the current status, else reset the flag
 *                           passed in parameter
 * Output         : None
 * Return         : FULL=0, LOW_EMPTY=1, HIGH_EMPTY=2
 * Attention		 : None
 *******************************************************************************/
AUDIO_PlaybackBuffer_Status AUDIO_PlaybackBuffer_GetStatus(
		AUDIO_PlaybackBuffer_Status value)
{
	if (value)
		audio_buffer_fill &= ~value;
	return audio_buffer_fill;
}

/*******************************************************************************
 * Function Name  : AUDIO_Playback_Stop
 * Description    : Stop the playback by stopping the DMA transfer
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention		 : None
 *******************************************************************************/
void AUDIO_Playback_Stop(void)
{
	DMA_Cmd(DMA2_Channel4, DISABLE); /* Disable DMA Channel for Audio */
	//SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, DISABLE);   /* Disable I2S DMA REQ */
	AUDIO_Playback_status = NO_SOUND;

	/* Shutdwon codec in order to avoid non expected voices */
	//codec_send( ACTIVE_CONTROL | INACTIVE );    /* WM8731 Disable */
	MP3_Data_Index = 0;
	OSSemPost(DMAComplete); /* 发送信号量 */
}

/*******************************************************************************
 * Function Name  : FillBuffer
 * Description    : Fill the buffer with Music from MP3 Data
 * Input          : - FileObject: file system
 *                  - Start: If 1, we will fill buffer nomatter the status of the playback,
 *                           used in beginning of file (pre-fill)
 * Output         : None
 * Return         : None
 * Attention		 : None
 *******************************************************************************/
int FillBuffer(FIL *FileObject, uint8_t start)
{
	uint8_t word_Allignment; /* Variable used to make sure DMA transfer is alligned to 32bit */
	int err = 0; /* Return value from MP3decoder */
	int offset; /* Used to save the offset to the next frame */

	uint16_t i;

	if (!start) /* If we are in start pos we overwrite bufferstatus */
		BufferStatus = AUDIO_PlaybackBuffer_GetStatus(
				(AUDIO_PlaybackBuffer_Status) 0);
	else
		BufferStatus = (AUDIO_PlaybackBuffer_Status) (LOW_EMPTY | HIGH_EMPTY);

	/* somewhat arbitrary trigger to refill buffer - should always be enough for a full frame */
	if (bytesLeft < 2 * MAINBUF_SIZE /*&& !eofReached*/)
	{
		/* move last, small chunk from end of buffer to start, then fill with new data */
		word_Allignment = 4 - (bytesLeft % 4); /* Make sure the first byte in writing pos. is word alligned */
		memmove(readBuf + word_Allignment, readPtr, bytesLeft);
		res = f_read(FileObject,
				(uint8_t *) readBuf + word_Allignment + bytesLeft,
				READBUF_SIZE - bytesLeft - word_Allignment, &n_Read);

		 if( res != FR_OK )
		 {
			 printf("Read=%d\n", res);
		 }

		if (n_Read != 0)
		{
			MP3_Data_Index += n_Read; /* 记录已播放大小 */
			/* zero-pad to avoid finding false sync word after last frame (from old data in readBuf) */
			if (n_Read < READBUF_SIZE - bytesLeft - word_Allignment)
			{
				memset(readBuf + bytesLeft + n_Read, 0,
						READBUF_SIZE - bytesLeft - n_Read);
			}
			bytesLeft += n_Read;
			readPtr = readBuf + word_Allignment;
		}
		else
		{
			outOfData = 1;
		}
	}

	if (outOfData == 1)
	{
		/* Stop Playback */
		AUDIO_Playback_Stop();
		return 0;
	}

	/* find start of next MP3 frame - assume EOF if no sync found */
	offset = MP3FindSyncWord(readPtr, bytesLeft);
	if (offset < 0)
	{
		bytesLeft = 0;
		return -1;
	}

	readPtr += offset; /* data start point */
	bytesLeft -= offset; /* in buffer */

	if (BufferStatus & LOW_EMPTY) /* Decode data to low part of buffer */
	{
		err = MP3Decode(hMP3Decoder, &readPtr, (int *) &bytesLeft,
				(short *) AudioBuffer, 0);

		//Convert to unsigned
		for( i = 0; i < mp3FrameInfo.outputSamps; i++ )
		{
			AudioBuffer[i] = AudioBuffer[i] + (1<<15);
		}

		BufferStatus = AUDIO_PlaybackBuffer_GetStatus(LOW_EMPTY);
	}
	else if (BufferStatus & HIGH_EMPTY) /* Decode data to the high part of buffer */
	{
		err = MP3Decode(hMP3Decoder, &readPtr, (int *) &bytesLeft,
				(short *) AudioBuffer + mp3FrameInfo.outputSamps, 0);

		//Convert to unsigned
		for( i = mp3FrameInfo.outputSamps; i < mp3FrameInfo.outputSamps*2; i++ )
		{
			AudioBuffer[i] = AudioBuffer[i] + (1<<15);
		}

		BufferStatus = AUDIO_PlaybackBuffer_GetStatus(HIGH_EMPTY);
	}
	if (err != 0)
	{
		bytesLeft = 0;
		readPtr = readBuf;
		printf("-- MP3Decode Error Occurred %s \r\n", MP3_GetStrResult(err));
		/* error occurred */
		switch (err)
		/* There is not implemeted any error handling. it will quit the playing in case of error */
		{
		/* do nothing - next call to decode will provide more mainData */
		default:
			return -1;
		}
	}
	return 0;
}

uint8_t PlayWaveFile(FIL *FileObject)
{
	unsigned int bytesRead = 0;
	uint32_t chunkSize = 0;

	waveHeader_t* wavefile = &Wavefile;

	memset(wavefile, 0, sizeof(waveHeader_t));
	memset(readBuf, 0, sizeof(readBuf));
	wavefile->is16Bit = 0;
	wavefile->isStereo = 0;


	if (f_lseek(FileObject, 0) != FR_OK)
		return WAVE_IO_ERROR + 2;

	/* Check RIFF-WAVE file header */
	if (f_read(FileObject, readBuf, 12, &bytesRead))
		return WAVE_IO_ERROR + 3;

	/* Make sure it is a WAVE file */
	if (bytesRead != 12 || LD_DWORD(readBuf + 8) != FCC('W', 'A', 'V', 'E'))
		return WAVE_INVALID_FILE;

	for (;;)
	{
		/* Get Chunk ID and size */
		if (f_read(FileObject, readBuf, 8, &bytesRead))
			return WAVE_IO_ERROR + 4;
		if (bytesRead != 8)
			return WAVE_INVALID_FILE;
		chunkSize = LD_DWORD(readBuf + 4); /* Chunk size */

		/* Switch by chunk type */
		switch (LD_DWORD(readBuf) )
		{
		case FCC('f', 'm', 't', ' '): /* 'fmt ' chunk */
			if (chunkSize & 1)
				chunkSize++;
			if (chunkSize > 128 || chunkSize < 16)
				return WAVE_INVALID_FILE; /* Check chunk size */
			if (f_read(FileObject, readBuf, chunkSize, &bytesRead))
				return WAVE_IO_ERROR + 5; /* Get the chunk content */
			if (bytesRead != chunkSize)
				return WAVE_INVALID_FILE;
			if (readBuf[FMT_FORMAT] != 1)
				return WAVE_INVALID_FILE; /* Check coding type (1: LPCM) */
			if (readBuf[FMT_NUM_CHANNELS] < 1 && readBuf[FMT_NUM_CHANNELS] > 2)
				return WAVE_INVALID_FILE; /* Check channels (1/2: Mono/Stereo) */

			wavefile->channelCount = readBuf[FMT_NUM_CHANNELS]; /* Save channel flag */
			/* Setup the offsets for stereo */
			if (wavefile->channelCount == 2)
			{
				wavefile->isStereo = 1;
			}

			if (readBuf[FMT_RESOLUTION] != 8 && readBuf[FMT_RESOLUTION] != 16)
				return WAVE_INVALID_FILE; /* Check resolution (8/16 bit) */
			wavefile->resolution = readBuf[FMT_RESOLUTION]; /* Save resolution flag */
			/* Setup the resolution byte offsets */
			if (wavefile->resolution == 16)
			{
				wavefile->is16Bit = 1;
			}

			wavefile->sampleRate = LD_DWORD(&readBuf[FMT_SAMPLERATE]); /* Check sampling freqency (8k-48k) */
			if (wavefile->sampleRate < WAVE_MINSAMPLE_RATE
					|| wavefile->sampleRate > WAVE_MAXSAMPLE_RATE)
				return WAVE_INVALID_FILE;

			if (wavefile->isStereo && wavefile->is16Bit && wavefile->sampleRate >= 44100)
			{
				//fastMode = 1;
			}
			else
			{
				//fastMode = 0;
			}

			/* Set interval timer (sampling period) */
			/* Use OCR2 */
			//waveSetSampleRate(wavefile->sampleRate);
			// = (F_CPU/32/wavefile->sampleRate) - 1;
			break;

		case FCC('d', 'a', 't', 'a'): /* 'data' chunk (start to play) */
			if (chunkSize < WAVE_MINIMUM_SAMPLES)
				return WAVE_INVALID_FILE; /* Check size */
			wavefile->dataSize = chunkSize;
			wavefile->data_start = f_tell(FileObject);
			return chunkSize;

		case FCC('D', 'I', 'S', 'P'): /* 'DISP' chunk (skip) */
		case FCC('f', 'a', 'c', 't'): /* 'fact' chunk (skip) */
		case FCC('L', 'I', 'S', 'T'): /* 'LIST' chunk (skip) */
			if (chunkSize & 1)
				chunkSize++;
			if (f_lseek(FileObject, f_tell(FileObject) + chunkSize))
				return WAVE_IO_ERROR + 6;
			break;

		default: /* Unknown chunk */
			return WAVE_INVALID_FILE;
		}
	}
}


/*******************************************************************************
 * Function Name  : WavePlayer_ConvertPCM
 * Description    : Accept, PCM u8, s16, mono and stereo. For used with DACs and PWM outputs
 * 					mono inputs will be turned into stereo.
 * Input          : either u8, s16, mono, stereo PCM
 * Output         : stereo u16
 * Return         : number of stereo u16 samples.
 * Attention		 : None
  *******************************************************************************/

uint16_t WavePlayer_ConvertPCM(uint8_t nChans, uint8_t bitlength, void* buff, uint16_t* outBuff, uint16_t nBytes)
{
	uint16_t i;
	uint16_t* u16buffer;
	uint8_t*  u8buffer;
	uint16_t  ret;

	if( bitlength == 16)
	{
		u16buffer = (uint16_t*)buff;
		if( nChans == 1 )
		{

			for( i = 0; i < nBytes/2; i++)
			{
				outBuff[2*i]   = (u16buffer[i]) + (1 << 15);
				outBuff[2*i+1] = (u16buffer[i]) + (1 << 15);
			}
			ret = i;
		}

		if( nChans == 2 )
		{
			for( i = 0; i < nBytes/2; i++)
			{
				outBuff[i] = (u16buffer[i]) + (1 << 15);
			}
			ret = i;
		}
	}

	if( bitlength == 8 )
	{
		u8buffer = (uint8_t*)buff;
		if( nChans == 1 )
		{

			for( i = 0; i < nBytes; i++)
			{
				outBuff[(2*i)] = (u8buffer[i]) << 8;
				outBuff[(2*i)+1] = (u8buffer[i]) << 8;
			}
			ret = i;
		}

		if( nChans == 2 )
		{
			for( i = 0; i < nBytes; i++)
			{
				outBuff[i] = (u8buffer[i]) << 8;
			}
			ret = i;
		}
	}

	return ret;

}

/*******************************************************************************
 * Function Name  : PlayAudioFile
 * Description    : None
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention		 : None
 *******************************************************************************/
void PlayAudioFile(FIL *FileObject, char *path)
{
	INT8U err;
	uint16_t i;
	uint16_t waveReadSize;
	/* Reset counters */
	bytesLeft = 0;
	outOfData = 0;

	readPtr = readBuf;

	WavNVIC_Configuration();
	res = f_open(FileObject, path, FA_OPEN_EXISTING | FA_READ);




	printf("FOPEN=%d\n", res);


	if ((strstr(path, "WAV") != NULL) || (strstr(path, "wav") != NULL))
	{
		PlayWaveFile(FileObject);
		Audio_Type = WAVE_FILE;
		printf("SampleRate: %d\n", Wavefile.sampleRate);
		printf("channelCount: %d\n", Wavefile.channelCount);
		printf("resolution: %d\n", Wavefile.resolution);
		printf("dataSize: %d\n", Wavefile.dataSize);
		printf("is16Bit: %d\n", Wavefile.is16Bit);
		printf("isStereo: %d\n", Wavefile.isStereo);

		NVIC_Configuration();
		I2S_Configuration(Wavefile.sampleRate);
		waveReadSize = (WAVE_OUTBUFFER_SIZE * Wavefile.channelCount * (Wavefile.resolution/8));
		DMA_Configuration((s8 *)&AudioBuffer, WAVE_OUTBUFFER_SIZE*2);

		res = f_lseek(FileObject, Wavefile.data_start);
		printf("LSEEK=%d\n", res);
		AUDIO_Playback_status = IS_PLAYING;

		BufferStatus = (AUDIO_PlaybackBuffer_Status) (LOW_EMPTY | HIGH_EMPTY);

		while (!(outOfData == 1) && !OSSemAccept(StopMP3Decode))
		{

			//Here is where we output the decoded WavData


			//if( OSSemAccept(DMAComplete) )
			{

			}

			BufferStatus = AUDIO_PlaybackBuffer_GetStatus((AUDIO_PlaybackBuffer_Status) 0);



			if (BufferStatus & LOW_EMPTY) /* Decode data to low part of buffer */
			{
				res = f_read(FileObject,
						(uint8_t *)&readBuf[0],
						waveReadSize, &n_Read);
				 if( res != FR_OK )
				 {
					 printf("Read=%d\n", res);
				 }
				//printf("LREAD=%d\n", res);
				WavePlayer_ConvertPCM(Wavefile.channelCount, Wavefile.resolution, &readBuf[0], (uint16_t*)&AudioBuffer[0], n_Read);
				BufferStatus = AUDIO_PlaybackBuffer_GetStatus(LOW_EMPTY);
			}


			if (BufferStatus & HIGH_EMPTY) /* Decode data to low part of buffer */
			{
				res = f_read(FileObject,
						(uint8_t *)&readBuf[0],
						waveReadSize, &n_Read);
				 if( res != FR_OK )
				 {
					 printf("Read=%d\n", res);
				 }
				//Start the high buffer.
				//printf("HREAD=%d\n", res);
				WavePlayer_ConvertPCM(Wavefile.channelCount, Wavefile.resolution, &readBuf[0], (uint16_t*)&AudioBuffer[0+WAVE_OUTBUFFER_SIZE*2], n_Read);
				BufferStatus = AUDIO_PlaybackBuffer_GetStatus(HIGH_EMPTY);
			}

//			if (DMA_GetITStatus(DMA2_IT_HT4)) /* DMA1 通道5 半传输中断 */
//			{
//				DMA_ClearITPendingBit(DMA2_IT_GL4 | DMA2_IT_HT4); /* DMA1 通道5 全局中断 */
//				audio_buffer_fill |= LOW_EMPTY;
//				OSSemPost(DMAComplete); /* 发送信号量 */
//			}
//
//			if (DMA_GetITStatus(DMA2_IT_TC4)) /* DMA1 通道5 传输完成中断 */
//			{
//				DMA_ClearITPendingBit(DMA2_IT_GL4 | DMA2_IT_TC4); /* DMA1 通道5 全局中断 */
//				audio_buffer_fill |= HIGH_EMPTY;
//				OSSemPost(DMAComplete); /* 发送信号量 */
//			}


			if(n_Read == 0)
			{
				OSSemPost(StopMP3Decode);
				OSSemPost(DMAComplete);
			}

			OSSemPend(DMAComplete, 0, &err);

		}

	}

	else if ((strstr(path, "MP3") != NULL) || (strstr(path, "mp3") != NULL))
	{

		Audio_Type = MP3_FILE;
		hMP3Decoder = MP3InitDecoder();
		memset(&mp3_info, 0, sizeof(mp3_info));

		Read_ID3V2(FileObject, &mp3_info);

		Read_ID3V1(FileObject, &mp3_info);

		MP3_Data_Index = 0;

		mp3_info.data_start = 0;

		/* 播放时间 */
		if ((strstr(path, "MP3") != NULL) || (strstr(path, "mp3") != NULL))
		{
			if ((strstr(path, "MP3") != NULL))
			{
				mp3_info.duration = ((path[strstr(strstr(path, "MP3") + 5, ":")
						- path - 2] - '0') * 10
						+ (path[strstr(strstr(path, "MP3") + 5, ":") - path - 1]
								- '0')) * 60; /* 分钟 */

				mp3_info.duration = mp3_info.duration
						+ (path[strstr(strstr(path, "MP3") + 5, ":") - path + 1]
								- '0') * 10
						+ (path[strstr(strstr(path, "MP3") + 5, ":") - path + 2]
								- '0'); /* 秒   */
			}
			else if ((strstr(path, "mp3") != NULL))
			{
				mp3_info.duration = ((path[strstr(strstr(path, "mp3") + 5, ":")
						- path - 2] - '0') * 10
						+ (path[strstr(strstr(path, "mp3") + 5, ":") - path - 1]
								- '0')) * 60; /* 分钟 */

				mp3_info.duration = mp3_info.duration
						+ (path[strstr(strstr(path, "mp3") + 5, ":") - path + 1]
								- '0') * 10
						+ (path[strstr(strstr(path, "mp3") + 5, ":") - path + 2]
								- '0'); /* 秒   */
			}
		}

		mp3_info.position = -1;

		res = f_lseek(FileObject, mp3_info.data_start);

		/* Read the first data to get info about the MP3 File */
		while (FillBuffer(FileObject, 1))
			; /* Must Read MP3 file header information */
		/* Get the length of the decoded data, so we can set correct play length */
		MP3GetLastFrameInfo(hMP3Decoder, &mp3FrameInfo);

		/* Select the correct samplerate and Mono/Stereo */
		AUDIO_Init_audio_mode((AUDIO_Length_enum) mp3FrameInfo.bitsPerSample,
				((MP3DecInfo *) hMP3Decoder)->samprate,
				(((MP3DecInfo *) hMP3Decoder)->nChans == 1) ? MONO : STEREO);

		/* Start the playback */
		AUDIO_Playback_status = IS_PLAYING;

		printf("DataStart = %d", mp3_info.data_start);

		while (!(outOfData == 1) && !OSSemAccept(StopMP3Decode))
		//while(1)
		{
			/* 获取信号量 */
			//Here is where we output the decoded WavData
			OSSemPend(DMAComplete, 0, &err);
			FillBuffer(FileObject, 0);

		}


		/* release mp3 decoder */
		MP3FreeDecoder(hMP3Decoder);

	}

	AUDIO_Playback_Stop();
	printf("FINISHED");
	f_close(FileObject);
}

/*******************************************************************************
 * Function Name  : MP3_GetStrResult
 * Description    : Convert file access error number in string
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention		 : None
 *******************************************************************************/
const char* MP3_GetStrResult(int err)
{
	switch (err)
	{
	case ERR_MP3_NONE:
		return "ERR_MP3_NONE";
	case ERR_MP3_INDATA_UNDERFLOW:
		return "ERR_MP3_INDATA_UNDERFLOW";
	case ERR_MP3_MAINDATA_UNDERFLOW:
		return "ERR_MP3_MAINDATA_UNDERFLOW";
	case ERR_MP3_FREE_BITRATE_SYNC:
		return "ERR_MP3_FREE_BITRATE_SYNC";
	case ERR_MP3_OUT_OF_MEMORY:
		return "ERR_MP3_OUT_OF_MEMORY";
	case ERR_MP3_NULL_POINTER:
		return "ERR_MP3_NULL_POINTER";
	case ERR_MP3_INVALID_FRAMEHEADER:
		return "ERR_MP3_INVALID_FRAMEHEADER";
	case ERR_MP3_INVALID_SIDEINFO:
		return "ERR_MP3_INVALID_SIDEINFO";
	case ERR_MP3_INVALID_SCALEFACT:
		return "ERR_MP3_INVALID_SCALEFACT";
	case ERR_MP3_INVALID_HUFFCODES:
		return "ERR_MP3_INVALID_HUFFCODES";
	case ERR_MP3_INVALID_DEQUANTIZE:
		return "ERR_MP3_INVALID_DEQUANTIZE";
	case ERR_MP3_INVALID_IMDCT:
		return "ERR_MP3_INVALID_IMDCT";
	case ERR_MP3_INVALID_SUBBAND:
		return "ERR_MP3_INVALID_SUBBAND";
	case ERR_UNKNOWN:
		return "ERR_UNKNOWN";
	default:
		return "?";
	}
}

uint16_t Sine12bit[32] =
{ 2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056, 3939, 3750, 3495,
		3185, 2831, 2447, 2047, 1647, 1263, 909, 599, 344, 155, 38, 0, 38, 155,
		344, 599, 909, 1263, 1647 };

/*******************************************************************************
 * Function Name  : DMA_Configuration
 * Description    : DMA1 channel5 configuration
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention		 : None
 *******************************************************************************/
void DMA_Configuration(s8 * addr, int size)
{
	DMA_InitTypeDef DMA_InitStructure;
	/* DMA2 Channel2 configuration ----------------------------------------------*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
	/* DMA Channel configuration ----------------------------------------------*/
	DMA_DeInit(DMA2_Channel4);

	/* Uses the original buffer	*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)addr; /* Set the buffer */
	DMA_InitStructure.DMA_BufferSize = size; /* Set the size */

	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&DAC->DHR12LD;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; /* DMA循环模式 */
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA2_Channel4, &DMA_InitStructure);
	/* Enable SPI DMA Tx request */
	DMA_ITConfig(DMA2_Channel4, DMA_IT_TC | DMA_IT_HT, ENABLE);
	//DMA_ITConfig(DMA2_Channel4, DMA_IT_TC, ENABLE);

	DMA_Cmd(DMA2_Channel4, ENABLE);
}

void DMA_ConfigurationMP3(s8 * addr, int size)
{
	DMA_InitTypeDef DMA_InitStructure;
	/* DMA2 Channel2 configuration ----------------------------------------------*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
	/* DMA Channel configuration ----------------------------------------------*/
	DMA_DeInit(DMA2_Channel4);

	/* Uses the original buffer	*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)addr; /* Set the buffer */
	DMA_InitStructure.DMA_BufferSize = size/2; /* Set the size */

	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&DAC->DHR12LD;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; /* DMA循环模式 */
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA2_Channel4, &DMA_InitStructure);
	/* Enable SPI DMA Tx request */
	DMA_ITConfig(DMA2_Channel4, DMA_IT_TC | DMA_IT_HT, ENABLE);
	//DMA_ITConfig(DMA2_Channel4, DMA_IT_TC, ENABLE);

	DMA_Cmd(DMA2_Channel4, ENABLE);
}

/*******************************************************************************
 * Function Name  : I2S_Configuration
 * Description    : I2S2 configuration
 * Input          : - I2S_AudioFreq: 采样频率
 * Output         : None
 * Return         : None
 * Attention		 : None
 *******************************************************************************/
void I2S_Configuration(uint32_t I2S_AudioFreq)
{

	DAC_InitTypeDef DAC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

	/* Once the DAC channel is enabled, the corresponding GPIO pin is automatically
	 connected to the DAC converter. In order to avoid parasitic consumption,
	 the GPIO pin should be configured in analog */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_DeInit(TIM7);
	/* TIM2 Configuration */
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = SystemCoreClock / I2S_AudioFreq;
	TIM_TimeBaseStructure.TIM_Prescaler = 0x00;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

	/* TIM2 TRGO selection */
	TIM_SetAutoreload(TIM7, (uint32_t)(SystemCoreClock / I2S_AudioFreq));
	TIM_SelectOutputTrigger(TIM7, TIM_TRGOSource_Update);
	//TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);

	DAC_DeInit();
	DAC_StructInit(&DAC_InitStructure);
	/* DAC channel1 Configuration */
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T7_TRGO;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	/* DAC channel2 Configuration */
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
	DAC_Init(DAC_Channel_2, &DAC_InitStructure);

	/* Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is
	 automatically connected to the DAC converter. */
	//DAC_Cmd(DAC_Channel_1, ENABLE);

	//Setup the DAC DMA on for Channel 2.
	DAC_DMACmd(DAC_Channel_1, ENABLE);
	DAC_DMACmd(DAC_Channel_2, ENABLE);
	/* Enable DAC Channel2: Once the DAC channel2 is enabled, PA.05 is
	 automatically connected to the DAC converter. */
	DAC_Cmd(DAC_Channel_1, ENABLE);
	DAC_Cmd(DAC_Channel_2, ENABLE);

	TIM_Cmd(TIM7, ENABLE);
}

/*******************************************************************************
 * Function Name  : DMA2_Channel2_IRQHandler
 * Description    : Handles the DMA2 global interrupt request
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention		 : in mono mode, stop managed by AUDIO_Cpy_Mono() else it must
 *                  be managed by the application
 *******************************************************************************/
void DMA2_Channel4_5_IRQHandler(void)
{
	CPU_SR cpu_sr;
	OS_ENTER_CRITICAL()	;
	OSIntNesting++;
	OS_EXIT_CRITICAL()	;

	if (DMA_GetITStatus(DMA2_IT_HT4)) /* DMA1 通道5 半传输中断 */
	{
		DMA_ClearITPendingBit(DMA2_IT_GL4 | DMA2_IT_HT4); /* DMA1 通道5 全局中断 */
		audio_buffer_fill |= LOW_EMPTY;
		OSSemPost(DMAComplete); /* 发送信号量 */
	}

	if (DMA_GetITStatus(DMA2_IT_TC4)) /* DMA1 通道5 传输完成中断 */
	{
		DMA_ClearITPendingBit(DMA2_IT_GL4 | DMA2_IT_TC4); /* DMA1 通道5 全局中断 */
		audio_buffer_fill |= HIGH_EMPTY;
		OSSemPost(DMAComplete); /* 发送信号量 */
	}



	OSIntExit();
}



void TIM7_IRQHandler(void)
{
	CPU_SR cpu_sr;
	OS_ENTER_CRITICAL()	;
	OSIntNesting++;
	OS_EXIT_CRITICAL()	;

	TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
	TimeOut = 1;

	OSIntExit();
}


/*******************************************************************************
 * Function Name  : NVIC_Configuration
 * Description    : Configures DMA IRQ channel.
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention		 : None
 *******************************************************************************/
static void WavNVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//NVIC_Init(&NVIC_InitStructure);

	/* DMA IRQ Channel configuration */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel4_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
 * Function Name  : AUDIO_Init_audio_mode
 * Description    : Initialization for Audio Mode use of the WM8731 codec.set WM8731
 *                  to Audio Mode via I2C. enable STM32 I2S communication to send
 *                  audio samples (SPI2/I2S2 port) in DMA mode
 * Input          : - length: 8 bits or 16 bits length sample
 *				   - frequency:	8 KHz, 16 KHz, 22 KHz, 44 KHz, 48 KHz sample
 *				   - format: mono, stereo
 * Output         : None
 * Return         : None
 * Attention		 : None
 *******************************************************************************/
void AUDIO_Init_audio_mode(AUDIO_Length_enum length, uint16_t frequency,
		AUDIO_Format_enum format)
{
	/* The MONO mode uses working buffer to dupplicate datas on the two channels
	 and switch buffer half by half => uses DMA in circular mode */

	mp3_info.length = length;
	mp3_info.sampling = frequency;
	mp3_info.bit_rate = mp3FrameInfo.bitrate;

	AUDIO_Playback_status = NO_SOUND;
	AUDIO_Format = format;

	/* Buffers are supposed to be empty here	*/
	audio_buffer_fill = (AUDIO_PlaybackBuffer_Status) (LOW_EMPTY | HIGH_EMPTY);

	WavNVIC_Configuration();
	I2S_Configuration(frequency);
	//codec_send( ACTIVE_CONTROL | ACTIVE );	   /* WM8731 Enable */
	DMA_ConfigurationMP3((s8 *) AudioBuffer, (mp3FrameInfo.outputSamps * 2));

	if (strlen(mp3_info.title) == 0)
	{
		strcpy(mp3_info.title, "找不到歌曲名");
	}

	if (strlen(mp3_info.title) > 20)
	{
		strcpy(mp3_info.title, "歌曲名太长、无法显示");
	}

	printf("-- MP3 file header information \r\n");
	printf("-------------------------------\r\n");
	printf("-- MP3 title %s \r\n", mp3_info.title);
	printf("-- MP3 artist %s \r\n", mp3_info.artist);
	printf("-- MP3 bitrate        = %d \r\n", mp3_info.bit_rate); /* 比特率 */
	printf("-- MP3 bitsPerSample  = %d \r\n", mp3_info.length); /* 每个采样需要的bit数 */
	printf("-- MP3 samprate       = %d \r\n", mp3_info.sampling); /* 采样率 */
	printf("-- MP3 nChans         = %d \r\n", AUDIO_Format); /* 声道 */
	/* 每帧数据的采样数 */
	printf("-- MP3 nGranSamps     = %d \r\n",
			((MP3DecInfo *) hMP3Decoder)->nGranSamps);
	/* 数据帧大小 FrameSize = (((MpegVersion == MPEG1 ? 144 : 72) * Bitrate) / SamplingRate) + PaddingBit */
	printf("-- MP3 outputSamps    = %d \r\n", mp3FrameInfo.outputSamps);
	printf("-- MP3 layer          = %d \r\n", mp3FrameInfo.layer);
	printf("-- MP3 version        = %d \r\n", mp3FrameInfo.version);
	printf("-- MP3 duration       = %d \r\n", mp3_info.duration);
	printf("-- MP3 datastart       = %d \r\n", mp3_info.data_start);
}

/*********************************************************************************************************
 END FILE
 *********************************************************************************************************/

