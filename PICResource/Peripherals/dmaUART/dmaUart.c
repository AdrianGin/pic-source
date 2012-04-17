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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "p24Hxxxx.h"

#include "stack/stack.h"
#include "dmaUart/dmaUart.h"

void* StackArray[sizeof(void*) * DMA_UART_STACK_SIZE];
STACK_t DMAUARTStack = {StackArray, 0, 0, DMA_UART_STACK_SIZE};
uint8_t DMAUARTBuffer[DMA_MEM_SIZE] __attribute__((space(dma)));


void DMA_UART_Init(PIC_DMA_UART_t* DMAUART, uint16_t uartModule)
{
    // One-Shot, Post-Increment, RAM-to-Perip
    *DMAUART->DMACON = 0x6001;
    *DMAUART->DMAREQ = uartModule;
    *DMAUART->DMAPAD = DMAUART->attachedUART->UXTXREG;
    *DMAUART->DMASTA = DMAUART->DMABuffer;
    DMAUART->DMAFlag = 0;
}

/* Quick fix for enabling the interrupts and priorities */
void DMA_UART_Enable(void)
{
    IPC1bits.DMA0IP = 3;
    IEC0bits.DMA0IE = 1; // Enable DMA interrupt
    IEC0bits.U1TXIE = 0; //The UART Interrupt should be disabled.
}

void DMA_SendUARTString(PIC_DMA_UART_t* DMAUART, char* string)
{
    uint8_t res = STACK_PushData(DMAUART->Stack, string);
    while(res == STACK_OVERFLOW)
    {
        res = STACK_PushData(DMAUART->Stack, string);
    }
    DMA_StartUART(DMAUART);
}


void DMA_StartUART(PIC_DMA_UART_t* DMAUART)
{
    if (DMAUART->DMAFlag == 0)
    {
        void* data = FIFO_PopData(DMAUART->Stack);
        if (data != NULL)
        {
            uint8_t dataLen;
            DMAUART->DMAFlag = 1;
            dataLen = strlen(data);
            memmove(DMAUART->DMABuffer, (char*) data, dataLen + 1);
            *DMAUART->DMACNT = dataLen - 1;
            if( (*DMAUART->DMACON & DMA_CHEN) == 0)
            {
                *DMAUART->DMACON |= DMA_CHEN; // Enable DMA Channel
                //Initate a Data Transfer if and only if the UART is
                //idle.
                if ((*(DMAUART->attachedUART->UXSTA) & (1 << TXMT)))
                {
                    *DMAUART->DMAREQ |= DMA_FORCE;
                }
            }
        }
    }
}


