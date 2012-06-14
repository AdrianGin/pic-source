


#include "midiparser.h"
#include <string.h>
#include "hardwareSpecific.h"

PROGRAM_CHAR MIDI_NOTE_OFF_STRING[] PROGRAM_SPACE = "NOTE OFF";
PROGRAM_CHAR MIDI_NOTE_ON_STRING[] PROGRAM_SPACE = "NOTE ON";
PROGRAM_CHAR MIDI_POLY_STRING[] PROGRAM_SPACE = "AFTRTCH";
PROGRAM_CHAR MIDI_CONTROL_CHANGE_STRING[] PROGRAM_SPACE = "CONT.CHNG";
PROGRAM_CHAR MIDI_PROGRAM_CHANGE_STRING[] PROGRAM_SPACE = "PROG.CHNG";
PROGRAM_CHAR MIDI_CHANNEL_PRESSURE_STRING[] PROGRAM_SPACE = "CHAN.PRES";
PROGRAM_CHAR MIDI_PITCH_WHEEL_STRING[] PROGRAM_SPACE = "PITCH W.";
PROGRAM_CHAR MIDI_SYSEX_STRING[] PROGRAM_SPACE = "SYSEX";

PROGRAM_CHAR MIDI_TIME_CODE_STRING[] PROGRAM_SPACE = "TIMECODE";
PROGRAM_CHAR MIDI_SONG_POS_STRING[] PROGRAM_SPACE = "SONG.POS";
PROGRAM_CHAR MIDI_SONG_SEL_STRING[] PROGRAM_SPACE = "SONG.SEL";

PROGRAM_CHAR MIDI_TUNE_REQUEST_STRING[] PROGRAM_SPACE = "TUNE REQ";
PROGRAM_CHAR MIDI_END_SYSEX_STRING[] PROGRAM_SPACE = "END SYSX";

PROGRAM_CHAR MIDI_TIMING_CLOCK_STRING[] PROGRAM_SPACE = "CLOCK";
PROGRAM_CHAR MIDI_SEQ_START_STRING[] PROGRAM_SPACE = "START";
PROGRAM_CHAR MIDI_SEQ_CONTINUE_STRING[] PROGRAM_SPACE = "CONTIN.";
PROGRAM_CHAR MIDI_SEQ_STOP_STRING[] PROGRAM_SPACE = "STOP";

PROGRAM_CHAR MIDI_ACTIVE_SENSING_STRING[] PROGRAM_SPACE = "ACT.SENS";
PROGRAM_CHAR MIDI_RESET_STRING[] PROGRAM_SPACE = "RESET";

PROGRAM_CHAR MIDI_UNDEFINED_STRING[] PROGRAM_SPACE = "UNDEF";


const MidiLookup_t MidiLookUpTable[] PROGRAM_SPACE = {
    {MIDI_NOTE_ON, MIDI_NOTE_ON_STRING, 3},
    {MIDI_NOTE_OFF, MIDI_NOTE_OFF_STRING, 3},
    {MIDI_AFTERTOUCH, MIDI_POLY_STRING, 3},
    {MIDI_CONTROL_CHANGE, MIDI_CONTROL_CHANGE_STRING, 3},
    {MIDI_PROGRAM_CHANGE, MIDI_PROGRAM_CHANGE_STRING, 2},
    {MIDI_CHANNEL_PRESSURE, MIDI_CHANNEL_PRESSURE_STRING, 2},
    {MIDI_PITCH_CHANGE, MIDI_PITCH_WHEEL_STRING, 3},
    {MIDI_SYSEX_START, MIDI_SYSEX_STRING, 3},
    {MIDI_TIME_CODE, MIDI_TIME_CODE_STRING, 2},
    {MIDI_SONG_POSITION, MIDI_SONG_POS_STRING, 3},
    {MIDI_SONG_SELECT, MIDI_SONG_SEL_STRING, 2},
    {MIDI_TUNE_REQUEST, MIDI_TUNE_REQUEST_STRING, 1},
    {MIDI_SYSEX_STOP, MIDI_END_SYSEX_STRING, 1},
    {MIDI_TIMING_CLOCK, MIDI_TIMING_CLOCK_STRING, 1},
    {MIDI_RT_TICK, MIDI_UNDEFINED_STRING, 1},
    {MIDI_RT_START, MIDI_SEQ_START_STRING, 1},
    {MIDI_RT_CONTINUE, MIDI_SEQ_CONTINUE_STRING, 1},
    {MIDI_RT_STOP, MIDI_SEQ_STOP_STRING, 1},
    {MIDI_RT_ACTIVE_SENSE, MIDI_ACTIVE_SENSING_STRING, 1},
    {MIDI_RT_RESET, MIDI_RESET_STRING, 1},
    {0, 0}
};

uint16_t MIDIParse_Header(MIDI_HEADER_CHUNK_t* header, void* data, uint32_t size)
{
    char* ptr = (char*)data;

    if (ptr)
    {
        uint16_t* tmp;
        tmp = &ptr[MIDI_HEADER_FMT];
        reverseOrder(tmp, 2);
        header->format = *tmp;

        tmp = &ptr[MIDI_HEADER_TRACK_COUNT];
        reverseOrder(tmp, 2);
        header->trackCount = *tmp;

        tmp = &ptr[MIDI_HEADER_PPQ];
        reverseOrder(tmp, 2);
        header->PPQ = *tmp;
        return (uint16_t)ptr - (uint16_t)data;
    }

    return 0;
}


/* Example */
//    position = position + MIDI_HEADER_SIZE;
//    for (i = 0; i<MIDIHdr.trackCount; i++)
//    {
//        f_lseek(&testFIL, position);
//        f_read(&testFIL, buffer, BUFFER_READ_SIZE, &br);
//        position = MIDIPopulate_HeaderTrack(&MIDIHdr, i, position, &buffer[0], BUFFER_READ_SIZE);
//    }
/* Returns the position of the MIDI file that the top level should read to */
uint32_t MIDIPopulate_HeaderTrack(MIDI_HEADER_CHUNK_t* header, uint8_t trackNo, uint32_t filePos, void* data, uint32_t size)
{
    uint8_t* temp;
    uint8_t i;
    header->Track[trackNo].startPtr = filePos + MIDI_TRACK_LENGTH + 4;
    temp = MIDIParse_Track(&header->Track[trackNo], data, size);
    return filePos + MIDI_TRACK_HEADER_SIZE + header->Track[trackNo].length;
}

//Returns the pointer to the first byte of the track data
void* MIDIParse_Track(MIDI_TRACK_CHUNK_t* track, void* data, uint32_t size)
{
    char* ptr = findSubString(data, MIDI_TRACK_STRING, size);
    if (ptr)
    {
        uint32_t* tmp;
        tmp = &ptr[MIDI_TRACK_LENGTH];
        reverseOrder(tmp, 4);
        track->length = *tmp;
        //myprintf("TRACKLENG: ", track->length);
        return &ptr[MIDI_TRACK_LENGTH+4];
    }

    return 0;
}

void* MIDIParse_Event(MIDI_EVENT_t* event, uint8_t* data)
{
    uint32_t byteOffset = 0;
    static uint8_t runningStatus;

    byteOffset = midiparse_variableLength(data, &event->deltaTime);

    event->eventType = data[byteOffset];
    //for running status
    if (event->eventType<MIDI_NOTE_OFF)
    {
        event->eventType = runningStatus;
        byteOffset--;
    }


    if (event->eventType==MIDI_SYSEX_START)
    {
        midiparse_variableLength(&data[byteOffset+1], &event->sysExEvent.length);
        
        event->sysExEvent.data = &data[byteOffset+2];
        //rawDump(event->sysExEvent.data, event->sysExEvent.length);
        return &data[byteOffset+2+event->sysExEvent.length];
    }

    if (event->eventType==MIDI_META_MSG)
    {
        event->metaEvent.type = data[byteOffset+1];
        
        midiparse_variableLength(&data[byteOffset+2], &event->metaEvent.length);
        
        event->metaEvent.data = &data[byteOffset+3];
        //myprintf("DATA: %s\n",event->metaEvent.data);
        //rawDumpStr(event->metaEvent.data, event->metaEvent.length);
        return &data[byteOffset+3+event->metaEvent.length];
    }

    uint8_t bytesToSend = MIDI_CommandSize(event->eventType&0xF0);
    if (bytesToSend>1)
    {
        runningStatus = event->eventType;
        event->chanEvent.parameter1 = data[byteOffset+1];
        if (bytesToSend>2)
        {
            event->chanEvent.parameter2 = data[byteOffset+2];
        }
    }

    return &data[byteOffset+bytesToSend];

}

void MIDI_PrintEventInfo(MIDI_EVENT_t* event)
{
    myprintf("DT: ", event->deltaTime);
    myprintf("ET: ", event->eventType);

    switch (event->eventType)
    {
        case MIDI_SYSEX_START:
            myprintf("LEN: ", event->sysExEvent.length);
            break;
            
        case MIDI_META_MSG:
            myprintf("TYPE: ", event->metaEvent.type);
            myprintf("LEN: ", event->metaEvent.length);
            break;

        default:
            myprintf("P1: ", event->chanEvent.parameter1);
            myprintf("P2: ", event->chanEvent.parameter2);
            break;
    }
}

/* Obtain the control code size */
uint8_t MIDI_CommandSize(uint8_t command)
{
    uint8_t i;
    for (i = 0; FLASH_GET_PGM_BYTE(MidiLookUpTable[i].MIDI_Commands); i++)
    {
        if (command==FLASH_GET_PGM_BYTE(MidiLookUpTable[i].MIDI_Commands))
        {
            return FLASH_GET_PGM_BYTE(MidiLookUpTable[i].MIDI_MsgSize);
        }
    }
    return 1;
}

void rawDumpStr(uint8_t* data, uint16_t len)
{
    uint16_t i;
    printf("Data:");
    for (i = 0; i<len; i++)
    {
        printf("%c", data[i]);
    }
    printf("\n");
}

void rawDump(uint8_t* data, uint16_t len)
{
    uint16_t i;
    printf("Data:");
    for (i = 0; i<len; i++)
    {
        printf("%X", data[i]);
    }
    printf("\n");

}

//Returns the length of the variable length field

uint8_t midiparse_variableLength(uint8_t* data, uint32_t* retValue)
{
    uint32_t value = 0;
    uint8_t i;
    for (i = 0; i<4; i++)
    {
        value = value+(data[i] & 0x7F);
        if (data[i] & 0x80)
        {
            value = value<<7;
        }
        else
        {
            break;
        }
    }
    *retValue = value;
    return i+1;
}

void reverseOrder(char* array, uint8_t len)
{
    uint8_t i;
    char tempByte;
    for (i = 0; i<len/2; i++)
    {
        tempByte = array[i];
        array[i] = array[len-i-1];
        array[len-i-1] = tempByte;
    }
}

void* findSubString(char* data, char* string, uint32_t numBytes)
{
    uint32_t i;
    for (i = 0; i<numBytes; i++)
    {
        char* ptr = 0;
        if (data[i]==string[0])
        {
            ptr = strstr(&data[i], string);
            if (ptr)
            {
                return ptr;
            }
        }
    }
    return 0;
}















