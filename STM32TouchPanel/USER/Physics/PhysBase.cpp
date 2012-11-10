

#include <stdint.h>
#include <stdlib.h>

#include "physics.h"
#include "GLCD.h"
#include "TouchPanel.h"

#include "PhysBase.h"

#include "vector2D.h"



//a = F / m
void PhysBase::applyForce(Vector2D force)
{
	velocity.x = force.x;
	velocity.y = force.y;
}

//a = F / m
void PhysBase::SetPosition(PHYS_FP x, PHYS_FP y)
{
	position.x = x;
	position.y = y;
}

void PhysBase::applyBounds()
{
	if( (position.x >= WORLD_X_SIZE) || (position.x < 0))
	{

		velocity.x = -velocity.x;
	}

	if( (position.y >= WORLD_Y_SIZE))
	{
		velocity.y = -velocity.y;
	}

}

void PhysBase::calculatePosition (PHYS_INT dTime)
{

	position.x = position.x + (velocity.x * dTime / TIME_BASE);
	position.y = position.y + (velocity.y * dTime / TIME_BASE);
}


void PhysBase::calculateSpeed (PHYS_INT dTime)
{

	velocity.x = (velocity.x) + (acceleration.x * dTime / TIME_BASE);
	velocity.y = (velocity.y) + (acceleration.y * dTime / TIME_BASE);
}

void PhysBase::Tick(PHYS_INT dTime)
{
	//calculateSpeed(dTime);
	calculatePosition(dTime);
}














