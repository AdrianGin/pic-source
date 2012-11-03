/*********************************************************************************************************
*
* File                : LCD.h
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : V1.0
* By                  : sasa_xiao
*
*                                  (c) Copyright 2005-2011, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

#ifdef __cplusplus    /* Insert start of extern C construct */
extern "C" {
#endif


#ifndef __LCD_H 
#define __LCD_H

/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include "stm32f10x.h"

/* Private define ------------------------------------------------------------*/
#define USE_16BIT_PMP

#define LCD_REG              (*((volatile unsigned short *) 0x60000000)) /* RS = 0 */
#define LCD_RAM              (*((volatile unsigned short *) 0x60020000)) /* RS = 1 */


/*********************************************************************
* Overview: Horizontal and vertical display resolution
*                  (from the glass datasheet).
*********************************************************************/
#define DISP_HOR_RESOLUTION				240
#define DISP_VER_RESOLUTION				320

//#define DISP_ORIENTATION					0
 #define DISP_ORIENTATION					0
// //#define DISP_ORIENTATION					180
// //#define DISP_ORIENTATION					270

// /* Private define ------------------------------------------------------------*/

#if  ( DISP_ORIENTATION == 90 ) || ( DISP_ORIENTATION == 270 )

#define  MAX_X  320
#define  MAX_Y  240   

#elif  ( DISP_ORIENTATION == 0 ) || ( DISP_ORIENTATION == 180 )

#define  MAX_X  240
#define  MAX_Y  320   

#endif

/*********************************************************************
* Overview: Horizontal synchronization timing in pixels
*                  (from the glass datasheet).
*********************************************************************/
//#define DISP_HOR_PULSE_WIDTH		20    /* 20 */
//#define DISP_HOR_BACK_PORCH			51	  /* 48	*/
//#define DISP_HOR_FRONT_PORCH		20	  /* 20 */

/*********************************************************************
* Overview: Vertical synchronization timing in lines
*                  (from the glass datasheet).
*********************************************************************/
//#define DISP_VER_PULSE_WIDTH		2	  /* 2 */
//#define DISP_VER_BACK_PORCH			12	  /* 16 */
//#define DISP_VER_FRONT_PORCH		4	  /* 4 */

/*********************************************************************
* Definition for SPI interface for HIMAX 8238-A relevant to hardware 
* layout; Hardware dependent!
*********************************************************************/
#define GPIO3 3
#define GPIO2 2
#define GPIO1 1
#define GPIO0 0
#define LCD_RESET (1<<GPIO3)	   /* LCD Reset signal (Reset for display panel, NOT ssd1963) */
#define LCD_SPENA (1<<GPIO0)	   /* SPI EN signal */
#define LCD_SPCLK (1<<GPIO1)	   /* SPI CLK */
#define LCD_SPDAT (1<<GPIO2)	   /* SPI DATA */

/* LCD color */
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

#define RGB565CONVERT(red, green, blue) (int) (((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3))

/* Private function prototypes -----------------------------------------------*/
void LCD_Initializtion(void);
void LCD_Clear(uint16_t Color);	
void LCD_BackLight(uint8_t intensity);
void LCD_BackLight_Init(void);
uint16_t LCD_GetPoint(uint16_t Xpos,uint16_t Ypos);
void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point);
void PutChar(uint16_t Xpos,uint16_t Ypos,uint8_t c,uint16_t charColor,uint16_t bkColor);
void LCD_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color );
void PutChinese(uint16_t Xpos,uint16_t Ypos,uint8_t *str,uint16_t Color,uint16_t bkColor); 
void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor);
void GUI_Chinese(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor);	
void LCD_DrawPicture(uint16_t StartX,uint16_t StartY,uint16_t EndX,uint16_t EndY,uint16_t *pic);



void LCD_SetCursor( uint16_t Xpos, uint16_t Ypos );
void delay_ms(uint16_t ms);

void LCD_VerticalScroll(uint16_t start, uint16_t end, uint16_t length);
void LCD_DMATxByte(uint16_t* buffer, uint16_t size);

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : LCD控制器寄存器地址
* Input          : - index: 寄存器地址
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
__inline void LCD_WriteIndex(uint16_t index)
{
	LCD_REG	= index;
/*
	Clr_Rs;
	Set_nRd;

	GPIOE->ODR = index;	 // GPIO_Write(GPIOE,index);

	Clr_nWr;
	Set_nWr;
*/
}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : LCD寄存器数据
* Input          : - index: 寄存器数据
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
__inline void LCD_WriteData(uint16_t data)
{
	LCD_RAM = data;
/*
	Set_Rs;

	GPIOE->ODR = data;	 //GPIO_Write(GPIOE,data);

	Clr_nWr;
	Set_nWr;
*/
}

/*******************************************************************************
* Function Name  : LCD_ReadData
* Description    : 读取控制器数据
* Input          : None
* Output         : None
* Return         : 返回读取到的数据
* Attention		 : None
*******************************************************************************/
__inline uint16_t LCD_ReadData(void)
{
//	uint16_t value;

//	Set_Rs;
//	Set_nWr;
//	Clr_nRd;

/*
    PE.00(D0), PE.01(D1), PE.02(D2), PE.03(D3), PE.04(D4), PE.05(D5), PE.06(D6), PE.07(D7), PE.08(D8)
    PE.09(D9), PE.10(D10), PE.11(D11), PE.12(D12), PE.13(D13), PE.14(D14), PE.15(D15)   */
/*
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
*/
//	GPIOE->CRH = 0x44444444;
//	GPIOE->CRL = 0x44444444;
/*
    value = GPIO_ReadInputData(GPIOE);
    value = GPIO_ReadInputData(GPIOE);
*/
//    value = GPIOE->IDR;
//    value = GPIOE->IDR;

/*
    PE.00(D0), PE.01(D1), PE.02(D2), PE.03(D3), PE.04(D4), PE.05(D5), PE.06(D6), PE.07(D7), PE.08(D8)
    PE.09(D9), PE.10(D10), PE.11(D11), PE.12(D12), PE.13(D13), PE.14(D14), PE.15(D15)   */
/*
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
*/
//    GPIOE->CRH = 0x33333333;
//    GPIOE->CRL = 0x33333333;

//    Set_nRd;

//    return value;
	return LCD_RAM;
}


/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Writes to the selected LCD register.
* Input          : - LCD_Reg: address of the selected register.
*                  - LCD_RegValue: value to write to the selected register.
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
__inline void LCD_WriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue)
{
	/* Write 16-bit Index, then Write Reg */
//	Clr_Cs;
	LCD_WriteIndex(LCD_Reg);
	/* Write 16-bit Reg */
	LCD_WriteData(LCD_RegValue);
//	Set_Cs;
}

/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Reads the selected LCD Register.
* Input          : None
* Output         : None
* Return         : LCD Register Value.
* Attention		 : None
*******************************************************************************/
__inline uint16_t LCD_ReadReg(uint16_t LCD_Reg)
{
//	uint16_t LCD_RAM;

	/* Write 16-bit Index (then Read Reg) */
//	Clr_Cs;
	LCD_WriteIndex(LCD_Reg);
//	delay_ms(1);  /* delay 5 ms */

	/* Read 16-bit Reg */
//	LCD_RAM = LCD_ReadData();
//	Set_Cs;
//	return LCD_RAM;
	return LCD_ReadData();
}


#endif 

#ifdef __cplusplus  /* Insert end of extern C construct. */
}                   /* The C header file can now be */
#endif              /* included in either C or C++ code. */

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

