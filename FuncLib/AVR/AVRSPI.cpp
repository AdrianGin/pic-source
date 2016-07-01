/*

Copyright (c) 2016 Adrian Gin (adrian.gin[at]gmail.com)

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
 
#include <avr/io.h>
#include "AVRSPI.h"
#include "SPI.h"

using Devices::SPI;

namespace AVR
{

volatile uint8_t spiTransferComplete = Devices::SPI::TRANSFER_COMPLETE;

/*ISR(SPI_STC_vect)
{
   spiTransferComplete = Devices::SPI::TRANSFER_COMPLETE;
}*/

/** Sets the micro up as a
 * SPI Master
 * MOSI configured as an output,
 * MISO as an input,
 * nSS as an output,
 * CLK as an output
 *
 * MSB first
 */

SPI::SPI(GPIO& SCK, GPIO& MISO, GPIO& MOSI, GPIO& nSS) noexcept :
		SCK(SCK), MISO(MISO), MOSI(MOSI), nSS(nSS)
{
}

void SPI::Init()
{
   /* Setup ports */
   SCK.Init( Devices::GPIO::OUTPUT );
   MOSI.Init( Devices::GPIO::OUTPUT );
   nSS.Init( Devices::GPIO::OUTPUT );
   MISO.Init( Devices::GPIO::INPUT );

   SCK.SetOutput(Devices::GPIO::LOW);
   MOSI.SetOutput(Devices::GPIO::HIGH);
   nSS.SetOutput(Devices::GPIO::HIGH);
   MISO.SetOutput(Devices::GPIO::LOW);
 
   /* Initiate as Master and Use CPHA = 1, MSB first*/
   SPCR |= ((1 << SPE) | (1 << MSTR) );
   
   /* set CLK speed to fclk/32 */
   //SPCR |= ((1 << SPR0) | (1 << SPR1));
   SPSR |= (1<<SPI2X);
   /*
   SPSR;
   SPCR |= (1 << SPIE);  */
   //SPCR |= (1 << SPIE);
}


uint8_t SPI::TxByte(uint8_t data)
{
   //spiTransferComplete = TRANSFER_INCOMPLETE;
   SPDR = data;
   /* This while loop has given me so many problems! Sometimes the interrupt is not set! */
   while( !( SPSR & (1 << SPIF)))
   {
   }
   
   return SPDR;
}


/* Transmits the first 'n' bytes of 'data' along the SPI bus
 * Return values are ignored. */
void SPI::TxBlock(uint8_t* data, uint8_t n)
{
   uint8_t i;
   
   for( i = 0 ; i < n ; i++ )
   {
      TxByte(data[i]);
   }
}


/* Reads 'n' bytes from the SPI bus and places them into data
 * data must be of correct lenght. */
void SPI::RxBlock(uint8_t* data, uint8_t n)
{
   uint8_t i;
   
   for( i = 0 ; i < n ; i++ )
   {
      data[i] = RxByte();
   }
}



uint8_t SPI::RxByte(void)
{
   return TxByte(0xFF);
}


}


