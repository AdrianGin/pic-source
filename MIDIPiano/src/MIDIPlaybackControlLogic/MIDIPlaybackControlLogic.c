


#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "hw_config.h"
#include "LightSys\LightSys.h"

#include "MIDICodes\MIDICodes.h"
#include "USBMIDI\USBMIDI.h"

#include "MIDIPianoLogic/MIDIPianoLogic.h"

#include "linkedlist/linkedlist.h"

#include "MIDIParser/midiparser.h"
#include "LightSys/ColourMixer/ColourMixer.h"

#include "MIDIUtils/MIDIUtils.h"

#include "MIDIPlaybackControlLogic/MIDIPlaybackControlLogic.h"
#include "rgbUtils/rgbutils.h"

#include "ProjectConfig/ProjectConfig.h"

#include "PlayStats/PlayStats.h"


uint16_t MPB_CL_ActiveChannelMap[MPB_CL_MAP_COUNT];
uint32_t MPB_CL_CorrectColour = RGB(0,255,0);

LINKED_LIST_t MPL_NoteHaltList[HALT_LIST_COUNT];
static uint8_t Halt_Count[HALT_LIST_COUNT] = {0,0};
//MIDI_CHAN_EVENT_t MPL_NoteHaltList[MAX_NOTES_TO_HALT];

static uint8_t Halt_Flag;
static uint8_t Match_Mode =  EXACT_MATCH;
static uint8_t Match_Flags = REQUIRE_NOTE_RELEASE | OCTAVE_MATCH_MODE;


uint8_t MPB_CL_IsChannelActive(MPB_CL_CH_MAP_t map, uint8_t channel)
{
	return ((MPB_CL_ActiveChannelMap[map] & (1<<channel)) != 0);
}

void MPB_CL_Init(void)
{
	MPB_CL_ActiveChannelMap[MPB_CL_LIGHTS] = 0xFFFF; //exclude the drums
	MPB_CL_ActiveChannelMap[MPB_CL_MIDIOUT] = 0xFFFF;
	MPB_CL_ActiveChannelMap[MPB_CL_HALT] = 0x0000;
	MPB_CL_ActiveChannelMap[MPB_CL_SOLO] = 0x0000;
}


void MPB_CL_SetChannelMaps(MPB_CL_CH_MAP_t map, uint16_t state)
{
	MPB_CL_ActiveChannelMap[map] = state;
}

uint16_t MPB_CL_ToggleChannel(MPB_CL_CH_MAP_t map, uint8_t channel)
{
	if( channel < MIDI_MAX_CHANNELS )
	{
		MPB_CL_ActiveChannelMap[map] ^= (1<<channel);
	}

	return MPB_CL_ActiveChannelMap[map];
}

//channel refers to the MIDI channel
uint16_t MPB_CL_ActivateChannel(MPB_CL_CH_MAP_t map, uint8_t channel)
{
	if( channel < MIDI_MAX_CHANNELS )
	{
		MPB_CL_ActiveChannelMap[map] |= (1<<channel);
	}
	return MPB_CL_ActiveChannelMap[map];
}


//channel refers to the MIDI channel
uint16_t MPB_CL_DisableChannel(MPB_CL_CH_MAP_t map, uint8_t channel)
{
	if( channel < MIDI_MAX_CHANNELS )
	{
		MPB_CL_ActiveChannelMap[map] &= ~(1<<channel);
	}
	return MPB_CL_ActiveChannelMap[map];
}



void MPB_CL_DeleteMarkedItemsFromList(MPB_CL_LIST_TYPE_t listMap)
{
	uint8_t i;
	LIST_NODE_t* 	   masterNode;
	MPB_CL_EVENT_t*    masterCLEvent;


	for( i = 0 ; i < Halt_Count[listMap]; i++)
	{
		masterNode = LL_ReturnNodeFromIndex(&MPL_NoteHaltList[listMap], i);
		masterCLEvent = (MPB_CL_EVENT_t*)masterNode->data;

		if( masterCLEvent->markForDeletion )
		{
			LL_Free(masterCLEvent);
			LL_Remove(&MPL_NoteHaltList[listMap], masterNode);
			Halt_Count[listMap]--;
			i--;
		}
	}
}

void MPB_CL_ClearSingleHaltList(MPB_CL_LIST_TYPE_t listMap)
{
	LL_DeleteListAndData(&MPL_NoteHaltList[listMap]);
	Halt_Count[listMap] = 0;
}



void MPB_CL_ClearHaltList(void)
{

	MPB_CL_ClearSingleHaltList(MASTER_HALT_LIST);
	MPB_CL_ClearSingleHaltList(TESTER_HALT_ON_LIST);
	MPB_CL_SetHaltFlag(0);
}

void MPB_CL_SetHaltFlag(uint8_t state)
{
	Halt_Flag = state;
}

uint8_t MPB_CL_GetHaltFlag(void)
{
	return Halt_Flag;
}

void MPB_CL_SetMatchMode(uint8_t mode)
{
	Match_Mode &= ~(MATCH_MODE_MASK);
	Match_Mode |= mode;
}

uint8_t MPB_CL_GetMatchMode(void)
{
	return Match_Mode & MATCH_MODE_MASK;
}

void MPB_CL_SetMatchFlags(uint8_t mode)
{
	Match_Flags &= ~MATCH_FLAG_MASK;
	Match_Flags |= mode & MATCH_FLAG_MASK;
}

uint8_t MPB_CL_GetMatchFlags(void)
{
	return Match_Flags & MATCH_FLAG_MASK;
}



void MPB_CL_ProcessNewHaltNote(uint8_t* midiDataArray)
{
	MPB_CL_AddTesterHaltNote(midiDataArray);
	MPB_CL_TesterHaltCancelNotes(midiDataArray);
}


//midiDataArray:: A 3 byte array pointer
void MPB_CL_AddHaltMasterNote(uint8_t* midiDataArray)
{
	if( (midiDataArray[0] & MIDI_MSG_TYPE_MASK) == MIDI_NOTE_ON &&
		(midiDataArray[2] != 0))
	{

		if( MPL_TestNoteInDetectRange(midiDataArray[1]) )
		{
			MPB_CL_EVENT_t* newNoteHalt;

			newNoteHalt = LL_Malloc(sizeof(MPB_CL_EVENT_t));

			newNoteHalt->markForDeletion = 0;
			newNoteHalt->midiClockTime = MIDIHdr.masterClock;
			newNoteHalt->event.eventType = midiDataArray[0];
			newNoteHalt->event.parameter1 = midiDataArray[1];
			newNoteHalt->event.parameter2 = midiDataArray[2];
			LL_AppendData(&MPL_NoteHaltList[MASTER_HALT_LIST], (void*)newNoteHalt);

			//Autoplay does not stop.
			if( MPB_CL_GetMatchMode() != AUTO_PLAY_FOLLOW )
			{
				MPB_CL_SetHaltFlag(HALT_FLAG_RAISED);
			}
			Halt_Count[MASTER_HALT_LIST]++;
		}

	}
}


//midiDataArray:: A 3 byte array pointer
void MPB_CL_AddTesterHaltNote(uint8_t* midiDataArray)
{
	//Only add non zero NoteOn's
	if( (midiDataArray[0] & MIDI_MSG_TYPE_MASK) == MIDI_NOTE_ON &&
		(midiDataArray[2] != 0))
	{
		MPB_CL_EVENT_t* newNoteHalt;
		newNoteHalt = LL_Malloc(sizeof(MPB_CL_EVENT_t));

		newNoteHalt->markForDeletion = 0;
		newNoteHalt->midiClockTime = MIDIHdr.masterClock;
		newNoteHalt->event.eventType = midiDataArray[0];
		newNoteHalt->event.parameter1 = midiDataArray[1];
		newNoteHalt->event.parameter2 = midiDataArray[2];

		LL_AppendData(&MPL_NoteHaltList[TESTER_HALT_ON_LIST], (void*)newNoteHalt);
		Halt_Count[TESTER_HALT_ON_LIST]++;
	}
}


//midiDataArray:: A 3 byte array pointer
// ignore the events across channels, imagine they are all on the same keyboard
void MPB_CL_TesterHaltCancelNotes(uint8_t* midiDataArray)
{
	uint8_t i;
	uint8_t eventType;
	uint8_t parameter1;
	uint8_t parameter2;

	LIST_NODE_t* onNode;
	MPB_CL_EVENT_t*    onCLEvent;
	MIDI_CHAN_EVENT_t* onEvent;


	eventType = midiDataArray[0];
	parameter1 = midiDataArray[1];
	parameter2 = midiDataArray[2];

	//We can only cancel against Note offs / NoteOn's with Vel=0.
	if( ((eventType & MIDI_MSG_TYPE_MASK) == MIDI_NOTE_OFF) ||
		(((eventType & MIDI_MSG_TYPE_MASK) == MIDI_NOTE_ON) && (parameter2 == 0)))
	{
		for( i = 0 ; i < Halt_Count[TESTER_HALT_ON_LIST];)
		{
			onNode = LL_ReturnNodeFromIndex(&MPL_NoteHaltList[TESTER_HALT_ON_LIST], i++);
			onCLEvent = (MPB_CL_EVENT_t*)onNode->data;
			onEvent = (MIDI_CHAN_EVENT_t*)&onCLEvent->event;

			if( (onEvent->parameter1 == parameter1) )
			{
				PS_RecordIncorrectNote(onCLEvent);
				onCLEvent->markForDeletion = MARK_EVENT_FOR_DELETION;
			}
		}
	}

	MPB_CL_DeleteMarkedItemsFromList(TESTER_HALT_ON_LIST);

}



void MPB_CL_ProcessHaltList(void)
{
	uint8_t i;
	uint8_t j;
	uint8_t matchCount = 0;
	uint8_t proceedFlag = 0;
	uint8_t matchMode = MPB_CL_GetMatchMode();

	LIST_NODE_t* masterNode;
	LIST_NODE_t* testerNode;
	MIDI_CHAN_EVENT_t* masterEvent;
	MIDI_CHAN_EVENT_t* testerEvent;

	MPB_CL_EVENT_t*    masterCLEvent;
	MPB_CL_EVENT_t*    testerCLEvent;


	for( i = 0 ; i < Halt_Count[MASTER_HALT_LIST]; i++)
	{
		masterNode = LL_ReturnNodeFromIndex(&MPL_NoteHaltList[MASTER_HALT_LIST], i);
		masterCLEvent = (MPB_CL_EVENT_t*)masterNode->data;
		masterEvent = (MIDI_CHAN_EVENT_t*)&masterCLEvent->event;

		//Compare all the NoteOn's against the Master Halt list.
		for( j = 0; j < Halt_Count[TESTER_HALT_ON_LIST]; j++)
		{
			uint8_t matchCountFlag = 0;

			testerNode = LL_ReturnNodeFromIndex(&MPL_NoteHaltList[TESTER_HALT_ON_LIST], j);
			testerCLEvent = (MPB_CL_EVENT_t*)testerNode->data;
			testerEvent = (MIDI_CHAN_EVENT_t*)&testerCLEvent->event;

			if( (masterEvent->parameter1 == testerEvent->parameter1) )
			{
				matchCountFlag = 1;
			}
			else if( MPB_CL_GetMatchFlags() & OCTAVE_MATCH_MODE )
			{
				if(MIDIUtils_GetMusicNote(masterEvent->parameter1) == MIDIUtils_GetMusicNote(testerEvent->parameter1))
				{
					matchCountFlag = 1;
				}
			}

			if( matchCountFlag )
			{
				matchCount++;
				if( (matchMode & MATCH_MODE_MASK) == AUTO_PLAY_FOLLOW )
				{
					MPB_CL_ProcessSingleCorrectKeyPress(masterEvent);
					PS_RecordSuccessNote(masterCLEvent, testerCLEvent->midiClockTime);
					//Remove them from the list.
					masterCLEvent->markForDeletion = MARK_EVENT_FOR_DELETION;
					testerCLEvent->markForDeletion = MARK_EVENT_FOR_DELETION;
				}
			}
		}
	}

	if( (matchMode & MATCH_MODE_MASK) == AUTO_PLAY_FOLLOW )
	{
		MPB_CL_DeleteMarkedItemsFromList(MASTER_HALT_LIST);
		MPB_CL_DeleteMarkedItemsFromList(TESTER_HALT_ON_LIST);
		return;
	}


	proceedFlag = MPB_CL_DetermineProceedAction(matchMode, matchCount);
	if( proceedFlag )
	{
		MPB_CL_ProcessAllCorrectKeyPress();

		if( (MPB_CL_GetMatchFlags() & MATCH_FLAG_MASK) & (ALLOW_SLIDE | ALLOW_PREHITS) )
		{
			if(( (MPB_CL_GetMatchFlags() & MATCH_FLAG_MASK) & REQUIRE_NOTE_RELEASE) )
			{
				MPB_CL_ClearSingleHaltList(TESTER_HALT_ON_LIST);
			}
		}

		MPB_CL_ClearSingleHaltList(MASTER_HALT_LIST);
		MPB_CL_SetHaltFlag(0);
	}

}


uint8_t MPB_CL_DetermineProceedAction(uint8_t matchMode, uint8_t matchCount)
{
	uint8_t proceedFlag = 0;

	switch( matchMode & MATCH_MODE_MASK)
	{
		case EXACT_MATCH:
			if( (matchCount == Halt_Count[MASTER_HALT_LIST]) &&
				(Halt_Count[TESTER_HALT_ON_LIST] <= Halt_Count[MASTER_HALT_LIST]))
			{
				proceedFlag = 1;
			}
			break;


		case EXACT_PLUS_MATCH:
			if(matchCount >= Halt_Count[MASTER_HALT_LIST])
			{
				proceedFlag = 1;
			}
			break;

		case HALF_CORRECT:
			if(matchCount*2 >= Halt_Count[MASTER_HALT_LIST])
			{
				proceedFlag = 1;
			}
			break;

		case MINIMUM_COUNT:
			if(Halt_Count[TESTER_HALT_ON_LIST] >= Halt_Count[MASTER_HALT_LIST])
			{
				proceedFlag = 1;
			}
			break;

		case PROCEED_ON_ANY:
			if(Halt_Count[TESTER_HALT_ON_LIST])
			{
				proceedFlag = 1;
			}
			break;

		case AUTO_PLAY_FOLLOW:
		{
			proceedFlag = 0;
			break;
		}
	}

	return proceedFlag;
}




void MPB_CL_CheckListExpiry(MPB_CL_LIST_TYPE_t listMap, uint32_t PPQ)
{
	uint8_t i;
	LIST_NODE_t* node;
	MPB_CL_EVENT_t*    clEvent;
	MIDI_CHAN_EVENT_t* event;

	PS_SetExpiryTolerance(PPQ);

	//Check the master list for expiry.
	for( i = 0 ; i < Halt_Count[listMap]; i++)
	{
		uint32_t nowTime = MIDIHdr.masterClock;

		node = LL_ReturnNodeFromIndex(&MPL_NoteHaltList[listMap], i);
		clEvent = (MPB_CL_EVENT_t*)node->data;
		event =  (MIDI_CHAN_EVENT_t*)&clEvent->event;
		//Check to expire HaltOn's and MasterList


		if( PS_CheckExpiry(clEvent->midiClockTime, nowTime) )
		{
			//xprintf("Expiry of Note %X, %X\n", event->eventType, event->parameter1);
			if( listMap == MASTER_HALT_LIST )
			{
				PS_RecordMissedNote(clEvent);
			}
			if( listMap == TESTER_HALT_ON_LIST )
			{
				PS_RecordIncorrectNote(clEvent);
			}

			clEvent->markForDeletion = MARK_EVENT_FOR_DELETION;
		}
	}

	MPB_CL_DeleteMarkedItemsFromList(listMap);

}







void MPB_CL_ProcessPulsateHaltList(void)
{
	uint8_t i;
	LIST_NODE_t* masterNode;
	MIDI_CHAN_EVENT_t* masterEvent;
	MPB_CL_EVENT_t* masterCLEvent;

	uint32_t colour;
	static uint8_t alternateFlag = 0;

	//Already been transposed

	for( i = 0 ; i < Halt_Count[MASTER_HALT_LIST]; i++)
	{
		masterNode = LL_ReturnNodeFromIndex(&MPL_NoteHaltList[MASTER_HALT_LIST], i);
		masterCLEvent = (MPB_CL_EVENT_t*)masterNode->data;
		masterEvent = (MIDI_CHAN_EVENT_t*)&masterCLEvent->event;

		colour = LS_GetColourFromMIDI(masterEvent->eventType, masterEvent->parameter1, masterEvent->parameter2);
		//CM_ReduceBrightnessPercentage(masterEvent->parameter1, -FADE_PERCENT_RESTORE, MIN_FADE_BRIGHTNESS);

		if( CM_GetBrightness(masterEvent->parameter1, colour) <= MPB_CL_PULSATE_THRESHOLD_LO )
		{
			alternateFlag = 1;
		}

		if( CM_GetBrightness(masterEvent->parameter1, colour) >= MPB_CL_PULSATE_THRESHOLD_HI )
		{
			alternateFlag = 0;
		}

		if( alternateFlag )
		{
			//Should really be to reduce the brightness of a particular colour mix.
			CM_ReduceBrightnessPercentage(masterEvent->parameter1, -PULSATE_PERCENT_INC, MIN_FADE_BRIGHTNESS);
		}
		else
		{
			//Should really be to reduce the brightness of a particular colour mix.
			CM_ReduceBrightnessPercentage(masterEvent->parameter1, PULSATE_PERCENT_DEC, MIN_FADE_BRIGHTNESS);
		}
		colour = CM_GetMixedColour(masterEvent->parameter1);
		LS_SetPixel(masterEvent->parameter1, colour);
	}
}



void MPB_CL_ProcessSingleCorrectKeyPress(MIDI_CHAN_EVENT_t* masterEvent)
{
	uint8_t i;
	uint32_t colour;

	colour = MPB_CL_CorrectColour;
	CM_AddColour(masterEvent->parameter1, colour, MAX_BRIGHTNESS);
	LS_AppendLightOn(((LS_CHANNEL(masterEvent->eventType) << 8) | masterEvent->parameter1), colour, DEFAULT_FADE_TIME);
	colour = CM_GetMixedColour(masterEvent->parameter1);
	LS_SetPixel(masterEvent->parameter1, colour);
}

//Makes the LED flash green
void MPB_CL_ProcessAllCorrectKeyPress(void)
{
	uint8_t i;
	LIST_NODE_t* masterNode;
	MIDI_CHAN_EVENT_t* masterEvent;
	MPB_CL_EVENT_t* masterCLEvent;

	uint32_t colour;
	//Already been transposed

	for( i = 0 ; i < Halt_Count[MASTER_HALT_LIST]; i++)
	{
		masterNode = LL_ReturnNodeFromIndex(&MPL_NoteHaltList[MASTER_HALT_LIST], i);
		masterCLEvent = (MPB_CL_EVENT_t*)masterNode->data;
		masterEvent = (MIDI_CHAN_EVENT_t*)&masterCLEvent->event;

		PS_RecordSuccessNote(masterCLEvent, masterCLEvent->midiClockTime);
		MPB_CL_ProcessSingleCorrectKeyPress(masterEvent);
	}
}










