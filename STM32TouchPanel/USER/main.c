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

	uint16_t i, j, k;
	uint8_t ret;
	uint8_t alternate;
	Coordinate* point;

	USART_Configuration();
	//GPIO_Configuration();
	NVIC_Configuration();

	delay_init();
  LCD_Initializtion();
  //LCD_BackLight_Init();
	LCD_Clear(White);
	//delay_init();
	//delay_ms(5000);

	gfxEngine_Init();

	//ret = f_mount(0, &fs);
	printf("Mount %d\n", ret);


  TP_Init();
  TouchPanel_Calibrate();
  /* Infinite loop */

  PhysicsInit();

  //while(1);

  LCD_PauseUpdateScreen();

  while (1)	
  {
	  //LCD_Clear(Black);

	  //for(i = 0; i < 240; i++ )
	  {

		  if( j >= 1 )
		  {
			  point = Read_Ads7846();
			  if( point != 0)
			  {
				  getDisplayPoint(&display, point, &matrix ) ;
				  SetTouchPoint(display.x, display.y);
				  i = display.x;
				  k = display.y;
				  //LCD_VSyncLow();

				  if(alternate)
				  {
					  //gfxDrawBMP();
					  //LCD_Clear(Yellow);
					  //gfxWriteString(i, k, "Fuck this world!");
					  //alternate = 0;
				  }
				  else
				  {
					  //LCD_Clear(Yellow);
					  alternate = 1;
				  }
				   //LCD_VSyncHigh();
				  //PhysicsMain();
			  }




			  //delay_ms(1);
			  //LCD_VSyncLow();
			  if( i > 320 )
			  {
				  i = 0;
			  }
			  if( k > 240 )
			  {
				  k = 0;
			  }

			  i++;
			  k++;
			  {
				  //LCD_Clear(Yellow);

			  }


			  j = 0;
			  //LCD_VSyncHigh();
		  }
		  j++;

		  //LCD_VSyncLow();
		  //delay_ms(2);
		  PhysicsMain(DESIRED_FPS);
		  //LCD_VSyncHigh();
		  //delay_ms(1);
		  delay_ms(TIME_BASE / DESIRED_FPS);
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
