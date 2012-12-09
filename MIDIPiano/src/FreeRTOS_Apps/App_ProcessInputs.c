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


#include "UARTProcessor/UARTProcessor.h"
#include "hw_config.h"

#include "stm32f10x.h"

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Task_ProcessInputs(void * pvArg);

/*******************************************************************************
 * Function Name  : void Task_GLCDScreen(void * pvArg)
 * Description    :
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention		 : None
 *******************************************************************************/
void App_ProcessInputsTaskCreate(void)
{
	xTaskCreate( Task_ProcessInputs, ( signed char * ) "ProcIn",
			APP_TASK_PROCESSINPUTS_STK_SIZE, NULL, APP_TASK_PROCESSINPUTS_PRIO,
			NULL);
}

void Task_ProcessInputs(void * pvArg)
{
	for (;;)
	{
		FluidTouchMain();
		ProcessUARTBuffer();
		ProcessUSBMIDIBuffer_LightSys();

		//LPD8806_Update();

		SemaphoreGive(Sem_LightSysUpdate);
		vTaskResume(LightSystemHandle);
		vTaskDelay(1);
	}
}

/*********************************************************************************************************
 END FILE
 *********************************************************************************************************/
