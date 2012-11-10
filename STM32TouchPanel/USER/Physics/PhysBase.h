#ifdef __cplusplus    /* Insert start of extern C construct */
extern "C" {
#endif

#ifndef _PHYSBASE_H
#define _PHYSBASE_H

#include <stdint.h>

#include "WorldPhysics.h"
#include "vector2D.h"



class PhysBase {

public:
	PHYS_FP mass;
	PHYS_FP elasticity; //bounce factor
	PHYS_FP kFriction;
    Vector2D acceleration;
    Vector2D velocity;
    Vector2D position;

    void (*draw)(void);

    void PhysicsModel(PHYS_FP Mass, PHYS_FP Elasticity){mass = Mass; elasticity = Elasticity;};

    void calculatePosition(PHYS_INT dTime);
    void calculateSpeed(PHYS_INT dTime);

    void applyBounds(void);
    void applyForce(Vector2D force);
    void SetPosition(PHYS_FP x, PHYS_FP y);
    void Tick(PHYS_INT dTime);

};


#endif


#ifdef __cplusplus  /* Insert end of extern C construct. */
}                   /* The C header file can now be */
#endif              /* included in either C or C++ code. */
