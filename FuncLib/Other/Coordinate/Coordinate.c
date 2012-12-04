//Empty for now


#include <stdint.h>
#include <stdlib.h>
#include "Coordinate.h"

void Coordinate_Clear(Coordinate* point)
{
	point->x = 0;
	point->y = 0;
}


void Coordinate_Sum(Coordinate* result, Coordinate* add2)
{
	result->x = result->x + add2->x;
	result->y = result->y + add2->y;
}

uint8_t Coordinate_IsZero(Coordinate* point)
{
	if( (point->x == 0) && (point->y == 0))
	{
		return 1;
	}
	return 0;
}

uint8_t Coordinate_IsLessThan(Coordinate* point, int16_t val)
{
	if( ( abs(point->x) < val) && (abs(point->y) < val))
	{
		return 1;
	}
	return 0;
}

