/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               uctsk_TouchScreen.c
** Descriptions:            The uctsk_TouchScreen application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-26
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
//#include <includes.h>

#include "app_cfg.h"
#include "intertaskComm.h"

#include "FluidTouch/FluidTouch.h"

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Task_TouchScreen(void * pvArg);


/*******************************************************************************
* Function Name  : App_TouchScreenTaskCreate
* Description    : 创建触摸屏任务
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void  App_TouchScreenTaskCreate (void)
{
    xTaskCreate( Task_TouchScreen , ( signed char * ) "TouchScreen" , APP_TASK_TOUCHSCREEN_STK_SIZE , NULL , APP_TASK_TOUCHSCREEN_PRIO , NULL );
}


void Task_TouchScreen(void * pvArg)
{
//   TP_Init();
//
//   while( SemaphoreTake(Sem_TouchPanelRdy, 10) == pdFAIL){}
//   TouchPanel_Calibrate();
//
//   SemaphoreGive(Sem_SystemRdy);

   	for(;;)
   	{   
   		FluidTouchMain();
   		vTaskDelay(10);
	   //TouchGetMsg(&msg);   		/* Get message from touch screen */
    }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
