/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
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
#include "stm32f10x.h"
#include "TouchPanel/TouchPanel.h"
#include "systick.h"
#include "GLCD/GLCD.h"

#include "USART/usart.h"

#include "hw_config.h"
#include "FatFS\ff.h"
#include "Graphics\gfxEngine.h"
#include "FSUtils\FSUtil.h"

#include "InertiaTouch/InertiaTouch.h"
#include "FluidTouch/FluidTouch.h"

#include "UserGUI.h"
#include "LPD8806\LPD8806.h"

#include "Graphics/BMPDraw.h"
#include "LEDArrayBMP/DA_LEDDisplay.h"

#include "LEDWall/LEDWall.h"

#include "printf/printf.h"

#include <stdlib.h>
#include <string.h>

FATFS fs;
volatile uint8_t globalFlag;

void FlashLEDs(uint16_t abit)
{
	LPD8806_Test();
	LPD8806_Update();
	delay_ms(abit);
	LPD8806_Clear();
	LPD8806_Update();
	delay_ms(abit);
}

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
int main(void)
{

	int16_t i, j, k;
	int16_t counters[10];
	uint8_t ret;
	uint8_t alternate;
	int8_t index = 0;
	char* LBItem;
	char* fnPath;
	Coordinate* point;
	FT_STATES state;
	Coordinate* inertia;
	DIR dir;
    RCC_ClocksTypeDef RCC_ClocksStatus;
	char path[255];
	uint16_t tickCounter = 0;
	static uint8_t Togstate = 0;


	USART_Configuration();
	GPIO_Configuration();
	NVIC_Configuration();


	LPD8806_Init();

	TIM_MIDI_Configuration();
	AUX_TIM_Configuration();

	GPIO_SetBits(GPIOC, GPIO_Pin_13);

	delay_init();
	delay_ms(10);

    LCD_Initializtion();
  //LCD_BackLight_Init();
	LCD_Clear(WHITE);
	//delay_init();
	//delay_ms(5000);

	gfxEngine_Init();

	ret = f_mount(0, &fs);
	xprintf("Mount %d\n", ret);

	strcpy(path, "");
	//scan_files(path);

  TP_Init();
  TouchPanel_Calibrate();
  /* Infinite loop */

  //PhysicsInit();
  FluidTouchInit();


  UserGUI_Init();


	LPD8806_SetPixel(0, RGB(0,255,255) );
	LPD8806_SetPixel(LED_COUNT-1, RGB(0,255,0) );
	LPD8806_SetPixel(LED_COUNT / 2, RGB(255,0,255) );

	//LPD8806_Test();
	LPD8806_Update();


	//FSUtil_OpenDir(&dir, "/MIDI");
	xprintf("OpenDir=%d\n", FSUtil_OpenDir(&dir, "/"));
	LCD_PauseUpdateScreen();

	while( 1 )
	{
		fnPath = FSUtil_GetDirObj(&dir);
		if( fnPath != NULL )
		{
			xprintf("Path:%s\n", fnPath);
			GFX_LB_AddItem(&GFX_LB, fnPath);
			//free(fnPath);
		}
		else
		{
			break;
		}
	}

  while (1)	
  {
	  //LCD_Clear(Black);

	  //for(i = 0; i < 240; i++ )
	  {

		  if( counters[2] >= 5 )
		  {
			  FluidTouchMain();
			  counters[2] = 0;

			  //LPD8806_Update();
		  }




		  if( globalFlag & 0x02 )
		  {
			  LCD_VSyncLow();
			  GPIO_ResetBits(GPIOC, GPIO_Pin_13);
			  globalFlag &= ~0x02;
		  }

		  if( globalFlag & 0x04 )
		  {
			  LCD_VSyncHigh();
			  GPIO_SetBits(GPIOC, GPIO_Pin_13);
			  globalFlag &= ~0x04;
		  }

//		  if( counters[3] >= 200 )
//		  {
//			  LCD_VSyncLow();
//			  GPIO_ResetBits(GPIOC, GPIO_Pin_13);
//			  delay_us(50);
//			  LCD_VSyncHigh();
//			  GPIO_SetBits(GPIOC, GPIO_Pin_13);
//			  delay_ms(16);
//			  counters[3] = 0;
//		  }

		  //if( counters[0] >= 200 )
		  if( globalFlag & 0x08 )
		  {

				if( !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) )	   /* JOY_RIGHT is press */
				{
					DALED_BlurMode = 1;
				}
				else
				{
					DALED_BlurMode = 0;
				}




			  globalFlag &= ~0x08;
			  point = FT_GetLastPoint();
			  if( GFX_LB_ProcessTouchInputs(&GFX_LB) == LB_REQUIRES_REDRAW)
			  {
				  //SetTouchPoint(point->x, point->y);

				  LBItem = (char*)GFX_LB_ReturnSelectedItemPtr(&GFX_LB);

				  if( LBItem )
				  {

						strcat(path, "/");
						strcat(path, LBItem);

						BMP_SetCursor(200,120);
						BMP_SetRotation(-1,1, 90);
						gfxDrawBMP(path);
						LCD_ResumeUpdateScreen();

						//while(1)
						{

							FlashLEDs(1000);
							FlashLEDs(1000);
							FlashLEDs(500);
							FlashLEDs(500);
							FlashLEDs(500);
							FlashLEDs(500);
							FlashLEDs(250);
							FlashLEDs(250);
							FlashLEDs(250);
							FlashLEDs(250);

							//LPD8806_Update();
							//delay_ms(1000);

							BMP_SetCursor(0,LED_COUNT / 2);

							gfxDrawBMPonLED(path);

							LPD8806_Clear();
							LPD8806_Update();
							delay_ms(1000);
						}


						memset(path, 0, sizeof(path));
				  }

				  i = point->x;
				  k = point->y;

				  LCD_Clear(WHITE);

				  SetClip(1);
				  GFX_LB_Draw(&GFX_LB);
				  SetClip(0);
				  setPixel(point->x,point->y);
				  gfxWriteString(point->x, point->y, "Hi");
			  }

			  counters[0] = 0;
		  }

		  if( globalFlag & 0x10 )
		  {
			  //Show1();
			  //LPD8806_Update();
			  globalFlag &= ~0x10;
		  }

		  counters[0]++;
		  counters[1]++;
		  counters[2]++;
		  counters[3]++;
	  }




  }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
