


#include "midiplayback.h"
#include "MIDIParser/midiparser.h"
#include <stdint.h>
#include "stack/stack.h"

#include "hardwareSpecific.h"

#include "LEDArray/LEDArray.h"

void* MPBarray[sizeof (void*)* MPB_EVENT_STACK_SIZE];
STACK_t MPBStack = {MPBarray, 0, 0, MPB_EVENT_STACK_SIZE};

FIL midifile;



void MPB_ResetMIDI(void)
{
    MIDI_EVENT_t event;
    uint8_t i;

    MIDI_Tx(MIDI_SYSEX_START);
    MIDI_Tx(0x7E);
    MIDI_Tx(0x7F);
    MIDI_Tx(0x09);
    MIDI_Tx(0x01);
    MIDI_Tx(MIDI_SYSEX_STOP);

    for (i = 0; i<MAX_MIDI_CHANNELS; i++)
    {
        event.eventType = MIDI_CONTROL_CHANGE|i;
        event.chanEvent.parameter1 = ALL_NOTES_OFF;
        event.chanEvent.parameter2 = 0;
        MPB_PlayEvent(&event, MPB_PB_ALL_ON);

        event.chanEvent.parameter1 = ALL_SOUND_OFF;
        MPB_PlayEvent(&event, MPB_PB_ALL_ON);

        event.chanEvent.parameter1 = ALL_CONTROLLERS_OFF;
        MPB_PlayEvent(&event, MPB_PB_ALL_ON);

        event.chanEvent.parameter1 = PORTAMENTO_TOGGLE;
        event.chanEvent.parameter2 = 0x00;
        MPB_PlayEvent(&event, MPB_PB_ALL_ON);
    }

}


//returns the number of seconds.
uint16_t MPB_CurrentTimePosition(MIDI_HEADER_CHUNK_t* MIDIHdr)
{

    uint32_t currentPosition = MIDIHdr->currentState.tickTime +
                               ((uint32_t)((MIDIHdr->masterClock - MIDIHdr->currentState.lastTempoChange)*60) / MIDIHdr->currentState.BPM);
    currentPosition = currentPosition / MIDIHdr->PPQ;
    return currentPosition;
}



uint16_t MPB_CurrentBarPosition(MIDI_HEADER_CHUNK_t* MIDIHdr)
{
    return (MIDIHdr->masterClock/MIDIHdr->PPQ)>>2;
}

void MPB_DetermineLength(MIDI_HEADER_CHUNK_t* MIDIHdr)
{
    uint8_t i;

    MIDIHdr->masterClock = 0xFFFFFFFF;
    while(MPB_ContinuePlay(MIDIHdr, MPB_PB_ALL_OFF) != MPB_FILE_FINISHED)
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


uint8_t MPB_PlayMIDIFile(MIDI_HEADER_CHUNK_t* MIDIHdr, uint8_t* filename)
{
    uint8_t ret;
    ret = MPB_OpenFile(filename);
    if( ret != FR_OK)
    {
        return ret;
    }
    _mpb_InitMIDIHdr(MIDIHdr);
    MPB_DetermineLength(MIDIHdr);
    MPB_InitMIDIHdr(MIDIHdr);
    return FR_OK;
}

void MPB_InitMIDIHdr(MIDI_HEADER_CHUNK_t* MIDIHdr)
{
    uint32_t lengthSecs;
    uint32_t maxLength;
    
    lengthSecs = MIDIHdr->currentState.trackLengthSecs;
    maxLength = MIDIHdr->currentState.maxLength;
    _mpb_InitMIDIHdr(MIDIHdr);
    MIDIHdr->currentState.trackLengthSecs = lengthSecs;
    MIDIHdr->currentState.maxLength = maxLength;

    MIDIHdr->NoteOnTime = MIDIHdr->PPQ / MPB_DEFAULT_NOTE_ON_DIVISOR;

}

void _mpb_InitMIDIHdr(MIDI_HEADER_CHUNK_t* MIDIHdr)
{
    uint8_t i;
    uint32_t position = 0;
    uint8_t* buf = &MIDIHdr->Track[0].buffer[0];
    uint8_t* ptr;
    
    memset(MIDIHdr, 0, sizeof(MIDI_HEADER_CHUNK_t));
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
    MPB_InitMIDIHdr(MIDIHdr);
    MIDIHdr->masterClock = position;
    return MPB_ContinuePlay(MIDIHdr, mode);
}


uint8_t MPB_RePositionTime(MIDI_HEADER_CHUNK_t* MIDIHdr, uint16_t timePosSec, uint8_t mode)
{
    uint16_t currentPos;
    uint32_t myMasterClock;
    uint32_t myMaxLength = MIDIHdr->currentState.maxLength;
    uint16_t i;
    MPB_InitMIDIHdr(MIDIHdr);
    
    
    for(i = 0 ; i < 30000 ; i++ )
    {
        MPB_ContinuePlay(MIDIHdr,mode);
        currentPos = MPB_CurrentTimePosition(MIDIHdr);

        if( currentPos >= timePosSec )
        {
            break;
        }

        if( currentPos < timePosSec)
        {
            myMasterClock = myMasterClock + (MIDIHdr->PPQ<<2);
            MIDIHdr->masterClock = myMasterClock;
        }

        if(myMasterClock > myMaxLength )
        {
            MIDIHdr->masterClock = myMaxLength;
            break;
        }
    }

    myprintfd("Min: ", MPB_CurrentTimePosition(MIDIHdr)/60);
    myprintfd("Sec: ", MPB_CurrentTimePosition(MIDIHdr)%60);
    myprintfd("Iter: ", i);
    return 0;
}


uint8_t MPB_ContinuePlay(MIDI_HEADER_CHUNK_t* MIDIHdr, uint8_t mode)
{
    uint8_t i;
    uint8_t ret = 0;

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


#define MPB_ON_TIME (1)
void MPB_ProcessGenericEvent(MIDI_HEADER_CHUNK_t* MIDIHdr, MIDI_TRACK_CHUNK_t* track, MIDI_EVENT_t* event, uint8_t mode)
{
    MPB_PlayEvent(event, mode);
    track->eventCount = MPB_PLAY_NEXT_EVENT;

    
    if( (event->eventType & 0xF0) == MIDI_NOTE_ON )
    {
        uint8_t midiChannel = (event->eventType & 0x0F);

        MIDIHdr->channelPolyphony[midiChannel]++;

        //The drum channel has an exception because sometimes there
        //might not be any offnotes. etc
        if( midiChannel == MIDI_DRUM_CHANNEL)
        {

            MIDIHdr->NoteOnTimer[event->chanEvent.parameter1] = MIDIHdr->NoteOnTime;

            uint8_t i;
            for( i = 0; i < MIDIHdr->channelPolyphony[MIDI_DRUM_CHANNEL]; i++)
            {
                if(MIDIHdr->NoteOnArray[i] == MPB_NULL_ON_ARRAY ||
                   MIDIHdr->NoteOnArray[i] == event->chanEvent.parameter1 )
                {
                    break;
                }
            }
            
            if( MIDIHdr->NoteOnTimer[event->chanEvent.parameter1] == 0 )
            {
                MIDIHdr->NoteOnArray[i] = event->chanEvent.parameter1;
                MIDIHdr->channelPolyphony[0x09]++;
            }
            MIDIHdr->NoteOnTimer[event->chanEvent.parameter1] = MIDIHdr->PPQ/8;
        }
    }
    else if( (event->eventType & 0xF0) == MIDI_NOTE_OFF )
    {
        

        //If however we receive a Note Off before the timer has
        //elapsed, then reset it all.
        if( MIDIHdr->NoteOnTime[event->chanEvent.parameter1] )
        {
            uint8_t j;
            for( j = 0; j < MIDIHdr->channelPolyphony[0x09]; j++ )
            {
                if( MIDIHdr.NoteOnArray[j] == event->chanEvent.parameter1)
                {
                    MIDIHdr.NoteOnArray[j] = MPB_NULL_ON_ARRAY;
                }
            }
            MIDIHdr->NoteOnTime[event->chanEvent.parameter1] = 0;
            MIDIHdr->channelPolyphony[0x09]--;
            //Turn respective LED off.
            LEDArray_SetLED(0, 0, 0, 0);
        }
    }
    
    if (event->eventType==MIDI_META_MSG)
    {
        MPB_ProcessMetaEvent(MIDIHdr, track, event);
    }
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
            MPB_ProcessGenericEvent(MIDIHdr, track, event, mode);
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
                MPB_ProcessGenericEvent(MIDIHdr, track, event, mode);
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
    //*to send a Timing signal we need to send 24 0xF8's per quarter note
    uint16_t usPerTick;
    usPerTick = ((US_PER_MINUTE/64)/(bpm)/PPQ);
    PR1 = 41*usPerTick;
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
            }

            break;

        case MIDI_META_LYRICS:
            //Lyrics parsed in play events.
            //DEBUGn(event->metaEvent.data, event->metaEvent.length);
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

    uint8_t i;

    switch (event->eventType)
    {
        case MIDI_SYSEX_START:
            MIDI_Tx(MIDI_SYSEX_START);
            //MIXI_TxDump(event->sysExEvent.data, event->sysExEvent.length);
            myprintf("MIDISYSX FOUND:", event->sysExEvent.length);

            for(i = 0; i < event->sysExEvent.length; i++)
            {
                MIDI_Tx(event->sysExEvent.data[i]);
                myprintf("B:", event->sysExEvent.data[i]);
            }

            //DEBUGn(event->sysExEvent.data, event->sysExEvent.length);

            break;

        case MIDI_META_MSG:

            if( mode == MPB_PB_NO_NOTES )
            {
                return;
            }
            
            switch(event->metaEvent.type)
            {
                case MIDI_META_LYRICS:
                    DEBUGn(event->metaEvent.data, event->metaEvent.length);
                    break;
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

            if( ( (event->eventType & 0x0F) == 0x09))
            {

                if( (event->eventType & 0xF0) == MIDI_NOTE_ON )
                {


                    if( (event->chanEvent.parameter1 == BASS_DRUM1) )
                    {
                        LEDArray_AppendLED(0, 7, 0, 0);
                        //myprintfd("YES:", 3);
                    }

                    if( (event->chanEvent.parameter1 == ELECTRIC_SNARE) )
                    {
                        LEDArray_AppendLED(0, 0, 7, 0);
                        //myprintfd("YES:", 7);
                    }

                    if( (event->chanEvent.parameter1 == RIDE_CYMBAL2) )
                    {
                        LEDArray_AppendLED(87, 0, 0, 7);
                        //myprintfd("YES:", 7);
                    }

                }
                else
                {

                    if( (event->chanEvent.parameter1 == BASS_DRUM1) )
                    {
                        //LEDArray_AppendLED(0, LA_MAX_BRIGHTNESS, 0, 0);
                        //myprintfd("YES:", 3);
                    }

                    if( (event->chanEvent.parameter1 == ELECTRIC_SNARE) )
                    {
                        //LEDArray_AppendLED(0, 0, LA_MAX_BRIGHTNESS, 0);
                        //myprintfd("YES:", 7);
                    }

                    if( (event->chanEvent.parameter1 == RIDE_CYMBAL2) )
                    {
                        //LEDArray_AppendLED(87, 0, 0, LA_MAX_BRIGHTNESS);
                        //myprintfd("YES:", 7);
                    }
                }
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














