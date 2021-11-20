/*
 * RandomModulator.h
 *
 *  Created on: 20/11/2021
 *      Author: Adrian
 */

#ifndef CUSTOMMODULATOR_H_
#define CUSTOMMODULATOR_H_

#include <cstdint>

class CustomModulator {

public:
	typedef int16_t (*CustomFunc)();
	CustomModulator(int16_t* variable, int16_t minVal, int16_t maxVal, uint16_t prescaler) :
		variable(variable), minVal(minVal), maxVal(maxVal), prescaler(prescaler), delayCounter(0) {};
	~CustomModulator() {};

	virtual int16_t GetVal() {return 0;}
	void update(void);

protected:
	int16_t* variable;
	int16_t minVal;
	int16_t maxVal;
	uint16_t prescaler;
	uint16_t delayCounter;

	bool doDelay(void);


};

#endif /* CUSTOMMODULATOR_H_ */
