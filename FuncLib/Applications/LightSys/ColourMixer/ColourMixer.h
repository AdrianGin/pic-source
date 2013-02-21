/*
 * ColourMixer.h
 *
 *  Created on: 20/02/2013
 *      Author: Adrian
 */

#ifndef COLOURMIXER_H_
#define COLOURMIXER_H_

#include <stdint.h>


//Typically the max number of LEDS for a piano
#define MAX_MIXER_COUNT	(128)
#define MAX_COLOUR_MIX	(8)
#define MAX_BRIGHTNESS	(255)

#define BRIGHTNESS_BITS (0xFF000000)
#define RGB_BITS		(0x00FFFFFF)

//Accepts colours in 24bit RGB format
#define APPLY_BRIGHTNESS(colour, brightness)	((colour & RGB_BITS) | (brightness << 24))
#define GET_BRIGHTNESS(brightness_colour)		((brightness_colour & 0xFF000000) >> 24)


#define CM_ClearAll()	CM_Init()

void CM_Init(void);
void CM_AddColour(uint8_t index, uint32_t colour, uint8_t brightness);
void CM_RemoveColour(uint8_t index, uint32_t colour);

uint8_t CM_GetBrightness(uint8_t index, uint32_t colour);
uint32_t CM_GetMixedColour(uint8_t index);

uint32_t _CM_NomaliseColour(uint32_t r, uint32_t g, uint32_t b);
void CM_ReduceBrightnessPercentage(uint8_t index, int16_t percentage, uint8_t minBrightness);


#endif /* COLOURMIXER_H_ */
