
#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x.h"
#include "hw_config.h"
#include "hardwareSpecific.h"


char* itoa(int val, int base)
{
	static char buf[32] = {0};


	xsprintf(buf, "%d", val);

	return buf;

//	int i = 30;
//	for(; val && i ; --i, val /= base)
//
//		buf[i] = "0123456789abcdef"[val % base];
//
//	return &buf[i+1];

}


void SetMIDITickTimer(uint16_t usPerTick)
{
	xprintf("MicroSec / Tick = %d\n", usPerTick);
	TIM_SetAutoreload(MIDI_TIM, (usPerTick) - 1);
}


void SetMIDITickTimerPrescale(uint16_t prescale)
{
	TIM_PrescalerConfig(MIDI_TIM, prescale*DEFAULT_MIDI_TIMER_PRESCALER, TIM_PSCReloadMode_Immediate);
}



void myprintf(char* string, uint32_t num)
{
    DEBUG(string);
    myPrintNum(num, 16);
    DEBUG("\n");
}

void myprintfd(char* string, uint32_t num)
{
    DEBUG(string);
    myPrintNum(num, 10);
    DEBUG("\n");
}

void myPrintNum(uint32_t num, uint8_t base)
{
    char* outputString;
    outputString = itoa(num, base);
    DEBUG(outputString);
}


void SendString(char* string)
{
    DEBUG(string);
    //uartTxString_P((PIC_USART_t*)&U2, string);
}
