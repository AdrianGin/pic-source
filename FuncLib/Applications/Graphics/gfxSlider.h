#ifndef _GFX_SLIDER_H
#define _GFX_SLIDER_H

#include <stdint.h>
#include "linkedlist\linkedlist.h"
#include "InertiaTouch\InertiaTouch.h"


#define SLIDER_REQUIRES_REDRAW	(1)
#define	SLIDER_NO_REDRAW		(0)

typedef enum {SLIDER_X = 0, SLIDER_Y} SLIDER_TYPE_t;

typedef struct
{
	//The fix X-Y fix points of the Slider
	int16_t fixedX, fixedY;
	//Dimensions of the Slider
	int16_t height, width;

	int16_t position;
	SLIDER_TYPE_t sliderType;
	uint16_t sliderColour;
	uint16_t bgColour;

	uint8_t (*execFunc)(void* self, void* data);

} GFX_Slider_t;

#define SLIDER_RESOLUTION (1000)


void GFX_SLIDER_Init(GFX_Slider_t* SLIDER, int16_t x, int16_t y, int16_t width, int16_t height, SLIDER_TYPE_t sliderType);
void GFX_SLIDER_SetColour(GFX_Slider_t* SLIDER, uint16_t sliderColour, uint16_t bgColour);
void GFX_SLIDER_Draw(GFX_Slider_t* SLIDER);


uint16_t GFX_SLIDER_GetPosition(GFX_Slider_t* SLIDER);
void GFX_SLIDER_SetPosition(GFX_Slider_t* SLIDER, Coordinate* point);
uint8_t GFX_SLIDER_ProcessTouchInputs(GFX_Slider_t* LB, uint8_t state);
void GFX_SLIDER_SetPositionRaw(GFX_Slider_t* SLIDER, uint16_t position);


#endif
