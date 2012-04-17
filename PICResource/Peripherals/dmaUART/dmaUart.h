/*

Copyright (c) 2011 Adrian Gin (adrian.gin[at]gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

 */

#ifndef _DMA_UART_H
#define _DMA_UART_H

#include <stdint.h>
#include "stack/stack.h"
#include "hardUart24H/hardUart.h"

#define DMA_CHEN (1<<15)
#define DMA_SIZE (1<<14)
#define DMA_DIR  (1<<13)
#define DMA_FORCE (1<<15)

#define UART1   (0x000C)
#define UART2   (0x001F)

//Must be a power of 2, number of queued DMA items.
#define DMA_UART_STACK_SIZE (16)

//The largest block of data to be sent at once.
#define DMA_MEM_SIZE (64)

extern STACK_t DMAUARTStack;
extern uint8_t DMAUARTBuffer[];

typedef struct
{
    volatile uint16_t* DMACON;
    volatile uint16_t* DMAREQ;
    volatile uint16_t* DMASTA;
    volatile uint16_t* DMAPAD;
    volatile uint16_t* DMACNT;
    STACK_t* Stack;
    uint8_t* DMABuffer;
    volatile PIC_USART_t* attachedUART;

    volatile uint8_t DMAFlag;


} PIC_DMA_UART_t;

void DMA_UART_Init(PIC_DMA_UART_t* DMAUART, uint16_t uartModule);
void DMA_UART_Enable(void);
void DMA_SendUARTString(PIC_DMA_UART_t* DMAUART, char* string);
void DMA_StartUART(PIC_DMA_UART_t* DMAUART);

#endif


