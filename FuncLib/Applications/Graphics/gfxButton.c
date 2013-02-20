
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "linkedlist\linkedlist.h"
#include "gfxEngine.h"
#include "gfxlistbox.h"
#include "gfxButton.h"
#include "fontRender.h"

#include "FluidTouch\FluidTouch.h"
#include "InertiaTouch/InertiaTouch.h"


#define REQUIRE_TOUCH_OFF	(0xF0)

void GFX_BUT_Init(GFX_Button_t* BUT, int16_t x, int16_t y, int16_t width, int16_t height)
{
	BUT->fixedX = x;
	BUT->fixedY = y;

	BUT->height = height;
	BUT->width = width;

	GFX_BUT_SetColour(BUT,0,0,0);

}

void GFX_BUT_SetColour(GFX_Button_t* BUT, uint16_t butOn, uint16_t butOff, uint16_t butTent)
{
	BUT->onColour = butOn;
	BUT->offColour = butOff;
	BUT->tentativeColour = butTent;
}


//Example of a DrawButton.
void GFX_BUT_Draw(GFX_Button_t* BUT, uint8_t state)
{
	//Colour the Box in the background fill colour
	//gfxFillRect()

	switch( state )
	{
	case BUT_ON:
		gfxSetColour(BUT->onColour);
		break;

	case BUT_OFF:
		gfxSetColour(BUT->offColour);
		break;

	case BUT_TENTATIVE:
		gfxSetColour(BUT->tentativeColour);
		break;

	default:
		break;
	}

	gfxFillRect(BUT->fixedX,
			BUT->fixedY,
			BUT->fixedX + BUT->width,
			BUT->fixedY + BUT->height);

	SetClip(1);
	SetClipRgn(BUT->fixedX, BUT->fixedY, BUT->fixedX + BUT->width, BUT->fixedY + BUT->height);
	SetClip(0);
}



void GFX_BUT_ResetTouchCounter(GFX_Button_t* BUT)
{
	BUT->counters[TOUCH_ON] = REQUIRE_TOUCH_OFF;
}

uint8_t GFX_BUT_ProcessTouchInputs(GFX_Button_t* BUT)
{
	FT_STATES state;
	Coordinate* point;
	uint8_t ret = BUT_OFF;
	state = FT_GetTouchState();

	switch( state )
	{

		//For tap debounce
		case TOUCH_TAP:
		{
			if( BUT->counters[TOUCH_ON]==0 )
			{
				point = FT_GetLastPoint();
				ret = BUT_ON;
			}
			break;
		}

		case TOUCH_LONG:
		case TOUCH_ON:
		{
			if( BUT->counters[TOUCH_ON]==0 )
			{
				point = FT_GetLastPoint();
				ret = BUT_TENTATIVE;
			}
			break;
		}

		case TOUCH_DRAG:
		{
			ret = BUT_CANCEL;
			break;
		}

		case TOUCH_OFF:
		{
			BUT->counters[TOUCH_ON] = 0;
			break;
		}

		default:
		{
			break;
		}

	}

	return ret;

}


