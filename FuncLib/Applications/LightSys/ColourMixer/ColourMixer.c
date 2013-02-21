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

uint32_t CM_MixerMap[MAX_MIXER_COUNT][MAX_COLOUR_MIX];


void CM_Init(void)
{
	memset(CM_MixerMap, 0, MAX_MIXER_COUNT*MAX_COLOUR_MIX*sizeof(uint32_t) );
}



void CM_AddColour(uint8_t index, uint32_t colour, uint8_t brightness)
{
	uint8_t i;

	if( index < MAX_MIXER_COUNT)
	{
		for( i = 0; i < MAX_COLOUR_MIX; i++)
		{
			if(CM_MixerMap[index][i] == 0)
			{
				CM_MixerMap[index][i] = APPLY_BRIGHTNESS(colour,brightness);
				break;
			}
		}
	}
}

void CM_RemoveColour(uint8_t index, uint32_t colour)
{
	uint8_t i;
	if( index < MAX_MIXER_COUNT)
	{
		for( i = 0; i < MAX_COLOUR_MIX; i++)
		{
			if( (CM_MixerMap[index][i] & RGB_BITS) == (colour & RGB_BITS))
			{
				CM_MixerMap[index][i] = 0;
				break;
			}
		}
	}
}

void CM_ReduceBrightnessPercentage(uint8_t index, int16_t percentage, uint8_t minBrightness)
{
	uint8_t i;
	int16_t brightness;
	int16_t deltaBrightness;


	if( index < MAX_MIXER_COUNT)
	{
		for( i = 0; i < MAX_COLOUR_MIX; i++)
		{
			if( (CM_MixerMap[index][i] & RGB_BITS) )
			{
				brightness = GET_BRIGHTNESS(CM_MixerMap[index][i]);
				deltaBrightness = (brightness * percentage) / 100;
				brightness = brightness - deltaBrightness;
				if( brightness < minBrightness )
				{
					brightness = minBrightness;
				}
				if( brightness > MAX_BRIGHTNESS )
				{
					brightness = MAX_BRIGHTNESS;
				}
				CM_MixerMap[index][i] = APPLY_BRIGHTNESS(CM_MixerMap[index][i], brightness);
			}
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


uint8_t CM_GetBrightness(uint8_t index, uint32_t colour)
{
	uint8_t i;
	if( index < MAX_MIXER_COUNT)
	{
		for( i = 0; i < MAX_COLOUR_MIX; i++)
		{
			if( (CM_MixerMap[index][i] & RGB_BITS) == (colour & RGB_BITS))
			{
				return GET_BRIGHTNESS(CM_MixerMap[index][i]);
			}
		}
	}
	return 0;
}

uint32_t CM_GetMixedColour(uint8_t index)
{
	uint8_t i;
	uint16_t r = 0;
	uint16_t g = 0;
	uint16_t b = 0;

	uint8_t brightness;

	if( index < MAX_MIXER_COUNT)
	{
		for( i = 0; i < MAX_COLOUR_MIX; i++)
		{
			brightness = GET_BRIGHTNESS(CM_MixerMap[index][i]);
			r  = r + ((R_RGB(CM_MixerMap[index][i]) * brightness) / MAX_COLOUR_VALUE);
			g  = g + ((G_RGB(CM_MixerMap[index][i]) * brightness) / MAX_COLOUR_VALUE);
			b  = b + ((B_RGB(CM_MixerMap[index][i]) * brightness) / MAX_COLOUR_VALUE);
		}
	}

	return _CM_NomaliseColour(r,g,b);
}









