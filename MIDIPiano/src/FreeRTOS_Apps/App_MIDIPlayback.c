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
#include "MIDIPlaybackControlLogic/MIDIPlaybackControlLogic.h"

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

	MIDI_CHAN_EVENT_t tempEvent;

	for (;;)
	{

		WAIT_FOR_MIDI_TICK();


		if( (MIDIHdr.playbackState == STATE_ACTIVE) &&
			(MPB_CL_GetHaltFlag() != HALT_FLAG_RAISED) )
		{

			MIDIHdr.masterClock++;
			if (MPB_ContinuePlay(&MIDIHdr, MPB_PB_ALL_ON) == MPB_FILE_FINISHED)
			{
				MPB_PausePlayback(&MIDIHdr);
				myprintf("End of MIDI File:  ", 1);
			}

			if(sliderUpdater-- == 0)
			{
				UG_UpdateSeekSlider();
				sliderUpdater = (MIDIHdr.currentState.maxLength / SEEK_UPDATE_RESOLUTION);
			}


			//LPD8806_Update();
			tickCounter++;
			if ((tickCounter >= ((MIDIHdr.PPQ / 24))))
			{
				MPB_CL_CheckListExpiry(MASTER_HALT_LIST, MIDIHdr.PPQ);
				MPB_CL_CheckListExpiry(TESTER_HALT_ON_LIST, MIDIHdr.PPQ);
				tickCounter = 0;
				//MIDI_Tx(0xF8);
			}
		}

		if( QueueReceive(Queue_MIDIEventInput, &tempEvent, 0) == pdTRUE )
		{
			MPB_CL_ProcessNewHaltNote((uint8_t*)&tempEvent);
		}

		MPB_CL_ProcessHaltList();
	}
}

/*********************************************************************************************************
 END FILE
 *********************************************************************************************************/
