/*
 * PlayStats.c
 *
 *  Created on: 4/03/2013
 *      Author: Adrian
 */

#include <stdint.h>
#include <stdlib.h>
#include "MIDIParser/midiparser.h"
#include "MIDIPlaybackControlLogic/MIDIPlaybackControlLogic.h"
#include "PlayStats.h"

#include "printf/printf.h"



static uint32_t PS_ExpiryTolerance = 0;
static uint32_t PS_NoteCount[PS_NOTE_COUNT];
static uint32_t PS_CorrectNoteAccuracy[PS_BIN_COUNT];
static uint8_t  PS_Tolerances[PS_BIN_COUNT] = {
		PS_PERFECT_TOLERANCE,
		PS_GREAT_TOLERANCE,
		PS_GOOD_TOLERANCE,
		PS_POOR_TOLERANCE};


const char PS_PerfectString[] = "Perfect";
const char PS_GreatString[] = "Great";
const char PS_GoodString[] = "Good";
const char PS_PoorString[] = "Poor";

const char* PS_CategoryStrings[] = {PS_PerfectString, PS_GreatString, PS_GoodString, PS_PoorString};


void PS_SetExpiryTolerance(uint32_t ticks)
{
	PS_ExpiryTolerance = ticks;
}


void PS_Init(void)
{
	PS_NoteCount[PS_MISSED_NOTES] = 0;
	PS_NoteCount[PS_INCORRECT_NOTE] = 0;

	PS_CorrectNoteAccuracy[PS_PERFECT] = 0;
	PS_CorrectNoteAccuracy[PS_GREAT] = 0;
	PS_CorrectNoteAccuracy[PS_GOOD] = 0;
	PS_CorrectNoteAccuracy[PS_POOR] = 0;
}



uint8_t PS_CheckExpiry(uint32_t time, uint32_t currentTime)
{

	if( currentTime >= time )
	{
		if( (currentTime - time) >= PS_ExpiryTolerance )
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

	return 1;
}


void PS_PrintSummary(void)
{
	uint8_t i;
	uint32_t correctNotes = 0;

	for( i = 0; i < PS_BIN_COUNT; i++)
	{
		correctNotes = correctNotes + PS_CorrectNoteAccuracy[i];
	}

	xprintf("Correct Notes: 	%d\n", correctNotes);
	xprintf("Perfect Notes: 	%d\n", PS_CorrectNoteAccuracy[PS_PERFECT]);
	xprintf("Great Notes:		%d\n", PS_CorrectNoteAccuracy[PS_GREAT]);
	xprintf("Good Notes: 		%d\n", PS_CorrectNoteAccuracy[PS_GOOD]);
	xprintf("Poor Notes: 		%d\n", PS_CorrectNoteAccuracy[PS_POOR]);

	xprintf("Missed Notes: 		%d\n", PS_NoteCount[PS_MISSED_NOTES]);
	xprintf("Incorrect Notes: 	%d\n", PS_NoteCount[PS_INCORRECT_NOTE]);
}

PS_BIN_t PS_CategoriseSuccessNote(int32_t deltaTime)
{
	uint8_t i;
	deltaTime = abs(deltaTime);

	for( i = 0; i < PS_BIN_COUNT; i++)
	{
		if( (deltaTime < (PS_ExpiryTolerance / PS_Tolerances[i])) )
		{
			PS_CorrectNoteAccuracy[i]++;
			return i;
		}
	}

	return PS_POOR;
}

uint8_t PS_RecordSuccessNote(MPB_CL_EVENT_t* masterEvent, uint32_t successTime)
{
	PS_BIN_t category;

	category = PS_CategoriseSuccessNote(successTime - masterEvent->midiClockTime);

	xprintf("%s Note: %X, %X, %X!\n",
			 PS_CategoryStrings[category],
			 masterEvent->event.eventType,
			 masterEvent->event.parameter1,
			 masterEvent->event.parameter2);
	return 0;
}


uint8_t PS_RecordMissedNote(MPB_CL_EVENT_t* masterEvent)
{

	xprintf("Missed Note!: %X, %X, %X\n",
			 masterEvent->event.eventType,
			 masterEvent->event.parameter1,
			 masterEvent->event.parameter2);


	PS_NoteCount[PS_MISSED_NOTES]++;

	return 0;

}


uint8_t PS_RecordIncorrectNote(MPB_CL_EVENT_t* masterEvent)
{

	xprintf("BAD Note!: %X, %X, %X\n",
			 masterEvent->event.eventType,
			 masterEvent->event.parameter1,
			 masterEvent->event.parameter2);

	PS_NoteCount[PS_INCORRECT_NOTE]++;

	return 0;

}


























