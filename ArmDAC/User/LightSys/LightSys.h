/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               TouchPanel.h
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
** Descriptions:            
**
*********************************************************************************************************/

#ifndef _LIGHTSYS_H_
#define _LIGHTSYS_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

void LS_Init(void);
void LS_ProcessAutoTurnOff(void);
void LS_AppendLightOn(uint16_t channelKey, uint8_t timer);

void LS_ProcessMIDINote(uint8_t command, uint8_t note, uint8_t velocity);
void LS_SetPixel(uint8_t note, uint32_t colour, uint8_t command);
uint32_t LS_GetColourFromMIDI(uint8_t command, uint8_t note, uint8_t velocity);
void LS_SetTranspose(uint8_t channel, int8_t semitones);
int8_t LS_GetTranspose(uint8_t channel);
uint8_t LS_ApplyTranspose(uint8_t channel, uint8_t note);

uint8_t LS_IncrementColourMode(void);
uint8_t LS_LookupFifthsColour(uint8_t note);

uint8_t LS_IsChannelActive(uint8_t channel);
uint16_t LS_ToggleChannel(uint8_t channel);
void LS_TurnOffChannel(uint8_t channel);

#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/


