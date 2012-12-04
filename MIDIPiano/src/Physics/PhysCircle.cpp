

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "physics.h"
#include "GLCD/GLCD.h"
#include "TouchPanel/TouchPanel.h"

#include "PhysBase.h"
#include "PhysCircle.h"
#include "vector2D.h"
#include "WorldPhysics.h"
#include "Graphics\primitiveGfx.h"

//Checks a collision with another circle
PHYS_FP PhysCircle::CheckCollision(PhysCircle* obj)
{
	if( obj == NULL )
	{
		return 0;
	}

	PHYS_FP dx;
	PHYS_FP dy;
	PHYS_FP dr;

	dx = this->position.x - obj->position.x;
	dy = this->position.y - obj->position.y;
	dr = this->radius +  obj->radius;

	if( (dx*dx + dy*dy) <= dr*dr)
	{
		return 1;
	}

	return 0;
}



void PhysCircle::applyFloor (PHYS_INT dTime)
{

	if( (position.y - radius) <= GROUND_LEVEL)
	{
		//applyForce( Vector2D(0, mass) );
		//Bounce back up.
		if( (abs(velocity.y) > (GRAVITY_CONST*dTime/TIME_BASE) ))
		{
			velocity.y = -(velocity.y*elasticity/10) - (GRAVITY_CONST*dTime/TIME_BASE);

			//velocity.y = velocity.y * 0.9;
			position.y = GROUND_LEVEL + radius;
		}
		else
		{
			velocity.y = 0;
		}
	}
	else
	{
		//Only apply the gravity if the result doesn't make us into the ground
		//if( ((position.y - radius) + (velocity.y - (GRAVITY_CONST*dTime/TIME_BASE))) > GROUND_LEVEL)
		{
			velocity.y = velocity.y - (GRAVITY_CONST*dTime/TIME_BASE);
		}
		//else
		{
			//velocity.y = velocity.y - (velocity.y*kFriction/10);
			//velocity.y = 0;
		}
		//applyForce( Vector2D(0, -mass) );
	}
}



float PhysCircle::CollisionAngle(PhysCircle* obj)
{
	float theta;
	float velAngle;
	float knownAngle;

	theta = atan2f(this->position.y - obj->position.y, this->position.x - obj->position.x);

	//Phi is the angle of attack

	velAngle = atan2f(this->velocity.y, this->velocity.x);

	knownAngle = (PI / 2) - theta;
	knownAngle = knownAngle - velAngle;
	return knownAngle;
}


void PhysCircle::ReDraw(void)
{
	SetGlobalColour(colour);
	fillCircle(position.x, position.y, radius);
}



