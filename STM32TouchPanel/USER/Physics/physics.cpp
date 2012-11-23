

#include <stdint.h>
#include <stdlib.h>

#include "physics.h"
#include "GLCD.h"
#include "TouchPanel.h"

#include "PhysBase.h"
#include "PhysCircle.h"

#include <math.h>

#include "vector2D.h"
#include "Graphics\primitiveGfx.h"

#include "linkedlist\linkedlist.h"

Coordinate BallLocation;

PhysCircle RedBall;
PhysCircle RedBall2;
PhysCircle RedBall3;
PhysCircle RedBall4;
LINKED_LIST_t ObjectList;


void PhysicsInit(void)
{

	LL_AppendData(&ObjectList, &RedBall);
	LL_AppendData(&ObjectList, &RedBall2);
	LL_AppendData(&ObjectList, &RedBall3);
	LL_AppendData(&ObjectList, &RedBall4);
	//LL_Remove(&ObjectList, 0);

	RedBall.acceleration.y = 0;
	RedBall.mass = 4000;
	RedBall.elasticity = 5;
	//RedBall.draw = ReDrawBall;
	RedBall.radius = 10;
	RedBall.colour = RED;
	RedBall.kFriction = 5;

	RedBall2.acceleration.y = 0;
	RedBall2.mass = 800;
	RedBall2.elasticity = 10.1;
	//RedBall2.draw = RedBall2.ReDraw;
	RedBall2.radius = 20;
	RedBall2.colour = WHITE;
	RedBall2.kFriction = 5;

	RedBall3.acceleration.y = 0;
	RedBall3.mass = 600;
	RedBall3.elasticity = 5;
	//RedBall3.draw = RedBall3.ReDraw;
	RedBall3.radius = 10;
	RedBall3.colour = RED;
	RedBall3.kFriction = 5;

	RedBall4.acceleration.y = 0;
	RedBall4.mass = 800;
	RedBall4.elasticity = 10.5;
	//RedBall2.draw = RedBall2.ReDraw;
	RedBall4.radius = 20;
	RedBall4.colour = WHITE;
	RedBall4.kFriction = 5;

}


//Call this at a rate of 100Hz.
void PhysicsMain(PHYS_INT dTime)
{
	static uint16_t tick;

	uint16_t oldColour;
	LIST_NODE_t* node;
	LIST_NODE_t* forNode;
	PhysCircle* obj;

	node = ObjectList.first;


	LCD_VSyncLow();
	//Move everything along
	while(node != NULL )
	{
		obj = (PhysCircle*)node->data;
		obj->applyBounds();
		obj->applyFloor(dTime);
		obj->Tick(dTime);
		node = node->next;
	}

	LCD_Clear(BLUE);

	node = ObjectList.first;
	while(node != NULL )
	{
		obj = (PhysCircle*)node->data;
		obj->colour = WHITE;
		forNode = ObjectList.first;
		while( (forNode != NULL) )
		{
			if( (forNode != node) )
			{
				if( obj->CheckCollision( (PhysCircle*)forNode->data) )
				{
					float temp;
					double tempMassX;
					double tempMassY;

					temp = obj->CollisionAngle((PhysCircle*)forNode->data);


					tempMassX = ((PhysCircle*)forNode->data)->velocity.x;
					tempMassY = ((PhysCircle*)forNode->data)->velocity.y;

					tempMassY = sqrt(tempMassX*tempMassX + tempMassY*tempMassY);


					obj->applyForce( Vector2D( tempMassY * cosf(temp), tempMassY * sinf(temp) ) );
					obj->colour = BLACK;
				}
				else
				{

				}
			}

			forNode = forNode->next;
		}


		obj->ReDraw();
		node = node->next;
	}


	LCD_VSyncHigh();

	//RedBall


}

void PhysicsTick(void)
{

}

void SetTouchPoint(int x, int y)
{
	static uint8_t alternate = 0;

	static LIST_NODE_t* node = ObjectList.first;
	PhysCircle* obj;

	if( node != NULL )
	{
		obj = (PhysCircle*)node->data;
		obj->SetPosition(x, y);
		obj->velocity.x = 0;
		obj->velocity.y = 0;
		obj->acceleration.x = 0;
		obj->acceleration.y = 0;
		obj->kFriction = 0.9;

		node = node->next;
		if( node == NULL )
		{
			node = ObjectList.first;
		}
	}
}


void ReDrawBall(void)
{
	static uint8_t alternate = 0;
	  //LCD_PauseUpdateScreen();

	alternate = 0;

	SetGlobalColour(RedBall.colour);
	fillCircle(RedBall.position.x, RedBall.position.y, RedBall.radius);

}


void ReDrawBall2(void)
{
	SetGlobalColour(RedBall2.colour);
	fillCircle(RedBall2.position.x, RedBall2.position.y, RedBall2.radius);
}












