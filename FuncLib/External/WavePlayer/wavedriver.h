#ifndef _WAVE_DRIVER_H
#define _WAVE_DRIVER_H

#include <stdint.h>
#include "waveplayerFS.h"

void waveAudioSetup(waveFile_t* wavefile);
void waveAudioOn(waveFile_t* wavefile);
void waveAudioOff(waveFile_t* wavefile);
void waveProcessBuffer(waveHeader_t* wavefile);
void waveSetSampleRate(uint16_t sampleRate);


#endif
