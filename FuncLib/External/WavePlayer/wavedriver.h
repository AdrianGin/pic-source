#ifndef _WAVE_DRIVER_H
#define _WAVE_DRIVER_H

#include <stdint.h>
#include "waveplayer.h"

void waveAudioSetup(void);
void waveAudioOn(void);
void waveAudioOff(void);
void waveProcessBuffer(waveHeader_t* wavefile);
void waveSetSampleRate(uint16_t sampleRate);


#endif
