#ifndef _MLL_LOGIC_H
#define _MLL_LOGIC_H



#include <stdint.h>

#define MLL_CHANNEL_IS_ACTIVE(map, channel) (MLL_ActiveChannelMap[map] & (1<<channel))

#define MLL_PULSATE_THRESHOLD_LO	(6)
#define MLL_PULSATE_THRESHOLD_HI	(100)



#define PULSATE_PERCENT_INC			(80)
#define PULSATE_PERCENT_DEC			(65)


#define FADE_PERCENT_RESTORE	    (((10000 / (100-PULSATE_PERCENT_DEC)) - 100) + 1)



#define MATCH_MODE_MASK			(0x1F)
#define MATCH_FLAG_MASK	(0xC0)
#define OCTAVE_MATCH_MODE    (1<<7)
#define REQUIRE_NOTE_RELEASE (1<<6)

typedef enum
{
	//Which MIDI channels are routed to the lights
	MLL_LIGHTS = 0,
	//Which MIDI channels are routed to the MIDI Out
	MLL_MIDIOUT,
	//Which MIDI channels require the user to enter the correct key before
	//proceeding
	MLL_HALT,
	MLL_SOLO,
	MLL_MAP_COUNT,
} MLL_CH_MAP_t;


enum {
	EXACT_MATCH = 0,
	EXACT_PLUS_MATCH,
	HALF_CORRECT,
	MINIMUM_COUNT,
	PROCEED_ON_ANY,
	AUTO_PLAY,
	MATCH_MODE_COUNT
};


void MLL_Init(void);
//channel refers to the MIDI channel
uint32_t MLL_ToggleChannel(MLL_CH_MAP_t map, uint8_t channel);
uint32_t MLL_ActivateChannel(MLL_CH_MAP_t map, uint8_t channel);
uint32_t MLL_DisableChannel(MLL_CH_MAP_t map, uint8_t channel);



void MLL_ProcessMIDIByte(uint8_t byte);

void MLL_OutputMIDI(void);
void MLL_OutputLighting(void);


#define HALT_FLAG_RAISED (1)
void MLL_ClearHaltList(void);
void MLL_SetHaltFlag(uint8_t state);
uint8_t MLL_GetHaltFlag(void);

void MLL_SetMatchMode(uint8_t mode);
uint8_t MLL_GetMatchMode(void);
void MLL_SetMatchFlags(uint8_t mode);
uint8_t MLL_GetMatchFlags(void);

void MLL_AddHaltMasterNote(uint8_t* midiDataArray);


void MLL_ProcessHaltNote(uint8_t* midiDataArray);
//midiDataArray:: A 3 byte array pointer
void MLL_AddTesterHaltNote(uint8_t* midiDataArray);
void MLL_TesterHaltCancelNotes(uint8_t* midiDataArray);
void MLL_CompareMasterTesterHaltList(uint8_t matchMode);
void MLL_ProcessPulsateHaltList(void);


#endif
