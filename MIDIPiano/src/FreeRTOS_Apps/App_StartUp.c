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

#include "FSUtils/FSUtil.h"

#include "Graphics\gfxEngine.h"
#include "UserGUI.h"

#include "stm32f10x.h"

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Task_StartUp(void * pvArg);

/*******************************************************************************
 * Function Name  : void Task_GLCDScreen(void * pvArg)
 * Description    :
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention		 : None
 *******************************************************************************/
void App_StartUpTaskCreate(void)
{
	xTaskCreate( Task_StartUp, ( signed char * ) "StartUp",
			APP_TASK_STARTUP_STK_SIZE, NULL, APP_TASK_STARTUP_PRIO,
			NULL);
}

void Task_StartUp(void * pvArg)
{


	App_LightSystemTaskCreate();
	App_MIDIPlaybackTaskCreate();

	App_GLCDScreenTaskCreate();

	App_TouchScreenTaskCreate();
	App_ProcessInputsTaskCreate();

	App_SystemMonitorTaskCreate();

	App_DummyTaskCreate();




	for (;;)
	{
		vTaskDelay(1000);
		//vTaskDelete(NULL);
	}
}

/*********************************************************************************************************
 END FILE
 *********************************************************************************************************/
