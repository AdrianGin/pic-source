#ifndef _GFX_LB_H
#define _GFX_LB_H

#include <stdint.h>
#include "linkedlist\linkedlist.h"
#include "InertiaTouch\InertiaTouch.h"


#define MAX_SELECTED_ITEMS (3)
#define NO_SELECTION		(0xFF)
#define LB_REQUIRES_REDRAW	(1)
#define	LB_NO_REDRAW		(0)


typedef struct
{
	//There are spacings, SPACE FONT SPACE LINE
	int16_t spacing;
	//current position of the LB
	int16_t x, y;

	//The fix X-Y fix points of the LB
	int16_t fixedX, fixedY;

	//Dimensions of the LB
	int16_t height, width;

	void* fontHdr;

	LINKED_LIST_t list;

	//font colour
	uint16_t fontColour;
	//Background colour
	uint16_t bkColour;

	//Selected Font Colour
	uint16_t fontSelColour;
	//Selected Item colour bk
	uint16_t selColour;

	//Separator colour
	uint16_t sepColour;

	uint8_t selectedItems[MAX_SELECTED_ITEMS];
	uint8_t margin;

	InertiaElement_t inertia;
	uint8_t counters[1];

	uint8_t (*execFunc)(void* self, void* data);

} GFX_Listbox_t;

typedef enum {LB_OS_TOP, LB_OS_BOTTOM, LB_OS_LEFT, LB_OS_RIGHT, LS_OS_NONE} LB_OVERSHOOT_STATES;


void GFX_LB_Init(GFX_Listbox_t* LB, int16_t x, int16_t y, int16_t width, int16_t height, void* fontHdr, uint16_t spacing);
void GFX_LB_SetColour(GFX_Listbox_t* LB, uint16_t font, uint16_t bkground, uint16_t fontSel, uint16_t select, uint16_t sep);
void GFX_LB_SelectItem(GFX_Listbox_t* LB, uint8_t index);

void GFX_LB_DeleteListboxItems(GFX_Listbox_t* LB);
void GFX_LB_AddItem(GFX_Listbox_t* LB, char* item);
void GFX_LB_Draw(GFX_Listbox_t* LB);

int16_t GFX_LB_GetPosition(GFX_Listbox_t* LB);
uint8_t GFX_LB_SetPosition(GFX_Listbox_t* LB, int16_t y);
void GFX_LB_Scroll(GFX_Listbox_t* LB, int16_t diff);
void GFX_LB_DrawSeparator(GFX_Listbox_t* LB);

uint16_t GFX_LB_GetFontHeight(GFX_Listbox_t* LB);
int8_t GFX_LB_CalculateSelectedItem(GFX_Listbox_t* LB, int16_t y);

uint8_t GFX_LB_ReturnSelectedItemIndex(GFX_Listbox_t* LB);
void* GFX_LB_ReturnSelectedItemPtr(GFX_Listbox_t* LB);

uint8_t GFX_LB_ProcessTouchInputs(GFX_Listbox_t* LB, uint8_t state);

void GFX_LB_ResetTouchCounter(GFX_Listbox_t* LB);

#endif
