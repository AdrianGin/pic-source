#ifndef _FLUID_TOUCH_H
#define _FLUID_TOUCH_H

#include <stdint.h>
#include "TouchPanel.h"
//Requires some sort of FAT file System

typedef enum {TOUCH_ON, TOUCH_OFF, TOUCH_FIRST, TOUCH_DRAG} FT_STATES;


void FluidTouchInit(void);
void FluidTouchMain(void);
void FluidTouch_ApplySlowdown(void);
void _FluidTouch_GetDifferential(Coordinate* newPoint);
Coordinate* FluidTouchGetPoint(void);
Coordinate* FluidTouch_GetIntertia(void);

void FluidTouchMain2(void);
Coordinate* FluidGetTouch(void);
void _FluidTouch_CalcDiff(Coordinate* newPoint);
FT_STATES FluidTouchGetState(Coordinate* point);


#endif
