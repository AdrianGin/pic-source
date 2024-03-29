
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "linkedlist\linkedlist.h"
#include "gfxEngine.h"
#include "gfxlistbox.h"
#include "fontRender.h"

#include "FluidTouch\FluidTouch.h"
#include "InertiaTouch/InertiaTouch.h"


#define REQUIRE_TOUCH_OFF	(0xF0)

void GFX_LB_Init(GFX_Listbox_t* LB, int16_t x, int16_t y, int16_t width, int16_t height, void* fontHdr, uint16_t spacing)
{
	LB->x = x;
	LB->y = y;

	LB->fixedX = x;
	LB->fixedY = y;

	LB->spacing = spacing;
	LB->height = height;
	LB->width = width;
	LB->fontHdr = fontHdr;
	GFX_LB_SetColour(LB,0,0,0,0,0);
	LB->selectedItems[0] = NO_SELECTION;
	LB->margin = 5;

	LB->counters[TOUCH_ON] = REQUIRE_TOUCH_OFF;

	FTI_InitInertia(&LB->inertia);
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

uint8_t GFX_LB_ReturnSelectedItemIndex(GFX_Listbox_t* LB)
{
	return LB->selectedItems[0];
}

void* GFX_LB_ReturnSelectedItemPtr(GFX_Listbox_t* LB)
{
	return LL_ReturnNodeDataFromIndex(&LB->list, GFX_LB_ReturnSelectedItemIndex(LB));
}

void GFX_LB_DeleteListboxItems(GFX_Listbox_t* LB)
{
	LL_DeleteListAndData(&LB->list);
}

void GFX_LB_AddItem(GFX_Listbox_t* LB, char* item)
{
	char* newLBItem;
	newLBItem = LL_Malloc(strlen(item)+1);
	strcpy(newLBItem, item);
	LL_AppendData(&LB->list, newLBItem);
}


int16_t GFX_LB_GetPosition(GFX_Listbox_t* LB)
{
	return LB->y;
}

//#define MIN_HEIGHT	(200)
//#define Y_BOUNDARY	(200)
uint8_t GFX_LB_SetPosition(GFX_Listbox_t* LB, int16_t y)
{

	FONT_HEADER* fontHdr;
	int16_t totalSpacing;

	fontHdr = (FONT_HEADER*)((FONT_FLASH*)LB->fontHdr)->address;

	totalSpacing = fontHdr->height + LB->spacing*2;


	if(y + (LL_Count(&LB->list) * totalSpacing) < (LB->height+LB->fixedY))
	{
		LB->y = (LB->fixedY+LB->height) - (LL_Count(&LB->list) * totalSpacing);
		y = LB->y;
		if(((LL_Count(&LB->list) * totalSpacing) < LB->height))
		{
			LB->y = LB->fixedY;
			return LB_OS_TOP;

		}

		return LB_OS_BOTTOM;

	}
	else
	{
		LB->y = y;
	}

	if( y > LB->fixedY )
	{
		LB->y = LB->fixedY;
		return LB_OS_TOP;
		//LB->y = y;
	}
	else
	{
		LB->y = y;
	}

	return LS_OS_NONE;
}


int8_t GFX_LB_CalculateSelectedItem(GFX_Listbox_t* LB, int16_t y)
{
	int16_t diff;
	int16_t divisor;
	uint16_t fontHeight;
	fontHeight =  GFX_LB_GetFontHeight(LB);

	diff = y - LB->y;

	if( diff < 0 )
	{
		return NO_SELECTION;
	}
	divisor = (LB->spacing * 2) + fontHeight;

	return (diff / divisor) ;
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

	uint8_t index = 0;
	uint16_t fontHeight;

	fontHeight =  GFX_LB_GetFontHeight(LB);

	node = LB->list.first;
	initPosition = LB->y;

	SetFontFlash( (void*)LB->fontHdr);


	//Colour the Box in the background fill colour
	//gfxFillRect()

	gfxSetColour(LB->bkColour);
	gfxFillRect(LB->fixedX,
			LB->fixedY,
			LB->fixedX + LB->width,
			LB->fixedY + LB->height);

	SetClip(1);
	SetClipRgn(LB->fixedX, LB->fixedY, LB->fixedX + LB->width, LB->fixedY + LB->height);


	while( node != NULL )
	{

		LB->y = LB->y + (LB->spacing);
		if( LB->y > MAX_LCD_Y)
		{
			break;
		}

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

	SetClip(0);
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





void GFX_LB_ResetTouchCounter(GFX_Listbox_t* LB)
{
	LB->counters[TOUCH_ON] = REQUIRE_TOUCH_OFF;
}

uint8_t GFX_LB_ProcessTouchInputs(GFX_Listbox_t* LB, uint8_t state)
{
	Coordinate* point;
	uint8_t ret = LB_NO_REDRAW;
	static uint8_t dragCount = 0;


	switch( state )
	{

		case TOUCH_TAP:
		case TOUCH_LONG:
		{
			if( LB->counters[TOUCH_ON]==0 )
			{
				point = FT_GetLastPoint();
				GFX_LB_SelectItem(LB, GFX_LB_CalculateSelectedItem(LB, point->y));
				dragCount = 0;
				ret = LB_REQUIRES_REDRAW;
			}
			break;
		}

		case TOUCH_ON:
		{
			//This here is a STOP while dragging.
			if( !FTI_InertiaIsZero(&LB->inertia) )
			{
				LB->counters[TOUCH_ON]++;
				if( LB->counters[TOUCH_ON] >= 2 )
				{
					FTI_ResetInertia(&LB->inertia);
					GFX_LB_Scroll(LB, LB->inertia.Value.y);
					ret = LB_REQUIRES_REDRAW;
				}
			}
			break;
		}

		case TOUCH_DRAG:
		{
			point = FT_GetDiff();
			//Acts as a filter. So DRAG STOPs result in Inertia being reset.
			if( Coordinate_IsLessThan(point, FT_MOVE_THRES) )
			{
				dragCount++;
				if( dragCount >= 2 )
				{
					FTI_ResetInertia(&LB->inertia);
				}
			}
			else
			{
				FTI_ResetInertia(&LB->inertia);
				dragCount = 0;
			}

			//Deselect while dragging
			GFX_LB_SelectItem(LB, NO_SELECTION);
			FTI_UpdateInertia(&LB->inertia, FT_GetDiff());
			GFX_LB_Scroll(LB, LB->inertia.Value.y);
			FTI_ApplySlowdown(&LB->inertia);
			FT_ResetDiff();
			ret = LB_REQUIRES_REDRAW;
			break;
		}

		case TOUCH_OFF:
		{
			LB->counters[TOUCH_ON] = 0;
			dragCount = 0;
			break;
		}

		default:
		{
			break;
		}

	}


	if( state != TOUCH_DRAG )
	{
		//The release of the Drag.
		if( !FTI_InertiaIsZero(&LB->inertia) )
		{
			GFX_LB_Scroll(LB, LB->inertia.Value.y);
			FTI_ApplySlowdown(&LB->inertia);
			FT_ResetDiff();
			ret = LB_REQUIRES_REDRAW;
		}
	}


	return ret;

}


