#ifndef _MLL_LOGIC_H
#define _MLL_LOGIC_H



#include <stdint.h>
#include "MIDIParser/midiparser.h"


void MLL_ProcessMIDIByte(uint8_t byte);
void MLL_OutputMIDI(void);
void MLL_OutputLighting(void);



#endif
