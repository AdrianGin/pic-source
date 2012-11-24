#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "InertiaTouch.h"


void FTI_InitInertia(InertiaElement_t* inertia)
{
	inertia->Value.x = 0;
	inertia->Value.y = 0;

	inertia->MinSpeed = FT_SLOWDOWN_MIN_SPEED;
	inertia->SlowDownSpeed = FT_SLOWDOWN_SPEED;
	inertia->SlowDownFactor = FT_SLOWDOWN_FACTOR;

}

void FTI_ApplySlowdown(InertiaElement_t* inertia)
{
	int16_t xSlowDown;
	int16_t ySlowDown;

	xSlowDown = inertia->SlowDownSpeed;
	ySlowDown = inertia->SlowDownSpeed;

	if( abs(inertia->Value.x) > inertia->MinSpeed )
	{
		inertia->Value.x = (inertia->Value.x * xSlowDown) / inertia->SlowDownFactor;
	}
	else
	{
		inertia->Value.x = 0;
	}

	if( abs(inertia->Value.y) > inertia->MinSpeed )
	{
		inertia->Value.y = (inertia->Value.y * ySlowDown) / inertia->SlowDownFactor;
	}
	else
	{
		inertia->Value.y = 0;
	}
}

uint8_t FTI_InertiaIsZero(InertiaElement_t* inertia)
{
	if( (inertia->Value.x == 0) && (inertia->Value.y == 0))
	{
		return 1;
	}
	else
	{
		return 0;
	}

}

void FTI_ResetInertia(InertiaElement_t* inertia)
{
	Coordinate_Clear(&inertia->Value);
}

void FTI_UpdateInertia(InertiaElement_t* inertia, Coordinate* diff)
{
	Coordinate_Sum(&inertia->Value, diff);
}

//Calculates the Point based on the last point and inertia, basically addes them.
Coordinate* FTI_GetPoint(InertiaElement_t* inertia, Coordinate* lastPoint)
{

	static Coordinate FT_CalcPoint;
	FT_CalcPoint.x = lastPoint->x + inertia->Value.x;
	FT_CalcPoint.y = lastPoint->y + inertia->Value.y;

	return &FT_CalcPoint;

}










