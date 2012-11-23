
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
	GFX_LB_SetColour(LB,0,0,0,0,0);
	LB->selectedItems[0] = NULL;
	LB->margin = 5;
}

void GFX_LB_SetColour(GFX_Listbox_t* LB, uint16_t font, uint16_t bkground, uint16_t fontSel, uint16_t select, uint16_t sep)
{
	LB->fontColour = font;
	LB->bkColour = bkground;
	LB->selColour = select;
	LB->fontSelColour = fontSel;
	LB->sepColour = sep;
}

void GFX_LB_SelectItem(GFX_Listbox_t* LB, uint8_t index)
{
	LB->selectedItems[0] = index;
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



uint8_t GFX_LB_SetPosition(GFX_Listbox_t* LB, int16_t y)
{

	FONT_HEADER* fontHdr;
	int16_t totalSpacing;

	fontHdr = (FONT_HEADER*)((FONT_FLASH*)LB->fontHdr)->address;

	totalSpacing = fontHdr->height + LB->spacing*2;


	if( y + ((LL_Count(&LB->list)) * totalSpacing) < Y_BOUNDARY )
	{
		LB->y = Y_BOUNDARY - (LL_Count(&LB->list)) * totalSpacing;
		y = LB->y;
		return LB_OS_BOTTOM;
	}
	else
	{
		LB->y = y;
	}

	if( y > MIN_HEIGHT )
	{
		LB->y = MIN_HEIGHT;
		return LB_OS_TOP;
		//LB->y = y;
	}
	else
	{
		LB->y = y;
	}

	return LS_OS_NONE;
}


int8_t GFX_LB_GetSelectedItem(GFX_Listbox_t* LB, int16_t y)
{
	uint8_t selItem = 0xFF;
	int16_t diff;
	int16_t divisor;
	uint16_t fontHeight;
	fontHeight =  GFX_LB_GetFontHeight(LB);

	diff = y - LB->y;

	divisor = (LB->spacing * 2) + fontHeight;

	return (diff / divisor);
}


void GFX_LB_DoOvershoot(GFX_Listbox_t* LB, int16_t diff)
{
	LB->y = LB->y + diff;
}



void GFX_LB_Scroll(GFX_Listbox_t* LB, int16_t diff)
{
	uint8_t osState;
	osState = GFX_LB_SetPosition(LB, LB->y + diff);

	if( osState == LB_OS_BOTTOM || osState == LB_OS_TOP )
	{
		GFX_LB_DoOvershoot(LB, diff);
	}
}


void GFX_LB_Draw(GFX_Listbox_t* LB)
{

	LIST_NODE_t* node;
	int16_t initPosition;

	uint16_t x,y;
	uint8_t index = 0;
	uint16_t fontHeight;

	fontHeight =  GFX_LB_GetFontHeight(LB);

	node = LB->list.first;
	initPosition = LB->y;

	SetFontFlash( (void*)LB->fontHdr);


	//Colour the Box in the background fill colour
	//gfxFillRect()

	gfxSetColour(LB->bkColour);
	//gfxFillRect(LB->x, LB->y, LB->x + LB->width, LB->y + LB->height);


	while( node != NULL )
	{

		LB->y = LB->y + (LB->spacing);

		gfxSetColour(LB->fontColour);
		if( index == LB->selectedItems[0] )
		{
			gfxSetColour(LB->selColour);
			gfxFillRect(LB->x, LB->y - LB->spacing + 1, LB->x + LB->width, LB->y + LB->spacing + fontHeight);
			gfxSetColour(LB->fontSelColour);
		}

		gfxWriteString(LB->x + LB->margin, LB->y, node->data);
		LB->y = LB->y +  fontHeight + (LB->spacing);
		node = node->next;
		gfxSetColour(LB->sepColour);
		GFX_LB_DrawSeparator(LB);



		index++;
	}

	LB->y = initPosition;
}


#define BAR_SIZE	(0)
void GFX_LB_DrawSeparator(GFX_Listbox_t* LB)
{
	gfxFillRect(LB->x, LB->y, LB->x + LB->width, LB->y + BAR_SIZE);
}


uint16_t GFX_LB_GetFontHeight(GFX_Listbox_t* LB)
{
	FONT_HEADER* fontHdr;
	fontHdr = (FONT_HEADER*)((FONT_FLASH*)LB->fontHdr)->address;
	return fontHdr->height;
}




