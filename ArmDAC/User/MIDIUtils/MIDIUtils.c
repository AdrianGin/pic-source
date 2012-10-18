/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               TouchPanel.c
** Descriptions:            The TouchPanel application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions: Lighting subsystem to interface with MIDI data and output it to the LED array / lighting system
** It is possible that each MIDI Channel will have it's own colour.
** Or that Velocity will control the colour of the LED.
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "LEDDriver/LPD8806.h"
#include "MIDICodes/MIDICodes.h"
#include "MIDIUtils.h"
#include "hw_config.h"


uint8_t MIDIUtils_GetMusicNote(uint8_t note)
{
	note = note - (NOTE_COUNT * MIDIUtils_GetOctave(note));
	return note;
}


/* Returns the octave of the passed note */
uint8_t MIDIUtils_GetOctave(uint8_t note)
{
  return (note / NOTE_COUNT);
}







/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
