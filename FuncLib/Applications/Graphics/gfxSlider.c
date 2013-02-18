
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "linkedlist\linkedlist.h"
#include "gfxEngine.h"
#include "gfxSlider.h"
#include "gfxWidget.h"
#include "fontRender.h"

#include "FluidTouch\FluidTouch.h"
#include "InertiaTouch/InertiaTouch.h"


void GFX_SLIDER_Init(GFX_Slider_t* SLIDER, int16_t x, int16_t y, int16_t width, int16_t height, SLIDER_TYPE_t sliderType)
{
	SLIDER->fixedX = x;
	SLIDER->fixedY = y;

	SLIDER->height = height;
	SLIDER->width = width;

	SLIDER->sliderType = sliderType;

	GFX_SLIDER_SetColour(SLIDER, RGB565CONVERT(0,0,0), RGB565CONVERT(255,255,255));

}

void GFX_SLIDER_SetColour(GFX_Slider_t* SLIDER, uint16_t sliderColour, uint16_t bgColour)
{
	SLIDER->sliderColour = sliderColour;
	SLIDER->bgColour = bgColour;
}

#define SLIDER_THICKNESS (2)

//Example of a DrawButton.
void GFX_SLIDER_Draw(GFX_Slider_t* SLIDER)
{
	//Colour the Box in the background fill colour

	int16_t xPosition;
	int16_t yPosition;


	SetClip(1);
	SetClipRgn(SLIDER->fixedX, SLIDER->fixedY, SLIDER->fixedX + SLIDER->width, SLIDER->fixedY + SLIDER->height);


	gfxSetColour(SLIDER->bgColour);
	gfxFillRect(SLIDER->fixedX,
			SLIDER->fixedY,
			SLIDER->fixedX + SLIDER->width,
			SLIDER->fixedY + SLIDER->height);

	gfxSetColour(SLIDER->sliderColour);


	if(SLIDER->sliderType == SLIDER_X)
	{
		gfxFillRect(SLIDER->fixedX + SLIDER->position - SLIDER_THICKNESS,
					SLIDER->fixedY,
					SLIDER->fixedX + SLIDER->position + SLIDER_THICKNESS,
					SLIDER->fixedY + SLIDER->height);

	}
	else
	{
		gfxFillRect(SLIDER->fixedX,
					SLIDER->fixedY + SLIDER->position - SLIDER_THICKNESS,
					SLIDER->fixedX + SLIDER->width,
					SLIDER->fixedY + SLIDER->position + SLIDER_THICKNESS);
	}



	SetClip(0);
}



#define SLIDER_RESOLUTION (1000)
int16_t GFX_SLIDER_GetPosition(GFX_Slider_t* SLIDER)
{

	int16_t normPos;

	if(SLIDER->sliderType == SLIDER_X)
	{
		normPos = (SLIDER->position * SLIDER_RESOLUTION) / SLIDER->fixedX;
	}
	else
	{
		normPos = (SLIDER->position * SLIDER_RESOLUTION) / SLIDER->fixedY;
	}
	return normPos;
}

void GFX_SLIDER_SetPosition(GFX_Slider_t* SLIDER, Coordinate* point)
{
	if(SLIDER->sliderType == SLIDER_X)
	{
		SLIDER->position = point->x - SLIDER->fixedX;
	}
	else
	{
		SLIDER->position = point->y - SLIDER->fixedY;
	}
}

uint8_t GFX_SLIDER_ProcessTouchInputs(GFX_Slider_t* SLIDER)
{
	FT_STATES state;
	Coordinate* point;
	uint8_t ret = PENDING_NO_FLAG;
	state = FT_GetTouchState();

	switch( state )
	{
		case TOUCH_LONG:
		case TOUCH_ON:
		case TOUCH_TAP:
		case TOUCH_DRAG:
		{
			{
				point = FT_GetLastPoint();
				GFX_SLIDER_SetPosition(SLIDER, point);
				ret = PENDING_REDRAW_FLAG;
			}
			break;
		}

		case TOUCH_OFF:
		{
			break;
		}

		default:
		{
			break;
		}
	}

	return ret;

}


