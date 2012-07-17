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
 
#include "p24Hxxxx.h"
#include "hardwareSpecific.h"
#include "spi.h"


volatile uint8_t spiTransferComplete = TRANSFER_COMPLETE;

//ISR(SIG_SPI)
//{
//   spiTransferComplete = TRANSFER_COMPLETE;
//}

/** Sets the micro up as a
 * SPI Master
 * MOSI configured as an output,
 * MISO as an input,
 * nSS as an output,
 * CLK as an output 
 */
void SPI_Init(PIC_SPI_t* port)
{
   /* Setup ports */
   SPI_DDR &= ~((1 << nSS) | (1 << SCK) | (1 << MOSI) );
   SPI_DDR |=  (1 << MISO);
 
   SPI_PORT |= ((1 << MOSI) | (1 <<nSS));
   SPI_PORT &= ~((1 << MISO) | (1 << SCK));

   /* set CLK speed */
   *port->SPIXCON1 &= ~(PRESCALER_MASK);
   *port->SPIXCON1 |= (PRESCALE_DIV4); //defaults to div4

   /* Initiate as Master and Use CPHA = 1*/
   *port->SPIXCON1 &= ~(1<<SMP);

    *port->SPIXCON1 &= ~(1<<CKE);
    *port->SPIXCON1 |= (1<<CKE);

   *port->SPIXCON1 |= (1<<MSTEN);
   *port->SPIXSTA  |= (1<<SPIEN);
   
   //*port->SPIXBUF = 0x0000;
   
   //Activate Interrupts
   IFS0bits.SPI1IF = 0x00;
   IFS2bits.SPI2IF = 0x00;
   IEC0bits.SPI1IE = 0x01;
   IEC2bits.SPI2IE = 0x01;

   IPC2bits.SPI1IP = 0x03;
   IPC2bits.SPI1IP = 0x03;

}

void SPI_DisablePort(PIC_SPI_t* port)
{
    *port->SPIXSTA  &= ~(1<<SPIEN);
}

void SPI_EnablePort(PIC_SPI_t* port)
{
    *port->SPIXSTA  |= (1<<SPIEN);
}

void SPI_SetClockPolarity(PIC_SPI_t* port, uint8_t polarity)
{
    *port->SPIXCON1 &= ~(1<<CKE);
    *port->SPIXCON1 |= (polarity<<CKE);
}


void SPI_SetSpeed(PIC_SPI_t* port, uint8_t divisor)
{

   /* Re-enable the SPI*/
   *port->SPIXSTA &= ~(1<<SPIEN);

   /* set CLK speed */
   *port->SPIXCON1 &= ~(PRESCALER_MASK);
   *port->SPIXCON1 |= (divisor); //defaults to div4
    
   *port->SPIXSTA |= (1<<SPIEN);
}


uint8_t SPI_TxByte(PIC_SPI_t* port, uint8_t data)
{
   //spiTransferComplete = TRANSFER_INCOMPLETE;
   *port->SPIXBUF = data;
   /* This while loop has given me so many problems! Sometimes the interrupt is not set! */
   while( !( *port->SPIXSTA & (1 << SPIRBF)))
   {
       //LATA ^= 1;
   }
   return *port->SPIXBUF;
}


/* Transmits the first 'n' bytes of 'data' along the SPI bus
 * Return values are ignored. */
void SPI_TxBlock(PIC_SPI_t* port, uint8_t* data, uint16_t n)
{
   uint16_t i;
   for( i = 0 ; i < n ; i++ )
   {
      SPI_TxByte(port, data[i]);
   }
}


/* Reads 'n' bytes from the SPI bus and places them into data
 * data must be of correct lenght. */
void SPI_RxBlock(PIC_SPI_t* port, uint8_t* data, uint16_t n)
{
   uint16_t i;
   if( data )
   {
       for( i = 0 ; i < n ; i++ )
       {
          data[i] = SPI_RxByte(port);
       }
   }
   else
   {
       for( i = 0 ; i < n ; i++ )
       {
          SPI_RxByte(port);
       }
   }
}



uint8_t SPI_RxByte(PIC_SPI_t* port)
{
   return SPI_TxByte(port, 0xFF);
}



