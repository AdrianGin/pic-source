
#ifdef __cplusplus    /* Insert start of extern C construct */
extern "C" {
#endif

#ifndef _WORLDPHYSICS_H
#define _WORLDPHYSICS_H

#include <stdint.h>


const double PI = 3.14159265358979f;

typedef float PHYS_FP;
typedef uint32_t PHYS_INT;

#define GROUND_LEVEL	(0)
#define GRAVITY_CONST	(9.8)

#define TIME_BASE		(100)
#define DESIRED_FPS		(30)

#define WORLD_X_SIZE	(310)
#define WORLD_Y_SIZE	(230)

#endif

#ifdef __cplusplus  /* Insert end of extern C construct. */
}                   /* The C header file can now be */
#endif              /* included in either C or C++ code. */
