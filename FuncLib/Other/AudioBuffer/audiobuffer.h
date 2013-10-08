/*
 * audiobuffer.h
 *
 *  Created on: 6/05/2013
 *      Author: Adrian
 */

#ifndef AUDIOBUFFER_H_
#define AUDIOBUFFER_H_

#include <stdint.h>
#include "hw_config.h"

#include "DAC_DMA/DAC_DMA.h"

typedef struct
{
	uint8_t buffer[MAX_AUDIO_PACKET_SIZE];
	uint16_t dataLen;
	uint16_t dataPtr;
} AudioBuffer_t;

#endif /* AUDIOBUFFER_H_ */
