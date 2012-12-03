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
#ifdef __cplusplus    /* Insert start of extern C construct */
extern "C" {
#endif


#ifndef _TOUCHPANEL_H_
#define _TOUCHPANEL_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "Common\Coordinate.h"

typedef struct Matrix 
{						
long double An,  
            Bn,     
            Cn,   
            Dn,    
            En,    
            Fn,     
            Divider ;
} Matrix ;

/* Private variables ---------------------------------------------------------*/
extern Coordinate ScreenSample[3];
extern Coordinate DisplaySample[3];
extern Matrix matrix ;
extern Coordinate  TouchPanel;

/* Private define ------------------------------------------------------------*/
/* AD通道选择命令字和工作寄存器 */
#define	CHX 	0x90 	/* 通道Y+的选择控制字 */	
#define	CHY 	0xd0	/* 通道X+的选择控制字 */





//Must choose either TP_USE_SPI1 or SPI2
//#define TP_USE_SPI1
#define TP_USE_SPI2

#ifdef TP_USE_SPI1

#define SPI_GPIO_PORT 	GPIOA
#define SPI_PINS		GPIO_Pin_5  | GPIO_Pin_6 | GPIO_Pin_7

#define TP_CS_GPIO		GPIOA
#define TP_CS_PIN		GPIO_Pin_4

#define TP_IRQ_GPIO		GPIOB
#define TP_IRQ_PIN		GPIO_Pin_6

#define RCC_APBxPeriphClockCmd RCC_APB2PeriphClockCmd
#define SPI_APB		RCC_APB2Periph_SPI1
#define SPI_MODULE	SPI1

#else

#define SPI_GPIO_PORT 	GPIOB
#define SPI_PINS		GPIO_Pin_15 | GPIO_Pin_13 | GPIO_Pin_14

#define TP_CS_GPIO		GPIOB
#define TP_CS_PIN		GPIO_Pin_12

#define TP_IRQ_GPIO		GPIOB
#define TP_IRQ_PIN		GPIO_Pin_0

#define RCC_APBxPeriphClockCmd RCC_APB1PeriphClockCmd
#define SPI_APB		RCC_APB1Periph_SPI2
#define SPI_MODULE	SPI2

#endif


#define TP_CS(x)	x ? GPIO_SetBits(TP_CS_GPIO,TP_CS_PIN): GPIO_ResetBits(TP_CS_GPIO,TP_CS_PIN)

#define TP_INT_IN   GPIO_ReadInputDataBit(TP_IRQ_GPIO,TP_IRQ_PIN)

/* Private function prototypes -----------------------------------------------*/				
void TP_Init(void);	
Coordinate *Read_Ads7846(void);
void TouchPanel_Calibrate(void);
void DrawCross(uint16_t Xpos,uint16_t Ypos);
void TP_DrawPoint(uint16_t Xpos,uint16_t Ypos);
FunctionalState setCalibrationMatrix( Coordinate * displayPtr,Coordinate * screenPtr,Matrix * matrixPtr);
FunctionalState getDisplayPoint(Coordinate * displayPtr,Coordinate * screenPtr,Matrix * matrixPtr );

void TP_BudgetGetDisplayPoint(Coordinate * displayPtr, Coordinate * screenPtr);

#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

#ifdef __cplusplus  /* Insert end of extern C construct. */
}                   /* The C header file can now be */
#endif              /* included in either C or C++ code. */



