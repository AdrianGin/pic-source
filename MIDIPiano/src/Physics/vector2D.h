#ifdef __cplusplus    /* Insert start of extern C construct */
extern "C" {
#endif

#ifndef _VECTOR2D_H
#define _VECTOR2D_H

#include <stdint.h>
#include "WorldPhysics.h"


class Vector2D {
	public:
	PHYS_FP x;
	PHYS_FP y;

	Vector2D(PHYS_FP X = 0, PHYS_FP Y = 0)
	{
		x = X;
		y = Y;
	}

};


#endif

#ifdef __cplusplus  /* Insert end of extern C construct. */
}                   /* The C header file can now be */
#endif              /* included in either C or C++ code. */
