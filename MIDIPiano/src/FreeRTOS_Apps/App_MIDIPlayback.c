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

#include "UserGUI.h"
#include "Graphics/gfxSlider.h"

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Task_MIDIPlayback(void * pvArg);

/*******************************************************************************
 * Function Name  : void Task_GLCDScreen(void * pvArg)
 * Description    :
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention		 : None
 *******************************************************************************/
void App_MIDIPlaybackTaskCreate(void)
{
	xTaskCreate( Task_MIDIPlayback, ( signed char * ) "MidiPB",
			APP_TASK_MIDIPLAYBACK_STK_SIZE, NULL, APP_TASK_MIDIPLAYBACK_PRIO,
			&MIDIPlayBackHandle);
}

#define SEEK_UPDATE_RESOLUTION (100)

void Task_MIDIPlayback(void * pvArg)
{
	uint16_t tickCounter = 0;
	uint16_t sliderUpdater = 0;

	for (;;)
	{

		WAIT_FOR_MIDI_TICK();


		if( (MIDIHdr.playbackState == STATE_ACTIVE) &&
			(MLL_GetHaltFlag() != HALT_FLAG_RAISED) )
		{

			MIDIHdr.masterClock++;
			if (MPB_ContinuePlay(&MIDIHdr, MPB_PB_ALL_ON) == MPB_FILE_FINISHED)
			{
				MPB_PausePlayback(&MIDIHdr);
				myprintf("End of MIDI File:  ", 1);
			}

			if(sliderUpdater-- == 0)
			{
				uint32_t position;
				position = (MIDIHdr.masterClock * SLIDER_RESOLUTION) / MIDIHdr.currentState.maxLength;
				GFX_SLIDER_SetPositionRaw(&GFX_SLD[SEEK_SLIDER_INDEX], position);

				gfxWidget_SetPendingFlags(GFX_WidgetHandles[SEEK_SLIDER_WIDGET_INDEX], PENDING_REDRAW_FLAG);

				sliderUpdater = (MIDIHdr.currentState.maxLength / SEEK_UPDATE_RESOLUTION);
			}


			//LPD8806_Update();
			tickCounter++;
			if ((tickCounter >= ((MIDIHdr.PPQ / 24))))
			{
				tickCounter = 0;
				//MIDI_Tx(0xF8);
			}
		}

		SemaphoreGive(Sem_LightSysUpdate);
		vTaskResume(LightSystemHandle);
	}
}

/*********************************************************************************************************
 END FILE
 *********************************************************************************************************/
