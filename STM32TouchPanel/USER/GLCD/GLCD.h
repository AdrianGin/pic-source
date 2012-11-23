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

/* Private define ------------------------------------------------------------*/
#define  ILI9320    0  /* 0x9320 */
#define  ILI9325    1  /* 0x9325 */
#define  ILI9328    2  /* 0x9328 */
#define  ILI9331    3  /* 0x9331 */
#define  SSD1298    4  /* 0x8999 */
#define  SSD1289    5  /* 0x8989 */
#define  ST7781     6  /* 0x7783 */
#define  LGDP4531   7  /* 0x4531 */
#define  SPFD5408B  8  /* 0x5408 */
#define  R61505U    9  /* 0x1505 0x0505 */
#define  HX8347D    10 /* 0x0047 */
#define  HX8347A    11 /* 0x0047 */
#define  LGDP4535   12 /* 0x4535 */
#define  SSD2119    13 /* 3.5 LCD 0x9919 */

extern uint8_t LCD_Code;

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
#define WHITE          0xFFFF
#define BLACK          0x0000
#define GREY           0xF7DE
#define BLUE           0x001F
#define BLUE2          0x051F
#define RED            0xF800
#define MAGENTA        0xF81F
#define GREEN          0x07E0
#define CYAN           0x7FFF
#define YELLOW         0xFFE0

#define RGB565CONVERT(red, green, blue) (int) (((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3))

/* Private function prototypes -----------------------------------------------*/
void LCD_Initializtion(void);
void LCD_Clear(uint16_t Color);	
//void LCD_SetBacklight(uint8_t intensity);
uint16_t LCD_GetPoint(uint16_t Xpos,uint16_t Ypos);
__inline void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point);
__inline void LCD_SetCursor( uint16_t Xpos, uint16_t Ypos );
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


__inline void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point)
{
	if( Xpos >= MAX_X || Ypos >= MAX_Y )
	{
		return;
	}
	LCD_SetCursor(Xpos,Ypos);
	LCD_WriteReg(0x0022,point);
}

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



__inline void LCD_SetCursor( uint16_t Xpos, uint16_t Ypos )
{

	  #if  ( DISP_ORIENTATION == 90 ) || ( DISP_ORIENTATION == 270 )

		uint16_t temp;
		Ypos = ( MAX_Y - 1 ) - Ypos;
		temp = Ypos;
		Ypos = Xpos;
		Xpos = temp;

	#elif  ( DISP_ORIENTATION == 0 ) || ( DISP_ORIENTATION == 180 )

		Ypos = ( MAX_Y - 1 ) - Ypos;

	#endif
  switch( LCD_Code )
  {
     default:		 /* 0x9320 0x9325 0x9328 0x9331 0x5408 0x1505 0x0505 0x7783 0x4531 0x4535 */
          LCD_WriteReg(0x0020, Xpos );
          LCD_WriteReg(0x0021, Ypos );
	      break;

     case SSD1298: 	 /* 0x8999 */
     case SSD1289:   /* 0x8989 */
	      LCD_WriteReg(0x004e, Xpos );
	      LCD_WriteReg(0x004f, Ypos );
	      break;

     case HX8347A: 	 /* 0x0047 */
     case HX8347D: 	 /* 0x0047 */
	      LCD_WriteReg(0x02, Xpos>>8 );
	      LCD_WriteReg(0x03, Xpos );

	      LCD_WriteReg(0x06, Ypos>>8 );
	      LCD_WriteReg(0x07, Ypos );

	      break;
     case SSD2119:	 /* 3.5 LCD 0x9919 */
	      break;
  }
}

//void LCD_SetCursor( uint16_t Xpos, uint16_t Ypos );
//void delay_ms(uint16_t ms);

#endif 

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/


#ifdef __cplusplus  /* Insert end of extern C construct. */
}                   /* The C header file can now be */
#endif              /* included in either C or C++ code. */

