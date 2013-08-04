/*
 * DAC_DMA.c
 *
 *  Created on: 7/05/2013
 *      Author: Adrian
 */

#include "DAC_DMA.h"

#include "hw_config.h"

#include <string.h>
#include "stm32f10x_dma.h"
#include "stm32f10x_dac.h"

//Private Declarations
volatile AUDIO_PlaybackBuffer_Status Audio_buffer_fill;
AUDIO_Playback_status_enum DAC_DMA_status;


uint16_t AudioBuffer[4608]; /* Playback buffer - Value must be 4608 to hold 2xMp3decoded frames */

#define DAC_CHANNEL_L	(GPIO_Pin_4)
#define DAC_CHANNEL_R	(GPIO_Pin_4)

#define DAC_DMA_TIMER		(TIM6)
#define DAC_DMA_TIMER_RCC	(RCC_APB1Periph_TIM6)
#define DAC_DMA_TIMER_TRGO	(DAC_Trigger_T6_TRGO)
#define DAC_DMA_FREQ 	    (AUDIO_FREQ)

uint8_t DAC_DMA_GetStatus(void)
{
	return (uint8_t)DAC_DMA_status;
}

void DAC_DMA_Init(void)
{


	DAC_InitTypeDef DAC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	RCC_APB1PeriphClockCmd(DAC_DMA_TIMER_RCC, ENABLE);

	/* Once the DAC channel is enabled, the corresponding GPIO pin is automatically
	 connected to the DAC converter. In order to avoid parasitic consumption,
	 the GPIO pin should be configured in analog */
	GPIO_InitStructure.GPIO_Pin = DAC_CHANNEL_L | DAC_CHANNEL_R;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_DeInit(DAC_DMA_TIMER);
	/* TIM2 Configuration */
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = SystemCoreClock / DAC_DMA_FREQ - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 0x00;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(DAC_DMA_TIMER, &TIM_TimeBaseStructure);

	/* TIM2 TRGO selection */
	TIM_SetAutoreload(DAC_DMA_TIMER, (uint32_t)(SystemCoreClock / DAC_DMA_FREQ) - 1);
	TIM_SelectOutputTrigger(DAC_DMA_TIMER, TIM_TRGOSource_Update);
	//TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);

	DAC_DeInit();
	DAC_StructInit(&DAC_InitStructure);
	/* DAC channel1 Configuration */
	DAC_InitStructure.DAC_Trigger = DAC_DMA_TIMER_TRGO;
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
	//DAC2 conflicts with SDIO
	//DAC_DMACmd(DAC_Channel_2, ENABLE);


	/* Enable DAC Channel2: Once the DAC channel2 is enabled, PA.05 is
	 automatically connected to the DAC converter. */
	DAC_Cmd(DAC_Channel_1, ENABLE);
	//DAC2 conflicts with SDIO
	DAC_Cmd(DAC_Channel_2, ENABLE);

	TIM_Cmd(DAC_DMA_TIMER, ENABLE);
	Audio_buffer_fill |= (LOW_EMPTY | HIGH_EMPTY);
	DAC_DMA_status = NO_SOUND;
}



/*******************************************************************************
 * Function Name  : DMA_Configuration
 * Description    : DMA1 channel5 configuration
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention		 : None
 *******************************************************************************/
void DAC_DMA_Configuration(uint8_t * addr, int size)
{
	DMA_InitTypeDef DMA_InitStructure;
	/* DMA2 Channel2 configuration ----------------------------------------------*/
	RCC_AHBPeriphClockCmd(DAC_DMA_RCC, ENABLE);
	/* DMA Channel configuration ----------------------------------------------*/
	DMA_DeInit(DAC_DMA_CH);

	/* Uses the original buffer	*/
	if( addr )
	{
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)addr; /* Set the buffer */
	}
	else
	{
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&AudioBuffer;
	}
	DMA_InitStructure.DMA_BufferSize = size; /* Set the size */

	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&DAC->DHR12LD;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	//DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; /* DMA循环模式 */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; /* DMA循环模式 */
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DAC_DMA_CH, &DMA_InitStructure);
	/* Enable SPI DMA Tx request */
	DMA_ITConfig(DAC_DMA_CH, DMA_IT_TC | DMA_IT_HT, ENABLE);
	//DMA_ITConfig(DMA2_Channel4, DMA_IT_TC, ENABLE);

	DAC_DMA_status = IS_PLAYING;
	DMA_Cmd(DAC_DMA_CH, ENABLE);
}

/*******************************************************************************
 * Function Name  : AUDIO_Playback_Stop
 * Description    : Stop the playback by stopping the DMA transfer
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention		 : None
 *******************************************************************************/
void DAC_DMA_Stop(void)
{
	DMA_Cmd(DAC_DMA_CH, DISABLE); /* Disable DMA Channel for Audio */
	//SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, DISABLE);   /* Disable I2S DMA REQ */
	DAC_DMA_status = NO_SOUND;
}


void DAC_DMA_NVIC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/* DMA IRQ Channel configuration */
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 12;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


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
		Audio_buffer_fill &= ~value;
	return Audio_buffer_fill;
}


uint16_t DAC_DMA_SendToDMABuffer(uint8_t* readBuf, uint16_t count, uint8_t bufIndex)
{
	AUDIO_PlaybackBuffer_Status BufferStatus;
	uint16_t ret = 0;

	BufferStatus = AUDIO_PlaybackBuffer_GetStatus((AUDIO_PlaybackBuffer_Status) 0);
	if( bufIndex == 0 )
	{
		ret = WavePlayer_ConvertPCM(MAX_AUDIO_CHANNELS, MAX_AUDIO_BIT_RESOLUTION, &readBuf[0], (uint16_t*)&AudioBuffer[0], count);
		//memcpy((void*)&AudioBuffer[0], &readBuf[0], count);
		BufferStatus = AUDIO_PlaybackBuffer_GetStatus(LOW_EMPTY);
	}
	else
	{
		ret = WavePlayer_ConvertPCM(MAX_AUDIO_CHANNELS, MAX_AUDIO_BIT_RESOLUTION, &readBuf[0], (uint16_t*)&AudioBuffer[MAX_AUDIO_FREQ*2], count);
		//memcpy((void*)&AudioBuffer[0], &readBuf[0], count);
		BufferStatus = AUDIO_PlaybackBuffer_GetStatus(HIGH_EMPTY);
	}




//	if (BufferStatus & HIGH_EMPTY) /* Decode data to low part of buffer */
//	{
//		//Start the high buffer.
//		ret = WavePlayer_ConvertPCM(1, 16, &readBuf[0], (uint16_t*)&AudioBuffer[0+WAVE_OUTBUFFER_SIZE*2], count);
//		//memcpy((void*)&AudioBuffer[0+WAVE_OUTBUFFER_SIZE*2], &readBuf[0], count);
//		BufferStatus = AUDIO_PlaybackBuffer_GetStatus(HIGH_EMPTY);
//	}

	return ret;
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
void DMA2_Channel3_IRQHandler(void)
{
#ifdef _USE_FREERTOS
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	portBASE_TYPE xYieldRequired = pdFALSE;

	if (DMA_GetITStatus(DAC_DMA_HT)) /* DMA1 通道5 半传输中断 */
	{
		DMA_ClearITPendingBit(DAC_DMA_GL | DAC_DMA_HT); /* DMA1 通道5 全局中断 */
		audio_buffer_fill |= LOW_EMPTY;
		xHigherPriorityTaskWoken = SemaphoreGiveISR(Sem_DMAComplete, &xHigherPriorityTaskWoken);
	}

	if (DMA_GetITStatus(DAC_DMA_TC)) /* DMA1 通道5 传输完成中断 */
	{
		DMA_ClearITPendingBit(DAC_DMA_GL | DAC_DMA_TC); /* DMA1 通道5 全局中断 */
		audio_buffer_fill |= HIGH_EMPTY;
		xHigherPriorityTaskWoken = SemaphoreGiveISR(Sem_DMAComplete, &xHigherPriorityTaskWoken);
	}

	if( xTaskIsTaskSuspended(mp3DecodeHandle) == pdTRUE )
	{
		xYieldRequired = xTaskResumeFromISR(mp3DecodeHandle);
	}

	 if( (xYieldRequired == pdTRUE) || (xHigherPriorityTaskWoken == pdTRUE) )
	 {
		 // We should switch context so the ISR returns to a different task.
		 // NOTE:  How this is done depends on the port you are using.  Check
		 // the documentation and examples for your port.
		 vPortYieldFromISR();
	 }
#endif
	if (DMA_GetITStatus(DAC_DMA_HT))
	{
		DMA_ClearITPendingBit(DAC_DMA_GL | DAC_DMA_HT);
		Audio_buffer_fill |= LOW_EMPTY;
	}

	if (DMA_GetITStatus(DAC_DMA_TC))
	{
		DMA_ClearITPendingBit(DAC_DMA_GL | DAC_DMA_TC);
		Audio_buffer_fill |= HIGH_EMPTY;
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

	if( bitlength == 24 )
	{

	}


	if( bitlength >= 16)
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





