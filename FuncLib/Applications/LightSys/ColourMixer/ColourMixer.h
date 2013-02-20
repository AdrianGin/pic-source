/*
 * ColourMixer.h
 *
 *  Created on: 20/02/2013
 *      Author: Adrian
 */

#ifndef COLOURMIXER_H_
#define COLOURMIXER_H_

#include <stdint.h>

void CM_Init(void);
void CM_AddColour(uint8_t note, uint32_t colour, uint8_t brightness);
void CM_RemoveColour(uint8_t note, uint32_t colour);
uint32_t CM_GetMixedColour(uint8_t note);

uint32_t _CM_NomaliseColour(uint32_t r, uint32_t g, uint32_t b);

#endif /* COLOURMIXER_H_ */
