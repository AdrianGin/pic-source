


#include "Graphics\Listbox.h"
//#include "TouchPanel\FluidTouch.h" //need to decouple this from hardware
#include "Graphics/Tahoma.h"
#include "Graphics/ComicSans.h"
#include "Graphics/arial.h"
#include "Graphics/droidsansAA.h"
#include "GLCD/GLCD.h"

GFX_Listbox_t* GFX_LB;


void UserGUI_Init(void* gfxLB)
{
	FONT_HEADER* fontHdr;

	GFX_LB = (GFX_Listbox_t*)gfxLB;

	fontHdr = (FONT_HEADER*)Tahoma_12.address;

	GFX_LB_Init(GFX_LB, 20, 120 , 190, 300, (void*)&Verdana_12 ,  5);
	//GFX_LB_Init(&GFX_LB, 20, 120 , 190, 300, (void*)&Tahoma_12 ,  5);
	GFX_LB->list.first = (LIST_NODE_t*)NULL;
	GFX_LB->list.last = (LIST_NODE_t*)NULL;

	GFX_LB_SetColour(GFX_LB, BLACK, WHITE, WHITE, BLACK, BLACK);

}
