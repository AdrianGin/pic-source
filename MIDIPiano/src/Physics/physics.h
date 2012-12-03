#ifdef __cplusplus    /* Insert start of extern C construct */
extern "C" {
#endif

#include <stdint.h>
#include "WorldPhysics.h"

void PhysicsMain(PHYS_INT dTime);
void ReDrawBall(void);
void SetTouchPoint(int x, int y);
void PhysicsTick(void);
void PhysicsInit(void);
void ReDrawBall2(void);

#ifdef __cplusplus  /* Insert end of extern C construct. */
}                   /* The C header file can now be */
#endif              /* included in either C or C++ code. */
