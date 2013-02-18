/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               TouchPanel.h
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

#ifndef _LPD8806_H_
#define _LPD8806_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private define ------------------------------------------------------------*/
#define LED_COUNT (48*5)
#define MAX_LED_BRIGHTNESS (255)

#define LPD8806_MAX_RES		(0x7F)
//This flag represents a non-reset flag.
#define LPD8806_FLAG		(0x80)

#define LPD8806_BPP (7)
#define LPD_RED(colour) 	((colour & 0xFF0000) >> 16)
#define LPD_GREEN(colour) 	((colour & 0x00FF00) >> 8)
#define LPD_BLUE(colour) 	(colour & 0x0000FF)
#define RGB(r,g,b)		((r << 16) | (g << 8) | (b))

#define NORM_RGB(r,g,b)	RGB( (r * MAX_LED_BRIGHTNESS) / (r+g+b), (g * MAX_LED_BRIGHTNESS) / (r+g+b), (b * MAX_LED_BRIGHTNESS) / (r+g+b))


#define SCALE(number, factor, divisor)	((number * factor) / divisor )
#define SCALE_COLOUR(colour, factor, divisor)	RGB( SCALE(LPD_RED(colour), factor, divisor), \
													 SCALE(LPD_GREEN(colour), factor, divisor), \
													 SCALE(LPD_BLUE(colour), factor, divisor))


#define LPD8806_USE_DMA
#define LPD8806_USE_SPI1
#define LPD_USE_ALTERNATE_SPI1
//#define LPD8806_USE SPI2

#ifdef  LPD8806_USE_SPI1
#define LPB8806_DMA_CHANNEL	DMA1_Channel3
#define LPD_SPI			SPI1
#define LPB_SPI_RCC		RCC_APB2Periph_SPI1
#define LPD_DMA_FLAG_TC	DMA1_FLAG_TC3

#ifdef LPD_USE_ALTERNATE_SPI1
#define LPB_GPIO_RCC	 RCC_APB2Periph_GPIOB
#define LPD_SPI_PORT	 (GPIOB)
#define SPI1_SCK		 (GPIO_Pin_3)
#define SPI1_MISO		 (GPIO_Pin_4)
#define SPI1_MOSI		 (GPIO_Pin_5)
#define LPD_SPI_PINS	 (SPI1_SCK | SPI1_MISO | SPI1_MOSI)

#else
#define LPB_GPIO_RCC	 RCC_APB2Periph_GPIOA
#define LPD_SPI_PORT	 (GPIOA)
#define LPD_SPI_PINS	 (GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7)
#endif


#define LPD_SPI_RCC_APBeriphClockCmd RCC_APB2PeriphClockCmd

#else
#define LPB8806_DMA_CHANNEL	DMA1_Channel5
#define LPD_DMA_FLAG_TC		DMA1_FLAG_TC5

#define LPD_SPI			SPI2
#define LPB_SPI_RCC		RCC_APB1Periph_SPI2
#define LPB_GPIO_RCC	RCC_APB2Periph_GPIOB

#define LPD_SPI_PORT	 (GPIOB)
#define LPD_SPI_PINS	 (GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15)

#define LPD_SPI_RCC_APBeriphClockCmd RCC_APB1PeriphClockCmd

#endif




#define INLINE_FUNCTION	__inline

/* Private function prototypes -----------------------------------------------*/				
void LPD8806_Init(void);
void LPD8806_Write (uint8_t cmd);

void LPD8806_Update(void);
void LPD8806_SetPixel(uint16_t index, uint32_t colour);

void LPD8806_Clear(void);
void LPD8806_Test(void);
uint32_t LPD8806_GetPixel(uint16_t index);


void LPD8806_SetBrightness(uint8_t brightness);
void LPD8806_ReducePercentage(uint16_t index, uint8_t percent, uint8_t minVal);
void LPD8806_DMA_Init(void);


#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/


