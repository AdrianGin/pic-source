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
#include <stdint.h>

/* Private define ------------------------------------------------------------*/
#define USE_16BIT_PMP

#define LCD_REG              (*((volatile unsigned short *) 0x60000000)) /* RS = 0 */
#define LCD_RAM              (*((volatile unsigned short *) 0x60020000)) /* RS = 1 */



/*********************************************************************
* Overview: Horizontal and vertical display resolution
*                  (from the glass datasheet).
*********************************************************************/
//#define DISP_HOR_RESOLUTION				320
//#define DISP_VER_RESOLUTION				240

//#define DISP_ORIENTATION					0
//#define DISP_ORIENTATION					90
//#define DISP_ORIENTATION					180
#define DISP_ORIENTATION					270

/* Private define ------------------------------------------------------------*/

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
//void LCD_SetBacklight(uint8_t intensity);
uint16_t LCD_GetPoint(uint16_t Xpos,uint16_t Ypos);
void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point);
void PutChar(uint16_t Xpos,uint16_t Ypos,uint8_t c,uint16_t charColor,uint16_t bkColor);
void LCD_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color );
void PutChinese(uint16_t Xpos,uint16_t Ypos,uint8_t *str,uint16_t Color,uint16_t bkColor);
void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor);
void GUI_Chinese(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor);
void LCD_DrawPicture(uint16_t StartX,uint16_t StartY,uint16_t EndX,uint16_t EndY,uint16_t *pic);


void LCD_PauseUpdateScreen(void);
void LCD_ResumeUpdateScreen(void);
void LCD_UpdateScreen(void);
void LCD_VSyncHigh(void);
void LCD_VSyncLow(void);

__inline void LCD_WriteIndex(uint16_t index);
__inline void LCD_WriteData(uint16_t data);
__inline uint16_t LCD_ReadData(void);
__inline void LCD_WriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue);
__inline uint16_t LCD_ReadReg(uint16_t LCD_Reg);



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

void LCD_SetCursor( uint16_t Xpos, uint16_t Ypos );
//void delay_ms(uint16_t ms);

#endif 

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/


#ifdef __cplusplus  /* Insert end of extern C construct. */
}                   /* The C header file can now be */
#endif              /* included in either C or C++ code. */

