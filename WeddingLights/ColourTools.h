#ifdef __cplusplus
 extern "C" {
#endif


#ifndef COLOUR_TOOLS_H
#define COLOUR_TOOLS_H


#include <stdint.h>
#include "rgbUtils/rgbutils.h"


uint32_t CT_addColours(uint32_t c1, uint32_t c2);
uint32_t CT_NomaliseColour(uint32_t r, uint32_t g, uint32_t b);

#endif


#ifdef __cplusplus
 }
#endif
