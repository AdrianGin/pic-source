
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "linkedlist\linkedlist.h"
#include "gfxEngine.h"

#include "listbox.h"

#include "fontRender.h"




void GFX_LB_Init(GFX_Listbox_t* LB, int16_t x, int16_t y, int16_t height, int16_t width, void* fontHdr, uint16_t spacing)
{
	LB->x = x;
	LB->y = y;
	LB->spacing = spacing;
	LB->height = height;
	LB->width = width;
	LB->fontHdr = fontHdr;
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

	FONT_HEADER* fontHdr;
	int16_t totalSpacing;

	fontHdr = (FONT_HEADER*)((FONT_FLASH*)LB->fontHdr)->address;

	totalSpacing = fontHdr->height + LB->spacing*2;


	if( y + ((LL_Count(&LB->list)) * totalSpacing) < Y_BOUNDARY )
	{
		LB->y = Y_BOUNDARY - (LL_Count(&LB->list)) * totalSpacing;
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

	FONT_HEADER* fontHdr;

	node = LB->list.first;
	initPosition = LB->y;

	SetFontFlash( (void*)LB->fontHdr);

	fontHdr = (FONT_HEADER*)((FONT_FLASH*)LB->fontHdr)->address;


	while( node != NULL )
	{

		gfxWriteString(LB->x, LB->y, node->data);
		LB->y = LB->y +  fontHdr->height + (LB->spacing);

		node = node->next;
		GFX_LB_DrawSeparator(LB);
		LB->y = LB->y + (LB->spacing);
	}

	LB->y = initPosition;
}


#define BAR_SIZE	(0)
void GFX_LB_DrawSeparator(GFX_Listbox_t* LB)
{
	gfxDrawRect(LB->x, LB->y, LB->x + LB->width, LB->y + BAR_SIZE);
}






