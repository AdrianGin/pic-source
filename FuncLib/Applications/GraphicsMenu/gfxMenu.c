/*
 * gfxMenu.c
 *
 *  Created on: 14/12/2012
 *      Author: Adrian
 */


#include <stdint.h>
#include <stddef.h>

#include "gfxMenu.h"


/* Example
typedef enum {

	NO_ENTRY = 0,
	MAIN_MENU,
	SETTINGS,



} GFX_MenuStates;


typedef enum {

	NO_WIDGET = 0,
	ENTER,
	EXIT,
	BUTTON1,
	BUTTON2,
	BUTTON3,

} GFX_Widgets;




MENU_TYPE gfxMenuEntry_t MainMenu[] =
{
		{EXIT, NULL},
		{BUTTON1, NULL},
		{NO_WIDGET, NULL},
};

MENU_TYPE gfxMenuOrganisation_t MenuStruct[] =
{
		{MAIN_MENU, (MENU_TYPE gfxMenuEntry_t**)&MainMenu},
		{NO_ENTRY,  (MENU_TYPE gfxMenuEntry_t**)NULL},
};
*/



void gfxMenu_Execute(gfxMenuOrganisation_t** menu, int parentState, int widget, void* data)
{
	int i;


	gfxMenuEntry_t* menuEntry = NULL;

	for( i = 0; menu[i]->parent; i++)
	{
		if( menu[i]->parent == parentState )
		{
			int j;
			for( j = 0; menu[i]->entries[j]; j++ )
			{
				if( menu[i]->entries[j]->widget == widget )
				{
					menuEntry = (gfxMenuEntry_t*)&menu[i]->entries;
					break;
				}
			}
			break;
		}
	}

	if( menuEntry != NULL )
	{
		if( menuEntry->function )
		{
			menuEntry->function(data);
		}
	}

}











