#ifndef _MLL_LOGIC_H
#define _MLL_LOGIC_H



#include <stdint.h>

#define MLL_CHANNEL_IS_ACTIVE(map, channel) (MLL_ActiveChannelMap[map] & (1<<channel))

void MLL_Init(void);

void MLL_ProcessMIDIByte(uint8_t byte);

void MLL_OutputMIDI(void);
void MLL_OutputLighting(void);


#define HALT_FLAG_RAISED (1)
void MLL_ClearHaltList(void);
void MLL_SetHaltFlag(uint8_t state);
uint8_t MLL_GetHaltFlag(void);
void MLL_AddHaltMasterNote(uint8_t* midiDataArray);


void MLL_ProcessHaltNote(uint8_t* midiDataArray);
//midiDataArray:: A 3 byte array pointer
void MLL_AddTesterHaltNote(uint8_t* midiDataArray);
void MLL_TesterHaltCancelNotes(uint8_t* midiDataArray);
void MLL_CompareMasterTesterHaltList(void);

#endif
