/*
 * RandomModulator.cpp
 *
 *  Created on: 20/11/2021
 *      Author: Adrian
 */

#include <RandomModulator.h>
#include <math.h>

RandomModulator::RandomModulator(int16_t* variable, int16_t minVal, int16_t maxVal, uint16_t prescaler, uint16_t phaseOffset) :
CustomModulator(variable, minVal, maxVal, prescaler)
{
	delayCounter = phaseOffset;
}

int16_t RandomModulator::GetVal()
{
	int16_t ret = rand() % maxVal;
	return ret;
}
