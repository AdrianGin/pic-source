
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "TouchPanel.h"
#include "fluidtouch.h"

//Holds the inertia movement of X & Y CoOrdinates
//This is a vector
static Coordinate FT_Inertia;
static Coordinate FT_LastPoint;
static Coordinate FT_CalcPoint;

typedef enum {TOUCH_ON, TOUCH_OFF, TOUCH_FREE_RUNNING, TOUCH_FIRST} FT_STATES;

static FT_STATES FT_State;

void FluidTouchInit(void)
{
	FT_State = TOUCH_OFF;
	FT_Inertia.x = 0;
	FT_Inertia.y = 0;

	FT_LastPoint.x = 0;
	FT_LastPoint.y = 0;

	FT_CalcPoint.x = 0;
	FT_CalcPoint.y = 0;
}

#define OFF_COUNT_THRESHOLD	(4)

//Executes the touch control subsystem.
void FluidTouchMain(void)
{
	static uint8_t offCount = 0xFF;

	Coordinate* point;
	point = Read_Ads7846();
	if( point != 0)
	{

		FT_State = TOUCH_ON;
		if( offCount > OFF_COUNT_THRESHOLD )
		{
			FT_State = TOUCH_FIRST;
			FT_LastPoint.x = 0;
			FT_LastPoint.y = 0;
			FT_Inertia.x = 0;
			FT_Inertia.y = 0;
			FT_CalcPoint.x = 0;
			FT_CalcPoint.y = 0;
		}

		TP_BudgetGetDisplayPoint(&TouchPanel, point);

//		printf("PosX=%d\n",TouchPanel.x);
//		printf("PosY=%d\n",TouchPanel.y);

		_FluidTouch_GetDifferential(&TouchPanel);
		offCount = 0;
	}
	else
	{;
		if( offCount > OFF_COUNT_THRESHOLD )
		{
			FT_State = TOUCH_OFF;
		}
		else
		{
			offCount++;
		}
		//FT_LastPoint.x = 0;
		//FT_LastPoint.y = 0;
		point = &FT_LastPoint;
		_FluidTouch_GetDifferential(point);


		//FluidTouchInit();
	}


	//printf("INER X:%d, Y:%d\n", FT_Inertia.x, FT_Inertia.y);
}


#define FT_SLOWDOWN_MIN_SPEED (2)
#define FT_SLOWDOWN_SPEED	(90)
#define FT_SLOWDOWN_FACTOR	(100)
void FluidTouch_ApplySlowdown(void)
{
	int16_t xSlowDown;
	int16_t ySlowDown;

	xSlowDown = FT_SLOWDOWN_SPEED;
//	if( FT_Inertia.x < 0 )
//	{
//		xSlowDown = -FT_SLOWDOWN_SPEED;
//	}

	ySlowDown = FT_SLOWDOWN_SPEED;
//	if( FT_Inertia.y < 0 )
//	{
//		ySlowDown = -FT_SLOWDOWN_SPEED;
//	}


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

//		printf("Diff= %d :: %d\n", newPoint->x - FT_LastPoint.x, newPoint->y - FT_LastPoint.y);
//		printf("INER= %d :: %d\n", FT_Inertia.x, FT_Inertia.y);

		FT_LastPoint.x = newPoint->x;
		FT_LastPoint.y = newPoint->y;
	}
}


Coordinate* FluidTouch_GetIntertia(void)
{
	return &FT_Inertia;
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









