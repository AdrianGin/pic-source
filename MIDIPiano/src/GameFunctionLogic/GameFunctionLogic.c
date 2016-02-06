#include "MIDILightLogic/MIDILightLogic.h"
#include <stdint.h>
#include "LightSys/LightSys.h"

#include "MIDIPlaybackControlLogic/MIDIPlaybackControlLogic.h"


/* Turns off all distractions such as lights,
   backing track and aims to allow the user to practice
   JUST the selected track.
*/
void GFL_User_InitiatePractice(uint8_t channel)
{

	MPB_StopAllSounds();

	MPB_CL_SetChannelMaps(MPB_CL_LIGHTS, (1<<channel));
	MPB_CL_SetChannelMaps(MPB_CL_MIDIOUT, 0xFFFF & ~(1<<channel));
	MPB_CL_SetChannelMaps(MPB_CL_HALT, (1<<channel));
	MPB_CL_SetChannelMaps(MPB_CL_SOLO, 0x0000);

	MPB_CL_SetMatchMode(EXACT_MATCH);
	MPB_CL_SetMatchFlags(ALLOW_PREHITS | ALLOW_SLIDE | REQUIRE_NOTE_RELEASE);

	LS_ClearLights();
	MPB_CL_ClearHaltList();
}

void GFL_User_InitiateFollow(uint8_t channel)
{

	MPB_CL_SetChannelMaps(MPB_CL_LIGHTS, (1<<channel));
	MPB_CL_SetChannelMaps(MPB_CL_MIDIOUT, 0xFFFF & ~(1<<channel));
	MPB_CL_SetChannelMaps(MPB_CL_HALT, (1<<channel));
	MPB_CL_SetChannelMaps(MPB_CL_SOLO, 0x0000);

	MPB_CL_SetMatchMode(AUTO_PLAY_FOLLOW);
	MPB_CL_SetMatchFlags(ALLOW_PREHITS | ALLOW_SLIDE | REQUIRE_NOTE_RELEASE);

	LS_ClearLights();
	MPB_CL_ClearHaltList();
}

void GFL_User_Spectate(uint8_t channel)
{

	MPB_CL_SetChannelMaps(MPB_CL_LIGHTS, (1<<channel));
	MPB_CL_SetChannelMaps(MPB_CL_MIDIOUT, 0xFFFF);
	MPB_CL_SetChannelMaps(MPB_CL_HALT, 0);
	MPB_CL_SetChannelMaps(MPB_CL_SOLO, 0x0000);

	MPB_CL_SetMatchMode(AUTO_PLAY_FOLLOW);
	MPB_CL_SetMatchFlags(ALLOW_PREHITS | ALLOW_SLIDE | REQUIRE_NOTE_RELEASE);

	LS_ClearLights();
	MPB_CL_ClearHaltList();
}

