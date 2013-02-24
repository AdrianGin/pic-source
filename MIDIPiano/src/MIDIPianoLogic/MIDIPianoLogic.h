#ifndef _MIDIPIANO_LOGIC_H
#define _MIDIPIANO_LOGIC_H



#include <stdint.h>

enum
{
	WAITING_FOR_LO_RANGE = 1,
	WAITING_FOR_HI_RANGE,
	WAITING_FOR_FASTFWD_EVENT

};




void   MPL_SetTranspose(int8_t value);
int8_t MPL_GetTranspose(void);

void MPL_RestartPlay(void);

uint8_t MPL_GetLoRange(void);
uint8_t MPL_GetHiRange(void);
void MPL_SetDetectRange(uint8_t lo, uint8_t hi);
uint8_t MPL_TestNoteInDetectRange(uint8_t note);

uint8_t MPL_PreviewNote(uint8_t channel);
uint8_t MPL_ProcessMIDINote(uint8_t* midiDataArray);


#endif
