
#include "Coordinate/Coordinate.h"
#include "linkedlist/linkedlist.h"

#include "gfxListbox.h"
#include "gfxButton.h"
#include "gfxSlider.h"
#include "gfxWidget.h"
#include "GLCD/GLCD.h"

#include "FluidTouch/FluidTouch.h"

void gfxWidget_Create(gfxWidget_t* widget, int type, int x, int y, int xe,
		int ye, void* instance)
{
	widget->type = type;
	widget->start.x = x;
	widget->start.y = y;
	widget->end.x = xe;
	widget->end.y = ye;
	widget->instance = instance;
	widget->currentState = 0;
	widget->pendingFlags = PENDING_REDRAW_FLAG;

}

void gfxWidget_SetPendingFlags(gfxWidget_t* widget, uint8_t flags)
{
	widget->pendingFlags = flags;
}

//Check to see if the point affects any widgets.
uint8_t gfxWidget_CheckBoundaries(gfxWidget_t* widget, Coordinate* point)
{
	//xprintf("PT: %d, %d\n", point->x, point->y);
	//xprintf("WD: %d->%d, %d->%d\n",  widget->start.x, widget->end.x, widget->start.y, widget->end.y);

	if ((point->x >= widget->start.x) && (point->x <= widget->end.x)
			&& (point->y >= widget->start.y) && (point->y <= widget->end.y))
	{
		return 1;
	}
	return 0;
}

uint8_t gfxWidget_ProcessInput(gfxWidget_t* widget)
{
	uint8_t redrawRequired = 0;
	uint8_t pendingAction = widget->pendingFlags & PENDING_ACTION_FLAG;

	switch (widget->type)
	{

	case GFX_LIST_BOX:
	{
		char* LBItem;
		GFX_Listbox_t* GFX_LB;

		GFX_LB = (GFX_Listbox_t*) widget->instance;

		//Any drag will stop any pending action
		{
			redrawRequired = GFX_LB_ProcessTouchInputs( GFX_LB, widget->currentState );
			widget->pendingFlags &= ~PENDING_ACTION_FLAG;
		}
		LBItem = (char*) GFX_LB_ReturnSelectedItemPtr(GFX_LB);

		if (LBItem != NULL)
		{
			if( GFX_LB->execFunc != NULL )
			{
				widget->pendingFlags = GFX_LB->execFunc(GFX_LB, LBItem);
				redrawRequired = 1;
			}
		}

		if (redrawRequired || (pendingAction && (widget->pendingFlags == PENDING_NO_FLAG))
				|| (widget->pendingFlags & PENDING_REDRAW_FLAG))
		{
			//LCD_Clear(WHITE);
			GFX_LB_Draw(GFX_LB);

		}
		break;
	}

	case GFX_BUTTON:
	{
		GFX_Button_t* GFX_BUT;
		GFX_BUT = (GFX_Button_t*) widget->instance;
		{
			uint8_t butState;
			butState = GFX_BUT_ProcessTouchInputs(GFX_BUT, widget->currentState);

			if( GFX_BUT->execFunc != NULL )
			{
				widget->pendingFlags = GFX_BUT->execFunc(GFX_BUT, &butState);
			}

			redrawRequired = 1;

//			if( butState != BUT_OFF)
//			{
//				widget->pendingFlags = PENDING_ACTION_FLAG;
//			}
//			else
//			{
//				widget->pendingFlags = PENDING_NO_FLAG;
//			}

			//GFX_BUT_Draw(GFX_BUT, butState);

		}
		break;
	}

	case GFX_SLIDER:
	{
		GFX_Slider_t* GFX_SLD;
		GFX_SLD = (GFX_Slider_t*) widget->instance;
		{
			uint8_t state = widget->pendingFlags;
			if( widget->pendingFlags == PENDING_ACTION_FLAG)
			{
				state = GFX_SLIDER_ProcessTouchInputs(GFX_SLD, widget->currentState);
			}

			if( GFX_SLD->execFunc != NULL )
			{
				widget->pendingFlags = GFX_SLD->execFunc(GFX_SLD, &state);
			}
			redrawRequired = 1;
			//GFX_BUT_Draw(GFX_BUT, butState);
		}
		break;
	}




	default:
		break;
	}

	return redrawRequired;
}

void gfxWidget_Redraw(gfxWidget_t* widget)
{
	switch (widget->type)
	{

		case GFX_LIST_BOX:
		{
			//GFX_LB_Draw(GFX_LB);
			break;
		}

		case GFX_BUTTON:
			break;

		default:
			break;
	}
}

