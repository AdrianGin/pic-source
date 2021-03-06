
#include <stdint.h>
#include "printf/printf.h"
#include "FSUtil.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "FatFS/ff.h"

/* Start node to be scanned (also used as work area) */
FRESULT scan_files (char* path)
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    int i;
    char *fn;   /* This function is assuming non-Unicode cfg. */
#if _USE_LFN
    static char lfn[_MAX_LFN + 1];
    fno.lfname = lfn;
    fno.lfsize = sizeof lfn;
#endif



    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        i = strlen(path);
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fname[0] == '.') continue;             /* Ignore dot entry */
#if _USE_LFN
            fn = *fno.lfname ? fno.lfname : fno.fname;
#else
            fn = fno.fname;
#endif
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
                xsprintf(&path[i], "/%s", fn);
                res = scan_files(path);
                if (res != FR_OK) break;
                path[i] = 0;
            } else {                                       /* It is a file. */
                xprintf("%s/%s\n", path, fn);
            }
        }
    }

    return res;
}


FRESULT FSUtil_OpenDir(DIR* dir, char* path)
{
	FRESULT res;
	res = f_opendir(dir, path);
	return res;
}



FSUTIL_t FSUtil_GetDirObj(DIR* dir, char* buffer)
{
    FILINFO fno;
    FRESULT res;
    int i;

    char *fn;   /* This function is assuming non-Unicode cfg. */
#if _USE_LFN
    static char lfn[_MAX_LFN + 1];
    fno.lfname = lfn;
    fno.lfsize = sizeof lfn;
#endif

    for(;;)
    {
		res = f_readdir(dir, &fno);                   /* Read a directory item */
		if (res != FR_OK || fno.fname[0] == 0)
		{
			return END_OF_LIST;  /* Break on error or end of dir */
		}


		#if _USE_LFN
		fn = *fno.lfname ? fno.lfname : fno.fname;
		#else
		fn = fno.fname;
		#endif

		if( buffer )
		{
			strcpy(buffer, fn);
			/* It is a directory */
			/* Ignore dot entry */
			if ((fno.fname[0] == '.') && (fno.fname[1] == 0))
			{
				return DOT_DIRECTORY;
			}

			if ((fno.fname[0] == '.') && (fno.fname[1] == '.') && (fno.fname[2] == 0))
			{
				return DDOT_DIRECTORY;
			}

			if (fno.fattrib & AM_DIR)
			{
				return DIRECTORY;
			}
			else
			{
				/* It is a file. */
				return VALID_FILE;
			}
		}
    }

    return END_OF_LIST;
}




uint8_t FSUtil_HasExtension(char* fn, char* extension)
{
	char upperStr[5];
	int i;
	for( i = 0; i < strlen(extension)+1; i++)
	{
		upperStr[i] = toupper(extension[i]);
	}

	if( strstr(fn, extension) || strstr(fn, upperStr))
	{
		return 1;
	}
	return 0;
}


