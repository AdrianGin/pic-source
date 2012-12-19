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


void ExecuteMIDIFile(char* LBItem)
{
	char path[255];
	FRESULT tmp;

	TIM_ITConfig(MIDI_TIM, TIM_IT_Update, DISABLE);
	TIM_Cmd(MIDI_TIM, DISABLE);

	vTaskDelay(10);

	strcpy(path, GFX_FB_CWD());
	strcat(path, "/");
	strcat(path, LBItem);
	MPB_ResetMIDI();

	LS_ClearLightTimers();
	LS_ClearLights();

	tmp =  MPB_PlayMIDIFile(&MIDIHdr, (uint8_t*)path);

	xprintf("SELECTED:: %s, FR=%d\n", path, tmp);

	TIM_ITConfig(MIDI_TIM, TIM_IT_Update, ENABLE);
	TIM_Cmd(MIDI_TIM, ENABLE);

	if (tmp == FR_OK)
	{
		xprintf("SUCCESS!!\n");
	}

}


void Task_GLCDScreen(void * pvArg)
{

	uint8_t actionDelay;
	Coordinate* point;
	char* LBItem = NULL;
	char* fnPath;

	FT_STATES state;

	uint8_t redrawFlag;

	actionDelay = 0;

	for (;;)
	{
		LCD_VSyncLow();
		GPIO_ResetBits(GPIOC, GPIO_Pin_13 );
		vTaskDelay(1);

		LCD_VSyncHigh();
		LS_ProcessAutoTurnOff();
		GPIO_SetBits(GPIOC, GPIO_Pin_13 );
		vTaskDelay(32);

		point = FT_GetLastPoint();
		redrawFlag = 0;


		//if( FluidGetTouch() != TOUCH_OFF )
		{
			redrawFlag = gfxFrame_ProcessInputs(&GFX_MainFrame, point);
		}
//
//		if ((LBItem == NULL) && (GFX_LB_ProcessTouchInputs(GFX_LB) == LB_REQUIRES_REDRAW))
//		{
//			redrawFlag = 1;
//			LBItem = (char*) GFX_LB_ReturnSelectedItemPtr(GFX_LB);
//		}
//
//		if (LBItem != NULL)
//		{
//			if (actionDelay++ >= EXECUTION_DELAY)
//			{
//				actionDelay = 0;
//				redrawFlag = 1;
//
//
//				if (GFX_FB_ProcessRequest(&GFX_FB.GFXLB, LBItem) == GFX_FB_DIR_SELECTED)
//				{
//
//					FTI_ResetInertia(&GFX_LB->inertia);
//					GFX_LB_SelectItem(GFX_LB, NO_SELECTION);
//				}
//				else
//				{
//					if( FSUtil_HasExtension(LBItem, ".mid") )
//					{
//						ExecuteMIDIFile(LBItem);
//					}
//				}
//				GFX_LB_ResetTouchCounter(GFX_LB);
//				LBItem = NULL;
//
//
//			}
//		}

//		if( redrawFlag )
//		{
//			LCD_Clear(WHITE);
//			SetClip(1);
//			GFX_LB_Draw(GFX_LB);
//			SetClip(0);
//			setPixel(point->x, point->y);
//			gfxWriteString(point->x, point->y, "Hi");
//		}

	}
}

/*********************************************************************************************************
 END FILE
 *********************************************************************************************************/
