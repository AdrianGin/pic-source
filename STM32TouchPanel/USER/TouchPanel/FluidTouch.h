#ifndef _FLUID_TOUCH_H
#define _FLUID_TOUCH_H

#include <stdint.h>
#include "TouchPanel.h"
//Requires some sort of FAT file System



void FluidTouchInit(void);
void FluidTouchMain(void);
void FluidTouch_ApplySlowdown(void);
void _FluidTouch_GetDifferential(Coordinate* newPoint);
Coordinate* FluidTouchGetPoint(void);
Coordinate* FluidTouch_GetIntertia(void);

#endif
