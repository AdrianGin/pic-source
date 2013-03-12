/*
 * MIDIInputProcessor.h
 *
 *  Created on: 12/03/2013
 *      Author: Adrian
 */

#ifndef MIDIINPUTPROCESSOR_H_
#define MIDIINPUTPROCESSOR_H_


#include "MIDICodes/MIDICodes.h"
#include "MIDIParser/midiparser.h"
#include <stdint.h>


void MIP_ProcessEvent(MIDI_CHAN_EVENT_t* event);
void MIP_AddEventToHaltQueue(MIDI_CHAN_EVENT_t* event);



#endif /* MIDIINPUTPROCESSOR_H_ */
