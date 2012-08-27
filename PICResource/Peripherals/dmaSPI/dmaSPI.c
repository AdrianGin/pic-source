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
#include "dmaSPI/dmaSPI.h"

//Requires 2 DMA Channels
void DMA_SPI_Init(PIC_DMA_SPI_t* DMASPI, PIC_DMA_SPI_t* DMATXSPI, uint16_t spiModule)
{
    // One-Shot, Post-Increment, Perip to RAM, Null Writes
    *DMASPI->DMACON = (0x4001);// | (DMA_NULLW));
    *DMASPI->DMAREQ = spiModule;
    *DMASPI->DMAPAD = (uint16_t)DMASPI->attachedSPI->SPIXBUF;
    *DMASPI->DMASTA = (uint16_t)DMASPI->DMABuffer;
    DMASPI->DMAFlag = 0;

    //One Shot Mode, no Post-Increment
    *DMATXSPI->DMACON = (0x6011);
    DMATXSPI->DMABuffer[0] = 0xFF;
    *DMATXSPI->DMAREQ = spiModule;
    *DMATXSPI->DMAPAD = (uint16_t)DMATXSPI->attachedSPI->SPIXBUF;
    *DMATXSPI->DMASTA = (uint16_t)DMATXSPI->DMABuffer;

    DMATXSPI->DMAFlag = 0;


}



/* Quick fix for enabling the interrupts and priorities */
void DMA_SPI_Enable(void)
{
    IPC3bits.DMA1IP = 2;
    IEC0bits.DMA1IE = 1; // Enable DMA interrupt

    IPC6bits.DMA2IP = 2;
    IEC1bits.DMA2IE = 0; // Enable DMA interrupt

    IEC0bits.SPI1IE = 0x00;
    IEC2bits.SPI2IE = 0x00;
}

void DMA_SPI_ExecuteTransfer(PIC_DMA_SPI_t* DMASPI, PIC_DMA_SPI_t* DMATXSPI, uint16_t byteCount)
{
    if( DMASPI->DMAFlag == 0)
    {
        DMASPI->DMAFlag = 1;

        *DMASPI->DMACNT = byteCount-1;
        *DMATXSPI->DMACNT = byteCount-1;

        if ((*DMASPI->DMACON&DMA_CHEN)==0)
        {
            *DMASPI->DMACON |= DMA_CHEN; // Enable DMA Channel
            *DMATXSPI->DMACON |= DMA_CHEN;
            //Initate a Data Transfer if and only if the SPI is
            //idle.
            //if ((*(DMASPI->attachedSPI->SPIXSTA) & (1<<SPIRBF)))
            {
                *DMATXSPI->DMAREQ |= DMA_FORCE;
                //*DMASPI->DMAREQ |= DMA_FORCE;
            }
        }
    }
}


void DMA_SPI_ReceiveBlock(PIC_DMA_SPI_t* DMASPI, PIC_DMA_SPI_t* DMATXSPI, uint8_t* buffer, uint16_t byteCount)
{
    if( byteCount != 0 )
    {
        *DMATXSPI->DMACON = (0x6011);
        DMATXSPI->DMABuffer[0] = 0xFF;
        DMA_SPI_ExecuteTransfer(DMASPI, DMATXSPI, byteCount);
        //Here it's polled
        while(DMASPI->DMAFlag);

        if( buffer )
        {
            memcpy(buffer, (uint8_t*)&DMASPI->DMABuffer[0], byteCount);
        }
    }
}


void DMA_SPI_SendBlock(PIC_DMA_SPI_t* DMASPI, PIC_DMA_SPI_t* DMATXSPI, uint8_t* buffer, uint16_t byteCount)
{
    if( byteCount != 0 )
    {
        //When we receive we send 0xFF's
        *DMATXSPI->DMACON = (0x6001);
        memcpy((uint8_t*)&DMASPI->DMABuffer[0], buffer, byteCount);
        DMA_SPI_ExecuteTransfer(DMASPI, DMATXSPI, byteCount);
        //Here it's polled
        while(DMASPI->DMAFlag);
        if( buffer )
        {
            memcpy(buffer, (uint8_t*)&DMASPI->DMABuffer[0], byteCount);
        }
    }
}