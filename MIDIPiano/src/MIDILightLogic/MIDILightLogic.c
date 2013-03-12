


#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "hw_config.h"
#include "LightSys\LightSys.h"

#include "MIDICodes\MIDICodes.h"
#include "USBMIDI\USBMIDI.h"

#include "MIDIPianoLogic/MIDIPianoLogic.h"

#include "MIDIParser/midiparser.h"
#include "MIDIUtils/MIDIUtils.h"


#include "MIDILightLogic/MIDILightLogic.h"
#include "MIDIPlaybackControlLogic/MIDIPlaybackControlLogic.h"


void MLL_OutputMIDI(void)
{
	uint8_t i;


	//TODO: Need to add UART output here too.

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


		if( MPB_CL_CHANNEL_IS_ACTIVE(MPB_CL_MIDIOUT, channel) || channel >= MIDI_SYSTEM_COMMON_MSG )
		{
			MLL_OutputMIDI();
		}

		if( MPB_CL_CHANNEL_IS_ACTIVE(MPB_CL_LIGHTS, channel))
		{
			MLL_OutputLighting();
		}

		if( MPB_CL_CHANNEL_IS_ACTIVE(MPB_CL_HALT, channel) &&
			(headerByte & MIDI_MSG_TYPE_MASK) == MIDI_NOTE_ON)
		{
			MPB_CL_AddHaltMasterNote(&MIDICable[USB_MIDI_CABLE_COUNT].msg.MIDIData[0]);
		}

	}
}












