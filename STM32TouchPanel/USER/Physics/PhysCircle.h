#ifdef __cplusplus    /* Insert start of extern C construct */
extern "C" {
#endif

#ifndef _PHYSCIRCLE_H
#define _PHYSCIRCLE_H

#include <stdint.h>

#include "WorldPhysics.h"
#include "vector2D.h"
#include "PhysBase.h"


class PhysCircle : public PhysBase{

public:
	PHYS_FP radius;
	uint16_t colour;

	void applyFloor(PHYS_INT dTime);
	PHYS_FP CheckCollision(PhysCircle* obj);
	float CollisionAngle(PhysCircle* obj);
	void ReDraw(void);
};


#endif


#ifdef __cplusplus  /* Insert end of extern C construct. */
}                   /* The C header file can now be */
#endif              /* included in either C or C++ code. */
