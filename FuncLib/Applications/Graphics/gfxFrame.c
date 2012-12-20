

#include "Coordinate/Coordinate.h"
#include "linkedlist/linkedlist.h"

#include "gfxListbox.h"

#include "gfxFrame.h"
#include "GLCD/GLCD.h"

#include "FluidTouch/FluidTouch.h"

void gfxFrameInit()
{



}


void gfxWidget_Create(gfxWidget_t* widget, int type, int x, int y, int xe, int ye, void* instance)
{
	widget->type = type;
	widget->start.x = x;
	widget->start.y = y;
	widget->end.x = xe;
	widget->end.y = ye;
	widget->instance = instance;
	widget->pendingFlags = 0;

}

void gfxFrame_AddWidget(gfxFrame_t* frame, int type, int x, int y, int xe, int ye, void* instance)
{
	gfxWidget_t* newWidget;
	newWidget = LL_Malloc(sizeof(gfxWidget_t));
	gfxWidget_Create(newWidget, type, x, y, xe, ye, instance);
	LL_AppendData(&frame->widgetList, (void*)newWidget);
}

//Check to see if the point affects any widgets.
uint8_t gfxFrame_CheckWidgetBoundaries(gfxWidget_t* widget, Coordinate* point)
{
	if( (point->x >= widget->start.x) && (point->x <= widget->end.x) &&
		(point->y >= widget->start.y) && (point->y <= widget->end.y) )
	{
		return 1;
	}
	return 0;
}

#define REDRAW	(1)
uint8_t gfxFrame_WidgetProcessInput(gfxWidget_t* widget)
{
	uint8_t redrawRequired = 0;
	uint8_t pendingAction = widget->pendingFlags & PEDNING_ACTION_FLAG;

	switch(widget->type)
	{

		case GFX_LIST_BOX:
		{
			char* LBItem;
			GFX_Listbox_t* GFX_LB;

			GFX_LB = (GFX_Listbox_t*)widget->instance;

			//Any drag will stop any pending action
			if(!(widget->pendingFlags & PEDNING_ACTION_FLAG) || (FT_GetTouchState() == TOUCH_DRAG) )
			{
				redrawRequired = GFX_LB_ProcessTouchInputs( GFX_LB );
				widget->pendingFlags &= ~PEDNING_ACTION_FLAG;
			}
			LBItem = (char*) GFX_LB_ReturnSelectedItemPtr(GFX_LB);

			if( LBItem != NULL )
			{
				widget->pendingFlags = GFX_LB->execFunc(GFX_LB, LBItem);
			}

			if(redrawRequired || (pendingAction && (widget->pendingFlags == 0)) )
			{
				//LCD_Clear(WHITE);
				GFX_LB_Draw(GFX_LB);
				redrawRequired = 1;
			}
			break;
		}

		case GFX_BUTTON:
			break;

		default:
			break;
	}

	return redrawRequired;
}


void gfxFrame_RedrawWidget(gfxWidget_t* widget)
{
	switch(widget->type)
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


uint8_t gfxFrame_ProcessInputs(gfxFrame_t* frame, uint8_t state, Coordinate* point)
{
	LIST_NODE_t* node;
	gfxWidget_t* widget;
	uint8_t redrawRequired = 0;

	node = frame->widgetList.first;

	while(node)
	{
		widget = (gfxWidget_t*)node->data;
		if( gfxFrame_CheckWidgetBoundaries(widget, point) ||
			(widget->pendingFlags & PEDNING_ACTION_FLAG) )
		{
			redrawRequired = gfxFrame_WidgetProcessInput(widget);
		}
		node = node->next;
	}



	return redrawRequired;
}

void gfxFrame_Delete(gfxFrame_t* frame)
{
	LL_DeleteListAndData(&frame->widgetList);
}












