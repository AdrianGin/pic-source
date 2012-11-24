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
#include "TouchPanel.h"
#include "systick.h"
#include "GLCD.h"

#include "USART/usart.h"
#include "Physics/physics.h"

#include "hw_config.h"
#include "ff.h"
#include "Graphics\gfxEngine.h"
#include "FSUtils\FSUtil.h"
#include "TouchPanel\FluidTouch.h"

#include "UserGUI.h"

#include <stdlib.h>
#include <string.h>

FATFS fs;

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

	char* fnPath;

	Coordinate* point;
	Coordinate* inertia;

	DIR dir;

    RCC_ClocksTypeDef RCC_ClocksStatus;




	char path[255];

	USART_Configuration();
	GPIO_Configuration();
	NVIC_Configuration();

	delay_init();
	delay_ms(10);

	printf("STARTING\n");

    LCD_Initializtion();
  //LCD_BackLight_Init();
	LCD_Clear(WHITE);
	//delay_init();
	//delay_ms(5000);

	gfxEngine_Init();

	ret = f_mount(0, &fs);
	printf("Mount %d\n", ret);

	strcpy(path, "");
	//scan_files(path);

  TP_Init();
  TouchPanel_Calibrate();
  /* Infinite loop */

  PhysicsInit();
  FluidTouchInit();

  //while(1);

  LCD_PauseUpdateScreen();

  UserGUI_Init();




	FSUtil_OpenDir(&dir, "/MIDI");

	while( 1 )
	{
		fnPath = FSUtil_GetDirObj(&dir);
		if( fnPath != NULL )
		{
			printf("Path:%s\n", fnPath);
			GFX_LB_AddItem(&GFX_LB, fnPath);
			//free(fnPath);
		}
		else
		{
			break;
		}
	}

//  GFX_LB_AddItem(&GFX_LB, "Hey Leo");
//  GFX_LB_AddItem(&GFX_LB, "What");
//  GFX_LB_AddItem(&GFX_LB, "is up?");
//  GFX_LB_AddItem(&GFX_LB, "Serena");
//  GFX_LB_AddItem(&GFX_LB, "Pang");
//  GFX_LB_AddItem(&GFX_LB, "is a HOT");
//  GFX_LB_AddItem(&GFX_LB, "goddess!");
//  GFX_LB_AddItem(&GFX_LB, "One");
//  GFX_LB_AddItem(&GFX_LB, "Day");
//  GFX_LB_AddItem(&GFX_LB, "I will");
//  GFX_LB_AddItem(&GFX_LB, "marry her!!");
	
  while (1)	
  {
	  //LCD_Clear(Black);

	  //for(i = 0; i < 240; i++ )
	  {

		  if( counters[2] >= 5 )
		  {
			  //FluidTouchMain();
			  FluidTouchMain2();
			  counters[2] = 0;
		  }

		  if( counters[1] >= 100 )
		  {
			  FluidTouch_ApplySlowdown();
			  counters[1] = 0;
		  }

		  if( counters[3] >= 200 )
		  {
			  LCD_VSyncLow();
			  delay_us(50);
			  LCD_VSyncHigh();
			  delay_ms(16);
			  counters[3] = 0;
		  }
		  counters[3]++;
//
//		  if( counters[4] >= 200 )
//		  {
//			  LCD_VSyncHigh();
//			  counters[4] = 0;
//		  }
//		  counters[4]++;



		  if( counters[0] >= 200 )
		  {


			  //FluidTouchMain();

			  //point = Read_Ads7846();
			  //point = FluidTouchGetPoint();
			  point = FluidGetTouch();
			  if( point != 0)
			  {

				  //TP_BudgetGetDisplayPoint(&TouchPanel, point);
				  //getDisplayPoint(&display, point, &matrix ) ;
				  SetTouchPoint(point->x, point->y);

				  i = point->x;
				  k = point->y;
				  //LCD_VSyncLow();

				  if(alternate)
				  {
					  LCD_Clear(WHITE);
					  BMP_SetCursor(i, k);
//					  gfxDrawBMP("folder13.bmp");

					  inertia = FluidTouch_GetIntertia();

					  //printf("Inert:X=%d, Y=%d\n", inertia->x, inertia->y);
					  if( inertia != NULL )
					  {
						  GFX_LB_Scroll(&GFX_LB, inertia->y);
					  }
					  SetClip(1);
					  //SetClipRgn(0, 100 ,320 ,200);



					  index = GFX_LB_GetSelectedItem(&GFX_LB, point->y);
					  printf("INDEX=%d\n", index);
					  GFX_LB_SelectItem(&GFX_LB, index);
					  if( index > LL_Count(&GFX_LB.list) )
					  {
						  index = 0;
					  }

					  GFX_LB_Draw(&GFX_LB);

					  SetClip(0);
					  //gfxWriteString(i, k, "Fuck this world!");
					  //alternate = 0;

				  }
				  else
				  {
					  //LCD_Clear(Yellow);
					  alternate = 1;
				  }

				  BMP_SetRotation(1, -1);
				  BMP_SetCursor(0, 239);

				  gfxDrawBMP("blueto~1.bmp");

				  BMP_SetCursor(100, 239);
				  //gfxDrawBMP("phone.bmp");

				  BMP_SetCursor(200, 239);
				  gfxDrawBMP("sms.bmp");


				  BMP_SetRotation(1, -1);
				  BMP_SetCursor(0, 20);
				  gfxDrawBMP("folder13.bmp");

				  BMP_SetCursor(0, 50);
				  gfxDrawBMP("folder13.bmp");

				  BMP_SetCursor(0, 80);
				  gfxDrawBMP("folder13.bmp");

				  BMP_SetCursor(0, 110);
				  gfxDrawBMP("folder13.bmp");

				  setPixel(point->x,point->y);
				  //DrawCross(point->x,point->y);

				  gfxWriteString(point->x, point->y, "Hi");
				   //LCD_VSyncHigh();
				   //delay_ms(1);
				  //PhysicsMain();
			  }
//			  else
//			  {
//				  LCD_VSyncLow();
//				  delay_ms(50);
//				  LCD_VSyncHigh();
//				  delay_ms(50);
//			  }


			  counters[0] = 0;
			  //counters[1] = 0;
			  //LCD_VSyncHigh();
		  }

//		  if( counters[1] >= 100)
//		  {
//			  LCD_VSyncLow();
//			  //delay_ms(2);
//			  //PhysicsMain(DESIRED_FPS);
//			  LCD_VSyncHigh();
//			  counters[1] = 0;
//			  //delay_ms(1);
//		  }

		  //delay_us(100);
		  counters[0]++;
		  counters[1]++;
		  counters[2]++;

		  //LCD_Clear(Yellow);
		  ///LCD_UpdateScreen();
		  //PhysicsTick();
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
