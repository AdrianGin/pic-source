/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               TouchPanel.h
** Descriptions:            The TouchPanel application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/
#ifdef __cplusplus    /* Insert start of extern C construct */
extern "C" {
#endif


#ifndef _COORDINATE_H_
#define _COORDINATE_H_

#include <stdint.h>

/* Private typedef -----------------------------------------------------------*/
typedef	struct POINT 
{
   int x;
   int y;
} Coordinate;


void Coordinate_Clear(Coordinate* point);
void Coordinate_Sum(Coordinate* result, Coordinate* add2);
uint8_t Coordinate_IsZero(Coordinate* point);
uint8_t Coordinate_IsLessThan(Coordinate* point, int16_t val);



#ifdef __cplusplus  /* Insert end of extern C construct. */
}                   /* The C header file can now be */
#endif              /* included in either C or C++ code. */



#endif
