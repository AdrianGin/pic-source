
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "linkedlist\linkedlist.h"
#include "gfxEngine.h"

#include "listbox.h"






void GFX_LB_Init(GFX_Listbox_t* LB, int16_t x, int16_t y, uint16_t spacing)
{
	LB->x = x;
	LB->y = y;
	LB->spacing = spacing;
}


void GFX_LB_AddItem(GFX_Listbox_t* LB, char* item)
{
	LL_AppendData(&LB->list, item);
}


int16_t GFX_LB_GetPosition(GFX_Listbox_t* LB)
{
	return LB->y;
}

#define MIN_HEIGHT	(200)
#define Y_BOUNDARY	(240)


void GFX_LB_SetPosition(GFX_Listbox_t* LB, int16_t y)
{
	if( y + ((LL_Count(&LB->list)) * LB->spacing) < Y_BOUNDARY )
	{
		LB->y = Y_BOUNDARY - (LL_Count(&LB->list)) * LB->spacing;
		y = LB->y;
	}
	else
	{
		LB->y = y;
	}

	if( y > MIN_HEIGHT )
	{
		LB->y = MIN_HEIGHT;
		//LB->y = y;
	}
	else
	{
		LB->y = y;
	}
}

void GFX_LB_Scroll(GFX_Listbox_t* LB, int16_t diff)
{
	GFX_LB_SetPosition(LB, LB->y + diff);
}

void GFX_LB_Draw(GFX_Listbox_t* LB)
{

	LIST_NODE_t* node;
	int16_t initPosition;

	node = LB->list.first;
	initPosition = LB->y;

	while( node != NULL )
	{
		gfxWriteString(LB->x, LB->y, node->data);
		LB->y = LB->y + LB->spacing;
		node = node->next;
	}

	LB->y = initPosition;
}






