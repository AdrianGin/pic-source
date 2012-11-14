#ifndef _FS_UTIL_H
#define _FS_UTIL_H

#include <stdint.h>
//Requires some sort of FAT file System
#include "ff.h"

/* Start node to be scanned (also used as work area) */
FRESULT scan_files ( char* path );

#endif



