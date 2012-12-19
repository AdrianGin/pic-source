/*
 * gfxMenu.h
 *
 *  Created on: 14/12/2012
 *      Author: Adrian
 */

#ifndef GFXMENU_H_
#define GFXMENU_H_


#define MENU_TYPE		const
#define MENU_ATTRIBUTE


#define MENU_NO_ENTRY	(0)

typedef struct
{
	int widget;
	void (*function)(void* data);
} gfxMenuEntry_t;



typedef struct
{
	int parent;
	MENU_TYPE gfxMenuEntry_t** entries;
} gfxMenuOrganisation_t;


#endif /* GFXMENU_H_ */
