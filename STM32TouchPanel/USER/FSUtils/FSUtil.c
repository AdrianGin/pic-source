
#include <stdint.h>
#include "printf/printf.h"

#include <string.h>
#include <stdlib.h>
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

//This function mallocs memory
char* FSUtil_GetDirObj(DIR* dir)
{
    FILINFO fno;
    FRESULT res;
    int i;
    char* buffer;
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
			return 0;  /* Break on error or end of dir */
		}

		/* Ignore dot entry */
		if (fno.fname[0] == '.')
		{
			continue;
		}
		#if _USE_LFN
		fn = *fno.lfname ? fno.lfname : fno.fname;
		#else
		fn = fno.fname;
		#endif

		/* It is a directory */
		if (fno.fattrib & AM_DIR)
		{
			break;
			//sprintf(&path[i], "/%s", fn);
			//res = scan_files(path);
			//if (res != FR_OK) break;
			//path[i] = 0;
		}
		else
		{
			/* It is a file. */
			buffer = (char*)malloc(strlen(fn)+1);
			if( buffer )
			{
				strcpy(buffer, fn);
			}
			return buffer;
			//printf("%s/%s\n", path, fn);
		}
    }

    return 0;

}


