#ifndef _INERTIA_TOUCH_H
#define _INERTIA_TOUCH_H

#include "Coordinate\Coordinate.h"
#include <stdint.h>


#define FT_SLOWDOWN_MIN_SPEED (2)
#define FT_SLOWDOWN_SPEED	(90)
#define FT_SLOWDOWN_FACTOR	(100)
#define FT_DRAGGING_MULT_FACTOR	(4)


typedef struct
{
	uint16_t MinSpeed;
	uint16_t SlowDownSpeed;
	uint16_t SlowDownFactor;
	Coordinate Value;
}InertiaElement_t;


void FTI_InitInertia(InertiaElement_t* inertia);

void FTI_ApplySlowdown(InertiaElement_t* inertia);
Coordinate* FTI_GetPoint(InertiaElement_t* inertia, Coordinate* lastPoint);

uint8_t FTI_InertiaIsZero(InertiaElement_t* inertia);
void FTI_ResetInertia(InertiaElement_t* inertia);
void FTI_UpdateInertia(InertiaElement_t* inertia, Coordinate* diff);



#endif
