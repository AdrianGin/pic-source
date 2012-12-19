
#ifndef USER_GUI_H
#define USER_GUI_H


#include "Graphics/gfxListbox.h"
#include "Graphics/gfxFileBrowser.h"
#include "Graphics/gfxFrame.h"


extern GFX_Listbox_t* GFX_LB;
extern GFX_FB_t GFX_FB;
extern gfxFrame_t GFX_MainFrame;

void UserGUI_Init(void* gfxLB);
uint8_t ProcessFB_Request(void* LB, void* data);


#endif
