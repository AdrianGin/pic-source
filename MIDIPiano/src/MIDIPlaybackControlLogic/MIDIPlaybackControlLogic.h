#ifndef _MPB_CL_LOGIC_H
#define _MPB_CL_LOGIC_H



#include <stdint.h>
#include "MIDIParser/midiparser.h"




#define MPB_CL_CHANNEL_IS_ACTIVE(map, channel) MPB_CL_IsChannelActive(map, channel)

#define MPB_CL_PULSATE_THRESHOLD_LO	(6)
#define MPB_CL_PULSATE_THRESHOLD_HI	(100)



#define PULSATE_PERCENT_INC			(80)
#define PULSATE_PERCENT_DEC			(65)


#define MARK_EVENT_FOR_DELETION	(1)

#define FADE_PERCENT_RESTORE	    (((10000 / (100-PULSATE_PERCENT_DEC)) - 100) + 1)

#define HALT_FLAG_RAISED (1)
#define MATCH_MODE_MASK			(0x1F)
#define MATCH_FLAG_MASK	(0xFF)
typedef enum
{
	ALLOW_SLIDE		     = (1<<4),
	ALLOW_PREHITS		 = (1<<5),
	REQUIRE_NOTE_RELEASE = (1<<6),
	OCTAVE_MATCH_MODE    = (1<<7),
} MPB_CL_MatchFlags;

#define MAX_NOTES_TO_HALT	(20)

typedef enum {
	MASTER_HALT_LIST = 0,
	TESTER_HALT_ON_LIST,
	HALT_LIST_COUNT
} MPB_CL_LIST_TYPE_t;


typedef enum
{
	//Which MIDI channels are routed to the lights
	MPB_CL_LIGHTS = 0,
	//Which MIDI channels are routed to the MIDI Out
	MPB_CL_MIDIOUT,
	//Which MIDI channels require the user to enter the correct key before
	//proceeding
	MPB_CL_HALT,
	MPB_CL_SOLO,
	MPB_CL_MAP_COUNT,
} MPB_CL_CH_MAP_t;


enum {
	EXACT_MATCH = 0,
	EXACT_PLUS_MATCH,
	HALF_CORRECT,
	MINIMUM_COUNT,
	PROCEED_ON_ANY,
	AUTO_PLAY_FOLLOW,
	MATCH_MODE_COUNT
};


typedef struct
{
	uint32_t midiClockTime;
	uint8_t  markForDeletion;
	MIDI_CHAN_EVENT_t event;
} MPB_CL_EVENT_t;

void MPB_CL_Init(void);
//channel refers to the MIDI channel
uint16_t MPB_CL_ToggleChannel(MPB_CL_CH_MAP_t map, uint8_t channel);
uint16_t MPB_CL_ActivateChannel(MPB_CL_CH_MAP_t map, uint8_t channel);
uint16_t MPB_CL_DisableChannel(MPB_CL_CH_MAP_t map, uint8_t channel);
void MPB_CL_SetChannelMaps(MPB_CL_CH_MAP_t map, uint16_t state);

uint8_t MPB_CL_IsChannelActive(MPB_CL_CH_MAP_t map, uint8_t channel);

void MPB_CL_ProcessMIDIByte(uint8_t byte);

void MPB_CL_OutputMIDI(void);
void MPB_CL_OutputLighting(void);

void MPB_CL_DeleteMarkedItemsFromList(MPB_CL_LIST_TYPE_t listMap);
void MPB_CL_ClearSingleHaltList(MPB_CL_LIST_TYPE_t listMap);
void MPB_CL_ClearHaltList(void);
void MPB_CL_SetHaltFlag(uint8_t state);
uint8_t MPB_CL_GetHaltFlag(void);

void MPB_CL_SetMatchMode(uint8_t mode);
uint8_t MPB_CL_GetMatchMode(void);
void MPB_CL_SetMatchFlags(uint8_t mode);
uint8_t MPB_CL_GetMatchFlags(void);

void MPB_CL_AddHaltMasterNote(uint8_t* midiDataArray);
void MPB_CL_ProcessNewHaltNote(uint8_t* midiDataArray);
//midiDataArray:: A 3 byte array pointer
void MPB_CL_AddTesterHaltNote(uint8_t* midiDataArray);
void MPB_CL_TesterHaltCancelNotes(uint8_t* midiDataArray);
void MPB_CL_ProcessHaltList(void);


uint8_t MPB_CL_DetermineProceedAction(uint8_t matchMode, uint8_t matchCount);

void MPB_CL_CheckListExpiry(MPB_CL_LIST_TYPE_t listMap, uint32_t PPQ);
void MPB_CL_ProcessPulsateHaltList(void);

void MPB_CL_ProcessSingleCorrectKeyPress(MIDI_CHAN_EVENT_t* masterEvent);
void MPB_CL_ProcessAllCorrectKeyPress(void);



#endif
