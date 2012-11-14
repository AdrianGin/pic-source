#ifndef _GFX_LB_H
#define _GFX_LB_H

#include <stdint.h>
#include "linkedlist\linkedlist.h"
//Requires some sort of FAT file System

typedef struct
{
	int16_t spacing;
	//position of the LB
	int16_t x, y;

	//Dimensions of the LB
	int16_t height, width;

	void* fontHdr;

	LINKED_LIST_t list;


} GFX_Listbox_t;

void GFX_LB_Init(GFX_Listbox_t* LB, int16_t x, int16_t y, int16_t height, int16_t width, void* fontHdr, uint16_t spacing);
void GFX_LB_AddItem(GFX_Listbox_t* LB, char* item);
void GFX_LB_Draw(GFX_Listbox_t* LB);

int16_t GFX_LB_GetPosition(GFX_Listbox_t* LB);
void GFX_LB_SetPosition(GFX_Listbox_t* LB, int16_t y);
void GFX_LB_Scroll(GFX_Listbox_t* LB, int16_t diff);
void GFX_LB_DrawSeparator(GFX_Listbox_t* LB);

#endif
