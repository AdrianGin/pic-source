

#include <stdlib.h>

//
//#define LL_Malloc(size)		pvPortMalloc(size)
//#define LL_Free(handle)		vPortFree(handle)
//

#define LL_Malloc(size)		malloc(size)
#define LL_Free(handle)		free(handle)
