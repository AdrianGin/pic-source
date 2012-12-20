


#include "Graphics\gfxListbox.h"
#include "Graphics/gfxFrame.h"
//#include "TouchPanel\FluidTouch.h" //need to decouple this from hardware
#include "Graphics/Tahoma.h"
#include "Graphics/ComicSans.h"
#include "Graphics/arial.h"
#include "Graphics/droidsansAA.h"
#include "GLCD/GLCD.h"

#include "Graphics/gfxFileBrowser.h"

GFX_Listbox_t* GFX_LB;
GFX_FB_t GFX_FB;

gfxFrame_t GFX_MainFrame;

#define EXECUTION_DELAY	(10)
uint8_t ProcessFB_Request(void* LB, void* data)
{
	char* LBItem = (char*)data;
	static uint8_t actionDelay;

	GFX_FB_SELECT_ITEM ret;
	GFX_Listbox_t* gfxlb = (GFX_Listbox_t*)LB;


	if (actionDelay++ >= EXECUTION_DELAY)
	{
		actionDelay = 0;

		ret = GFX_FB_ProcessRequest(gfxlb, LBItem);

		if (ret != GFX_FB_NO_DIR_SELECTED)
		{
			if( ret == GFX_FB_DIR_SELECTED )
			{
				GFX_LB_SetPosition(gfxlb, gfxlb->fixedY);
			}
			FTI_ResetInertia(&gfxlb->inertia);
			GFX_LB_SelectItem(gfxlb, NO_SELECTION);
		}
		else
		{
			GFX_LB_SelectItem(gfxlb, NO_SELECTION);
			if( FSUtil_HasExtension(LBItem, ".mid") )
			{
				ExecuteMIDIFile(LBItem);
			}
		}
		GFX_LB_ResetTouchCounter(gfxlb);
		LBItem = NULL;
		return 0;
	}

	return PEDNING_ACTION_FLAG;
}


void UserGUI_Init(void* gfxLB)
{
	FONT_HEADER* fontHdr;

	GFX_LB = (GFX_Listbox_t*)gfxLB;
	fontHdr = (FONT_HEADER*)Tahoma_12.address;
	GFX_LB_Init(GFX_LB, 20, 20 , 180, 300, (void*)&Verdana_12 ,  5);
	//GFX_LB_Init(&GFX_LB, 20, 120 , 190, 300, (void*)&Tahoma_12 ,  5);
	GFX_LB->list.first = (LIST_NODE_t*)NULL;
	GFX_LB->list.last = (LIST_NODE_t*)NULL;
	GFX_LB->execFunc = ProcessFB_Request;


	GFX_LB_SetColour(GFX_LB, BLACK, WHITE, WHITE, BLACK, BLACK);


	gfxFrame_AddWidget(&GFX_MainFrame, GFX_LIST_BOX,
					   GFX_LB->fixedX,
					   GFX_LB->fixedY,
					   GFX_LB->fixedX + GFX_LB->width,
					   GFX_LB->fixedY + GFX_LB->height,
					   GFX_LB);


}








