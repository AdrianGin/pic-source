/*
 * RandomModulator.h
 *
 *  Created on: 20/11/2021
 *      Author: Adrian
 */

#ifndef RANDOMMODULATOR_H_
#define RANDOMMODULATOR_H_

#include <CustomModulator.h>

class RandomModulator: public CustomModulator {
public:
	RandomModulator(int16_t* variable, int16_t minVal, int16_t maxVal, uint16_t prescaler, uint16_t phaseOffset = 0);
	int16_t GetVal() override;

};

#endif /* RANDOMMODULATOR_H_ */
