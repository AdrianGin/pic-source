/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               uctsk_MicrochipGUI.c
** Descriptions:            The MicrochipGUI application function
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
#include <includes.h> 
#include <menu.h>
#include "app_cfg.h"
#include "intertaskComm.h"

/* Private variables ---------------------------------------------------------*/
GOL_MSG msg;       /* GOL message structure to interact with GOL */
//static  OS_STK  AppMicrochipGUITaskStk[APP_TASK_MICROCHIP_GUI_STK_SIZE];


/* Private function prototypes -----------------------------------------------*/
void uctsk_MicrochipGUI(void * pvArg);


/*******************************************************************************
* Function Name  : App_MicrochipGUITaskCreate
* Description    : ����GUI����
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void  App_MicrochipGUITaskCreate (void)
{
	xTaskCreate( uctsk_MicrochipGUI , ( signed char * ) "MicrochipGUI" , APP_TASK_MICROCHIP_GUI_STK_SIZE , NULL , APP_TASK_MICROCHIP_GUI_PRIO , NULL );
}

void uctsk_MicrochipGUI(void * pvArg)
{

	GOLInit(); 	
	SemaphoreTake(Sem_SystemRdy, portMAX_DELAY);
	StartMenu();

	for(;;)
   	{	
		if ( GOLDraw() )				/* Draw GOL object */
      	{				               
         	GOLMsg(&msg);        		/* Process message */
		}  
		vTaskDelay(50/portTICK_RATE_MS);
    }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

