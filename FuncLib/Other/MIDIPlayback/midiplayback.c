


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

    for (i = 0; i<=MAX_MIDI_CHANNEL; i++)
    {
        event.eventType = MIDI_CONTROL_CHANGE|i;
        event.chanEvent.parameter1 = ALL_NOTES_OFF;
        event.chanEvent.parameter2 = 0;
        MPB_PlayEvent(&event, MPB_PB_ALL_ON);

        event.chanEvent.parameter1 = ALL_SOUND_OFF;
        MPB_PlayEvent(&event, MPB_PB_ALL_ON);

        event.chanEvent.parameter1 = ALL_CONTROLLERS_OFF;
        MPB_PlayEvent(&event, MPB_PB_ALL_ON);
    }

}

void MPB_CurrentTimePosition(MIDI_HEADER_CHUNK_t* MIDIHdr)
{

    MIDIHdr->currentState.tickTime = MIDIHdr->currentState.tickTime +
                                     ((uint32_t)((MIDIHdr->masterClock - MIDIHdr->currentState.lastTempoChange)*60) / MIDIHdr->currentState.BPM);


    MIDIHdr->currentState.trackLengthSecs = MIDIHdr->currentState.tickTime / MIDIHdr->PPQ;
    myprintfd("Min: ", MIDIHdr->currentState.trackLengthSecs/60);
    myprintfd("Sec: ", MIDIHdr->currentState.trackLengthSecs%60);
}

void MPB_DetermineLength(MIDI_HEADER_CHUNK_t* MIDIHdr)
{
    uint8_t i;

    while(MPB_RePosition(MIDIHdr, 0xFFFFFFFF, MPB_PB_ALL_OFF) != MPB_FILE_FINISHED)
    {
        if(MIDIHdr->trackCount == 0)
        {
            break;
        }
    }
    uint32_t* maxLength = &MIDIHdr->currentState.maxLength;
    for (i = 0; i<MIDIHdr->trackCount; i++)
    {
        if( MIDIHdr->Track[i].trackClock > *maxLength )
        {
            *maxLength = MIDIHdr->Track[i].trackClock;
        }
    }

    if( MIDIHdr->currentState.BPM == 0 )
    {
        MIDIHdr->currentState.BPM = MPB_DEFAULT_TEMPO;
    }

    if( MIDIHdr->PPQ == 0 )
    {
        MIDIHdr->PPQ = MPB_DEFAULT_PPQ;
    }


    MIDIHdr->currentState.tickTime = MIDIHdr->currentState.tickTime +
                                     ((uint32_t)((*maxLength - MIDIHdr->currentState.lastTempoChange)*60) / MIDIHdr->currentState.BPM);
    

    MIDIHdr->currentState.trackLengthSecs = MIDIHdr->currentState.tickTime / MIDIHdr->PPQ;
    myprintfd("Min: ", MIDIHdr->currentState.trackLengthSecs/60);
    myprintfd("Sec: ", MIDIHdr->currentState.trackLengthSecs%60);
    myprintf("MaxLength: ", *maxLength >> 16);
    myprintf("MaxLength: ", *maxLength);

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
        return;
        //return ret;
    }
    MPB_ReadToBuffer(position, buf);
    ptr = findSubString(buf, MIDI_HEADER_STRING, MIDI_TRACK_BUFFER_SIZE);
    if( ptr )
    {
        if( (ptr - buf + MIDI_HEADER_SIZE) > MIDI_TRACK_BUFFER_SIZE)
        {
            MPB_ReadToBuffer(ptr-buf, buf);
            position = ptr-buf;
            ptr = buf;
        }
    }

    position = position + MIDIParse_Header(MIDIHdr, buf, MIDI_TRACK_BUFFER_SIZE);
    position = position+MIDI_HEADER_SIZE;
    for (i = 0; i<MIDIHdr->trackCount; i++)
    {
        MPB_ReadToBuffer(position, buf);
        position = MIDIPopulate_HeaderTrack(MIDIHdr, i, position, buf, MIDI_TRACK_BUFFER_SIZE);
        MIDIHdr->Track[i].eventCount = MPB_NEW_DATA;
        MIDIHdr->Track[i].trackIndex = i;
    }


}

uint8_t MPB_RePosition(MIDI_HEADER_CHUNK_t* MIDIHdr, uint32_t position, uint8_t mode)
{
    uint8_t i;
    uint8_t ret = 0;

    MIDIHdr->masterClock = position;

    for (i = 0; i<MIDIHdr->trackCount; i++)
    {
        ret = 0;
        if((MIDIHdr->Track[i].eventCount != MPB_TRACK_STOPPED) )
        {
            while( 1 )
            {
                ret = MPB_PlayTrack(MIDIHdr, &MIDIHdr->Track[i], mode);
                if((ret == MPB_REPOSITION_EVENT))
                {
                    break;
                }
                else if(ret == MPB_TRACK_STOPPED)
                {
                    MIDIHdr->currentState.trackState++;
                    if(MIDIHdr->currentState.trackState == MIDIHdr->trackCount)
                    {
                        return MPB_FILE_FINISHED;
                    }
                    break;
                }
                else
                {
                    
                }
            }
        }
    }

    if(MIDIHdr->trackCount == 0)
    {
        return MPB_FILE_FINISHED;
    }

    return MPB_FILE_STILL_PLAYING;
}



uint8_t MPB_PlayTrack(MIDI_HEADER_CHUNK_t* MIDIHdr, MIDI_TRACK_CHUNK_t* track, uint8_t mode)
{
    uint32_t position = (uint32_t)track->startPtr;
    uint32_t oldPosition;
    uint8_t* readPtr = &track->buffer[0] + track->bufferOffset;
    uint8_t* originPtr = &track->buffer[0];
    MIDI_EVENT_t* event = &track->trackEvent;

    
    if (track->eventCount==MPB_TRACK_STOPPED)
    {
        return MPB_TRACK_STOPPED;
    }

    if (track->eventCount==MPB_NEW_DATA)
    {
    }
    else
    {
        if( track->trackClock <= MIDIHdr->masterClock)
        {
            MPB_PlayEvent(event, mode);
            track->eventCount = MPB_PLAY_NEXT_EVENT;
            
            MIDIHdr->currentState.trackTime += (event->deltaTime * 60) / MIDIHdr->currentState.BPM;

            if (event->eventType==MIDI_META_MSG)
            {
                MPB_ProcessMetaEvent(MIDIHdr, track, event);
            }
        }
        else
        {
            //Not yet time to play;
            //event->deltaTime--;
            return MPB_REPOSITION_EVENT;
        }
    }

    while ((track->eventCount != MPB_TRACK_STOPPED))
    {
        //If the buffer doesn't contain enough information for a full parse
        //Or if we are requested for new data.
        if( (track->eventCount==MPB_NEW_DATA) || ((track->bufferOffset+6)>MIDI_TRACK_BUFFER_SIZE) )
        {
            originPtr = MPB_ReadToBuffer(track->startPtr, &track->buffer[0]);
            readPtr = originPtr;
            track->bufferOffset = 0;
        }
        
        oldPosition = (uint32_t)readPtr;
        //MIDI Parse Event returns the ptr to the next event.
        readPtr = MIDIParse_Event(track, event, readPtr);
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
                if (event->eventType==MIDI_META_MSG)
                {
                    MPB_ProcessMetaEvent(MIDIHdr, track, event);
                }
            }
        }
        else
        {
            //If the parse was not fully completed due to incomplete read data,
            //We must read it again.
            track->eventCount = MPB_NEW_DATA;
            //We skip if the lengths are too long
            if (event->eventType==MIDI_META_MSG)
            {
                if (event->metaEvent.length>(MIDI_TRACK_BUFFER_SIZE-6))
                {
                    MPB_ProcessMetaEvent(MIDIHdr, track, event);
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
    //myprintfd("BPM: ", bpm);
    //myprintfd("PPQ: ", PPQ);
    //myprintfd("usPerTick: ", PR1);
}


void MPB_ProcessMetaEvent(MIDI_HEADER_CHUNK_t* MIDIHdr, MIDI_TRACK_CHUNK_t* track, MIDI_EVENT_t* event)
{

    uint8_t length = TRACK_MAX_NAME_LENGTH;
    uint16_t existingBPM;
    if( TRACK_MAX_NAME_LENGTH > event->metaEvent.length )
    {
        length = event->metaEvent.length;
    }

    switch(event->metaEvent.type)
    {
        case MIDI_META_TRACK_NAME:
            memcpy(&track->name, event->metaEvent.data, event->metaEvent.length);
            DEBUG("Track ");
            myprintfd("", track->trackIndex);
            DEBUGn(&track->name, length);
            DEBUG("\n");
            break;

        case MIDI_META_TEMPO:
            existingBPM = MIDIHdr->currentState.BPM;
            reverseOrder(event->metaEvent.data, event->metaEvent.length);
            memcpy(&MIDIHdr->currentState.BPM, event->metaEvent.data, event->metaEvent.length); //*bpmptr;

            MIDIHdr->currentState.BPM = BPM(MIDIHdr->currentState.BPM);
            MPB_SetTickRate(MIDIHdr->currentState.BPM, MIDIHdr->PPQ);

            
            if( (existingBPM != MIDIHdr->currentState.BPM) && existingBPM)
            {
                MIDIHdr->currentState.tickTime += ((track->trackClock-MIDIHdr->currentState.lastTempoChange)*60)/(existingBPM);
                MIDIHdr->currentState.lastTempoChange = track->trackClock;
//                myprintf("TT: ", MIDIHdr->currentState.tickTime>>16);
//                myprintf("TT: ", MIDIHdr->currentState.tickTime);
//
//                myprintfd("DT: ", event->deltaTime>>16);
//                myprintfd("DT: ", event->deltaTime);
//                myprintfd("EXTEMPO: ", existingBPM);
//                myprintf("LASTTEMPO: ", MIDIHdr->currentState.lastTempoChange >> 16);
//                myprintf("LASTTEMPO: ", MIDIHdr->currentState.lastTempoChange);
            }

            
            


            break;

        case MIDI_META_LYRICS:
            DEBUGn(event->metaEvent.data, event->metaEvent.length);
            break;

        case MIDI_META_TIME_SIG:
            MIDIHdr->currentState.timeSignature = 0;
            MIDIHdr->currentState.timeSignature |= event->metaEvent.data[0] << 4;
            MIDIHdr->currentState.timeSignature |= 1 << event->metaEvent.data[1];
            myprintfd("Numerator: ", MIDIHdr->currentState.timeSignature >> 4);
            myprintfd("Denominator: ", MIDIHdr->currentState.timeSignature & 0x0F);
            break;
            
        case MIDI_META_KEY_SIG:
            MIDIHdr->currentState.keySignature = event->metaEvent.data[0];
            //The major or minor scale
            MIDIHdr->currentState.keyScale = event->metaEvent.data[1];
            DEBUG("Key Signature: ");
            DEBUG(MIDIParse_KeySignature(MIDIHdr->currentState.keySignature, MIDIHdr->currentState.keyScale));
            DEBUG("\n");
            break;

        case MIDI_META_TRACKEND:
            myprintfd("TrackFinished: ", track->trackIndex);
            track->eventCount = MPB_TRACK_STOPPED;
            break;

        default:
            break;
    }
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
            break;

        default:
            //MIDI_PrintEventInfo(event);
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














