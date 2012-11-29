/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               TouchPanel.c
** Descriptions:            The TouchPanel application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPD8806.h"
#include <string.h>


//holds 21bit colour data in the format brg
//uint8_t LPD8806_GFXRAM[LED_COUNT*3];

uint8_t LPD8806_DMABUFFER[LED_COUNT*3+2];

uint8_t* LPD8806_GFXRAM = &LPD8806_DMABUFFER[1];

uint8_t LPD8806_Brightness;

#define LPD8806_USE_DMA
#define LPB8806_DMA_CHANNEL	DMA1_Channel3
#define LPD8806_USE_SPI1
//#define LPD8806_USE SPI2

#ifdef  LPD8806_USE_SPI1

#define LPB_USE_ALTERNATE_SPI1

#define LPD_SPI			SPI1
#define LPB_SPI_RCC		RCC_APB2Periph_SPI1


#ifdef LPB_USE_ALTERNATE_SPI1
#define LPB_GPIO_RCC	 RCC_APB2Periph_GPIOB
#define LPD_SPI_PORT	 (GPIOB)
#define LPD_SPI_PINS	 (GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5)

#else
#define LPB_GPIO_RCC	 RCC_APB2Periph_GPIOA
#define LPD_SPI_PORT	 (GPIOA)
#define LPD_SPI_PINS	 (GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7)
#endif


#define LPD_SPI_RCC_APBeriphClockCmd RCC_APB2PeriphClockCmd

#else

#define LPD_SPI			SPI2
#define LPB_SPI_RCC		RCC_APB1Periph_SPI2
#define LPB_GPIO_RCC	RCC_APB2Periph_GPIOB

#define LPD_SPI_PORT	 (GPIOB)
#define LPD_SPI_PINS	 (GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15)

#define LPD_SPI_RCC_APBeriphClockCmd RCC_APB1PeriphClockCmd

#endif




uint8_t _Apply_Brightness(uint8_t value)
{
	if( value )
	{
		value = ((value * LPD8806_Brightness) / MAX_LED_BRIGHTNESS);
		if( value == 0 )
		{
			value = 1;
		}
	}
	return value;
}

uint8_t _Revert_Brightness(uint8_t value)
{

	uint8_t oldValue;

	oldValue = value;

	if( value == 0 )
	{
		return value;
	}

	if( (value + 1) > LPD8806_MAX_RES )
	{
		value = LPD8806_MAX_RES;
	}
	else
	{
		if( LPD8806_Brightness != MAX_LED_BRIGHTNESS)
		{
			//value = value + 1;
		}
	}

	value = (value * MAX_LED_BRIGHTNESS) / LPD8806_Brightness;
	if( value > oldValue )
	{
		value = oldValue;
	}


	if( value > 127 )
	{
		value = 127;
	}
	if( value == 0 )
	{
		value = 1;
	}

	return value;
}

/*******************************************************************************
* Function Name  : ADS7843_SPI_Init
* Description    : ADS7843 SPI 初始化
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void LPD8806_SPI_Init(void)
{ 
  SPI_InitTypeDef  SPI_InitStructure;
  LPD_SPI_RCC_APBeriphClockCmd(LPB_SPI_RCC, ENABLE);
  /* DISABLE SPI1 */

#ifdef LPB_USE_ALTERNATE_SPI1
  //Need to perform a remap of pins.
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);
#endif


  SPI_Cmd(LPD_SPI, DISABLE);
  /* SPI1 Config -------------------------------------------------------------*/ 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master; 
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; 
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; 
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; 
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
  SPI_InitStructure.SPI_CRCPolynomial = 7; 

  SPI_Init(LPD_SPI, &SPI_InitStructure);
  /* Enable SPI1 */ 
  SPI_Cmd(LPD_SPI, ENABLE);
} 

/*******************************************************************************
* Function Name  : TP_Init
* Description    : ADS7843端口初始化
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LPD8806_Init(void)
{ 
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(LPB_GPIO_RCC | RCC_APB2Periph_AFIO , ENABLE);
  /* Configure SPI1 pins: SCK, MISO and MOSI ---------------------------------*/ 
  GPIO_InitStructure.GPIO_Pin =  LPD_SPI_PINS;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LPD_SPI_PORT, &GPIO_InitStructure);
  LPD8806_SPI_Init();

  memset(LPD8806_DMABUFFER, 0x00 , sizeof(LPD8806_DMABUFFER));
  LPD8806_SetBrightness(MAX_LED_BRIGHTNESS);

  LPD8806_DMA_Init();
  //Have to enable the SPI DMA.
  SPI_I2S_DMACmd(LPD_SPI, SPI_I2S_DMAReq_Tx, ENABLE);

} 




void LPD8806_DMA_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;


	/* DMA2 Channel2 configuration ----------------------------------------------*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/* DMA Channel configuration ----------------------------------------------*/
	DMA_DeInit(LPB8806_DMA_CHANNEL);

	/* Uses the original buffer	*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&LPD8806_DMABUFFER[0]; /* Set the buffer */
	DMA_InitStructure.DMA_BufferSize = (LED_COUNT*3)+2; /* Set the size */

	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&LPD_SPI->DR;
	//DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&DAC->DHR12L1;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; /* DMA循环模式 */
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(LPB8806_DMA_CHANNEL, &DMA_InitStructure);
	/* Enable SPI DMA Tx request */
	DMA_ITConfig(LPB8806_DMA_CHANNEL, DMA_IT_TC, ENABLE);
	//DMA_ITConfig(DMA2_Channel4, DMA_IT_TC, ENABLE);

	DMA_Cmd(LPB8806_DMA_CHANNEL, ENABLE);
}



void LPB8806_DMA_Start(void)
{
	/* DMA2 Channel2 configuration ----------------------------------------------*/
	/* DMA Channel configuration ----------------------------------------------*/
	DMA_Cmd(LPB8806_DMA_CHANNEL, DISABLE);
	DMA_SetCurrDataCounter(LPB8806_DMA_CHANNEL, (LED_COUNT*3)+2);
	//DMA_ITConfig(DMA2_Channel4, DMA_IT_TC, ENABLE);
	DMA_Cmd(LPB8806_DMA_CHANNEL, ENABLE);
}

/*******************************************************************************
* Function Name  : WR_CMD
* Description    : 向 ADS7843写数据
* Input          : - cmd: 传输的数据
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void LPD8806_Write (uint8_t cmd)
{ 
  /* Wait for SPI1 Tx buffer empty */ 
  while (SPI_I2S_GetFlagStatus(LPD_SPI, SPI_I2S_FLAG_TXE) == RESET);
  /* Send SPI1 data */ 
  SPI_I2S_SendData(LPD_SPI,cmd);
  /* Wait for SPI1 data reception */ 
  while (SPI_I2S_GetFlagStatus(LPD_SPI, SPI_I2S_FLAG_RXNE) == RESET);
  /* Read SPI1 received data */ 
  SPI_I2S_ReceiveData(LPD_SPI);
} 


//Converts 24bit RGB to 21bit brg (LPD8806 format)

/*******************************************************************************
* Function Name  : LPD8806_SetPixel
* Description    : converts colour to 21bit BRG (LPD8806 format), uses most significant 8 bits
* Input          : colour is a 24bit RGB, but is automatically truncated to 7bit
* Output         :
* Return         : None
* Attention		 : None
*******************************************************************************/
void LPD8806_SetPixel(uint16_t index, uint32_t colour)
{

	uint8_t r,g,b;


	if( index >= LED_COUNT )
	{
		return;
	}


	r = LPD_RED(colour) 	>> 1;
	g = LPD_GREEN(colour) 	>> 1;
	b = LPD_BLUE(colour) 	>> 1;

	r = _Apply_Brightness(r);
	g = _Apply_Brightness(g);
	b = _Apply_Brightness(b);

	//Blue
	LPD8806_GFXRAM[index*3] = b | LPD8806_FLAG;
	//Red
	LPD8806_GFXRAM[(index*3)+1] = r | LPD8806_FLAG;
	//Green
	LPD8806_GFXRAM[(index*3)+2] = g | LPD8806_FLAG;
}



uint32_t LPD8806_GetPixel(uint16_t index)
{


	uint8_t r,g,b;

	r = (LPD8806_GFXRAM[(index*3)+1] & LPD8806_MAX_RES);
	g = (LPD8806_GFXRAM[(index*3)+2] & LPD8806_MAX_RES);
	b = (LPD8806_GFXRAM[index*3] & LPD8806_MAX_RES);

	r = _Revert_Brightness(r);
	g = _Revert_Brightness(g);
	b = _Revert_Brightness(b);

	return ( RGB(r<<1,g<<1,b<<1) );
}

//If minVal is nonzero, the value will not drop below the minVal.
void LPD8806_ReducePercentage(uint16_t index, uint8_t percent, uint8_t minVal)
{
	uint16_t r,g,b;
	uint16_t nr, ng, nb;

	uint32_t colour;

	colour = LPD8806_GetPixel(index);

	r = LPD_RED(colour);
	g = LPD_GREEN(colour);
	b = LPD_BLUE(colour);


	//Multiply values by 100, and then divide 100.
	nr = r * 100;
	ng = g * 100;
	nb = b * 100;

	nr = (nr - r*percent) / 100;
	ng = (ng - g*percent) / 100;
	nb = (nb - b*percent) / 100;

	if( nr <= minVal && r != 0 )
	{
		nr = minVal;
	}
	if( ng <= minVal && g != 0)
	{
		ng = minVal;
	}
	if( nb <= minVal && b != 0)
	{
		nb = minVal;
	}

	//Have to upscale the 7bits to 8bits.
	colour = RGB(nr, ng, nb);
	LPD8806_SetPixel(index, colour);
}



void LPD8806_Update(void)
{

	//Should use DMA here.
#ifndef LPD8806_USE_DMA
	uint16_t i;
	uint16_t outputValue;
	LPD8806_Write(0);
	for( i = 0; i < LED_COUNT*3; i++ )
	{
		outputValue = (LPD8806_GFXRAM[i] & LPD8806_MAX_RES) * LPD8806_Brightness / MAX_LED_BRIGHTNESS;
		LPD8806_Write(outputValue | LPD8806_FLAG );
	}
	LPD8806_Write(0);
#else
	while( DMA_GetFlagStatus(DMA1_FLAG_TC3) == RESET)
	{}

	LPB8806_DMA_Start();

#endif
}


void LPD8806_SetBrightness(uint8_t brightness)
{
	uint16_t i;
	uint32_t colour;
	uint8_t oldBrightness;

	oldBrightness = LPD8806_Brightness;
	if( brightness == oldBrightness )
	{
		return;
	}

	for( i = 0; i < LED_COUNT*3; i++ )
	{
		LPD8806_Brightness = oldBrightness;
		colour = LPD8806_GetPixel(i);
		LPD8806_Brightness = brightness;
		LPD8806_SetPixel(i, colour);
	}
}


/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
