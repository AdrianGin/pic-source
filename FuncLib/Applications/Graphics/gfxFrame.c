

#include "Coordinate/Coordinate.h"
#include "linkedlist/linkedlist.h"

#include "gfxListbox.h"

#include "gfxFrame.h"
#include "gfxWidget.h"
#include "GLCD/GLCD.h"

#include "FluidTouch/FluidTouch.h"

void gfxFrameInit()
{



}


void gfxFrame_AddWidget(gfxFrame_t* frame, int type, int x, int y, int xe, int ye, void* instance)
{
	gfxWidget_t* newWidget;
	newWidget = LL_Malloc(sizeof(gfxWidget_t));
	gfxWidget_Create(newWidget, type, x, y, xe, ye, instance);
	LL_AppendData(&frame->widgetList, (void*)newWidget);
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
		if( gfxWidget_CheckBoundaries(widget, point) ||
			(widget->pendingFlags) )
		{
			redrawRequired = gfxWidget_ProcessInput(widget);
			widget->pendingFlags &= ~PENDING_REDRAW_FLAG;
		}
		node = node->next;
	}



	return redrawRequired;
}

void gfxFrame_Delete(gfxFrame_t* frame)
{
	LL_DeleteListAndData(&frame->widgetList);
}












