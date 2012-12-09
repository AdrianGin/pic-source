

#include <stdint.h>
#include <string.h>

#include "Listbox.h"


#include "gfxFileBrowser.h"
#include "FSUtils/FSUtil.h"




void GFX_FB_Init(GFX_FB_t* FB)
{
	strcpy(FB->cwd, "/");
}

char* GFX_FB_CWD(GFX_FB_t* FB)
{
	f_getcwd (FB->cwd, sizeof(FB->cwd));
	return FB->cwd;
}

void GFX_FB_OpenDir(GFX_FB_t* FB, char* path)
{
	f_chdir(path);
}

void GFX_FB_OpenDirRel(GFX_FB_t* FB, char* relPath)
{
	f_chdir(relPath);
}


/*Include DIR

if
0: Dont list Directories
1: List Directories, but not the '..'s
2: List the '..' but not the Directories
3: List all directories and ..'s

*/
void GFX_FB_RepopulateList(GFX_FB_t* FB, uint8_t includeDir, char* extensionFilter)
{
	DIR dir;
	FSUTIL_t ret;
	char fnPath[100];

	FSUtil_OpenDir(&dir, GFX_FB_CWD(FB));
	while (1)
	{
		ret = FSUtil_GetDirObj(&dir, fnPath);

		switch(ret)
		{

			case DOT_DIRECTORY:
				if( !(includeDir & INCLUDE_DOT_DIR) )
				{
					ret = NO_PRINT;
				}
				break;

			case DIRECTORY:
				if( !(includeDir & INCLUDE_DIRS) )
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
					memmove(&fnPath[5], fnPath, strlen(fnPath)+1);
					strncpy(fnPath, "<DIR>", 5);
				}

				xprintf("Path:%s\n", fnPath);
				GFX_LB_AddItem(&FB->GFXLB, fnPath);
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



