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

#ifndef _DMA_H
#define _DMA_H

#define DMA_CHEN (1<<15)
#define DMA_SIZE (1<<14)
#define DMA_DIR  (1<<13)
#define DMA_FORCE (1<<15)
#define DMA_NULLW  (1<<11)

#define DMA_UART1   (0x000C)
#define DMA_UART2   (0x001F)
#define DMA_SPI1    (0x000A)

//The largest block of data to be sent at once.
#define DMA_MEM_SIZE (2048)
#define DMA_TX_SIZE  (64)
#define DMA_RX_SIZE  (DMA_MEM_SIZE-DMA_TX_SIZE)

#define DMA_NOT_STARTING (0)
#define DMA_STARTING     (1)


extern uint8_t DMABuffer[];
extern uint8_t* DMATxBuffer;
extern uint8_t* DMARxBuffer;

#endif


