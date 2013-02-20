/*
 * ColourMixer.c
 *
 *  Created on: 20/02/2013
 *      Author: Adrian
 */


#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "rgbUtils/rgbutils.h"
#include "ColourMixer.h"
//Typically the max number of LEDS
#define MAX_NOTE_COUNT	(88)
#define MAX_COLOUR_MIX	(5)
uint32_t CM_MixerMap[MAX_NOTE_COUNT][MAX_COLOUR_MIX];

#define BRIGHTNESS_BITS (0xFF000000)
#define RGB_BITS		(0x00FFFFFF)

//Accepts colours in 24bit RGB format
#define APPLY_BRIGHTNESS(colour, brightness)	(colour | (brightness << 24))
#define GET_BRIGHTNESS(brightness_colour)		((brightness_colour & 0xFF000000) >> 24)

void CM_Init(void)
{
	memset(CM_MixerMap, 0, MAX_NOTE_COUNT*MAX_COLOUR_MIX*sizeof(uint32_t) );
}


void CM_AddColour(uint8_t note, uint32_t colour, uint8_t brightness)
{
	uint8_t i;
	for( i = 0; i < MAX_COLOUR_MIX; i++)
	{
		if(CM_MixerMap[note][i] == 0)
		{
			CM_MixerMap[note][i] = APPLY_BRIGHTNESS(colour,brightness);
			break;
		}
	}
}

void CM_RemoveColour(uint8_t note, uint32_t colour)
{
	uint8_t i;
	for( i = 0; i < MAX_COLOUR_MIX; i++)
	{
		if( (CM_MixerMap[note][i] & RGB_BITS) == (colour & RGB_BITS))
		{
			CM_MixerMap[note][i] = 0;
			break;
		}
	}
}

uint32_t _CM_NomaliseColour(uint32_t r, uint32_t g, uint32_t b)
{
	uint16_t maxVal;
	maxVal = r;
	if( g > maxVal )
	{
		maxVal = g;
	}
	if( b > maxVal )
	{
		maxVal = b;
	}

	//No need to normalise
	if( maxVal <= MAX_COLOUR_VALUE )
	{
	}
	else
	{
		//normalise rgb colours with MAX_COLOUR_VALUE
		r = (r * MAX_COLOUR_VALUE) / maxVal;
		g = (g * MAX_COLOUR_VALUE) / maxVal;
		b = (b * MAX_COLOUR_VALUE) / maxVal;
	}

	return RGB(r,g,b);
}

uint32_t CM_GetMixedColour(uint8_t note)
{
	uint8_t i;
	uint16_t r = 0;
	uint16_t g = 0;
	uint16_t b = 0;

	uint8_t brightness;

	for( i = 0; i < MAX_COLOUR_MIX; i++)
	{
		brightness = GET_BRIGHTNESS(CM_MixerMap[note][i]);
		r  = r + ((R_RGB(CM_MixerMap[note][i]) * brightness) / MAX_COLOUR_VALUE);
		g  = g + ((G_RGB(CM_MixerMap[note][i]) * brightness) / MAX_COLOUR_VALUE);
		b  = b + ((B_RGB(CM_MixerMap[note][i]) * brightness) / MAX_COLOUR_VALUE);
	}

	return _CM_NomaliseColour(r,g,b);
}









