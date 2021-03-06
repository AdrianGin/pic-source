
#ifndef _GFX_FILEBROWSER_H
#define _GFX_FILEBROWSER_H



#include <stdint.h>
#include "ffconf.h"



typedef struct
{
	GFX_Listbox_t GFXLB;
} GFX_FB_t;



void GFX_FB_Init(GFX_FB_t* FB);

char* GFX_FB_CWD(void);

void GFX_FB_OpenDir(GFX_FB_t* FB, char* path);
void GFX_FB_OpenDirRel(GFX_FB_t* FB, char* relPath);


/*Include DIR

if
0: Dont list Directories
1: List Directories, but not the '..'s
2: List the '..' but not the Directories
3: List all directories and ..'s

*/

#define UP_LEVEL_TAG "<- Up one level"
#define DIR_TAG		 "<DIR>"

typedef enum {NO_DIRECTORIES 	= 0x00,
			  INC_SELF_DIR  	= (1<<0),
			  INC_ALL_DIRS		= (1<<1)} INCLUDE_MODE;


typedef enum {GFX_FB_DIR_SELECTED 		= 0x00,
			  GFX_FB_NO_DIR_SELECTED  	= (1<<0),
			  GFX_FB_DDOT_SELECTED 	    = (1<<1),
} GFX_FB_SELECT_ITEM;



void GFX_FB_RepopulateList(GFX_Listbox_t* LB, uint8_t includeDir, char* extensionFilter);
GFX_FB_SELECT_ITEM GFX_FB_ProcessRequest(GFX_Listbox_t* LB, char* item);


#endif




