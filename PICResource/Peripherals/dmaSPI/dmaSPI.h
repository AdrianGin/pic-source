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

#ifndef _DMA_SPI_H
#define _DMA_SPI_H

#include <stdint.h>
#include "dma/dmadef.h"
#include "spi/spi.h"
#include "hardUART24H/hardUart.h"
#include "stack/stack.h"


typedef struct
{
    volatile uint16_t* DMACON;
    volatile uint16_t* DMAREQ;
    volatile uint16_t* DMASTA;
    volatile uint16_t* DMAPAD;
    volatile uint16_t* DMACNT;
    STACK_t* Stack;
    uint8_t* DMABuffer;
    volatile PIC_SPI_t* attachedSPI;

    volatile uint8_t DMAFlag;
} PIC_DMA_SPI_t;


void DMA_SPI_Init(PIC_DMA_SPI_t* DMASPI, PIC_DMA_SPI_t* DMATXSPI, uint16_t spiModule);
void DMA_SPI_Enable(void);
void DMA_SendSPIString(PIC_DMA_SPI_t* DMASPI, char* string);
void DMA_SendSPIString_NoStart(PIC_DMA_SPI_t* DMASPI, char* string);
uint8_t DMA_StartSPI(PIC_DMA_SPI_t* DMASPI);
void DMA_SPI_ReceiveBytes(volatile PIC_DMA_SPI_t* DMASPI, volatile PIC_DMA_SPI_t* DMATXSPI, uint16_t byteCount);
void DMA_SPI_ReceiveBlock(volatile PIC_DMA_SPI_t* DMASPI, volatile PIC_DMA_SPI_t* DMATXSPI, uint8_t* buffer, uint16_t byteCount);

#endif


