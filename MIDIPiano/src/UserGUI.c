


#include "Graphics\gfxListbox.h"
#include "Graphics/gfxButton.h"

#include "Graphics/BMPDraw.h"
#include "Graphics/gfxEngine.h"
#include "Graphics/gfxFrame.h"
#include "Graphics/gfxWidget.h"
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

GFX_Button_t GFX_BUT1;


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

	return PENDING_ACTION_FLAG;
}


uint8_t UserGUI_PlayButton(void* but, void* data )
{
	uint8_t* state;
	GFX_Button_t* BUT;
	static uint8_t lastState = BUT_NO_STATE;

	state = (uint8_t*)data;
	BUT = (GFX_Button_t*)but;

	BMP_SetCursor(BUT->fixedX, BUT->fixedY + BUT->height);
	BMP_SetRotation(1,-1, 0);


	if( lastState == *state)
	{
		return PENDING_NO_FLAG;
	}

	switch( *state )
	{
	case BUT_ON:
		gfxDrawBMP("1:/PlayDown.bmp");
		break;

	case BUT_OFF:
		gfxDrawBMP("1:/PlayUp.bmp");
		break;

	case BUT_TENTATIVE:
		gfxDrawBMP("1:/PlayDown.bmp");
		break;

	default:
		break;
	}

	lastState = *state;

	return PENDING_ACTION_FLAG;

}


void UserGUI_Init(void* gfxLB)
{
	FONT_HEADER* fontHdr;
	GFX_Button_t* gfxBUT1;


	gfxBUT1 = (GFX_Button_t*)(&GFX_BUT1);

	GFX_LB = (GFX_Listbox_t*)gfxLB;
	fontHdr = (FONT_HEADER*)Tahoma_12.address;
	GFX_LB_Init(GFX_LB, 20, 20 , MAX_LCD_Y-40-20, 300, (void*)&Verdana_12 ,  5);
	GFX_LB->list.first = (LIST_NODE_t*)NULL;
	GFX_LB->list.last = (LIST_NODE_t*)NULL;
	GFX_LB->execFunc = ProcessFB_Request;
	GFX_LB_SetColour(GFX_LB, BLACK, WHITE, WHITE, BLACK, BLACK);



	GFX_BUT_Init(gfxBUT1, 20, MAX_LCD_Y-40 , 40, 56);
	gfxBUT1->execFunc = UserGUI_PlayButton;
	GFX_BUT_SetColour(gfxBUT1, BLACK, WHITE, BLUE);


	gfxFrame_AddWidget(&GFX_MainFrame, GFX_LIST_BOX,
					   GFX_LB->fixedX,
					   GFX_LB->fixedY,
					   GFX_LB->fixedX + GFX_LB->width,
					   GFX_LB->fixedY + GFX_LB->height,
					   GFX_LB);


	gfxFrame_AddWidget(&GFX_MainFrame, GFX_BUTTON,
			gfxBUT1->fixedX,
			gfxBUT1->fixedY,
			gfxBUT1->fixedX + gfxBUT1->width,
			gfxBUT1->fixedY + gfxBUT1->height,
			gfxBUT1);



}








