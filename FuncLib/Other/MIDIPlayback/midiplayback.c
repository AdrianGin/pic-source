


#include "midiplayback.h"
#include "MIDIParser/midiparser.h"
#include <stdint.h>
#include "stack/stack.h"

#include "hardwareSpecific.h"


void* MPBarray[sizeof(void*) * MPB_EVENT_STACK_SIZE];
STACK_t MPBStack = {MPBarray, 0 ,0 ,MPB_EVENT_STACK_SIZE};


void MPB_PlayTrack(MIDI_HEADER_CHUNK_t* MIDIHdr, MIDI_TRACK_CHUNK_t* track)
{
    uint32_t position = 0;
    uint32_t oldPosition = 0;
    uint8_t* readPtr;
    uint8_t* originPtr;
    uint8_t i;
    uint16_t j;
    uint8_t ret;
    MIDI_EVENT_t* event = &track->trackEvent;

    if( track->eventCount == MPB_TRACK_STOPPED)
    {
        return;
    }

    if( track->eventCount == MPB_NEW_DATA)
    {
        event->deltaTime = 0;
    }
    else
    {
        if( event->deltaTime == 0 )
        {
            MPB_PlayEvent(event);
            track->eventCount = MPB_NEW_DATA;
        }
        else
        {
            event->deltaTime--;
            return;
        }
    }

    while( event->deltaTime == 0 )
    {
        if( track->eventCount == MPB_NEW_DATA )
        {
            originPtr = MPB_ReadToBuffer(track->startPtr, &track->buffer[0]);
            originPtr = MPB_ReadPosition(track->startPtr);
            readPtr = originPtr;
            oldPosition = readPtr;
            position = track->startPtr;
            track->bufferOffset = 0;
        }

        //If the buffer doesn't contain enough information for a full parse
        if (((readPtr) - (originPtr)+6)>BUFFER_READ_SIZE)
        {
            originPtr = MPB_ReadPosition(position);
            readPtr = originPtr;
        }

        oldPosition = readPtr;
        //MIDI Parse Event returns the ptr to the next event.
        readPtr = MIDIParse_Event(event, readPtr);
        //On successful parse, we save the position.
        if ((readPtr- originPtr)<=BUFFER_READ_SIZE)
        {
            position = (position+(uint32_t)(readPtr)-oldPosition);
            track->startPtr = position;
            
            if( event->deltaTime )
            {
                track->eventCount = MPB_BUFFERED_DATA;
                event->deltaTime--;
                return;
            }
            else
            {
                MPB_PlayEvent(event);
                track->eventCount = MPB_EXISTING_DATA;
            }
        }
        else
        {
            //If the parse was not fully completed due to incomplete read data
            originPtr = MPB_ReadPosition(position);
            readPtr = originPtr;
        }


        if (event->eventType==MIDI_META_MSG&&event->metaEvent.type==MIDI_META_TEMPO)
        {
            uint32_t bpm;
            reverseOrder(event->metaEvent.data, event->metaEvent.length);
            memcpy(&bpm, event->metaEvent.data, 4); //*bpmptr;
            MPB_SetTickRate(BPM(bpm), MIDIHdr->PPQ);
            TimerStart();
        }

        if (event->eventType==MIDI_META_MSG&&event->metaEvent.type==MIDI_META_TRACKEND)
        {
            track->eventCount = MPB_TRACK_STOPPED;
            break;
        }
    }
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
    usPerTick = ((US_PER_MINUTE/64) / (bpm) /PPQ);
    PR1 = 39 * usPerTick;
    T1CONbits.TCKPS0 = 0;
    T1CONbits.TCKPS1 = 1;
    //myprintfd("usPerTick: ", PR1);
}

void MPB_PlayEvent(MIDI_EVENT_t* event)
{
    static uint8_t runningStatus;

    switch (event->eventType)
    {
        case MIDI_SYSEX_START:
            //myprintf("LEN: ", event->sysExEvent.length);
            break;

        case MIDI_META_MSG:

            if(event->metaEvent.type == MIDI_META_LYRICS)
            {
                DEBUGn(event->metaEvent.data, event->metaEvent.length);
            }


            //myprintf("TYPE: ", event->metaEvent.type);
            //myprintf("LEN: ", event->metaEvent.length);
            break;

        default:
            
            if( event->eventType == runningStatus)
            {
                
            }
            else
            {
                runningStatus = event->eventType;
                uartTx(PrimaryUART, event->eventType);
            }
            
            uartTx(PrimaryUART, event->chanEvent.parameter1);

            if( MIDI_CommandSize(event->eventType & 0xF0) == 3 )
            {
                uartTx(PrimaryUART, event->chanEvent.parameter2);
            }
            break;
    }
    
}



















