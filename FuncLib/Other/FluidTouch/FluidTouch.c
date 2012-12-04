
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


#include "fluidtouch.h"

//Holds the inertia movement of X & Y CoOrdinates
//This is a vector

Coordinate FT_FirstPoint;
Coordinate FT_LastPoint;
Coordinate FT_CalcPoint;
Coordinate FT_Diff;
Coordinate FT_CurrentPoint;

FT_STATES FT_State;


uint16_t OffCount = OFF_COUNT_THRESHOLD;
uint16_t OnCount  = 0x00;
uint8_t HasMoved = 0;

void FluidTouchInit(void)
{
	FT_State = TOUCH_OFF;

	FT_FirstPoint.x = 0;
	FT_FirstPoint.y = 0;

	FT_LastPoint.x = 0;
	FT_LastPoint.y = 0;

	FT_CalcPoint.x = 0;
	FT_CalcPoint.y = 0;

	FT_Diff.x = 0;
	FT_Diff.y = 0;


	FT_CurrentPoint.x = 0;
	FT_CurrentPoint.y = 0;
}




//Executes the touch control subsystem.

// The most basic routine, simply filters possible noise from the Touch Panel,
// Returns either TOUCH_ON or TOUCH_OFF and updates the passed in Point structure.
FT_STATES FluidTouchGetState_FromPanel(Coordinate* point)
{
	static uint8_t offCount = OFF_COUNT_THRESHOLD;
	FT_STATES ret = TOUCH_OFF;
	point = GET_RAW_TOUCH_POINT();

	if( point == NULL )
	{
		offCount++;
		if( offCount >= OFF_COUNT_THRESHOLD )
		{
			ret = TOUCH_OFF;
			offCount = OFF_COUNT_THRESHOLD;
		}
		else
		{
			ret = TOUCH_ON;
		}
	}
	else
	{
		offCount = 0;
		ret = TOUCH_ON;
		GET_NORMALISED_TOUCH_POINT(&FT_CurrentPoint, point);
	}
	return ret;
}

void _FluidTouch_CalcDiff(Coordinate* newPoint)
{
	if( (FT_LastPoint.x != 0) && (FT_LastPoint.y != 0))
	{
		FT_Diff.x = FT_Diff.x + (newPoint->x - FT_LastPoint.x);
		FT_Diff.y = FT_Diff.y + (newPoint->y - FT_LastPoint.y);
	}
	FT_LastPoint.x = newPoint->x;
	FT_LastPoint.y = newPoint->y;
}



FT_STATES FT_UpdateTouchCounters(void)
{
	Coordinate* point;
	FT_STATES touchState;
	touchState = FluidTouchGetState_FromPanel(point);

	if( touchState == TOUCH_ON )
	{

		//Initial Touch goes here, reset all parameters.
		if( OffCount >= OFF_COUNT_THRESHOLD)
		{
			HasMoved = 0;
			OnCount = 0;
			FT_Diff.x = 0;
			FT_Diff.y = 0;
			FT_LastPoint.x = 0;
			FT_LastPoint.y = 0;
			FT_FirstPoint.x = FT_CurrentPoint.x;
			FT_FirstPoint.y = FT_CurrentPoint.y;
		}

		//Dont overflow.
		if( OnCount < MAX_ON_COUNT)
		{
			OnCount++;
		}
		OffCount = 0;
	}
	else
	{
		OffCount++;
		if( OffCount >= OFF_COUNT_THRESHOLD )
		{
			OffCount = OFF_COUNT_THRESHOLD+1;
		}
	}

	return touchState;
}

void FT_DecodeTouchGesture(void)
{
	FT_STATES	state;
	Coordinate* point;

	point = &FT_Diff;
	if( !FT_IS_MAG_LESS_THAN(point, FT_MOVE_THRES) )
	{
		HasMoved = 1;
	}
	state = FluidTouchGetState_FromPanel(point);
	if(state == TOUCH_ON)
	{
		//Tentative Touch, undetermined Long Touch / Tap.
		FT_State = TOUCH_ON;

		point = &FT_Diff;
		//If the cursor has moved less than the threshold.
		if( FT_IS_MAG_LESS_THAN(point, FT_MOVE_THRES) && (HasMoved == 0) )
		{
			if( (OnCount > ON_COUNT_TAP_THRESHOLD) )
			{
				FT_State = TOUCH_LONG;
				return;
			}
		}
		else
		{
			HasMoved = 1;
			{
				FT_State = TOUCH_DRAG;
				return;
			}
		}



	}
	else
	{
		//Cursor is off
		point = &FT_Diff;
		//If the cursor has moved less than the threshold.
		if( FT_IS_MAG_LESS_THAN(point, FT_MOVE_THRES) && (HasMoved == 0) )
		{
			if( (OnCount >= ON_COUNT_TAP_MIN_THRESHOLD) && (OnCount <= ON_COUNT_TAP_THRESHOLD) && (OffCount >= OFF_COUNT_THRESHOLD) )
			{
				OnCount = 0;
				FT_State = TOUCH_TAP;
				return;
			}
		}
		return;
	}
}


void FluidTouchMain(void)
{
	if( FT_UpdateTouchCounters() == TOUCH_ON )
	{
		_FluidTouch_CalcDiff(&FT_CurrentPoint);
	}
	FT_DecodeTouchGesture();

}

FT_STATES FluidGetTouch(void)
{
	FT_STATES ret;
	if( FT_State != TOUCH_OFF)
	{
		ret = FT_State;
		FT_State = TOUCH_OFF;
		return ret;
	}

	return FT_State;

}



void FT_ResetDiff(void)
{
	FT_Diff.x = 0;
	FT_Diff.y = 0;
}










