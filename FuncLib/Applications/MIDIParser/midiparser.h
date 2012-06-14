

#ifndef MIDI_PARSER_H
#define MIDI_PARSER_H


#include <stdint.h>
#include "MIDIParser/MIDICodes.h"

#define MIDI_HEADER_STRING   "MThd"
#define MIDI_TRACK_STRING    "MTrk"

//MIDI Header / Track byte offsets
#define MIDI_HEADER_FMT          (0x08)
#define MIDI_HEADER_TRACK_COUNT  (0x0A)
#define MIDI_HEADER_PPQ          (0x0C)

#define MIDI_HEADER_SIZE         (0x0E)
#define MIDI_TRACK_HEADER_SIZE   (0x08)
#define MIDI_TRACK_LENGTH        (0x04)

#define MIDI_META_MSG            (0xFF)
#define MIDI_META_SEQUENCE       (0x00)
#define MIDI_META_TEXT           (0x01)
#define MIDI_META_COPY_WRITE     (0x02)
#define MIDI_META_TRACK_NAME     (0x03)
#define MIDI_META_INSTNAME       (0x04)
#define MIDI_META_LYRICS         (0x05)
#define MIDI_META_MARKER         (0x06)
#define MIDI_META_CUEPOINT       (0x07)
#define MIDI_META_CHANPREFIX     (0x20)
#define MIDI_META_TRACKEND       (0x2F)
#define MIDI_META_TEMPO          (0x51)
#define MIDI_META_SMPTE          (0x54)
#define MIDI_META_TIME_SIG       (0x58)
#define MIDI_META_KEY_SIG        (0x59)
#define MIDI_META_SEQ_SPEC       (0x7F)

#define US_PER_MINUTE            (60000000)
#define BPM(x)                   (US_PER_MINUTE/(x))

#define BUFFER_READ_SIZE         (512)
#define MAX_MIDI_TRACKS          (16)

#define TRACK_EVENT_BUFFER_SIZE  (2)

#define PROGRAM_CHAR const char
#define PROGRAM_SPACE
#define FLASH_GET_PGM_BYTE

typedef struct {

   uint8_t MIDI_Commands;
   PROGRAM_CHAR* MIDI_CommandString;
   uint8_t MIDI_MsgSize;

} MidiLookup_t;




typedef struct
{
    uint8_t   parameter1;
    uint8_t   parameter2;   
} MIDI_CHAN_EVENT_t;

typedef struct
{
    uint32_t  length;
    uint8_t*  data;   
} MIDI_SYSEX_EVENT_t;

typedef struct
{
    uint8_t   type;
    uint32_t  length;
    uint8_t*  data;   
} MIDI_META_EVENT_t;

typedef struct
{
    uint32_t  deltaTime;
    uint8_t   eventType;
    
    union
    {
       MIDI_CHAN_EVENT_t  chanEvent;
       MIDI_SYSEX_EVENT_t sysExEvent;
       MIDI_META_EVENT_t  metaEvent;
    };    
    
} MIDI_EVENT_t;

#define MIDI_TRACK_BUFFER_SIZE  (32)

typedef struct
{
    uint32_t startPtr;
    uint32_t length;
    uint32_t position;
    uint8_t  eventCount;
    MIDI_EVENT_t trackEvent;
    uint8_t  buffer[MIDI_TRACK_BUFFER_SIZE];
    uint8_t  bufferOffset;
} MIDI_TRACK_CHUNK_t;

typedef struct
{
    uint16_t format;
    uint16_t trackCount;
    uint16_t PPQ;
    MIDI_TRACK_CHUNK_t Track[MAX_MIDI_TRACKS];
} MIDI_HEADER_CHUNK_t;


void rawDump(uint8_t* data, uint16_t len);
void rawDumpStr(uint8_t* data, uint16_t len);

uint16_t MIDIParse_Header(MIDI_HEADER_CHUNK_t* header, void* data, uint32_t size);
uint32_t MIDIPopulate_HeaderTrack(MIDI_HEADER_CHUNK_t* header, uint8_t trackNo, uint32_t filePos, void* data, uint32_t size);
void* MIDIParse_Track(MIDI_TRACK_CHUNK_t* track, void* data, uint32_t size);
void* MIDIParse_Event(MIDI_EVENT_t* event, uint8_t* data);

void MIDI_PrintEventInfo(MIDI_EVENT_t* event);

uint8_t MIDI_CommandSize(uint8_t command);
//Returns the length of the variable length field
uint8_t midiparse_variableLength(uint8_t* data, uint32_t* retValue);


void reverseOrder(char* array, uint8_t len);
void* findSubString(char* data, char* string, uint32_t numBytes);






#endif