/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               TouchPanel.h
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
** Descriptions:            
**
*********************************************************************************************************/

#ifndef _LIGHTSYS_H_
#define _LIGHTSYS_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "MIDICodes/MIDICodes.h"

//#define LIGHT_OFFSET (-MIDI_PIANO_A0)
#define LIGHT_OFFSET (MIDI_PIANO_A0)
#define LIGHT_COUNT	 (128)

#define EMPTY_SLOT	  (0xFF)
#define FREE_ELEMENT  (0xFF)
#define FADE_PERCENT	  			(15)


#define WAIT_FOR_NOTE_OFF (0xFF)
#define DEFAULT_FADE_TIME	(0x20)


#define MIN_ON_BRIGHTNESS (1<<1)

#define MIN_FADE_BRIGHTNESS (100/FADE_PERCENT)

#define LS_CHANNEL(x)	(x & 0x0F)
#define LS_GETCHANNEL(x)	(x >> 8)

//Every single possible note has its own individual countdown.
#define MAX_POLYPHONY (MIDI_PIANO_KEY_COUNT)
#define LS_SETPIXEL(index, colour)	LPD8806_SetPixel( index, colour);

typedef uint32_t LS_ChannelColour_t;

extern uint8_t LS_MinBrightness;

enum {
	CHANNEL_COLOUR,
	NOTE_COLOUR,
	NOTE_FIFTHS_COLOUR,
	COLOUR_MODE_COUNT,
};

typedef struct
{
	//In the format of: CHANNEL << 8 | KEY;
	uint8_t channelKey;
	uint32_t colour;
	uint8_t	 timer;
} LS_CountdownElement_t;


void LS_Init(void);
void _LS_ClearLightTimers(void);
void LS_ClearLights(void);
void LS_ProcessAutoTurnOff(void);

void LS_AppendLightOn(uint8_t channelKey, uint32_t colour, uint8_t timer);
void LS_DeactivateTimer(uint8_t channelKey, uint32_t colour , uint8_t timer);

void LS_ProcessMIDINote(uint8_t command, uint8_t note, uint8_t velocity);
void LS_SetPixel(uint8_t note, uint32_t colour);
uint32_t LS_GetColourFromMIDI(uint8_t command, uint8_t note, uint8_t velocity);
void LS_SetTranspose(uint8_t channel, int8_t semitones);
int8_t LS_GetTranspose(uint8_t channel);
uint8_t LS_ApplyTranspose(uint8_t channel, uint8_t note);

uint8_t LS_IncrementColourMode(void);
uint8_t LS_LookupFifthsColour(uint8_t note);

uint8_t LS_IsChannelActive(uint8_t channel);
uint16_t LS_ToggleChannel(uint8_t channel);
void LS_TurnOffChannel(uint8_t channel);


void LS_SetMinBrightness(uint8_t minBrightness);
void LS_SetMaxBrightness(uint8_t maxBrightness);

#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/


