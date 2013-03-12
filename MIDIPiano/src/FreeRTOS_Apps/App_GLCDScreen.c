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
#include "Graphics/LCDAbstraction.h"
#include "Graphics/primitiveGfx.h"
#include "Graphics/gfxEngine.h"

#include "Coordinate/Coordinate.h"

#include "LightSys/LightSys.h"
#include "stm32f10x.h"
#include "Graphics/gfxListbox.h"

#include "FluidTouch/FluidTouch.h"

#include "UserGUI.h"

#include "printf/printf.h"

#include "MIDIPlaybackControlLogic/MIDIPlaybackControlLogic.h"

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Task_GLCDScreen(void * pvArg);

/*******************************************************************************
 * Function Name  : void Task_GLCDScreen(void * pvArg)
 * Description    :
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention		 : None
 *******************************************************************************/
void App_GLCDScreenTaskCreate(void)
{
	xTaskCreate( Task_GLCDScreen, ( signed char * ) "GLCD",
			APP_TASK_GLCDSCREEN_STK_SIZE, NULL, APP_TASK_GLCDSCREEN_PRIO, NULL);
}




void Task_GLCDScreen(void * pvArg)
{

	uint8_t actionDelay;
	Coordinate* point;
	char* LBItem = NULL;
	char* fnPath;

	FT_STATES state;

	RedrawRequired = 1;

	actionDelay = 0;

	for (;;)
	{
		LCD_VSyncLow();
		GPIO_ResetBits(GPIOC, GPIO_Pin_13 );
		vTaskDelay(1);

		LCD_VSyncHigh();
		//LS_ProcessAutoTurnOff();
		//LS_ProcessAutoTurnOff();
		//MLL_ProcessPulsateHaltList();

		GPIO_SetBits(GPIOC, GPIO_Pin_13 );
		vTaskDelay(32);

		state = FT_GetTouchState();
		point = FT_GetLastPoint();

		//TODO: Fast Forward and Reverse's don't remove halts and lights
		if( (FT_GetTouchState() != TOUCH_OFF) || (RedrawRequired == 1)  )
		{
			RedrawRequired = gfxFrame_ProcessInputs(&GFX_MainFrame, state, point);
		}


		//Clear the touch state.
		FT_ClearTouchState();

	}
}

/*********************************************************************************************************
 END FILE
 *********************************************************************************************************/
