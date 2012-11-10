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
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/
#define  OS_VIEW_MODULE                  DEF_DISABLED	     	/* DEF_ENABLED = Present, DEF_DISABLED = Not Present        */


/*
*********************************************************************************************************
*                                              TASKS NAMES
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/ 

#include "FreeRTOS.h"
#include "task.h"


#define  APP_TASK_START_PRIO                               tskIDLE_PRIORITY+4
#define  APP_TASK_MP3DECODE_PRIO                           tskIDLE_PRIORITY+4
#define APP_TASK_LED_PRIO									( tskIDLE_PRIORITY + 3 )
#define APP_TASK_LED2_PRIO									( tskIDLE_PRIORITY + 0 )
#define  APP_TASK_MICROCHIP_GUI_PRIO					   tskIDLE_PRIORITY+2
#define  APP_TASK_TOUCHSCREEN_PRIO                         tskIDLE_PRIORITY+3
#define  APP_TASK_READBUTTON_PRIO                         tskIDLE_PRIORITY+5

/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/

#define  APP_TASK_START_STK_SIZE                         256u
#define  APP_TASK_MP3DECODE_STK_SIZE                   	 768u
#define  APP_TASK_MICROCHIP_GUI_STK_SIZE                 384u
#define  APP_TASK_TOUCHSCREEN_STK_SIZE					 256u
#define  APP_TASK_READBUTTON_STK_SIZE                    128u
#define APP_TASK_LED_STK_SIZE			( configMINIMAL_STACK_SIZE )
#define APP_TASK_LED2_STK_SIZE			( 512u )

#define APP_TASK_COUNT	(10)

#define  APP_TASK_OSVIEW_TERMINAL_STK_SIZE   			 128u
#define  OS_VIEW_TASK_STK_SIZE                           128u






/*
*********************************************************************************************************
*                                                  LIB
*********************************************************************************************************
*/

#define  uC_CFG_OPTIMIZE_ASM_EN                 DEF_ENABLED
#define  LIB_STR_CFG_FP_EN                      DEF_DISABLED //DEF_DISABLED



#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

