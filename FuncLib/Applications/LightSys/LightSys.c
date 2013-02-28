/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               TouchPanel.c
** Descriptions:            The TouchPanel application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions: Lighting subsystem to interface with MIDI data and output it to the LED array / lighting system
** It is possible that each MIDI Channel will have it's own colour.
** Or that Velocity will control the colour of the LED.
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <string.h>

#include "LPD8806/LPD8806.h"
#include "MIDICodes/MIDICodes.h"
#include "LightSys.h"
#include "hw_config.h"
#include "MIDIUtils/MIDIUtils.h"

#include "ColourMixer/ColourMixer.h"



volatile uint8_t	LS_CountdownCount;
volatile uint16_t LS_ChannelActive = 0xFFFF;

uint8_t LS_MinBrightness;

static uint8_t mode = CHANNEL_COLOUR;


LS_CountdownElement_t LS_Countdown[MAX_POLYPHONY];
uint8_t LS_AutoTurnOffElements[MAX_POLYPHONY];

//Either  +/- from middle C up to +/- 24 semi tones.
int8_t LS_TransposeMap[MIDI_MAX_CHANNELS];


LS_ChannelColour_t LS_ChanColours[MIDI_MAX_CHANNELS] =
		{		RGB(255, 0, 0),
				RGB(0, 255, 0),
				RGB(0, 0, 255),
				RGB(255, 255, 0),
				RGB(0, 255, 255),
				RGB(255, 0, 255),
				RGB(255, 255, 255),
				RGB(255, 127, 0),
				RGB(127, 255, 0),
				RGB(0, 255, 127),
				RGB(0, 127, 255),
				RGB(127, 0, 255),
				RGB(255, 0, 127),
				RGB(0, 255, 0),
				RGB(0, 255, 0),
				RGB(0, 255, 0),
		};




#define MAX_COLOUR (MAX_LED_BRIGHTNESS)
#define HALF_COLOUR	(MAX_COLOUR/2)

LS_ChannelColour_t LS_NoteColours[NOTE_COUNT] =
		{		NORM_RGB(MAX_COLOUR, 	0, 					0),				//C
				NORM_RGB(MAX_COLOUR,  	HALF_COLOUR, 		0),				//C#
				NORM_RGB(MAX_COLOUR,  	MAX_COLOUR, 		0),				//D
				NORM_RGB(HALF_COLOUR, 	MAX_COLOUR, 		0),				//D#
				NORM_RGB(0, 			MAX_COLOUR, 		0),				//E
				NORM_RGB(0, 			MAX_COLOUR,			HALF_COLOUR),	//F
				NORM_RGB(0, 			MAX_COLOUR, 		MAX_COLOUR),	//F#
				NORM_RGB(0, 			HALF_COLOUR, 		MAX_COLOUR),	//G
				NORM_RGB(0, 			0, 					MAX_COLOUR),	//G#
				NORM_RGB(HALF_COLOUR, 	0, 					MAX_COLOUR),	//A
				NORM_RGB(MAX_COLOUR, 	0, 					MAX_COLOUR),	//A#
				NORM_RGB(MAX_COLOUR, 	0, 					HALF_COLOUR),	//B
		};


uint8_t LS_FifthsColourMap[] =
		{ 	MN_C,
			MN_G,
			MN_D,
			MN_A,
			MN_E,
			MN_B,
			MN_FS,
			MN_CS,
			MN_GS,
			MN_DS,
			MN_AS,
			MN_F,
		};



void LS_Init(void)
{
	CM_Init();
	LS_ClearLights();

	memset(LS_TransposeMap, 0, sizeof(LS_TransposeMap));
	LS_MinBrightness = MIN_ON_BRIGHTNESS;
}


//Deactive ALL timers.
void _LS_ClearLightTimers(void)
{
	uint16_t i;
	for( i = 0; i < MAX_POLYPHONY; i++)
	{
		LS_Countdown[i].channelKey = FREE_ELEMENT;
		LS_Countdown[i].timer = 0;
	}
	LS_CountdownCount = 0;
}

//Turns off all the lights
void LS_ClearLights(void)
{
	uint16_t i;
	CM_ClearAll();
	_LS_ClearLightTimers();
	for( i = 0; i < LIGHT_COUNT; i++)
	{
		LS_SETPIXEL(i , 0x00)
	}
}


void LS_SetMinBrightness(uint8_t minBrightness)
{
	LS_MinBrightness = minBrightness;
}


void LS_SetMaxBrightness(uint8_t maxBrightness)
{
	LPD8806_SetBrightness(maxBrightness);
}




//This uses the Colour Mixer to process the fade out.
void LS_ProcessAutoTurnOff(void)
{
	uint8_t i;
	uint8_t j;
	uint32_t colour;

	memset(LS_AutoTurnOffElements, FREE_ELEMENT, sizeof(LS_AutoTurnOffElements));


	for( i=0, j=0; (i < LS_CountdownCount) && (j < MAX_POLYPHONY); j++ )
	{
		if( LS_Countdown[j].channelKey != FREE_ELEMENT)
		{
			//If the timer has elapsed, remove the colour completely.
			if(LS_Countdown[j].timer != WAIT_FOR_NOTE_OFF )
			{
				if( LS_Countdown[j].timer == 0 )
				{
					CM_RemoveColour(LS_Countdown[j].channelKey, LS_Countdown[j].colour);
					colour = CM_GetMixedColour((LS_Countdown[j].channelKey & MIDI_MAX_KEY));
					LS_SetPixel((LS_Countdown[j].channelKey & MIDI_MAX_KEY), colour);

					LS_DeactivateTimer(LS_Countdown[j].channelKey, LS_Countdown[j].colour, 0);
				}
				else
				{
					LS_Countdown[j].timer--;
				}
			}
			i++;
			LS_AutoTurnOffElements[LS_Countdown[j].channelKey & MIDI_MAX_KEY] = j;
		}
	}


	//Fade out the lights
	for( i = 0; i < MAX_POLYPHONY; i++)
	{
		if( LS_AutoTurnOffElements[i] != FREE_ELEMENT)
		{
			j = LS_AutoTurnOffElements[i];
			CM_ReduceBrightnessPercentage((LS_Countdown[j].channelKey & MIDI_MAX_KEY), FADE_PERCENT, LS_MinBrightness);
			colour = CM_GetMixedColour((LS_Countdown[j].channelKey & MIDI_MAX_KEY));
			LS_SetPixel((LS_Countdown[j].channelKey & MIDI_MAX_KEY), colour);
		}
	}

}



//When a note on is received.
//Because we must remember which lights have been on for how long
void LS_AppendLightOn(uint8_t channelKey, uint32_t colour, uint8_t timer)
{
	uint16_t j;

	for( j=0; j < MAX_POLYPHONY; j++ )
	{
		//Don't record duplicates.
		//TODO: Because otherwise the fade will be twice as quick...
		//But this quick fix doesn't work. Since we now need to take into account colours
		//It's not just one element per key.
		if( (LS_Countdown[j].channelKey & MIDI_MAX_KEY) == (channelKey & MIDI_MAX_KEY) )
		{
			//break;
		}

		if( LS_Countdown[j].channelKey == FREE_ELEMENT)
		{
			LS_Countdown[j].channelKey = channelKey;
			LS_Countdown[j].colour = colour;
			LS_Countdown[j].timer = timer;
			LS_CountdownCount++;
			break;
		}
	}
}

//When a Note Off is received
//channelKey =  channel | note;
// eg. 0x0165 = Channel x1, note x65
void LS_DeactivateTimer(uint8_t channelKey, uint32_t colour , uint8_t timer)
{
	uint16_t j;

	for( j=0; j < MAX_POLYPHONY; j++ )
	{
		if( (LS_Countdown[j].channelKey == channelKey) &&
			(LS_Countdown[j].colour == colour) &&
			(LS_Countdown[j].timer == timer))
		{
			LS_Countdown[j].channelKey = FREE_ELEMENT;
			LS_Countdown[j].colour = 0;
			LS_Countdown[j].timer = 0;
			if( LS_CountdownCount )
			{
				LS_CountdownCount--;
			}
			break;
		}
	}
}

void LS_TurnOffChannel(uint8_t channel)
{
	uint16_t i;
	uint16_t j;

	uint16_t count = LS_CountdownCount;

	for( i=0, j=0; (i < count) && (j < MAX_POLYPHONY) ; j++ )
	{
		if( LS_GETCHANNEL(LS_Countdown[j].channelKey) == channel)
		{
			LS_SETPIXEL( (LS_Countdown[j].channelKey & MIDI_MAX_KEY) + LIGHT_OFFSET, 0);
			LS_Countdown[j].channelKey = FREE_ELEMENT;
			LS_Countdown[j].timer = 0;

			if( LS_CountdownCount )
			{
				LS_CountdownCount--;
			}
			i++;
		}
	}
}




void LS_SetPixel(uint8_t note, uint32_t colour)
{
	LS_SETPIXEL(note + LIGHT_OFFSET, colour);
}



// eg. x91, x44, x64 for a NoteOn Channel 1, Note x44 Vel = x64
void LS_ProcessMIDINote(uint8_t command, uint8_t note, uint8_t velocity)
{
	uint32_t colour;

	//Already been transposed
	colour = LS_GetColourFromMIDI(command, note, velocity);
	if(colour == 0)
	{
		return;
	}

	if( ((command & MIDI_MSG_TYPE_MASK) == MIDI_NOTE_OFF) || velocity == 0)
	{
		velocity = 0;
		CM_RemoveColour(note, colour);
	}
	else
	{
		velocity = (velocity << 1) + 1;
		CM_AddColour(note, colour, velocity);
	}

	//colour = SCALE_COLOUR(colour , velocity, MIDI_MAX_KEY);


	if( LS_IsChannelActive(LS_CHANNEL(command)) )
	{
		if (colour && velocity)
		{
			LS_AppendLightOn(((LS_CHANNEL(command) << 8) | note), colour, WAIT_FOR_NOTE_OFF);
		}
		else
		{
			LS_DeactivateTimer(((LS_CHANNEL(command) << 8) | note), colour, WAIT_FOR_NOTE_OFF);
		}
		//Reverse on LED Strip
		//LS_SetPixel((LED_COUNT / 2) - note, colour, command);
		//02/2013 Added Colour Mixer
		colour = CM_GetMixedColour(note);
		LS_SetPixel(note, colour);
	}
}


uint8_t LS_ApplyTranspose(uint8_t channel, uint8_t note)
{
	int16_t newNote;

	newNote = note + LS_TransposeMap[channel];
	if( newNote < 0 )
	{
		newNote = 0;
	}
	if( newNote > MIDI_MAX_KEY)
	{
		newNote = MIDI_MAX_KEY;
	}


	return (uint8_t)newNote;
}

void LS_SetTranspose(uint8_t channel, int8_t semitones)
{
	LS_TransposeMap[channel] = semitones;
	LS_TurnOffChannel(channel);
}


int8_t LS_GetTranspose(uint8_t channel)
{
	return LS_TransposeMap[channel];
}


uint8_t LS_LookupFifthsColour(uint8_t note)
{
	uint8_t i = 0;

	for( i = 0; i < NOTE_COUNT; i++ )
	{
		if( LS_FifthsColourMap[i] == note )
		{
			break;
		}
	}
	return i;
}

uint32_t LS_GetColourFromMIDI(uint8_t command, uint8_t note, uint8_t velocity)
{
	uint32_t colour = 0;
	uint8_t musicNote;


	if( (command & MIDI_MSG_TYPE_MASK) != MIDI_NOTE_OFF &&
		(command & MIDI_MSG_TYPE_MASK) != MIDI_NOTE_ON)
	{
		colour = 0;
	}

	//Currently Note = Pixel Colour, We can also make all C's a type of colour etc.
	switch( mode )
	{
		case CHANNEL_COLOUR:
		{
			colour = LS_ChanColours[LS_CHANNEL(command)];
			break;
		}

		case NOTE_COLOUR:
		{
			colour = LS_NoteColours[MIDIUtils_GetMusicNote(note)];
			break;
		}

		case NOTE_FIFTHS_COLOUR:
		{
			musicNote = MIDIUtils_GetMusicNote(note);
			colour = LS_NoteColours[LS_LookupFifthsColour(musicNote)];
			break;
		}

		default:
		{
			break;
		}
	}
	return colour;
}


uint8_t LS_IncrementColourMode(void)
{
	mode = mode + 1;
	if( mode >= COLOUR_MODE_COUNT )
	{
		mode = CHANNEL_COLOUR;
	}
	return mode;
}

uint8_t LS_IsChannelActive(uint8_t channel)
{
	return (LS_ChannelActive & (1 << channel)) ? 1 : 0;
}

uint16_t LS_ToggleChannel(uint8_t channel)
{
	LS_ChannelActive ^= (1 << channel);

	if( !LS_IsChannelActive(channel) )
	{
		LS_TurnOffChannel(channel);
	}

	return LS_ChannelActive;
}


/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
