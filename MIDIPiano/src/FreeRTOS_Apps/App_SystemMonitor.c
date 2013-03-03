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

#include "ADC/ADC.h"

#include "MIDICodes/MIDICodes.h"
#include "MIDIPlayback/midiplayback.h"

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

	uint16_t AD_value_1;
	uint16_t AD_value_2;

    MIDI_EVENT_t event;

	for (;;)
	{
		vTaskDelay(1000);

//		AD_value_1 = ADC_GetSample(0);
//		xprintf("The current AD1 value = %d \r\n", AD_value_1);

//        event.event.eventType = MIDI_CONTROL_CHANGE|0;
//        event.event.chanEvent.parameter1 = CHANNEL_VOLUME;
//        event.event.chanEvent.parameter2 = AD_value_1 >> 5;
        //MPB_PlayEvent(&event, MPB_PB_ALL_ON);


		//vTaskList(buffer);
		//xprintf("%s\n", buffer);
		//display_mallinfo();
		vTaskDelay(1000);
		vTaskDelay(1000);
	}
}

/*********************************************************************************************************
 END FILE
 *********************************************************************************************************/
