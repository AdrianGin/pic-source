
#ifndef _HARDWARE_SPECIFIC_H
#define _HARDWARE_SPECIFIC_H


#include <string.h>

#include "printf/printf.h"

#include "FatFS/ff.h"
#include "USART/usart.h"
#include "hw_config.h"

#define FS_UTIL_MALLOC(size)	 malloc(size)


#define DEBUG(string)  xprintf("%s", string) //LCDInterface_Print(&PrimaryDisplay, string, 0)
#define DEBUGn(string,n) DebugDump(string, n) //LCDInterface_Print(&PrimaryDisplay, string, n)


void myprintf(char* string, uint32_t num);
void myprintfd(char* string, uint32_t num);
void myPrintNum(uint32_t num, uint8_t base);




#define SET_TIMER_INTERVAL(usPerTick)	SetMIDITickTimer(usPerTick)
#define SET_TIMER_PRESCALER(scale)		SetMIDITickTimerPrescale(scale)
#define MIDI_Tx(x)          MLL_ProcessMIDIByte(x)//USBMIDI_PutByte(x, 0)
//#define MIXI_TxDump(x, n)   uartTxDump(PrimaryUART, x, n)

#endif

