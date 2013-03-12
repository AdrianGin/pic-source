/*
 * MIDIInputProcessor.c
 *
 *  Created on: 12/03/2013
 *      Author: Adrian
 */



#include "MIDIInputProcessor/MIDIInputProcessor.h"
#include "MIDIPlaybackControlLogic/MIDIPlaybackControlLogic.h"
#include "MIDIPianoLogic/MIDIPianoLogic.h"
#include "LightSys/LightSys.h"


#include "intertaskComm.h"

void MIP_ProcessEvent(MIDI_CHAN_EVENT_t* event)
{
	uint8_t waitForNoteFlag;

	waitForNoteFlag = MPL_ProcessMIDINote( (uint8_t*)event);
	LS_ProcessMIDINote(event->eventType, event->parameter1, event->parameter1);

	//If we're waiting for notes, we don't process them.
	if( waitForNoteFlag == 0 )
	{
		MIP_AddEventToHaltQueue(event);
	}
}


void MIP_AddEventToHaltQueue(MIDI_CHAN_EVENT_t* event)
{
	//Ignore all other event types.
	if( ((event->eventType & MIDI_MSG_TYPE_MASK) == MIDI_NOTE_ON) ||
		((event->eventType & MIDI_MSG_TYPE_MASK) == MIDI_NOTE_OFF) )
	{
		QueueSend(Queue_MIDIEventInput, event, 0);
	}

}
