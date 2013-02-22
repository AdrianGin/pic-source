


#include "midiplayback.h"
#include "MIDIParser/midiparser.h"
#include <stdint.h>
#include "stack/stack.h"
#include "linkedlist/linkedlist.h"

#include "ReplaceChar/replacechar.h"

#include "hardwareSpecific.h"

//#include "LEDArray/LEDArray.h"

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

    for (i = 0; i< MIDI_MAX_CHANNELS ; i++)
    {
        event.event.eventType = MIDI_CONTROL_CHANGE|i;
        event.event.chanEvent.parameter1 = ALL_NOTES_OFF;
        event.event.chanEvent.parameter2 = 0;
        MPB_PlayEvent(&event, MPB_PB_ALL_ON);

        event.event.chanEvent.parameter1 = ALL_SOUND_OFF;
        MPB_PlayEvent(&event, MPB_PB_ALL_ON);

        event.event.chanEvent.parameter1 = ALL_CONTROLLERS_OFF;
        MPB_PlayEvent(&event, MPB_PB_ALL_ON);

        event.event.chanEvent.parameter1 = PORTAMENTO_TOGGLE;
        event.event.chanEvent.parameter2 = 0x00;
        MPB_PlayEvent(&event, MPB_PB_ALL_ON);
    }

    _clearEventBuffer();

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
    uint32_t* maxLength;

    MIDIHdr->masterClock = 0xFFFFFFFF;
    while(MPB_ContinuePlay(MIDIHdr, MPB_PB_ALL_OFF) != MPB_FILE_FINISHED)
    {
        if(MIDIHdr->trackCount == 0)
        {
            break;
        }
    }
    maxLength = &MIDIHdr->currentState.maxLength;
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

void MPB_EnablePlayback(MIDI_HEADER_CHUNK_t* MIDIHdr)
{
	MIDIHdr->playbackState = STATE_ACTIVE;
}

void MPB_PausePlayback(MIDI_HEADER_CHUNK_t* MIDIHdr)
{
	MIDIHdr->playbackState = STATE_INACTIVE;
}

void MPB_SetPlaybackState(MIDI_HEADER_CHUNK_t* MIDIHdr, MidiPlaybackState_t state)
{
	if( state == STATE_ACTIVE || state == STATE_INACTIVE)
	{
		MIDIHdr->playbackState = state;
	}
}

MidiPlaybackState_t MPB_GetPlaybackState(MIDI_HEADER_CHUNK_t* MIDIHdr)
{
	return MIDIHdr->playbackState;
}

void MPB_TogglePlayback(MIDI_HEADER_CHUNK_t* MIDIHdr)
{
	MIDIHdr->playbackState = MIDIHdr->playbackState ^ 1;
}

void MPB_InitMIDIHdr(MIDI_HEADER_CHUNK_t* MIDIHdr)
{
    uint32_t lengthSecs;
    uint32_t maxLength;
    uint8_t playbackState;
    
    lengthSecs = MIDIHdr->currentState.trackLengthSecs;
    maxLength = MIDIHdr->currentState.maxLength;
    playbackState = MIDIHdr->playbackState;
    _mpb_InitMIDIHdr(MIDIHdr);

    MIDIHdr->playbackState = playbackState;
    MIDIHdr->currentState.trackLengthSecs = lengthSecs;
    MIDIHdr->currentState.maxLength = maxLength;

    MIDIHdr->NoteOnTime = MIDIHdr->PPQ/8;
}

void _mpb_InitMIDIHdr(MIDI_HEADER_CHUNK_t* MIDIHdr)
{
    uint8_t i;
    uint32_t position = 0;
    uint8_t* buf;
    uint8_t* ptr;
    
    buf = (uint8_t*)&MIDIHdr->Track[0].buffer;
    memset(MIDIHdr, 0, sizeof(MIDI_HEADER_CHUNK_t));
    MPB_ReadToBuffer(position, buf);

    ptr = findSubString( (char*)buf, MIDI_HEADER_STRING, MIDI_TRACK_BUFFER_SIZE);
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

uint8_t MPB_RePosition(MIDI_HEADER_CHUNK_t* MIDIHdr, uint32_t position, MIDI_PB_MODE mode)
{
    MPB_InitMIDIHdr(MIDIHdr);
    MIDIHdr->masterClock = position;
    return MPB_ContinuePlay(MIDIHdr, mode);
}

static MPB_FastFwd_t	 FastFwd_Status;
static MIDI_CHAN_EVENT_t FastFwd_Event;
//Finds the next 'MIDI Command' from the given position.
//
uint8_t MPB_FastFwd_ToEvent(MIDI_HEADER_CHUNK_t* MIDIHdr, uint32_t position, MIDI_PB_MODE mode, MIDI_CHAN_EVENT_t* event, MPB_FF_MODE_t ffMode)
{
    MPB_RePosition(MIDIHdr, position, MPB_PB_ALL_OFF);

    FastFwd_Status.foundEventStatus = 0;
    FastFwd_Status.foundEventFlag = FAST_FWD_ACTIVE;
    FastFwd_Status.searchMode = ffMode;

    FastFwd_Event.eventType = event->eventType;
    FastFwd_Event.parameter1 = event->parameter1;
    FastFwd_Event.parameter2 = 1; //A non zero velocity.


    while(FastFwd_Status.foundEventFlag)
    {
		MIDIHdr->masterClock++;
		if (MPB_ContinuePlay(MIDIHdr, MPB_PB_ALL_OFF) == MPB_FILE_FINISHED)
		{
			break;
		}
    }

    if(FastFwd_Status.foundEventFlag == 0)
    {
    	position = MIDIHdr->masterClock-1;
    }

    if( position == 0 )
    {
    	position = 1;
    }

    MPB_RePosition(MIDIHdr, position-1, mode);

    return FastFwd_Status.foundEventStatus;
}


void MPB_FastFwd_TestEvent(MIDI_EVENT_t* event)
{
	if( event->event.eventType >= MIDI_NOTE_OFF &&
		event->event.eventType <= MIDI_PITCH_CHANGE)
	{
		if(event->event.chanEvent.eventType == FastFwd_Event.eventType )
		{
			FastFwd_Status.foundEventStatus = FAST_FWD_FIND_COMMAND;

			if(event->event.chanEvent.parameter1 == FastFwd_Event.parameter1 )
			{
				FastFwd_Status.foundEventStatus = FAST_FWD_FIND_KEY;
				if(event->event.chanEvent.parameter2 >= FastFwd_Event.parameter2 )
				{
					FastFwd_Status.foundEventStatus = FAST_FWD_FIND_VELOCITY;
				}
			}
		}

		if(FastFwd_Status.searchMode == FastFwd_Status.foundEventStatus)
		{
			FastFwd_Status.foundEventFlag = 0;
		}
	}
}




uint8_t MPB_RePositionTime(MIDI_HEADER_CHUNK_t* MIDIHdr, uint16_t timePosSec, MIDI_PB_MODE mode)
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


uint8_t MPB_ContinuePlay(MIDI_HEADER_CHUNK_t* MIDIHdr, MIDI_PB_MODE mode)
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
void MPB_ProcessGenericEvent(MIDI_HEADER_CHUNK_t* MIDIHdr, MIDI_TRACK_CHUNK_t* track, MIDI_EVENT_t* event, MIDI_PB_MODE mode)
{
	uint8_t midiChannel;

	MPB_ApplyTranspose(event, MIDIHdr->transpose);
    MPB_PlayEvent(event, mode);
    track->eventCount = MPB_PLAY_NEXT_EVENT;

    midiChannel = (event->event.eventType & 0x0F);
    

	if( FastFwd_Status.foundEventFlag )
	{
		MPB_FastFwd_TestEvent(event);
	}

    //Keep track of polyphony here
    if( (event->event.eventType & 0xF0) == MIDI_NOTE_ON )
    {
        //A note on with a velocity of 0, can be NOTE_OFFs.
        if(event->event.chanEvent.parameter2 == 0)
        {
            if(MIDIHdr->channelPolyphony[midiChannel])
            {
                MIDIHdr->channelPolyphony[midiChannel]--;
            }
            if(MIDIHdr->totalPolyphony)
            {
                MIDIHdr->totalPolyphony--;
            }
        }
        else
        {
            MIDIHdr->channelPolyphony[midiChannel]++;
            MIDIHdr->totalPolyphony++;
        }
        //The drum channel has an exception because sometimes there
        //might not be any offnotes. etc
        if( midiChannel == MIDI_DRUM_CHANNEL)
        {
            MIDIHdr->NoteOnTimer[event->event.chanEvent.parameter1] = MIDIHdr->NoteOnTime;
        }
    }
    else if( (event->event.eventType & 0xF0) == MIDI_NOTE_OFF )
    {
        if(MIDIHdr->channelPolyphony[midiChannel])
        {
            MIDIHdr->channelPolyphony[midiChannel]--;
        }
        if(MIDIHdr->totalPolyphony)
        {
            MIDIHdr->totalPolyphony--;
        }
        //If however we receive a Note Off before the timer has
        //elapsed, then reset it all.
        if( midiChannel == MIDI_DRUM_CHANNEL)
        {
            MIDIHdr->NoteOnTimer[event->event.chanEvent.parameter1] = 0;
        }
        //Turn off the respective LED.
        //LEDArray_SetLED(0, 0, 0, 0);
    }
    
    if (event->event.eventType==MIDI_META_MSG)
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
            MPB_ReadToBuffer(track->startPtr, &track->buffer[0]);
            originPtr = &track->buffer[0];
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
            if (event->event.eventType==MIDI_META_MSG)
            {
                if (event->event.metaEvent.length>(MIDI_TRACK_BUFFER_SIZE-6))
                {
                	MPB_ProcessMetaEvent(MIDIHdr, track, event);
                    position = (position+(uint32_t)(readPtr)-oldPosition);
                    track->startPtr = position;
                }
            }
            if (event->event.eventType==MIDI_SYSEX_START)
            {
                if (event->event.sysExEvent.length>(MIDI_TRACK_BUFFER_SIZE-6))
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

#define MIN_PPQ	(48)
#define MIN_BMP	(20)
#define MIN_BMP_PPQ_PRODUCT (MIN_BMP*MIN_PPQ)
#define MIN_BMP_PPQ_PRESCALER	(64)
void MPB_SetTickRate(uint16_t bpm, uint16_t PPQ)
{
    //*to send a Timing signal we need to send 24 0xF8's per quarter note
    uint16_t usPerTick;
    //Use a 1x prescaler.
    usPerTick = ((US_PER_MINUTE)/(bpm)/PPQ);

    //Use a 64x prescaler if the speed is too slow
    if( (bpm * PPQ) < MIN_BMP_PPQ_PRODUCT)
    {
    	usPerTick = ((US_PER_MINUTE/MIN_BMP_PPQ_PRESCALER)/(bpm)/PPQ);
    	SET_TIMER_PRESCALER(64);
    }
    else
    {
    	SET_TIMER_PRESCALER(1);
    }


    SET_TIMER_INTERVAL(usPerTick);

    //PR1 = 41*usPerTick;
    //T1CONbits.TCKPS0 = 0;
    //T1CONbits.TCKPS1 = 1;
    myprintfd("BPM: ", bpm);
    myprintfd("PPQ: ", PPQ);
    myprintfd("usPerTick: ", usPerTick);
}

void MPB_ProcessMetaEvent(MIDI_HEADER_CHUNK_t* MIDIHdr, MIDI_TRACK_CHUNK_t* track, MIDI_EVENT_t* event)
{

    uint8_t length = TRACK_MAX_NAME_LENGTH;
    uint16_t existingBPM;
    if( TRACK_MAX_NAME_LENGTH > event->event.metaEvent.length )
    {
        length = event->event.metaEvent.length;
    }

    switch(event->event.metaEvent.metaType)
    {
        case MIDI_META_TRACK_NAME:
            memcpy(&track->name, event->event.metaEvent.data, length);
            DEBUG("Track ");
            myprintfd("", track->trackIndex);
            DEBUGn((char*)&track->name, length);
            DEBUG("\n");
            break;

        case MIDI_META_TEMPO:
            existingBPM = MIDIHdr->currentState.BPM;
            reverseOrder( (char*)event->event.metaEvent.data, event->event.metaEvent.length);
            memcpy(&MIDIHdr->currentState.BPM, event->event.metaEvent.data, event->event.metaEvent.length); //*bpmptr;

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
            //DEBUGn(event->event.metaEvent.data, event->event.metaEvent.length);
            break;

        case MIDI_META_TIME_SIG:
            MIDIHdr->currentState.timeSignature = 0;
            MIDIHdr->currentState.timeSignature |= event->event.metaEvent.data[0] << 4;
            MIDIHdr->currentState.timeSignature |= 1 << event->event.metaEvent.data[1];
            myprintfd("Numerator: ", MIDIHdr->currentState.timeSignature >> 4);
            myprintfd("Denominator: ", MIDIHdr->currentState.timeSignature & 0x0F);
            break;
            
        case MIDI_META_KEY_SIG:
            MIDIHdr->currentState.keySignature = event->event.metaEvent.data[0];
            //The major or minor scale
            MIDIHdr->currentState.keyScale = event->event.metaEvent.data[1];
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

void MPB_SetTranspose(MIDI_HEADER_CHUNK_t* MIDIHdr, int8_t transpose)
{
	MIDIHdr->transpose = transpose;
}

void MPB_ApplyTranspose(MIDI_EVENT_t* event, int8_t transpose)
{
	//Apply transpose here
    if( ((event->event.eventType & 0xF0) == MIDI_NOTE_ON) ||
        ((event->event.eventType & 0xF0) == MIDI_NOTE_OFF))
    {
    	uint8_t chan;
    	chan = event->event.eventType & 0x0F;
    	if( chan != MIDI_DRUM_CHANNEL )
    	{
    		event->event.chanEvent.parameter1 += transpose;
    	}
    }

}




void MPB_PlayEvent(MIDI_EVENT_t* event, MIDI_PB_MODE mode)
{
    if( mode == MPB_PB_ALL_OFF)
    {
        return;
    }

    switch (event->event.eventType)
    {
    	uint8_t i;
        case MIDI_SYSEX_START:
            MIDI_Tx(MIDI_SYSEX_START);
            //MIXI_TxDump(event->event.sysExEvent.data, event->event.sysExEvent.length);
            myprintf("MIDISYSX FOUND:", event->event.sysExEvent.length);

            for(i = 0; i < event->event.sysExEvent.length; i++)
            {
                MIDI_Tx(event->event.sysExEvent.data[i]);
                myprintf("B:", event->event.sysExEvent.data[i]);
            }

            //DEBUGn(event->event.sysExEvent.data, event->event.sysExEvent.length);

            break;

        case MIDI_META_MSG:

            if( mode == MPB_PB_NO_NOTES )
            {
                break;
            }
            
            switch(event->event.metaEvent.metaType)
            {
            	case MIDI_META_TEXT:
                case MIDI_META_LYRICS:

                	replace_char( (char*)event->event.metaEvent.data, '\\', '\n');
                	replace_char( (char*)event->event.metaEvent.data, '/', '\n');

                	DEBUGn(event->event.metaEvent.data, event->event.metaEvent.length);
                    //myprintf(event->event.metaEvent.data, event->event.metaEvent.length);

                    break;
            }
            
            break;

        default:
            //MIDI_PrintEventInfo(event);



            if( mode == MPB_PB_NO_NOTES )
            {
                if( ((event->event.eventType & 0xF0) == MIDI_NOTE_ON) ||
                    ((event->event.eventType & 0xF0) == MIDI_NOTE_OFF))
                {
                	break;
                }
            }
            else if( mode == MPB_PB_SAVE_MIDI_STATUS )
            {
                if( ((event->event.eventType & 0xF0) >= MIDI_AFTERTOUCH) &&
                    ((event->event.eventType & 0xF0) < MIDI_SYSEX_START))
                {
                	MPB_SaveMIDIStatus(&event->event.chanEvent);
                }
                break;
            }

            MPB_OutputMIDIChanEvent(&event->event.chanEvent);
            
            break;
    }
}


void MPB_OutputMIDIChanEvent(MIDI_CHAN_EVENT_t* chanEvent)
{
	static uint8_t runningStatus = 0x00;
    if (chanEvent->eventType==runningStatus)
    {

    }
    else
    {
        runningStatus = chanEvent->eventType;
        MIDI_Tx(chanEvent->eventType);
    }
    MIDI_Tx(chanEvent->parameter1);
    if (MIDI_CommandSize(chanEvent->eventType&0xF0)==3)
    {
        MIDI_Tx(chanEvent->parameter2);
    }
}

#define MPB_IGNORE_BYTE	(0xFF)
#define MPB_SAVE_BUFFER_SIZE	(16)

//#define MPB_STATUS_BUFFER_ARRAY

#ifdef MPB_STATUS_BUFFER_ARRAY
MIDI_CHAN_EVENT_t StatusBuffer[MIDI_MAX_CHANNELS][MPB_SAVE_BUFFER_SIZE];
#else
LINKED_LIST_t StatusBuffer[MIDI_MAX_CHANNELS];
#endif

uint16_t StatusBufferCount[MIDI_MAX_CHANNELS];

//Saves the channel event. Does not save Note On / Offs
void MPB_SaveMIDIStatus(MIDI_CHAN_EVENT_t* chanEvent)
{

	/*   MIDI_AFTERTOUCH = 0xA0,
   MIDI_CONTROL_CHANGE = 0xB0,
   MIDI_PROGRAM_CHANGE = 0xC0,
   MIDI_CHANNEL_PRESSURE = 0xD0,
   MIDI_PITCH_CHANGE = 0xE0,*/

	MIDI_CHAN_EVENT_t* foundEvent = NULL;

	//All messages are channel specific.
	switch(chanEvent->eventType & 0xF0)
	{

	case MIDI_AFTERTOUCH:
	case MIDI_CONTROL_CHANGE:
		foundEvent = _findChanEventInBuffer(chanEvent->eventType, chanEvent->parameter1);
		if( foundEvent )
		{
			foundEvent->parameter2 = chanEvent->parameter2;
		}
		break;

	case MIDI_PROGRAM_CHANGE:
	case MIDI_CHANNEL_PRESSURE:
	case MIDI_PITCH_CHANGE:
		foundEvent = _findChanEventInBuffer(chanEvent->eventType, MPB_IGNORE_BYTE);
		if( foundEvent )
		{
			foundEvent->parameter1 = chanEvent->parameter1;
			foundEvent->parameter2 = chanEvent->parameter2;
		}
		break;

	default:
		foundEvent = NULL;
		break;
	}

	if( foundEvent == NULL)
	{
		_addEventToBuffer(chanEvent);
	}

}

void MPB_ReplayStatusBuffer(void)
{
	int i, j;
	int bufferCount;

	MIDI_CHAN_EVENT_t* chanEvent;

	for( bufferCount = 0, j = 0; j < MIDI_MAX_CHANNELS; j++)
	{
		for( i = 0; i < StatusBufferCount[j]; i++)
		{
			chanEvent = _getEventFromBuffer(j, i);
			if( chanEvent )
			{
				MPB_OutputMIDIChanEvent(chanEvent);
			}
		}
		bufferCount = bufferCount + StatusBufferCount[j];
	}

	myprintfd("TotalStatusCount=", bufferCount);
}


void _addEventToBuffer(MIDI_CHAN_EVENT_t* chanEvent)
{

	uint8_t channel;
	MIDI_CHAN_EVENT_t* newChanEvent;

	channel = chanEvent->eventType & 0x0F;

#ifdef MPB_STATUS_BUFFER_ARRAY
	if( StatusBufferCount[channel] < MPB_SAVE_BUFFER_SIZE)
	{
		memcpy(&StatusBuffer[channel][StatusBufferCount[channel]], chanEvent, sizeof(MIDI_CHAN_EVENT_t));
		StatusBufferCount[channel]++;
	}
#else
	newChanEvent = LL_Malloc(sizeof(MIDI_CHAN_EVENT_t));
	memcpy(newChanEvent, chanEvent, sizeof(MIDI_CHAN_EVENT_t));
	LL_AppendData(&StatusBuffer[channel], (void*)newChanEvent);
	StatusBufferCount[channel]++;
#endif

}

MIDI_CHAN_EVENT_t* _getEventFromBuffer(uint8_t channel, uint16_t index)
{
#ifdef MPB_STATUS_BUFFER_ARRAY
	return &StatusBuffer[channel][index];
#else
	return (MIDI_CHAN_EVENT_t*)LL_ReturnNodeDataFromIndex(&StatusBuffer[channel], index);
#endif
}


void _clearEventBuffer(void)
{
	int i;

#ifdef MPB_STATUS_BUFFER_ARRAY
	for( i = 0; i < MIDI_MAX_CHANNELS; i++)
	{
		memset(&StatusBuffer[i], 0, sizeof(MIDI_CHAN_EVENT_t)*MPB_SAVE_BUFFER_SIZE);
		StatusBufferCount[i] = 0;
	}

#else
	for( i = 0; i < MIDI_MAX_CHANNELS; i++)
	{
		LL_DeleteListAndData(&StatusBuffer[i]);
		StatusBufferCount[i] = 0;
	}
#endif
}


MIDI_CHAN_EVENT_t* _findChanEventInBuffer(uint8_t firstByte, uint8_t secondByte)
{
	int i;
	MIDI_CHAN_EVENT_t* foundEvent;
	uint8_t channel;

	channel = firstByte & 0x0F;

	for( i = 0 ; i < StatusBufferCount[channel]; i++ )
	{
		foundEvent = _getEventFromBuffer(channel, i);
		if(foundEvent->eventType == firstByte )
		{
			if( secondByte == MPB_IGNORE_BYTE)
			{
				return foundEvent;
			}
			else
			{
				if(foundEvent->parameter1 == secondByte)
				{
					return foundEvent;
				}
			}
		}
	}

	return NULL;
}







uint8_t _MIDI_fileopen(char* filename)
{
    return f_open(&midifile, (char*)filename, FA_OPEN_EXISTING|FA_READ);
}

void _MIDI_readbuf(uint32_t position, uint8_t* buf, uint16_t size)
{
	//Ouch a nasty bug here :(
	UINT br;
    f_lseek(&midifile, position);
    f_read(&midifile, buf, size, (UINT*)&br);
    //return buf;
}














