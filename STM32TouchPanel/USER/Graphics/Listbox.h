#ifndef _GFX_LB_H
#define _GFX_LB_H

#include <stdint.h>
#include "linkedlist\linkedlist.h"
//Requires some sort of FAT file System

typedef struct
{
	int16_t spacing;
	int16_t x, y;

	LINKED_LIST_t list;


} GFX_Listbox_t;

void GFX_LB_Init(GFX_Listbox_t* LB, int16_t x, int16_t y, uint16_t spacing);
void GFX_LB_AddItem(GFX_Listbox_t* LB, char* item);
void GFX_LB_Draw(GFX_Listbox_t* LB);

int16_t GFX_LB_GetPosition(GFX_Listbox_t* LB);
void GFX_LB_SetPosition(GFX_Listbox_t* LB, int16_t y);
void GFX_LB_Scroll(GFX_Listbox_t* LB, int16_t diff);

#endif
