
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "TouchPanel.h"
#include "fluidtouch.h"

//Holds the inertia movement of X & Y CoOrdinates
//This is a vector
static Coordinate FT_Inertia;
static Coordinate FT_FirstPoint;
static Coordinate FT_LastPoint;
static Coordinate FT_CalcPoint;
static Coordinate FT_Diff;
static Coordinate FT_CurrentPoint;



static FT_STATES FT_State;

void FluidTouchInit(void)
{
	FT_State = TOUCH_OFF;
	FT_Inertia.x = 0;
	FT_Inertia.y = 0;

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



#define OFF_COUNT_THRESHOLD	(5)
#define ON_COUNT_THRESHOLD	(5)
#define ON_COUNT_TAP_THRESHOLD (550)
#define MAX_ON_COUNT		(10000)
#define MAX_OFF_COUNT		(10000)
#define HOLDING_THRESHOLD	(30)
#define STOP_DRAG_THRESHOLD	(30)


#define FT_IS_INERTIA_ZERO() ((FT_Inertia.x == 0) && (FT_Inertia.y == 0))
#define FT_MOVE_THRES	(10)

#define FT_SLOWDOWN_MIN_SPEED (2)
#define FT_SLOWDOWN_SPEED	(90)
#define FT_SLOWDOWN_FACTOR	(100)
#define FT_DRAGGING_MULT_FACTOR	(4)
//Executes the touch control subsystem.

// The most basic routine, simply filters possible noise from the Touch Panel,
// Returns either TOUCH_ON or TOUCH_OFF and updates the passed in Point structure.
FT_STATES FluidTouchGetState(Coordinate* point)
{
	static uint8_t offCount = OFF_COUNT_THRESHOLD;
	static uint8_t onCount = 0;
	FT_STATES ret = TOUCH_OFF;
	point = Read_Ads7846();

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
		TP_BudgetGetDisplayPoint(&FT_CurrentPoint, point);
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

uint16_t OffCount = OFF_COUNT_THRESHOLD;
uint16_t OnCount  = 0x00;
uint8_t HasMoved = 0;

void FluidTouchMain2(void)
{

	Coordinate* point;
	uint8_t touchState;
	touchState = FluidTouchGetState(point);

	if( touchState == TOUCH_ON )
	{

		_FluidTouch_CalcDiff(&FT_CurrentPoint);
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

}


Coordinate* FluidGetTouch(void)
{

	static Coordinate ret;

	static uint8_t 	  debounceFlag = 0;
	FT_STATES	state;
	Coordinate* point;

	if( (abs(FT_Diff.x) < FT_MOVE_THRES) && (abs(FT_Diff.y) < FT_MOVE_THRES) )
	{

	}
	else
	{
		HasMoved = 1;
	}


	state = FluidTouchGetState(point);

	if(state == TOUCH_ON)
	{

		printf("FT_Frst X=%d, Y  =%d\n", FT_FirstPoint.x, FT_FirstPoint.y);
		printf("FT_Last X=%d, Y  =%d\n", FT_LastPoint.x, FT_LastPoint.y);
		printf("FT_Diff X=%d, Y  =%d\n", FT_Diff.x, FT_Diff.y);
		printf("       ON=%d, OFF=%d, HAS_MV=%d\n", OnCount, OffCount, HasMoved);

		if( (abs(FT_Diff.x) < FT_MOVE_THRES) && (abs(FT_Diff.y) < FT_MOVE_THRES) && (HasMoved == 0) )
		{
			if( (OnCount > ON_COUNT_TAP_THRESHOLD) )
			{
				printf("LONG TOUCH! ON=%d, OFF=%d\n", OnCount, OffCount);
				//LONG TOUCH
				return &FT_LastPoint;
			}

			if( !FT_IS_INERTIA_ZERO() && OnCount > STOP_DRAG_THRESHOLD)
			{
				printf("STOP DRAG! ON=%d, OFF=%d\n", OnCount, OffCount);
				FT_Inertia.x = 0;
				FT_Inertia.y = 0;
				//Stop Drag
				return &FT_LastPoint;
			}

		}
		else
		{

			if( !FT_IS_INERTIA_ZERO() && (OffCount >= OFF_COUNT_THRESHOLD))
			{
				printf("CONTINUE DRAG ON=%d, OFF=%d\n", OnCount, OffCount);
				HasMoved = 1;
				//Continue Drag
				FT_Inertia.x = (FT_Inertia.x + FT_Diff.x);
				FT_Inertia.y = (FT_Inertia.y + FT_Diff.y);

				FT_Diff.x = 0;
				FT_Diff.y = 0;

				return &FT_Inertia;


			}
			else
			{
				printf("DRAG!! ON=%d, OFF=%d\n", OnCount, OffCount);
				HasMoved = 1;
				FT_Inertia.x = FT_Diff.x;
				FT_Inertia.y = FT_Diff.y;

				FT_Diff.x = 0;
				FT_Diff.y = 0;

				return &FT_Inertia;
				//Drag
			}


		}
		return &FT_LastPoint;
	}
	else
	{
		if( (abs(FT_Diff.x) < FT_MOVE_THRES) && (abs(FT_Diff.y) < FT_MOVE_THRES) && (HasMoved == 0) )
		{
			if( (OnCount) && (OnCount <= ON_COUNT_TAP_THRESHOLD) && (OffCount >= OFF_COUNT_THRESHOLD) )
			{
				printf("TOFF FT_Frst X=%d, Y  =%d\n", FT_FirstPoint.x, FT_FirstPoint.y);
				printf("TOFF FT_Last X=%d, Y  =%d\n", FT_LastPoint.x, FT_LastPoint.y);
				printf("TOFF FT_Diff X=%d, Y  =%d\n", FT_Diff.x, FT_Diff.y);
				printf("TOFF        ON=%d, OFF=%d, HAS_MV=%d\n", OnCount, OffCount, HasMoved);

				printf("TAP! ON=%d, OFF=%d\n", OnCount, OffCount);

				FT_Diff.x = 0;
				FT_Diff.y = 0;
				OnCount = 0;
				debounceFlag = 1;
				return &FT_LastPoint;
				//TAP
			}
		}

		if( !FT_IS_INERTIA_ZERO() )
		{
			return &FT_Inertia;

		}
		return NULL;
		//return &FT_Inertia;
	}
}



void FluidTouchMain(void)
{
	static uint8_t offCount = 0xFF;
	static uint8_t onCount  = 0x00;
	static uint8_t holdingCount = 0x00;

	Coordinate* point;
	point = Read_Ads7846();
	if( point != 0)
	{

		//FT_State = TOUCH_ON;


		TP_BudgetGetDisplayPoint(&TouchPanel, point);

		//Detects the first touch.
		if( offCount > OFF_COUNT_THRESHOLD )
		{
			FT_State = TOUCH_FIRST;
			FT_LastPoint.x = 0;
			FT_LastPoint.y = 0;
			FT_CalcPoint.x = 0;
			FT_CalcPoint.y = 0;
			_FluidTouch_GetDifferential(&TouchPanel);
		}

		//Detects the DRAG.
		if( onCount > ON_COUNT_THRESHOLD )
		{
			FT_State = TOUCH_DRAG;
			FT_Diff.x = FT_Diff.x + (TouchPanel.x - FT_LastPoint.x);
			FT_Diff.y = FT_Diff.y + (TouchPanel.y - FT_LastPoint.y);

			if( (abs(TouchPanel.x - FT_LastPoint.x) <  FT_MOVE_THRES) && \
				(abs(TouchPanel.y - FT_LastPoint.y) <  FT_MOVE_THRES) )
			{
				holdingCount++;

				if( holdingCount > HOLDING_THRESHOLD )
				{
					//printf("HOLDING\n");
					FT_Inertia.x = 0;
					FT_Inertia.y = 0;
					holdingCount = 0;
				}
			}
			_FluidTouch_GetDifferential(&TouchPanel);
			//onCount = 0;
		}

		offCount = 0;

		onCount++;
		if( onCount > ON_COUNT_TAP_THRESHOLD)
		{
			onCount = ON_COUNT_TAP_THRESHOLD;
		}

	}
	else
	{
		if( offCount > OFF_COUNT_THRESHOLD )
		{
			FT_State = TOUCH_OFF;

			//Represents a tap
			if( (onCount < ON_COUNT_TAP_THRESHOLD) && (onCount > 0) )
			{
				printf("TAP!\n");
			}

			if( onCount )
			{
				printf("TAP=%d\n", onCount);
			}

			onCount = 0;
			point = &FT_LastPoint;
			_FluidTouch_GetDifferential(point);
		}
		else
		{
			offCount++;
		}
	}

}



void FluidTouch_ApplySlowdown(void)
{
	int16_t xSlowDown;
	int16_t ySlowDown;

	xSlowDown = FT_SLOWDOWN_SPEED;
	ySlowDown = FT_SLOWDOWN_SPEED;

	if( abs(FT_Inertia.x) > FT_SLOWDOWN_MIN_SPEED )
	{
		FT_Inertia.x = (FT_Inertia.x * xSlowDown) / FT_SLOWDOWN_FACTOR;
	}
	else
	{
		FT_Inertia.x = 0;
	}

	if( abs(FT_Inertia.y) > FT_SLOWDOWN_MIN_SPEED )
	{
		FT_Inertia.y = (FT_Inertia.y * ySlowDown) / FT_SLOWDOWN_FACTOR;
	}
	else
	{
		FT_Inertia.y = 0;
	}


}

void _FluidTouch_GetDifferential(Coordinate* newPoint)
{
	//if( FT_State == TOUCH_ON )
	{

		if( (FT_LastPoint.x != 0) && (FT_LastPoint.y != 0))
		{
			FT_Inertia.x = FT_Inertia.x + (newPoint->x - FT_LastPoint.x);
			FT_Inertia.y = FT_Inertia.y + (newPoint->y - FT_LastPoint.y);
		}

		FT_LastPoint.x = newPoint->x;
		FT_LastPoint.y = newPoint->y;


	}
}


Coordinate* FluidTouch_GetIntertia(void)
{
	static Coordinate ret;

	if( FT_State == TOUCH_OFF )
	{
		return &FT_Inertia;
	}


	if( FT_State == TOUCH_DRAG )
	{
		ret.x = FT_Diff.x;
		ret.y = FT_Diff.y;

		FT_Diff.x = 0;
		FT_Diff.y = 0;

		return &ret; // added
	}

	ret.x = 0;
	ret.y = 0;

	return NULL;
}

Coordinate* FluidTouchGetPoint(void)
{

	//return &FT_LastPoint;

	if( FT_State == TOUCH_OFF )
	{

		if((FT_Inertia.x == 0) && (FT_Inertia.y == 0))
		{
			return NULL;
		}

		FT_CalcPoint.x = FT_CalcPoint.x + FT_Inertia.x;
		FT_CalcPoint.y = FT_CalcPoint.y + FT_Inertia.y;
	}
	else
	{
		FT_CalcPoint.x = FT_LastPoint.x;
		FT_CalcPoint.y = FT_LastPoint.y;
	}


//	printf("CalcPoint =%d, %d\n", FT_CalcPoint.x, FT_CalcPoint.y);

	//if( (FT_CalcPoint.x != 0) && (FT_CalcPoint.y != 0))
	{

		return &FT_CalcPoint;
	}

	return NULL;
}









