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



/* SPI Routines for the ATMEGA Micro
 * Author: Adrian Gin
 * Date: 27/06/07
 *
 */

/** \file spi.h
	 \brief SPI Drivers for the AVR Core.
*/

/**	 

	\ingroup avr_peripheral
 	\defgroup spi SPI Hardware Driver.
 	\code #include "spi.h" \endcode

 	 
	 \par Description
	 SPI Communications protocol for the AVR core.
 	 
*/
//@{
 

#ifndef	_SPI_ROUTINES
#define	_SPI_ROUTINES

#include <stdint.h>

/* SPI Interface, these should be defined in hardwareSpecific.h */
/*
#define SPI_DDR   (DDRB)
#define SPI_PORT  (PORTB)
#define SCK       (PB5)
#define MISO      (PB4)
#define MOSI      (PB3)
#define nSS       (PB2) */

//SPIxSTAT Register
#define SPIEN_BIT   (15)
#define SPITBF_BIT  (1)
#define SPIRBF_BIT  (0)

//SPIxCON1 Register
#define MODE16_BIT  (10)
#define SMP_BIT     (9)
#define CKE_BIT     (8)
#define CKP_BIT     (6)
#define MSTEN_BIT   (5)
#define DISSCLK_BIT (12)

//Clock polarity
#define SAMPLE_ON_RISING  (1)
#define SAMPLE_ON_FALLING (0)


//Set Speed defines
#define PRESCALER_MASK   (0x1F)
#define PRESCALE_DIV2    (0x1B)
#define PRESCALE_DIV3    (0x17)
#define PRESCALE_DIV4    (0x1E)
#define PRESCALE_DIV8    (0x03)
#define PRESCALE_DIV16   (0x1D)
#define PRESCALE_DIV32   (0x19)
#define PRESCALE_DIV64   (0x1C)
#define PRESCALE_DIV128  (0x18)
#define PRESCALE_DIV256  (0x10)

#define TRANSFER_COMPLETE     (1)
#define TRANSFER_INCOMPLETE   (0)

#define MODE_16BIT  (1)
#define MODE_8BIT  (0)

typedef struct
{
   volatile uint16_t* SPIXSTA;
   volatile uint16_t* SPIXCON1;
   volatile uint16_t* SPIXCON2;
   volatile uint16_t* SPIXBUF;

} PIC_SPI_t;


void SPI_Init(PIC_SPI_t* port);

void SPI_SetSpeed(PIC_SPI_t* port, uint8_t divisor);
void SPI_SetClockPolarity(PIC_SPI_t* port, uint8_t polarity);
void SPI_SetDataMode(PIC_SPI_t* port, uint8_t mode);

uint8_t SPI_TxByte(PIC_SPI_t* port, uint16_t data);
/** SPI_RxByte, the same as SPI_TxByte(0xFF) */
uint8_t SPI_RxByte(PIC_SPI_t* port);

void SPI_RxBlock(PIC_SPI_t* port, uint8_t* data, uint16_t n);

void SPI_TxBlock(PIC_SPI_t* port, uint8_t* data, uint16_t n);

void SPI_DisablePort(PIC_SPI_t* port);
void SPI_EnablePort(PIC_SPI_t* port);

#endif
