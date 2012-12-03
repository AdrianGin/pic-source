


#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "hw_config.h"
#include "MIDILightLogic.h"
#include "LightSys\LightSys.h"

#include "MIDICodes\MIDICodes.h"
#include "USBMIDI\USBMIDI.h"

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
	MLL_ActiveChannelMap[MLL_LIGHTS] = 0xFFFF;
	MLL_ActiveChannelMap[MLL_MIDIOUT] = 0xFFFF;
	MLL_ActiveChannelMap[MLL_HALT] = 0;
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

	}


}





























