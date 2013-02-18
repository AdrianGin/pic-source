
#ifndef GFX_WIDGET_H
#define GFX_WIDGET_H


#include "Coordinate/Coordinate.h"
#include "linkedlist/linkedlist.h"
#include <stdint.h>




typedef enum
{
	PENDING_NO_FLAG	    = (0<<0),
	PENDING_ACTION_FLAG	= (1<<0),
	PENDING_REDRAW_FLAG	= (1<<1)

} gfxPendingActions;


typedef enum
{
	GFX_LIST_BOX = 0,
	GFX_BUTTON,
	GFX_SLIDER,


} gfxWidgetType;


typedef struct
{
	int 		type;
	uint8_t		pendingFlags;
	Coordinate 	start;
	Coordinate 	end;
	void* 		instance;
} gfxWidget_t;


void gfxWidget_Create(gfxWidget_t* widget, int type, int x, int y, int xe, int ye, void* instance);
//Check to see if the point affects any widgets.;;
uint8_t gfxWidget_CheckBoundaries(gfxWidget_t* widget, Coordinate* point);
uint8_t gfxWidget_ProcessInput(gfxWidget_t* widget);









#endif


