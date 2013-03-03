#include "MIDILightLogic/MIDILightLogic.h"
#include <stdint.h>
#include "LightSys/LightSys.h"


/* Turns off all distractions such as lights,
   backing track and aims to allow the user to practice
   JUST the selected track.
*/
void GFL_User_InitiatePractice(uint8_t channel)
{

	MPB_StopAllSounds();

	MLL_SetChannelMaps(MLL_LIGHTS, (1<<channel));
	MLL_SetChannelMaps(MLL_MIDIOUT, 0xFFFF & ~(1<<channel));
	MLL_SetChannelMaps(MLL_HALT, (1<<channel));
	MLL_SetChannelMaps(MLL_SOLO, 0x0000);

	MLL_SetMatchMode(EXACT_MATCH);
	MLL_SetMatchFlags(REQUIRE_NOTE_RELEASE);

	LS_ClearLights();
	MLL_ClearHaltList();
}

void GFL_User_InitiateFollow(uint8_t channel)
{

	MLL_SetChannelMaps(MLL_LIGHTS, (1<<channel));
	MLL_SetChannelMaps(MLL_MIDIOUT, 0xFFFF & ~(1<<channel));
	MLL_SetChannelMaps(MLL_HALT, 0x0000);
	MLL_SetChannelMaps(MLL_SOLO, 0x0000);

	MLL_SetMatchMode(EXACT_MATCH);
	MLL_SetMatchFlags(REQUIRE_NOTE_RELEASE);

	LS_ClearLights();
	MLL_ClearHaltList();
}
