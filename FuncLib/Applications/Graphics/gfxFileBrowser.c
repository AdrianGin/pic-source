

#include <stdint.h>
#include <string.h>

#include "gfxListbox.h"


#include "gfxFileBrowser.h"
#include "FSUtils/FSUtil.h"
#include "Alphasort/alphasort.h"

const char UP_ONE_LEVEL_TAG[] = UP_LEVEL_TAG;
const char DIRECTORY_TAG[]	  = DIR_TAG;

void GFX_FB_Init(GFX_FB_t* FB)
{
	//strcpy(FB->cwd, "/");
}

char* GFX_FB_CWD(void)
{
	static char cwd[_MAX_LFN];
	f_getcwd (cwd, sizeof(cwd));
	return cwd;
}

void GFX_FB_OpenDir(GFX_FB_t* FB, char* path)
{
	f_chdir(path);
}

void GFX_FB_OpenDirRel(GFX_FB_t* FB, char* relPath)
{
	f_chdir(relPath);
}


GFX_FB_SELECT_ITEM GFX_FB_ProcessRequest(GFX_Listbox_t* LB, char* item)
{

	uint8_t refreshList = GFX_FB_NO_DIR_SELECTED;

	if( strcmp(item, UP_ONE_LEVEL_TAG) == 0)
	{
		f_chdir("..");
		refreshList = GFX_FB_DDOT_SELECTED;
	}

	if( strstr(item, DIRECTORY_TAG) )
	{
		f_chdir( &item[sizeof(DIRECTORY_TAG)-1] );
		refreshList = GFX_FB_DIR_SELECTED;
		xprintf("Changing Dir... %s", &item[sizeof(DIRECTORY_TAG)-1]);
	}


	if( refreshList == GFX_FB_DDOT_SELECTED ||
		refreshList == GFX_FB_DIR_SELECTED	)
	{
		GFX_LB_DeleteListboxItems(LB);
		GFX_FB_RepopulateList(LB, INC_ALL_DIRS, NULL);
		alphasort_linkedList(&LB->list, SORT_ASCENDING);
		return refreshList;
	}

	return GFX_FB_NO_DIR_SELECTED;

}

/*Include DIR

if
0: Dont list Directories
1: List Directories, but not the '..'s
2: List the '..' but not the Directories
3: List all directories and ..'s

*/
void GFX_FB_RepopulateList(GFX_Listbox_t* LB, uint8_t includeDir, char* extensionFilter)
{
	DIR dir;
	FSUTIL_t ret;
	char fnPath[100];

	FSUtil_OpenDir(&dir, GFX_FB_CWD() );
	while (1)
	{
		ret = FSUtil_GetDirObj(&dir, fnPath);

		switch(ret)
		{
			case DOT_DIRECTORY:
				if( !(includeDir & INC_SELF_DIR) )
				{
					ret = NO_PRINT;
				}
				break;

			case DDOT_DIRECTORY:
				strcpy(fnPath, UP_ONE_LEVEL_TAG);
				ret = VALID_FILE;
			case DIRECTORY:
				if( !(includeDir & INC_ALL_DIRS) )
				{
					ret = NO_PRINT;
				}
				break;

			case VALID_FILE:
				if( extensionFilter )
				{
					if( strstr(fnPath, extensionFilter) == NULL)
					{
						ret = NO_PRINT;
					}
				}
				break;

			case END_OF_LIST:
			default:
				break;
		}

		if (ret != END_OF_LIST)
		{
			if(ret != NO_PRINT)
			{
				if(ret == DOT_DIRECTORY || ret == DIRECTORY)
				{
					memmove(&fnPath[sizeof(DIRECTORY_TAG)-1], fnPath, strlen(fnPath)+1);
					strncpy(fnPath, DIRECTORY_TAG, sizeof(DIRECTORY_TAG)-1);
				}

				xprintf("Path:%s\n", fnPath);
				GFX_LB_AddItem(LB, fnPath);
				fnPath[0] = 0;
				//free(fnPath);
			}
		}
		else
		{
			break;
		}
	}
}



