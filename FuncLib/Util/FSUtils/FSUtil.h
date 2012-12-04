#ifndef _FS_UTIL_H
#define _FS_UTIL_H

#include <stdint.h>
//Requires some sort of FAT file System
#include "FatFS/ff.h"
#include "hardwareSpecific.h"


#ifndef FS_UTIL_MALLOC
#define FS_UTIL_MALLOC(size) malloc(size)
#endif

//Put this into the project config.h
//#define FS_UTIL_MALLOC(size)	 malloc(size)
//#define FS_UTIL_MALLOC(size)	 pvPortMalloc(size)

/* Start node to be scanned (also used as work area) */
FRESULT scan_files ( char* path );
FRESULT FSUtil_OpenDir(DIR* dir, char* path);
//This function mallocs memory
char* FSUtil_GetDirObj(DIR* dir);

#endif



