#ifndef _FLUID_TOUCH_H
#define _FLUID_TOUCH_H

#include <stdint.h>
#include <stdlib.h>

#include "Coordinate/Coordinate.h"
#include "TouchPanel/TouchPanel.h"

//Requires some sort of FAT file System

typedef enum {TOUCH_ON=0, TOUCH_OFF, TOUCH_FIRST, TOUCH_TAP, TOUCH_DRAG, TOUCH_LONG, TOUCH_DRAG_CONTINUE, TOUCH_DRAG_STOP} FT_STATES;


#define OFF_COUNT_THRESHOLD	(5)
#define ON_COUNT_THRESHOLD	(5)

#define ON_COUNT_TAP_MIN_THRESHOLD (10)
#define ON_COUNT_TAP_THRESHOLD (250)
#define MAX_ON_COUNT		(10000)
#define MAX_OFF_COUNT		(10000)
#define HOLDING_THRESHOLD	(30)
#define STOP_DRAG_THRESHOLD	(30)


#define FT_IS_MAG_LESS_THAN( point , magnitude )  ((abs(point->x) < magnitude) && (abs(point->y) < magnitude))
#define FT_MOVE_THRES	(10)



extern Coordinate FT_FirstPoint;
extern Coordinate FT_LastPoint;
extern Coordinate FT_CalcPoint;
extern Coordinate FT_Diff;
extern Coordinate FT_CurrentPoint;

extern FT_STATES FT_State;




void FluidTouchInit(void);
void FluidTouchMain(void);

FT_STATES FluidGetTouch(void);
void _FluidTouch_CalcDiff(Coordinate* newPoint);
FT_STATES FluidTouchGetState_FromPanel(Coordinate* point);

FT_STATES FT_UpdateTouchCounters(void);
void FT_DecodeTouchGesture(void);
void FT_ResetDiff(void);

//Function Macros
#define FT_GetFirstPoint() &FT_FirstPoint
#define FT_GetLastPoint() &FT_LastPoint
#define FT_GetDiff() &FT_Diff
#define FT_GetTouchState() FT_State



#endif
