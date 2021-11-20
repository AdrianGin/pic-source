/*
 * RandomModulator.cpp
 *
 *  Created on: 20/11/2021
 *      Author: Adrian
 */

#include <CustomModulator.h>

bool CustomModulator::doDelay(void)
{
	if( ++delayCounter == prescaler )
	{
		delayCounter = 0;
		return true;
	}
	else
	{
		return false;
	}
}

void CustomModulator::update(void)
{
   if( !doDelay() )
   {
      return;
   }

   int16_t val = GetVal();
   if( val <= minVal )
   {
   	val = minVal;
   }

   if( val >= maxVal)
   {
   	val = maxVal;
   }
   *variable = val;
}
