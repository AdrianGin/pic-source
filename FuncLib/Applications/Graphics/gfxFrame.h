
#ifndef GFX_FRAME_H
#define GFX_FRAME_H


#include "Coordinate/Coordinate.h"
#include "linkedlist/linkedlist.h"
#include <stdint.h>


typedef struct
{
	LINKED_LIST_t widgetList;
	uint32_t bkcolour;
} gfxFrame_t;




void gfxFrameInit(void);
void* gfxFrame_AddWidget(gfxFrame_t* frame, int type, int x, int y, int xe, int ye, void* instance);
uint8_t gfxFrame_ProcessInputs(gfxFrame_t* frame, uint8_t state, Coordinate* point);
void gfxFrame_Delete(gfxFrame_t* frame);









#endif


