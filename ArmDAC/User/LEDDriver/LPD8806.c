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
uint8_t LPD8806_GFXRAM[LED_COUNT*3];
uint8_t LPD8806_Brightness;
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
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
  /* DISABLE SPI1 */ 
  SPI_Cmd(SPI2, DISABLE);
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
  SPI_Init(SPI2, &SPI_InitStructure);
  /* Enable SPI1 */ 
  SPI_Cmd(SPI2, ENABLE);
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
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  /* Configure SPI1 pins: SCK, MISO and MOSI ---------------------------------*/ 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  LPD8806_SPI_Init();
  LPB8806_SetBrightness(MAX_LED_BRIGHTNESS);

  memset(LPD8806_GFXRAM, 0, sizeof(LPD8806_GFXRAM));


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
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
  /* Send SPI1 data */ 
  SPI_I2S_SendData(SPI2,cmd);
  /* Wait for SPI1 data reception */ 
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
  /* Read SPI1 received data */ 
  SPI_I2S_ReceiveData(SPI2);
} 


//Converts 24bit RGB to 21bit brg (LPD8806 format)

/*******************************************************************************
* Function Name  : LPD8806_SetPixel
* Description    : converts colour to 21bit BRG (LPD8806 format), uses most significant 8 bits
* Input          : colour is a 24bit RGB
* Output         :
* Return         : None
* Attention		 : None
*******************************************************************************/
void LPD8806_SetPixel(uint16_t index, uint32_t colour)
{

	if( index >= LEDCOUNT )
	{
		return;
	}

	uint8_t r,g,b;
	r = RED(colour) 	>> 1;
	g = GREEN(colour) 	>> 1;
	b = BLUE(colour) 	>> 1;
	//Blue
	LPD8806_GFXRAM[index*3] = b;
	//Red
	LPD8806_GFXRAM[(index*3)+1] = r;
	//Green
	LPD8806_GFXRAM[(index*3)+2] = g;
}



uint32_t LPD8806_GetPixel(uint16_t index)
{
	return (LPD8806_GFXRAM[(index*3)+1] << 17) | (LPD8806_GFXRAM[(index*3)+2]<<9) | (LPD8806_GFXRAM[index*3]);
}


//If minVal is nonzero, the value will not drop below the minVal.
void LPB8806_ReducePercentage(uint16_t index, uint8_t percent, uint8_t minVal)
{
	uint16_t r,g,b;
	uint16_t nr, ng, nb;

	r = LPD8806_GFXRAM[(index*3)+1];
	g = LPD8806_GFXRAM[(index*3)+2];
	b = LPD8806_GFXRAM[index*3];

	//Multiply values by 100, and then divide 100.
	nr = r * 100;
	ng = g * 100;
	nb = b * 100;

	nr = (nr - r*percent) / 100;
	ng = (ng - g*percent) / 100;
	nb = (nb - b*percent) / 100;

	if( nr < minVal && r != 0 )
	{
		nr = minVal;
	}
	if( ng < minVal && g != 0)
	{
		ng = minVal;
	}
	if( nb < minVal && b != 0)
	{
		nb = minVal;
	}

	LPD8806_GFXRAM[(index*3)+1] = nr;
	LPD8806_GFXRAM[(index*3)+2] = ng;
	LPD8806_GFXRAM[(index*3)] = nb;
}



void LPD8806_Update(void)
{
	uint16_t i;
	uint16_t outputValue;

	LPD8806_Write(0);
	for( i = 0; i < LED_COUNT*3; i++ )
	{
		outputValue = LPD8806_GFXRAM[i] * LPD8806_Brightness / MAX_LED_BRIGHTNESS;
		LPD8806_Write(outputValue | 0x80 );
	}
	LPD8806_Write(0);
}

void LPB8806_SetBrightness(uint8_t brightness)
{
	LPD8806_Brightness = brightness;
}


/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
