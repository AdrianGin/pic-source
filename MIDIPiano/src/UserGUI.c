


#include "Graphics/gfxListbox.h"
#include "Graphics/gfxButton.h"
#include "Graphics/gfxSlider.h"

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

#include "MIDIPlayback/midiplayback.h"
#include "ProjectConfig/ProjectConfig.h"

#include "UserGUI.h"

#include "intertaskComm.h"

GFX_Listbox_t* GFX_LB;
GFX_FB_t GFX_FB;

gfxFrame_t GFX_MainFrame;

GFX_Button_t GFX_BUT1;
GFX_Slider_t GFX_SLD[2];

gfxWidget_t* GFX_WidgetHandles[10];


#define EXECUTION_DELAY	(10)
//Filebrowser Request.
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

	if( lastState == *state)
	{
		return PENDING_NO_FLAG;
	}

	BMP_SetCursor(BUT->fixedX, BUT->fixedY + BUT->height);
	BMP_SetRotation(1,-1, 0);
	switch( *state )
	{
	case BUT_ON:
		gfxDrawBMP("1:/PlayDown.bmp");
		break;

	case BUT_OFF:
	case BUT_CANCEL:
		gfxDrawBMP("1:/PlayUp.bmp");
		break;

	case BUT_TENTATIVE:
		gfxDrawBMP("1:/PlayDown.bmp");
		break;

	default:
		break;
	}

	if( (*state == BUT_ON) || (lastState == BUT_TENTATIVE && *state == BUT_OFF))
	{
		MPB_TogglePlayback(&MIDIHdr);
		lastState = *state;
		return PENDING_NO_FLAG;
	}
	else
	{
		lastState = *state;
		return PENDING_REDRAW_FLAG;
	}



}



uint8_t UserGUI_Slider(void* sld, void* data)
{
	uint8_t* state;
	GFX_Slider_t* SLD;
	static uint8_t lastState = 0xFF;

	state = (uint8_t*)data;
	SLD = (GFX_Slider_t*)sld;

	if( lastState == 0xFF )
	{
		GFX_SLIDER_Draw(SLD);
	}

	switch( *state )
	{
		case PENDING_ACTION_FLAG:
		case PENDING_NO_FLAG:
		case PENDING_REDRAW_FLAG:
			GFX_SLIDER_Draw(SLD);
			break;

		default:
			break;
	}

	lastState = *state;

	return PENDING_NO_FLAG;
}



uint8_t UserGUI_SliderReposition(void* sld, void* data)
{
	uint8_t* state;
	GFX_Slider_t* SLD;
	static uint8_t lastState = 0xFF;

	state = (uint8_t*)data;
	SLD = (GFX_Slider_t*)sld;

	if( lastState == 0xFF )
	{
		GFX_SLIDER_Draw(SLD);
	}

	xprintf("SLIDER: %d",  *state );

	switch( *state )
	{
		case PENDING_ACTION_FLAG:
			//if( MPB_GetPlaybackState(&MIDIHdr) == STATE_ACTIVE )
			{
				uint32_t position;
				MidiPlaybackState_t playbackState;
				position = GFX_SLIDER_GetPosition(SLD);

				xprintf("Position: %d\n",  position );

				position = (position * (MIDIHdr.currentState.maxLength / SLIDER_RESOLUTION));

				playbackState = MPB_GetPlaybackState(&MIDIHdr);
				MPB_PausePlayback(&MIDIHdr);
				MPB_ResetMIDI();
				MPB_RePosition(&MIDIHdr, position, MPB_PB_SAVE_MIDI_STATUS);
				MPB_ReplayStatusBuffer();
				MPB_SetPlaybackState(&MIDIHdr, playbackState);

				LS_ClearLights();
				MLL_ClearHaltList();
			}
			GFX_SLIDER_Draw(SLD);
			break;

		case PENDING_NO_FLAG:
			break;

		case PENDING_REDRAW_FLAG:
			GFX_SLIDER_Draw(SLD);

			break;

		default:
			break;
	}



	lastState = *state;

	return PENDING_NO_FLAG;

}




void UserGUI_Init(void* gfxLB)
{
	FONT_HEADER* fontHdr;
	GFX_Button_t* gfxBUT1;
	GFX_Slider_t* gfxSLD1;
	GFX_Slider_t* gfxSLD2;


	gfxBUT1 = (GFX_Button_t*)(&GFX_BUT1);

	GFX_LB = (GFX_Listbox_t*)gfxLB;
	fontHdr = (FONT_HEADER*)Tahoma_12.address;
	GFX_LB_Init(GFX_LB, 20, 20 , 300, MAX_LCD_Y-40-20, (void*)&Verdana_12 ,  5);
	GFX_LB->list.first = (LIST_NODE_t*)NULL;
	GFX_LB->list.last = (LIST_NODE_t*)NULL;
	GFX_LB->execFunc = ProcessFB_Request;
	GFX_LB_SetColour(GFX_LB, BLACK, WHITE, WHITE, BLACK, BLACK);



	GFX_BUT_Init(gfxBUT1, 20, MAX_LCD_Y-40 , 56, 40);
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


	gfxSLD1 = (GFX_Slider_t*)(&GFX_SLD[0]);
	GFX_SLIDER_Init(gfxSLD1, 90, MAX_LCD_Y-39 , 140, 20, SLIDER_X);

	gfxSLD1->execFunc = UserGUI_Slider;

	gfxFrame_AddWidget(&GFX_MainFrame, GFX_SLIDER,
			gfxSLD1->fixedX,
			gfxSLD1->fixedY,
			gfxSLD1->fixedX + gfxSLD1->width,
			gfxSLD1->fixedY + gfxSLD1->height,
			gfxSLD1);

	gfxSLD2 = (GFX_Slider_t*)(&GFX_SLD[1]);
	GFX_SLIDER_Init(gfxSLD2, 90, MAX_LCD_Y-19 , 140, 20, SLIDER_X);

	gfxSLD2->execFunc = UserGUI_SliderReposition;

	GFX_WidgetHandles[SEEK_SLIDER_WIDGET_INDEX] = gfxFrame_AddWidget(&GFX_MainFrame, GFX_SLIDER,
			gfxSLD2->fixedX,
			gfxSLD2->fixedY,
			gfxSLD2->fixedX + gfxSLD2->width,
			gfxSLD2->fixedY + gfxSLD2->height,
			gfxSLD2);

}



void UG_UpdateSeekSlider(void)
{
	uint32_t position;
	position = (MIDIHdr.masterClock * SLIDER_RESOLUTION) / MIDIHdr.currentState.maxLength;
	GFX_SLIDER_SetPositionRaw(&GFX_SLD[SEEK_SLIDER_INDEX], position);

	RedrawRequired = 1;
	gfxWidget_SetPendingFlags(GFX_WidgetHandles[SEEK_SLIDER_WIDGET_INDEX], PENDING_REDRAW_FLAG);
}




