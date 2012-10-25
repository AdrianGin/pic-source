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

#define LPD8806_BPP (7)
#define RED(colour) 	((colour & 0xFF0000) >> 16)
#define GREEN(colour) 	((colour & 0x00FF00) >> 8)
#define BLUE(colour) 	(colour & 0x0000FF)
#define RGB(r,g,b)		((r << 16) | (g << 8) | (b))

#define NORM_RGB(r,g,b)	RGB( (r * MAX_LED_BRIGHTNESS) / (r+g+b), (g * MAX_LED_BRIGHTNESS) / (r+g+b), (b * MAX_LED_BRIGHTNESS) / (r+g+b))

/* Private function prototypes -----------------------------------------------*/				
void LPD8806_Init(void);
void LPD8806_Write (uint8_t cmd);

void LPD8806_Update(void);
void LPD8806_SetPixel(uint16_t index, uint32_t colour);
uint32_t LPD8806_GetPixel(uint16_t index);
void LPB8806_SetBrightness(uint8_t brightness);
void LPB8806_ReducePercentage(uint16_t index, uint8_t percent, uint8_t minVal);

#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

