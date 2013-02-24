/*
 * MIDIPianoLogic.c
 *
 *  Created on: 19/02/2013
 *      Author: Adrian
 */

#include "printf/printf.h"


#include "ProjectConfig/ProjectConfig.h"
#include "MIDIPlayback/midiplayback.h"
#include "MIDIPianoLogic.h"
#include "Semaphore/osa_semaphore.h"
#include "Queue/osa_queue.h"

#include "intertaskComm.h"
#include "UserGUI.h"

#define MPL_MAX_TRANSPOSE (24)
static int8_t Global_TransposeValue = 0;


//Global Tempo in percent
static uint8_t Global_TempoValue = 100;

static uint8_t MPL_LoDetect = MIDI_PIANO_A0;
static uint8_t MPL_HiDetect = MIDI_PIANO_C8;

void   MPL_SetTranspose(int8_t value)
{
	Global_TransposeValue = value;
	MPB_SetTranspose(&MIDIHdr, Global_TransposeValue);
}


int8_t MPL_GetTranspose(void)
{
	return Global_TransposeValue;
}


uint8_t MPL_GetLoRange(void)
{
	return MPL_LoDetect;
}

uint8_t MPL_GetHiRange(void)
{
	return MPL_HiDetect;
}

void MPL_SetDetectRange(uint8_t lo, uint8_t hi)
{
	MPL_LoDetect = lo;
	MPL_HiDetect = hi;
	xprintf("HiNote=%x\n", MPL_HiDetect);
	xprintf("LoNote=%x\n", MPL_LoDetect);
}

uint8_t MPL_TestNoteInDetectRange(uint8_t note)
{
	if( (note >= MPL_LoDetect) && (note <= MPL_HiDetect))
	{
		return 1;
	}
	return 0;
}



uint8_t MPL_PreviewNote(uint8_t channel)
{
	MIDI_CHAN_EVENT_t event;

	event.eventType = MIDI_NOTE_ON | channel;
	event.parameter1 = FAST_FWD_IGNORE_PARAM1;
	event.parameter2 = FAST_FWD_NON_ZERO_PARAM2;

	if( MIDIHdr.channelStateBitmap & (1<<channel) )
	{

		MPB_PausePlayback(&MIDIHdr);
		MPB_ResetMIDI();
		LS_ClearLights();
		MLL_ClearHaltList();

		MLL_ActivateChannel(MLL_SOLO, channel);
		MPB_FastFwd_ToEvent(&MIDIHdr, 0, MPB_PB_SAVE_MIDI_STATUS, &event, FAST_FWD_FIND_PARAM2);
		MPB_ReplayStatusBuffer();
		MPB_SetPlaybackState(&MIDIHdr, STATE_ACTIVE);

	}

	return 0;
}

uint8_t MPL_ProcessMIDINote(uint8_t* midiDataArray)
{
	if( WaitForMIDIInputValue == WAITING_FOR_HI_RANGE )
	{
		MPL_SetDetectRange(MPL_LoDetect, midiDataArray[1]);
	}

	if( WaitForMIDIInputValue == WAITING_FOR_LO_RANGE )
	{
		MPL_SetDetectRange(midiDataArray[1], MPL_HiDetect);
	}

	if( WaitForMIDIInputValue == WAITING_FOR_FASTFWD_EVENT )
	{
		uint32_t position;
		MidiPlaybackState_t playbackState;

		position = MIDIHdr.masterClock;
		playbackState = MPB_GetPlaybackState(&MIDIHdr);
		MPB_PausePlayback(&MIDIHdr);
		MPB_ResetMIDI();

		LS_ClearLights();
		MLL_ClearHaltList();

		midiDataArray[2] = FAST_FWD_NON_ZERO_PARAM2;
		MPB_FastFwd_ToEvent(&MIDIHdr, MIDIHdr.masterClock, MPB_PB_SAVE_MIDI_STATUS, midiDataArray, FAST_FWD_FIND_PARAM2);
		MPB_ReplayStatusBuffer();
		MPB_SetPlaybackState(&MIDIHdr, playbackState);
		UG_UpdateSeekSlider();
	}

	if(WaitForMIDIInputValue)
	{
		WaitForMIDIInputValue = 0;
		return 1;
	}
	return 0;
}

void MPL_RestartPlay(void)
{
	MPB_ResetMIDI();
	MPB_RePosition(&MIDIHdr, MIDIHdr.masterClock, MPB_PB_SAVE_MIDI_STATUS);
	MPB_ReplayStatusBuffer();
	MPB_EnablePlayback(&MIDIHdr);
	LS_ClearLights();
}
