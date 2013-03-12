
#ifndef USER_GUI_H
#define USER_GUI_H


#include "Graphics/gfxListbox.h"
#include "Graphics/gfxFileBrowser.h"
#include "Graphics/gfxFrame.h"
#include "Graphics/gfxSlider.h"
#include "Graphics/gfxWidget.h"

extern GFX_Listbox_t* GFX_LB;
extern GFX_FB_t GFX_FB;
extern gfxFrame_t GFX_MainFrame;
extern GFX_Slider_t GFX_SLD[];
extern gfxWidget_t* GFX_WidgetHandles[];

#define SEEK_SLIDER_INDEX (1)
#define SEEK_SLIDER_WIDGET_INDEX (1)

void UserGUI_Init(void* gfxLB);
uint8_t ProcessFB_Request(void* LB, void* data);
void ExecuteMIDIFile(char* LBItem);

void UG_UpdateSeekSlider(void);

#endif
