/*
 * PlayStats.h
 *
 *  Created on: 4/03/2013
 *      Author: Adrian
 */

#ifndef PLAYSTATS_H_
#define PLAYSTATS_H_

#include "MIDIParser/midiparser.h"
#include <stdint.h>
#include "MIDIPlaybackControlLogic/MIDIPlaybackControlLogic.h"

typedef enum
{
	PS_MISSED_NOTES = 0,
	PS_INCORRECT_NOTE,
	PS_NOTE_COUNT
} PS_NOTE_COUNT_t;

typedef enum
{
	PS_PERFECT = 0,
	PS_GREAT,
	PS_GOOD,
	PS_POOR,
	PS_BIN_COUNT
} PS_BIN_t;


typedef enum
{
	PS_PERFECT_TOLERANCE 	= 32,
	PS_GREAT_TOLERANCE 		= 16,
	PS_GOOD_TOLERANCE		= 8,
	PS_POOR_TOLERANCE		= 1,
} PS_PPQ_BINS_t;





void PS_SetExpiryTolerance(uint32_t ticks);
void PS_Init(void);
uint8_t PS_CheckExpiry(uint32_t time, uint32_t currentTime);
void PS_PrintSummary(void);
PS_BIN_t PS_CategoriseSuccessNote(int32_t deltaTime);
uint8_t PS_RecordSuccessNote(MPB_CL_EVENT_t* masterEvent, uint32_t successTime);
uint8_t PS_RecordMissedNote(MPB_CL_EVENT_t* masterEvent);
uint8_t PS_RecordIncorrectNote(MPB_CL_EVENT_t* masterEvent);




#endif /* PLAYSTATS_H_ */
