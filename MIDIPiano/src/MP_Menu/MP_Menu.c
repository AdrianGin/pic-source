#include <stdint.h>
#include <stddef.h>
#include "GraphicsMenu/gfxMenu.h"

#include "MP_Menu.h"




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





void MP_MenuInit(void)
{

}







