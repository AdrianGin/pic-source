
#include <stdint.h>
#include <stdio.h>
#include <string.h>


#include "TouchPanel.h"
#include "fluidtouch.h"

//Holds the inertia movement of X & Y CoOrdinates
//This is a vector
static Coordinate FT_Inertia;
static Coordinate FT_LastPoint;

typedef enum {TOUCH_ON, TOUCH_OFF} FT_STATES;

static FT_STATES FT_State;

void FluidTouchInit(void)
{
	FT_State = TOUCH_OFF;
	FT_Inertia.x = 0;
	FT_Inertia.y = 0;
}

//Executes the touch control subsystem.
void FluidTouchMain(void)
{
	Coordinate* point;
	point = Read_Ads7846();
	if( point != 0)
	{
		FT_State = TOUCH_ON;
		TP_BudgetGetDisplayPoint(&TouchPanel, point);
		_FluidTouch_GetDifferential(point);
	}
	else
	{

		FT_State = TOUCH_OFF;
	}
}


#define FT_SLOWDOWN_SPEED	(2)
void _FluidTouch_ApplySlowdown(void)
{
	FT_Inertia.x = FT_Inertia.x - FT_SLOWDOWN_SPEED;
	FT_Inertia.y = FT_Inertia.y - FT_SLOWDOWN_SPEED;

	if( FT_Inertia.x < 0 )
	{
		FT_Inertia.x = 0;
	}

	if( FT_Inertia.y < 0 )
	{
		FT_Inertia.y = 0;
	}

}

void _FluidTouch_GetDifferential(Coordinate* newPoint)
{

	if( FT_State == TOUCH_ON )
	{
		FT_Inertia.x = FT_Inertia.x + (newPoint.x - FT_LastPoint.x);
		FT_Inertia.y = FT_Inertia.y + (newPoint.y - FT_LastPoint.y);
	}

}

void FluidTouchGetPoint(Coordinate * displayPtr, Coordinate * screenPtr)
{


}
