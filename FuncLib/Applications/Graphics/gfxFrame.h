
#ifndef GFX_FRAME_H
#define GFX_FRAME_H


#include "Coordinate/Coordinate.h"
#include "linkedlist/linkedlist.h"
#include <stdint.h>

typedef enum
{
	GFX_LIST_BOX = 0,
	GFX_BUTTON,


} gfxWidgetType;


typedef struct
{
	int 		type;
	uint8_t		pendingAction;
	Coordinate 	start;
	Coordinate 	end;
	void* 		instance;
} gfxWidget_t;


typedef struct
{
	LINKED_LIST_t widgetList;
} gfxFrame_t;


#define PEDNING_ACTION	(1)

void gfxFrameInit();
void gfxWidget_Create(gfxWidget_t* widget, int type, int x, int y, int xe, int ye, void* instance);
void gfxFrame_AddWidget(gfxFrame_t* frame, int type, int x, int y, int xe, int ye, void* instance);
//Check to see if the point affects any widgets.;;
uint8_t gfxFrame_CheckWidgetBoundaries(gfxWidget_t* widget, Coordinate* point);
uint8_t gfxFrame_WidgetProcessInput(gfxWidget_t* widget);
uint8_t gfxFrame_ProcessInputs(gfxFrame_t* frame, Coordinate* point);
void gfxFrame_Delete(gfxFrame_t* frame);









#endif


