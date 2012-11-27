#ifndef _MLL_LOGIC_H
#define _MLL_LOGIC_H



#include <stdint.h>

#define MLL_CHANNEL_IS_ACTIVE(map, channel) (MLL_ActiveChannelMap[map] & (1<<channel))

void MLL_Init(void);

void MLL_ProcessMIDIByte(uint8_t byte);

void MLL_OutputMIDI(void);
void MLL_OutputLighting(void);







#endif
