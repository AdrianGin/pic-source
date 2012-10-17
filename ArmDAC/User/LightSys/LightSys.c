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
#include "LEDDriver/LPD8806.h"
#include "MIDICodes/MIDICodes.h"
#include "LightSys.h"
#include "hw_config.h"

typedef uint32_t LS_ChannelColour_t;
//Every single possible note has its own individual countdown.

#define MAX_POLYPHONY (256)
#define FREE_ELEMENT  (0xFFFF)
#define FADE_RATE	  (15)
#define MIN_ON_BRIGHTNESS (1)

#define LS_CHANNEL(x)	(x & 0x0F)
#define LS_GETCHANNEL(x)	(x >> 8)

volatile uint8_t	LS_CountdownCount;
volatile uint16_t LS_ChannelActive = 0xFFFF;

typedef struct
{
	//In the format of: CHANNEL << 8 | KEY;
	uint16_t channelKey;
	uint8_t	 timer;
} LS_CountdownElement_t;

LS_CountdownElement_t LS_Countdown[MAX_POLYPHONY];



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


void LS_Init(void)
{
	uint16_t i;
	for( i = 0; i < MAX_POLYPHONY; i++)
	{
		LS_Countdown[i].channelKey = FREE_ELEMENT;
		LS_Countdown[i].timer = 0;
	}
	LS_CountdownCount = 0;
}

void LS_ProcessAutoTurnOff(void)
{
	uint8_t i;
	uint8_t j;
	uint32_t colour;

	for( i=0, j=0; i < LS_CountdownCount; j++ )
	{
		if( LS_Countdown[j].channelKey != FREE_ELEMENT)
		{
			if( (--LS_Countdown[j].timer) == 0 )
			{
				//Turn off the Pixel
				//TODO: doesn't support Channel differentiation
				//LPD8806_SetPixel(LS_Countdown[j].channelKey & MIDI_MAX_KEY, 0);
				LS_Countdown[j].channelKey = FREE_ELEMENT;
				LS_Countdown[j].timer = 0;

				if( LS_CountdownCount )
				{
					LS_CountdownCount--;
				}
			}
			else
			{
				//Implement Fade here.
				LPB8806_ReducePercentage(LS_Countdown[j].channelKey & MIDI_MAX_KEY, FADE_RATE, MIN_ON_BRIGHTNESS);
			}

			i++;
		}
	}
}

//When a note on is received.
void LS_AppendLightOn(uint16_t channelKey, uint8_t timer)
{
	uint8_t i;
	uint16_t j;

	for( j=0; j < MAX_POLYPHONY; j++ )
	{
		if( LS_Countdown[j].channelKey == FREE_ELEMENT)
		{
			LS_Countdown[j].channelKey = channelKey;
			LS_Countdown[j].timer = timer;
			LS_CountdownCount++;
			break;
		}
	}

}

//When a Note Off is received
void LS_DeactivateTimer(uint16_t channelKey)
{
	uint8_t i;
	uint16_t j;

	for( j=0; j < MAX_POLYPHONY; j++ )
	{
		if( LS_Countdown[j].channelKey == channelKey)
		{
			LS_Countdown[j].channelKey = FREE_ELEMENT;
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
			LPD8806_SetPixel(LS_Countdown[j].channelKey & MIDI_MAX_KEY, 0);
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



// eg. x91, x44, x64 for a NoteOn Channel 1, Note x44 Vel = x64
void LS_ProcessMIDINote(uint8_t command, uint8_t note, uint8_t velocity)
{
	uint8_t g, r;
	uint32_t colour;
	if( (command & 0xF0) == MIDI_NOTE_OFF )
	{
		colour = 0;
	}
	else
	{
		colour = LS_ChanColours[LS_CHANNEL(command)];
		if( velocity == 0)
		{
			colour = 0;
		}
	}

	if( LS_IsChannelActive(LS_CHANNEL(command)) )
	{
		LPD8806_SetPixel(note, colour);
		if( colour )
		{
			LS_AppendLightOn( ((LS_CHANNEL(command) << 8) | note), 50 );
		}
		else
		{
			LS_DeactivateTimer(((LS_CHANNEL(command) << 8) | note));
		}
	}
}



uint32_t _LS_GetColourFromMIDI(uint8_t command, uint8_t note, uint8_t velocity)
{
	return 0;
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
