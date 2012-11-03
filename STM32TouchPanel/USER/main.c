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

	uint32_t i;

	Coordinate* point;

	delay_init();
  LCD_Initializtion();
  //LCD_BackLight_Init();
	LCD_Clear(Red);
	//delay_init();
	//delay_ms(5000);
  TP_Init();
  TouchPanel_Calibrate();
  /* Infinite loop */
  while (1)	
  {




	  LCD_Clear(Black);

	  for(i = 0; i < 240; i++ )
	  {
		  point = Read_Ads7846();
		  GUI_Text(point->x,0, "HelloMate", Red, Blue);
		  delay_ms(1);
	  }

	  delay_ms(3000);


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
