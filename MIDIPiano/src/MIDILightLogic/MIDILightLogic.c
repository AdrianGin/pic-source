


#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "hw_config.h"
#include "MIDILightLogic.h"
#include "LightSys\LightSys.h"

#include "MIDICodes\MIDICodes.h"
#include "USBMIDI\USBMIDI.h"

#include "MIDIPianoLogic/MIDIPianoLogic.h"

#include "linkedlist/linkedlist.h"

#include "MIDIParser/midiparser.h"
#include "LightSys/ColourMixer/ColourMixer.h"

typedef enum
{
	//Which MIDI channels are routed to the lights
	MLL_LIGHTS = 0,
	//Which MIDI channels are routed to the MIDI Out
	MLL_MIDIOUT,
	//Which MIDI channels require the user to enter the correct key before
	//proceeding
	MLL_HALT,
	MLL_MAP_COUNT,
} MLL_CH_MAP_t;


uint16_t MLL_ActiveChannelMap[MLL_MAP_COUNT];


void MLL_Init(void)
{
	MLL_ActiveChannelMap[MLL_LIGHTS] = 0xFDFF; //exclude the drums
	MLL_ActiveChannelMap[MLL_MIDIOUT] = 0xFFFF;
	MLL_ActiveChannelMap[MLL_HALT] = 0x0001;
}

//channel refers to the MIDI channel
uint32_t MLL_ActivateChannel(MLL_CH_MAP_t map, uint8_t channel)
{
	if( channel < MIDI_MAX_CHANNELS )
	{
		MLL_ActiveChannelMap[map] |= (1<<channel);
	}
	return MLL_ActiveChannelMap[map];
}


//channel refers to the MIDI channel
uint32_t MLL_DisableChannel(MLL_CH_MAP_t map, uint8_t channel)
{
	if( channel < MIDI_MAX_CHANNELS )
	{
		MLL_ActiveChannelMap[map] &= ~(1<<channel);
	}
	return MLL_ActiveChannelMap[map];
}



void MLL_OutputMIDI(void)
{
	uint8_t i;

	for( i = 0; i < MIDICable[USB_MIDI_CABLE_COUNT].bytesToReceive; i++)
	{
		USBMIDI_PutByte(MIDICable[USB_MIDI_CABLE_COUNT].msg.MIDIData[i], 0);
	}
}

void MLL_OutputLighting(void)
{
	uint8_t i;
	uint8_t byte;
	uint16_t ret;
	uint8_t lsBuf[3];
	uint8_t bufCount = 0;

	for( i = 0; i < MIDICable[USB_MIDI_CABLE_COUNT].bytesToReceive; i++)
	{
		byte = MIDICable[USB_MIDI_CABLE_COUNT].msg.MIDIData[i];

		if( ((byte & 0xF0) == MIDI_NOTE_ON) || ((byte & 0xF0) == MIDI_NOTE_OFF) || (bufCount))
		{
			lsBuf[bufCount] = byte;
			bufCount++;
		}
		else
		{
			bufCount = 0;
		}

		if( bufCount == 3)
		{
			bufCount = 0;
			LS_ProcessMIDINote(lsBuf[0], lsBuf[1], lsBuf[2]);
		}
	}
}



//Passes the relevant bytes to the LightControl System.
//Also routes the relevant bytes to the USB MIDI etc.
//Performs filtering / volume control.
void MLL_ProcessMIDIByte(uint8_t byte)
{
	uint8_t midiReady;
	//The last MIDICable is used for us.
	//We're buffering it
	midiReady = MIDIDataReady(byte, &MIDICable[USB_MIDI_CABLE_COUNT]);

	if( midiReady )
	{
		uint8_t i;
		uint8_t headerByte = MIDICable[USB_MIDI_CABLE_COUNT].msg.MIDIData[0];
		uint8_t channel;

		//Determine what channel this message relates to.
		channel = (headerByte & MIDI_SYSTEM_COMMON_MSG);
		if( channel < MIDI_SYSTEM_COMMON_MSG)
		{
			channel = headerByte & MIDI_CHANNEL_MASK;
		}
		else
		{

		}

		if( MLL_CHANNEL_IS_ACTIVE(MLL_MIDIOUT, channel) || channel >= MIDI_SYSTEM_COMMON_MSG )
		{
			MLL_OutputMIDI();
		}

		if( MLL_CHANNEL_IS_ACTIVE(MLL_LIGHTS, channel))
		{
			MLL_OutputLighting();
		}

		if( MLL_CHANNEL_IS_ACTIVE(MLL_HALT, channel) &&
			(headerByte & MIDI_MSG_TYPE_MASK) == MIDI_NOTE_ON)
		{

			MLL_AddHaltMasterNote(&MIDICable[USB_MIDI_CABLE_COUNT].msg.MIDIData[0]);
		}


	}


}


#define MAX_NOTES_TO_HALT	(20)

enum {
	MASTER_HALT_LIST = 0,
	TESTER_HALT_ON_LIST,
	//TESTER_HALT_OFF_LIST,
	HALT_LIST_COUNT
};

LINKED_LIST_t MPL_NoteHaltList[HALT_LIST_COUNT];
static uint8_t Halt_Count[HALT_LIST_COUNT] = {0,0};
//MIDI_CHAN_EVENT_t MPL_NoteHaltList[MAX_NOTES_TO_HALT];

static uint8_t Halt_Flag;


void MLL_ClearHaltList(void)
{
	LL_DeleteListAndData(&MPL_NoteHaltList[MASTER_HALT_LIST]);
	LL_DeleteListAndData(&MPL_NoteHaltList[TESTER_HALT_ON_LIST]);
	Halt_Count[MASTER_HALT_LIST] = 0;
	Halt_Count[TESTER_HALT_ON_LIST] = 0;
	MLL_SetHaltFlag(0);
}

void MLL_SetHaltFlag(uint8_t state)
{
	Halt_Flag = state;
}

uint8_t MLL_GetHaltFlag(void)
{
	return Halt_Flag;
}



void MLL_ProcessHaltNote(uint8_t* midiDataArray)
{
	//if( MLL_GetHaltFlag() == HALT_FLAG_RAISED)
	{
		MLL_AddTesterHaltNote(midiDataArray);
		MLL_TesterHaltCancelNotes(midiDataArray);
		MLL_CompareMasterTesterHaltList();
	}
}


//midiDataArray:: A 3 byte array pointer
void MLL_AddHaltMasterNote(uint8_t* midiDataArray)
{
	if( (midiDataArray[0] & MIDI_MSG_TYPE_MASK) == MIDI_NOTE_ON &&
		(midiDataArray[2] != 0))
	{
		MIDI_CHAN_EVENT_t* newNoteHalt;

		newNoteHalt = LL_Malloc(sizeof(MIDI_CHAN_EVENT_t));
		newNoteHalt->eventType = midiDataArray[0];
		newNoteHalt->parameter1 = midiDataArray[1];
		newNoteHalt->parameter2 = midiDataArray[2];
		LL_AppendData(&MPL_NoteHaltList[MASTER_HALT_LIST], (void*)newNoteHalt);

		MLL_SetHaltFlag(HALT_FLAG_RAISED);
		Halt_Count[MASTER_HALT_LIST]++;
	}
}


//midiDataArray:: A 3 byte array pointer
void MLL_AddTesterHaltNote(uint8_t* midiDataArray)
{
	//Only add non zero NoteOn's
	if( (midiDataArray[0] & MIDI_MSG_TYPE_MASK) == MIDI_NOTE_ON &&
		(midiDataArray[2] != 0))
	{
		MIDI_CHAN_EVENT_t* newNoteHalt;
		newNoteHalt = LL_Malloc(sizeof(MIDI_CHAN_EVENT_t));
		newNoteHalt->eventType = midiDataArray[0];
		newNoteHalt->parameter1 = midiDataArray[1];
		newNoteHalt->parameter2 = midiDataArray[2];

		LL_AppendData(&MPL_NoteHaltList[TESTER_HALT_ON_LIST], (void*)newNoteHalt);
		Halt_Count[TESTER_HALT_ON_LIST]++;
	}
}


//midiDataArray:: A 3 byte array pointer
// ignore the events across channels, imagine they are all on the same keyboard
void MLL_TesterHaltCancelNotes(uint8_t* midiDataArray)
{
	uint8_t i;
	uint8_t j;
	uint8_t eventType;
	uint8_t parameter1;
	uint8_t parameter2;

	LIST_NODE_t* onNode;
	LIST_NODE_t* offNode;
	MIDI_CHAN_EVENT_t* onEvent;
	MIDI_CHAN_EVENT_t* offEvent;

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
			onEvent = (MIDI_CHAN_EVENT_t*)onNode->data;

			if( (onEvent->parameter1 == parameter1) )
			{
				LL_Free(onNode->data);
				LL_Remove(&MPL_NoteHaltList[TESTER_HALT_ON_LIST], onNode);
				Halt_Count[TESTER_HALT_ON_LIST]--;
				i--;
			}
		}
	}
}


void MLL_ProcessPulsateHaltList(void)
{
	uint8_t i;
	LIST_NODE_t* masterNode;
	MIDI_CHAN_EVENT_t* masterEvent;

	uint32_t colour;
	static uint8_t alternateFlag = 0;

	//Already been transposed

	for( i = 0 ; i < Halt_Count[MASTER_HALT_LIST]; i++)
	{
		masterNode = LL_ReturnNodeFromIndex(&MPL_NoteHaltList[MASTER_HALT_LIST], i);
		masterEvent = (MIDI_CHAN_EVENT_t*)masterNode->data;

		colour = LS_GetColourFromMIDI(masterEvent->eventType, masterEvent->parameter1, masterEvent->parameter2);
		//CM_ReduceBrightnessPercentage(masterEvent->parameter1, -FADE_PERCENT_RESTORE, MIN_FADE_BRIGHTNESS);

		if( CM_GetBrightness(masterEvent->parameter1, colour) <= MLL_PULSATE_THRESHOLD_LO )
		{
			alternateFlag = 1;
		}

		if( CM_GetBrightness(masterEvent->parameter1, colour) >= MLL_PULSATE_THRESHOLD_HI )
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



void MLL_CompareMasterTesterHaltList(void)
{
	uint8_t i;
	uint8_t j;
	uint8_t matchCount = 0;

	LIST_NODE_t* masterNode;
	LIST_NODE_t* testerNode;
	MIDI_CHAN_EVENT_t* masterEvent;
	MIDI_CHAN_EVENT_t* testerEvent;

	if(Halt_Count[MASTER_HALT_LIST] == Halt_Count[TESTER_HALT_ON_LIST])
	{
		for( i = 0 ; i < Halt_Count[MASTER_HALT_LIST]; i++)
		{
			masterNode = LL_ReturnNodeFromIndex(&MPL_NoteHaltList[MASTER_HALT_LIST], i);
			masterEvent = (MIDI_CHAN_EVENT_t*)masterNode->data;
			for( j = 0; j < Halt_Count[TESTER_HALT_ON_LIST]; j++)
			{
				testerNode = LL_ReturnNodeFromIndex(&MPL_NoteHaltList[TESTER_HALT_ON_LIST], j);
				testerEvent = (MIDI_CHAN_EVENT_t*)testerNode->data;
				if( (masterEvent->parameter1 == testerEvent->parameter1) )
				{
					matchCount++;
				}
			}
		}
	}

	//If we have matched the halt requirements, delete everything and continue
	if(matchCount == Halt_Count[MASTER_HALT_LIST])
	{
		LL_DeleteListAndData(&MPL_NoteHaltList[MASTER_HALT_LIST]);
		Halt_Count[MASTER_HALT_LIST] = 0;
		MLL_SetHaltFlag(0);
	}

}
















