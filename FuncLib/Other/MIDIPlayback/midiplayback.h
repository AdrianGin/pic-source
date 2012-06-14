#ifndef MIDI_PLAYBACK_H
#define MIDI_PLAYBACK_H

#include "MIDIParser/midiparser.h"
#include <stdint.h>

#define MPB_EVENT_STACK_SIZE (16)


enum
{
    MPB_NEW_DATA  = (0),
    MPB_BUFFERED_DATA,
    MPB_EXISTING_DATA,
    MPB_TRACK_STOPPED,
};


void MPB_PlayTrack(MIDI_HEADER_CHUNK_t* MIDIHdr, MIDI_TRACK_CHUNK_t* track);
uint8_t MPB_AddEvent(MIDI_EVENT_t* event);
void MPB_PlayEvent(MIDI_EVENT_t* event);
void MPB_SetTickRate(uint16_t BPM, uint16_t PPQ);

#define MPB_ReadPosition(position) locall_read(position)
#define MPB_ReadToBuffer(position, buf) local_readbuf(position, buf, MIDI_TRACK_BUFFER_SIZE)

MIDI_EVENT_t* MPB_GetNextEvent(void);
MIDI_EVENT_t* MPB_ConfirmEventTx(void);

#endif




