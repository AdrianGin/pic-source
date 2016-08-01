#include "RtMidi.h"
#include <time.h>
#include <iostream>
#include <cstdlib>

#include "midiplayback.h"

RtMidiOut* midiout = 0;

void wait_ms(unsigned int diffTime)
{
   clock_t now = clock();
   while( (clock_t)(now + diffTime) > clock())
   {

   }
}


void SendMIDIEvent(MIDI_CHAN_EVENT_t* event)
{
   std::vector<unsigned char> message;
   message.push_back(event->eventType);
   message.push_back(event->parameter1);
   message.push_back(event->parameter2);

   midiout->sendMessage( &message );
}


int main(void)
{


   midiout = new RtMidiOut();

   std::vector<unsigned char> message;



   MIDI_HEADER_CHUNK_t MIDIHdr;


   midiout->openPort(0);

   MPB_PlayMIDIFile(&MIDIHdr, "BeatIt.mid");

   int usPerTick = MPB_SetTickRate(MIDIHdr.currentState.BPM, MIDIHdr.PPQ);

   printf("USPERTICK = %d\n", usPerTick);

   int cumulativeWait = 0;

   for (;;)
   {

      cumulativeWait += usPerTick;
      if( cumulativeWait >= 1000 )
      {
         wait_ms(cumulativeWait / 1000);
         cumulativeWait = cumulativeWait - ((cumulativeWait / 1000)*1000);
      }

      MIDIHdr.masterClock += 1;
      if (MPB_ContinuePlay(&MIDIHdr, MPB_PB_ALL_ON) == MPB_FILE_FINISHED)
      {
         MPB_PausePlayback(&MIDIHdr);
      }

   }



   // Program change: 192, 5
   message.push_back( 192 );
   message.push_back( 5 );
   midiout->sendMessage( &message );
   // Control Change: 176, 7, 100 (volume)
   message[0] = 176;
   message[1] = 7;
   message.push_back( 100 );
   midiout->sendMessage( &message );

   message[0] = 144;
   message[1] = 64;
   message[2] = 90;



   while(1)
   {
      midiout->sendMessage( &message );
      printf("Hello world!\n");
      wait_ms(1000);
   }



   return 0;
}
