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

#include "MIDIPlayback/midiplayback.h"
#include "LightSys\LightSys.h"
#include "LPD8806\LPD8806.h"

#include "MIDILightLogic/MIDILightLogic.h"

#include "LightSys/LightSys.h"
#include "stm32f10x.h"

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Task_LightSystem(void * pvArg);

/*******************************************************************************
 * Function Name  : void Task_GLCDScreen(void * pvArg)
 * Description    :
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention		 : None
 *******************************************************************************/
void App_LightSystemTaskCreate(void)
{
	xTaskCreate( Task_LightSystem, ( signed char * ) "LiSys",
			APP_TASK_LIGHTSYS_STK_SIZE, NULL, APP_TASK_LIGHTSYS_PRIO,
			&LightSystemHandle);
}

void Task_LightSystem(void * pvArg)
{
	LPD8806_Update();
	for (;;)
	{
		LS_ProcessAutoTurnOff();
		MLL_ProcessPulsateHaltList();

		vTaskDelay(10);

		//WAIT_FOR_LIGHTSYS_UPDATE();
		//LPD8806_Update();
	}
}

/*********************************************************************************************************
 END FILE
 *********************************************************************************************************/
