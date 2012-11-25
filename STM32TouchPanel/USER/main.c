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
#include "Common/InertiaTouch.h"

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

	char* LBItem;

	char* fnPath;

	Coordinate* point;
	FT_STATES state;
	Coordinate* inertia;

	DIR dir;

    RCC_ClocksTypeDef RCC_ClocksStatus;




	char path[255];

	USART_Configuration();
	GPIO_Configuration();
	NVIC_Configuration();

	delay_init();
	delay_ms(10);

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
			  FluidTouchMain();
			  counters[2] = 0;
		  }


		  if( counters[3] >= 200 )
		  {
			  LCD_VSyncLow();
			  delay_us(50);
			  LCD_VSyncHigh();
			  delay_ms(16);
			  counters[3] = 0;
		  }

		  if( counters[0] >= 200 )
		  {
			  point = FT_GetLastPoint();
			  if( GFX_LB_ProcessTouchInputs(&GFX_LB) == LB_REQUIRES_REDRAW)
			  {
				  SetTouchPoint(point->x, point->y);

				  LBItem = (char*)GFX_LB_ReturnSelectedItemPtr(&GFX_LB);

				  if( LBItem )
				  {
					  printf("SELECTED:: %s\n", LBItem);
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
