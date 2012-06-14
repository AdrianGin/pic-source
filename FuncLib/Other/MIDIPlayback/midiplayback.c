


#include "midiplayback.h"
#include "MIDIParser/midiparser.h"
#include <stdint.h>
#include "stack/stack.h"

#include "hardwareSpecific.h"

void* MPBarray[sizeof (void*)* MPB_EVENT_STACK_SIZE];
STACK_t MPBStack = {MPBarray, 0, 0, MPB_EVENT_STACK_SIZE};

FIL midifile;


void MPB_ResetMIDI(void)
{
    MIDI_EVENT_t event;
    uint8_t i;

    for (i = 0; i<MAX_MIDI_CHANNEL; i++)
    {
        event.eventType = MIDI_CONTROL_CHANGE|i;
        event.chanEvent.parameter1 = ALL_NOTES_OFF;
        event.chanEvent.parameter2 = 0;
        MPB_PlayEvent(&event, MPB_PB_ALL_ON);
    }
}

void MPB_PlayMIDIFile(MIDI_HEADER_CHUNK_t* MIDIHdr, uint8_t* filename)
{
    uint8_t i;
    uint32_t position = 0;
    uint8_t* buf = &MIDIHdr->Track[0].buffer[0];
    uint8_t* ptr;
    uint8_t ret;

    MPB_ResetMIDI();

    ret = MPB_OpenFile(filename);
    memset(MIDIHdr, 0, sizeof(MIDI_HEADER_CHUNK_t));

    if( ret != FR_OK)
    {
        return ret;
    }
    MPB_ReadToBuffer(position, buf);
    ptr = findSubString(buf, MIDI_HEADER_STRING, MIDI_TRACK_BUFFER_SIZE);
    if( ptr )
    {
        if( (ptr - buf + MIDI_HEADER_SIZE) > MIDI_TRACK_BUFFER_SIZE)
        {
            MPB_ReadToBuffer(ptr-buf, buf);
            position = ptr-buf;
        }
    }

    position = position + MIDIParse_Header(MIDIHdr, buf, BUFFER_READ_SIZE);
    position = position+MIDI_HEADER_SIZE;
    for (i = 0; i<MIDIHdr->trackCount; i++)
    {
        MPB_ReadToBuffer(position, buf);
        position = MIDIPopulate_HeaderTrack(MIDIHdr, i, position, buf, BUFFER_READ_SIZE);
    }

    for (i = 0; i<MIDIHdr->trackCount; i++)
    {
        MIDIHdr->Track[i].eventCount = MPB_NEW_DATA;
    }
}

uint8_t MPB_ContinuePlay(MIDI_HEADER_CHUNK_t* MIDIHdr, uint16_t trackBitmap)
{
    uint8_t i;
    for (i = 0; i<MIDIHdr->trackCount; i++)
    {
        if( ((1<<i)&trackBitmap) && (MIDIHdr->Track[i].eventCount != MPB_TRACK_STOPPED) )
        {
            if( MPB_PlayTrack(MIDIHdr, &MIDIHdr->Track[i], MPB_PB_ALL_ON) == MPB_TRACK_STOPPED)
            {
                MIDIHdr->TrackFlags++;
                myprintfd("TrackFinish: ", MIDIHdr->TrackFlags);
                if(MIDIHdr->TrackFlags == MIDIHdr->trackCount)
                {
                    return MPB_FILE_FINISHED;
                }

            }
        }
    }
    return MPB_FILE_STILL_PLAYING;
}

uint8_t MPB_RePosition(MIDI_HEADER_CHUNK_t* MIDIHdr, uint16_t trackBitmap)
{
    uint8_t i;
    uint8_t ret = 0;
    for (i = 0; i<MIDIHdr->trackCount; i++)
    {
        ret = 0;
        if( ((1<<i)&trackBitmap) && (MIDIHdr->Track[i].eventCount != MPB_TRACK_STOPPED) )
        {
            while( ret == 0 )
            {
                ret = MPB_PlayTrack(MIDIHdr, &MIDIHdr->Track[i], MPB_PB_NO_NOTES);
                if((ret == MPB_REPOSITION_EVENT))
                {
                    ret = 1;
                }
                else
                {
                    if(ret == MPB_TRACK_STOPPED)
                    {
                        MIDIHdr->TrackFlags++;
                        myprintfd("TrackFinish: ", MIDIHdr->TrackFlags);
                        ret = 1;
                    }
                    else
                    {
                        ret = 0;
                    }
                }
            }
        }
    }
    return MPB_FILE_STILL_PLAYING;
}



uint8_t MPB_PlayTrack(MIDI_HEADER_CHUNK_t* MIDIHdr, MIDI_TRACK_CHUNK_t* track, uint8_t mode)
{
    uint32_t position = 0;
    uint32_t oldPosition = 0;
    uint8_t* readPtr;
    uint8_t* originPtr;
    MIDI_EVENT_t* event = &track->trackEvent;

    if (track->eventCount==MPB_TRACK_STOPPED)
    {
        return MPB_TRACK_STOPPED;
    }

    if (track->eventCount==MPB_NEW_DATA)
    {
        event->deltaTime = 0;
    }
    else
    {
        if( track->trackClock <= MIDIHdr->masterClock)
        //if (event->deltaTime==0)
        {
            MPB_PlayEvent(event, mode);
            track->eventCount = MPB_PLAY_NEXT_EVENT;
            event->deltaTime = 0;
        }
        else
        {
            //Not yet time to play;
            //event->deltaTime--;
            return MPB_REPOSITION_EVENT;
        }
    }

    while ((event->deltaTime==0) && (track->eventCount != MPB_TRACK_STOPPED))
    {
        if (track->eventCount==MPB_NEW_DATA)
        {
            originPtr = MPB_ReadToBuffer(track->startPtr, &track->buffer[0]);
            readPtr = originPtr;
            oldPosition = readPtr;
            position = track->startPtr;
            track->bufferOffset = 0;
        }

        //If the buffer doesn't contain enough information for a full parse
        if ((track->bufferOffset+6)>MIDI_TRACK_BUFFER_SIZE)
        {
            track->eventCount = MPB_NEW_DATA;
            continue;
        }

        oldPosition = readPtr;
        //MIDI Parse Event returns the ptr to the next event.
        readPtr = MIDIParse_Event(event, readPtr);
        track->bufferOffset = readPtr-originPtr;

        //On successful parse, we save the position.
        if (track->bufferOffset<=MIDI_TRACK_BUFFER_SIZE)
        {
            position = (position+(uint32_t)(readPtr)-oldPosition);
            track->startPtr = position;

            if (event->deltaTime)
            {
                track->trackClock = track->trackClock + event->deltaTime;
                track->eventCount = MPB_QUEUED_EVENT;
                //event->deltaTime--;
                return MPB_QUEUED_EVENT;
            }
            else
            {
                MPB_PlayEvent(event, mode);
                track->eventCount = MPB_PLAY_NEXT_EVENT;
                
                if (event->eventType==MIDI_META_MSG&&event->metaEvent.type==MIDI_META_TEMPO)
                {
                    uint32_t bpm;
                    reverseOrder(event->metaEvent.data, event->metaEvent.length);
                    memcpy(&bpm, event->metaEvent.data, 4); //*bpmptr;
                    MPB_SetTickRate(BPM(bpm), MIDIHdr->PPQ);
                }

                if (event->eventType==MIDI_META_MSG&&event->metaEvent.type==MIDI_META_TRACKEND)
                {
                    track->eventCount = MPB_TRACK_STOPPED;
                    return MPB_TRACK_STOPPED;
                }
            }
        }
        else
        {
            //If the parse was not fully completed due to incomplete read data,
            //We must read it again.
            track->eventCount = MPB_NEW_DATA;
            event->deltaTime = 0;

            //We skip if the lengths are too long
            if (event->eventType==MIDI_META_MSG)
            {
                if (event->metaEvent.length>(MIDI_TRACK_BUFFER_SIZE-6))
                {
                    position = (position+(uint32_t)(readPtr)-oldPosition);
                    track->startPtr = position;
                }
            }
            if (event->eventType==MIDI_SYSEX_START)
            {
                if (event->sysExEvent.length>(MIDI_TRACK_BUFFER_SIZE-6))
                {
                    position = (position+(uint32_t)(readPtr)-oldPosition);
                    track->startPtr = position;
                }
            }
            //Need to put in a check here since some events could be
            //larger than the buffer size, in which case we shall ignore.
        }
    }

    return track->eventCount;
}

uint8_t MPB_AddEvent(MIDI_EVENT_t* event)
{
    uint8_t ret;
    ret = STACK_PushData(&MPBStack, event);
    return ret;
}

MIDI_EVENT_t* MPB_GetNextEvent(void)
{
    MIDI_EVENT_t* event;
    event = FIFO_PeekData(&MPBStack);
    return event;
}

MIDI_EVENT_t* MPB_ConfirmEventTx(void)
{
    MIDI_EVENT_t* event;
    event = FIFO_PopData(&MPBStack);
    return event;
}

void MPB_SetTickRate(uint16_t bpm, uint16_t PPQ)
{
    uint16_t usPerTick;
    usPerTick = ((US_PER_MINUTE/64)/(bpm)/PPQ);
    PR1 = 39*usPerTick;
    T1CONbits.TCKPS0 = 0;
    T1CONbits.TCKPS1 = 1;
    myprintfd("usPerTick: ", PR1);
}

void MPB_PlayEvent(MIDI_EVENT_t* event, uint8_t mode)
{
    static uint8_t runningStatus;

    if( mode == MPB_PB_ALL_OFF)
    {
        return;
    }

    switch (event->eventType)
    {
        case MIDI_SYSEX_START:
            //myprintf("LEN: ", event->sysExEvent.length);
            break;

        case MIDI_META_MSG:

            if( mode == MPB_PB_NO_NOTES )
            {
                return;
            }
            
            if (event->metaEvent.type==MIDI_META_LYRICS)
            {
                DEBUGn(event->metaEvent.data, event->metaEvent.length);
            }

            break;

        default:

            if( mode == MPB_PB_NO_NOTES )
            {
                if( ((event->eventType & 0xF0) == MIDI_NOTE_ON) ||
                    ((event->eventType & 0xF0) == MIDI_NOTE_OFF))
                {
                    return;
                }
            }

            if (event->eventType==runningStatus)
            {

            }
            else
            {
                runningStatus = event->eventType;
                MIDI_Tx(event->eventType);
            }

            MIDI_Tx(event->chanEvent.parameter1);
            if (MIDI_CommandSize(event->eventType&0xF0)==3)
            {
                MIDI_Tx(event->chanEvent.parameter2);
            }
            break;
    }
}


uint8_t _MIDI_fileopen(uint8_t* filename)
{
    return f_open(&midifile, filename, FA_OPEN_EXISTING|FA_READ);
}

void* _MIDI_readbuf(uint32_t position, uint8_t* buf, uint16_t size)
{
    uint16_t br;
    f_lseek(&midifile, position);
    f_read(&midifile, buf, size, &br);
    return buf;
}














