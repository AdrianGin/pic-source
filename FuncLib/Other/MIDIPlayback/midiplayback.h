#ifndef MIDI_PLAYBACK_H
#define MIDI_PLAYBACK_H

#include "MIDIParser/midiparser.h"
#include "hardwareSpecific.h"
#include <stdint.h>

#define MPB_EVENT_STACK_SIZE (16)
#define MPB_DEFAULT_TEMPO (120)
#define MPB_DEFAULT_PPQ (96)
//Continue Playing Status
enum
{
    MPB_FILE_STILL_PLAYING = 0,
    MPB_FILE_FINISHED,
    
};

//Playback Modes
enum
{
    MPB_PB_ALL_ON = 0,
    MPB_PB_NO_NOTES, //no notes and no lyrics too
    MPB_PB_ALL_OFF,
};


enum
{
    MPB_TRACK_STOPPED = (0),
    MPB_NEW_DATA,
    MPB_PLAY_NEXT_EVENT,
    MPB_QUEUED_EVENT,
    MPB_REPOSITION_EVENT,
};



void MPB_PlayMIDIFile(MIDI_HEADER_CHUNK_t* MIDIHdr, uint8_t* filename);
uint8_t MPB_PlayTrack(MIDI_HEADER_CHUNK_t* MIDIHdr, MIDI_TRACK_CHUNK_t* track, uint8_t mode);
uint8_t MPB_AddEvent(MIDI_EVENT_t* event);
void MPB_ProcessMetaEvent(MIDI_HEADER_CHUNK_t* MIDIHdr, MIDI_TRACK_CHUNK_t* track, MIDI_EVENT_t* event);
void MPB_PlayEvent(MIDI_EVENT_t* event, uint8_t mode);
void MPB_SetTickRate(uint16_t BPM, uint16_t PPQ);
void MPB_ResetMIDI(void);

void MPB_CurrentTimePosition(MIDI_HEADER_CHUNK_t* MIDIHdr);
void MPB_DetermineLength(MIDI_HEADER_CHUNK_t* MIDIHdr);

uint8_t MPB_RePosition(MIDI_HEADER_CHUNK_t* MIDIHdr, uint32_t position, uint8_t mode);
uint8_t MPB_ContinuePlay(MIDI_HEADER_CHUNK_t* MIDIHdr, uint16_t trackBitmap);
void*   _MIDI_readbuf(uint32_t position, uint8_t* buf, uint16_t size);
uint8_t _MIDI_fileopen(uint8_t* filename);

#define MPB_ReadToBuffer(position, buf) _MIDI_readbuf(position, buf, MIDI_TRACK_BUFFER_SIZE)
#define MPB_OpenFile(x) _MIDI_fileopen(x)

#define MIDI_Tx(x) uartTx(PrimaryUART, x);

MIDI_EVENT_t* MPB_GetNextEvent(void);
MIDI_EVENT_t* MPB_ConfirmEventTx(void);

#endif




