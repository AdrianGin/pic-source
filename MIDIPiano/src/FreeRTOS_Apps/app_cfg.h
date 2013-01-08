/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               app_cfg.h
** Descriptions:            ucosii configuration
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-9
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

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__
					  


/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/ 

#include "FreeRTOS.h"
#include "task.h"

#include "MIDIPlayback/midiplayback.h"

#define  APP_TASK_STARTUP_PRIO                         			tskIDLE_PRIORITY+5

#define  APP_TASK_LIGHTSYS_PRIO                           		tskIDLE_PRIORITY+5
#define  APP_TASK_MIDIPLAYBACK_PRIO                           	tskIDLE_PRIORITY+4


#define  APP_TASK_GLCDSCREEN_PRIO					   			tskIDLE_PRIORITY+2
#define  APP_TASK_TOUCHSCREEN_PRIO                         		tskIDLE_PRIORITY+2

#define  APP_TASK_PROCESSINPUTS_PRIO                         	tskIDLE_PRIORITY+2

#define  APP_TASK_SYSTEMMONITOR_PRIO                         	tskIDLE_PRIORITY+1

/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/
#define  APP_TASK_STARTUP_STK_SIZE							128u

#define  APP_TASK_LIGHTSYS_STK_SIZE                        	128u
#define  APP_TASK_MIDIPLAYBACK_STK_SIZE                   	256u
#define  APP_TASK_GLCDSCREEN_STK_SIZE                 		512u
#define  APP_TASK_TOUCHSCREEN_STK_SIZE					 	128u
#define  APP_TASK_PROCESSINPUTS_STK_SIZE                    128u
#define  APP_TASK_SYSTEMMONITOR_STK_SIZE                    256u


#define APP_TASK_COUNT	(10)

//Application list
void App_TouchScreenTaskCreate   		(void);
void App_GLCDScreenTaskCreate (void);
void App_MIDIPlaybackTaskCreate(void);
void App_ProcessInputsTaskCreate(void);
void App_LightSystemTaskCreate(void);
void App_SystemMonitorTaskCreate(void);
void App_StartUpTaskCreate(void);
void App_DummyTaskCreate(void);


extern MIDI_HEADER_CHUNK_t MIDIHdr;

#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

