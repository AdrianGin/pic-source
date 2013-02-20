#ifndef _GFX_BUTTON_H
#define _GFX_BUTTON_H

#include <stdint.h>
#include "linkedlist\linkedlist.h"
#include "InertiaTouch\InertiaTouch.h"



typedef enum {BUT_OFF, BUT_ON, BUT_TENTATIVE, BUT_CANCEL, BUT_NO_STATE} BUTTON_STATES;


typedef struct
{
	//The fix X-Y fix points of the BUT
	int16_t fixedX, fixedY;
	//Dimensions of the BUT
	int16_t height, width;

	//On Colour
	uint16_t onColour;
	//Off Colour
	uint16_t offColour;
	//Tentative Select colour
	uint16_t tentativeColour;

	uint16_t counters[1];

	uint8_t (*execFunc)(void* self, void* data);

} GFX_Button_t;



void GFX_BUT_Init(GFX_Button_t* BUT, int16_t x, int16_t y, int16_t width, int16_t height);
void GFX_BUT_SetColour(GFX_Button_t* BUT, uint16_t butOn, uint16_t butOff, uint16_t butTent);

void GFX_BUT_Draw(GFX_Button_t* BUT, uint8_t state);
uint8_t GFX_BUT_ProcessTouchInputs(GFX_Button_t* BUT);
void GFX_BUT_ResetTouchCounter(GFX_Button_t* BUT);

#endif
