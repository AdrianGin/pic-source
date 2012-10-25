/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The FreeRTOS application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-8
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
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "hw_config.h"

/* Private define ------------------------------------------------------------*/
#define LED_TASK_STACK_SIZE			( configMINIMAL_STACK_SIZE )
#define LED_TASK_PRIORITY			( tskIDLE_PRIORITY + 3 )

#define LED_TASK_STACK_SIZE2			( configMINIMAL_STACK_SIZE )
#define LED_TASK_PRIORITY2			( tskIDLE_PRIORITY + 2 )


xQueueHandle xQueue;
 

/* Private function prototypes -----------------------------------------------*/
static void prvSetupHardware( void );
void vLEDTask(void * pvArg);
void vLEDTask2(void * pvArg);
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
  prvSetupHardware();
	
	xQueue = xQueueCreate(1, 1);	
	
  xTaskCreate( vLEDTask , ( signed char * ) NULL , LED_TASK_STACK_SIZE , NULL , LED_TASK_PRIORITY , NULL );
	xTaskCreate( vLEDTask2 , ( signed char * ) NULL , LED_TASK_STACK_SIZE2 , NULL , LED_TASK_PRIORITY2 , NULL );
  /* Start the scheduler. */
  vTaskStartScheduler();	

  return 0;
}

/*******************************************************************************
* Function Name  : vLEDTask
* Description    : LED Task
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void vLEDTask(void * pvArg)
{
	uint8_t pxRxedMessage;
	pxRxedMessage = 1;
  GPIO_Configuration();  
  while(1)
  {
    /*====LED-ON=======*/
				GPIO_SetBits(GPIOB , GPIO_Pin_12);
		xQueueReceive( xQueue, &pxRxedMessage,  0);
		
				vTaskDelay(1000);	
			/*====LED-OFF=======*/  
			GPIO_ResetBits(GPIOB , GPIO_Pin_12);  
		xQueueSendToBack( xQueue, &pxRxedMessage,  0);	
				vTaskDelay(1000);	
						
	}
}


void vLEDTask2(void * pvArg)
{
  GPIO_Configuration();  
  while(1)
  {
		 if( uxQueueMessagesWaiting( xQueue ) != 0 )
        {
    /*====LED-ON=======*/
    GPIO_SetBits(GPIOB , GPIO_Pin_14);
    vTaskDelay(100);	
	/*====LED-OFF=======*/  
	GPIO_ResetBits(GPIOB , GPIO_Pin_14);  
    vTaskDelay(100);					
				}
  }
}

/*******************************************************************************
* Function Name  : prvSetupHardware
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void prvSetupHardware( void )
{
  //SystemInit();
  /* Configure HCLK clock as SysTick clock source. */
  SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );
  GPIO_Configuration();
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
