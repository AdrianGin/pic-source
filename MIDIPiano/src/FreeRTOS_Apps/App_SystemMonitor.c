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

#include "FreeRTOS.h"
#include "task.h"

#include "stm32f10x.h"

#include "hw_config.h"

#include "UserGUI.h"

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Task_SystemMonitor(void * pvArg);

/*******************************************************************************
 * Function Name  : void Task_GLCDScreen(void * pvArg)
 * Description    :
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention		 : None
 *******************************************************************************/
void App_SystemMonitorTaskCreate(void)
{
	xTaskCreate( Task_SystemMonitor, ( signed char * ) "SysMon",
			APP_TASK_SYSTEMMONITOR_STK_SIZE, NULL, APP_TASK_SYSTEMMONITOR_PRIO,
			NULL);
}




void Task_SystemMonitor(void * pvArg)
{
	uint8_t buffer[APP_TASK_COUNT*40];
	size_t heapSize;

	for (;;)
	{
		vTaskDelay(1000);
		//vTaskList(buffer);
		//xprintf("%s\n", buffer);
		display_mallinfo();
		vTaskDelay(1000);
		vTaskDelay(1000);
	}
}

/*********************************************************************************************************
 END FILE
 *********************************************************************************************************/
