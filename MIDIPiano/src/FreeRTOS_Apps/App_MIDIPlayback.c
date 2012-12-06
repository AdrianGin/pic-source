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
void Task_MIDIPlayback(void * pvArg);

/*******************************************************************************
 * Function Name  : void Task_GLCDScreen(void * pvArg)
 * Description    :
 * Input          : None
 * Output         : None
 * Return         : None
 * Attention		 : None
 *******************************************************************************/
void App_MIDIPlaybackTaskCreate(MIDI_HEADER_CHUNK_t* MIDIHdr)
{
	xTaskCreate( Task_MIDIPlayback, ( signed char * ) "MidiPB",
			APP_TASK_MIDIPLAYBACK_STK_SIZE, (void*)MIDIHdr, APP_TASK_MIDIPLAYBACK_PRIO,
			&MIDIPlayBackHandle);
}

void Task_MIDIPlayback(void * pvArg)
{

	MIDI_HEADER_CHUNK_t* MIDIHdr;
	uint16_t tickCounter = 0;

	MIDIHdr = (MIDI_HEADER_CHUNK_t*) pvArg;

	for (;;)
	{

		WAIT_FOR_MIDI_TICK();

		MIDIHdr->masterClock++;
		if (MPB_ContinuePlay(MIDIHdr, MPB_PB_ALL_ON) == MPB_FILE_FINISHED)
		{
			TIM_ITConfig(MIDI_TIM, TIM_IT_Update, DISABLE);
			TIM_Cmd(MIDI_TIM, DISABLE);

			myprintf("End of MIDI File:  ", 1);
		}
		LPD8806_Update();

		tickCounter++;
		if ((tickCounter >= ((MIDIHdr->PPQ / 24))))
		{
			tickCounter = 0;
			//MIDI_Tx(0xF8);
		}

	}
}

/*********************************************************************************************************
 END FILE
 *********************************************************************************************************/
