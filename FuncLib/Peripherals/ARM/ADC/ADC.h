/*
 * ADC.h
 *
 *  Created on: 25/02/2013
 *      Author: Adrian
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>


void ADC_Configuration(void);
uint16_t ADC_GetSample(uint8_t channel);


#endif /* ADC_H_ */
