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

/* Serial Port routines for the AVR mpu's.
*  These functions utilise the hardware UART registers
*
* Filename: hardUart.c
* By Adrian Gin (amg94@student.canterbury.ac.nz)
* Created: 16-06-06
* 
* For AVR Core
* Since the UART has true ouputs, a MAX232 is required to interface
* with a computer's serial port.
*/


#include "hardUart.h"
#include "RingBuffer/ringbuffer.h"
#include <pgm.h>

static char txbuffer[TXBUFFER_SIZE];
volatile RINGBUFFER_T TransmitBuffer = {txbuffer, sizeof(txbuffer)};
static char rxbuffer[RXBUFFER_SIZE];
volatile RINGBUFFER_T ReceiveBuffer = {rxbuffer, sizeof(rxbuffer)};

volatile uint8_t transmitState;

/* Example
volatile PIC_USART_t PrimaryUART = {
   &UCSR1A,
   &UCSR1B,
   &UCSR1C,
   &UBRR1H,
   &UBRR1L,
   &UDR1,

   &ReceiveBuffer,
   &TransmitBuffer
};
*/


/* uartInit:
 * Initialises the baudrate, parity, stop bit generation and 8bit mode
 * It must be called before any hardUart function is used 
 *
 * Interrupts are not set by default.
 *
 */
void uartInit(PIC_USART_t* port)
{
	/*Setup the U2X Bit*/

	*port->TXSTAx |= ((1<<TXEN) | (1<<BGRH));	/*Enable Rx and Tx modules*/
	*port->RCSTAx |= ((1<<SPEN) | (1<<CREN));	/*Enable Rx and Tx modules*/
    
    *port->IEREGx |= ((1<<RCIE) | (1<<TXIE));

}

/* uartSetBaud:
 * Changes the baudrate to the specified value.
 * See the datasheet for more details on what the
 * Baudrate generation registers should be.
 */
void uartSetBaud(PIC_USART_t* port, uint8_t baudrateH, uint8_t baudrateL)
{
    *port->BAUDCONx |= 	(1<<BRG16);
	*port->SPBRGHx = 	baudrateH;
	/* The lower 8bits must be written last as the baudrate generator
	 * is updated as soon as UBRRL is written to*/
	*port->SPBRGLx	=	baudrateL; 

}


/* Disables the Receiver and Transmitter modules*/
void uartDisable(PIC_USART_t* port)
{
	*port->RCSTAx &= ~((1<<SPEN) | (1<<CREN));	/*Enable Rx and Tx modules*/
	*port->IEREGx &= ~((1<<RCIE) | (1<<TXIE));
}

/* uartTx:
 *
 * Transmits the passed byte to the Uart.Tx pin.
 *
 */
//void uartTx(PIC_USART_t* port, uint8_t outbyte)
//{
//	/*Wait until output shift register is empty*/	
//	while( (*port->TXSTAx & (1 << TRMT)) == 0 );
//		
//	/*Send byte to output buffer*/
//	*port->TXREGx	= outbyte;
//}
//


void uartTx(PIC_USART_t* port, uint8_t byte)
{
   *port->IEREGx |= ((1<<RCIE) | (1<<TXIE));
   //* If the buffer is full, then we have to wait until we have to send the data
   // * to prevent data loss
   while(ringbuffer_put((RINGBUFFER_T*)port->TransmitBuffer, byte) == BUFFER_OVERFLOW)
   {
   }

   if( (*port->TXSTAx & (1 << TRMT)) && !ringbuffer_isEmpty((RINGBUFFER_T*)port->TransmitBuffer))
   {
      *port->TXREGx = ringbuffer_get((RINGBUFFER_T*)port->TransmitBuffer);
   }
}


/** Writes nbytes of buffer to the UART */
void uartTxDump(PIC_USART_t* port, uint8_t* buffer, uint16_t nbytes )
{
	uint16_t i = 0;
	while( i++ < nbytes )
	{
		uartTx(port, *buffer++);
	}
}

/** Writes nbytes of buffer to the UART */
void uartTxDump_P(PIC_USART_t* port, PGM_P buffer, uint16_t nbytes )
{
	uint16_t i = 0;
	while( i++ < nbytes )
	{
      uint8_t c;
      c = (*buffer++);
		uartTx(port, c);
	}
}



/* uartTxString:
 * Outputs the passed string to the UART.Tx pin
 * The output is true ouput, not inverted, so a MAX232 or some sort of
 * TTL -> +/- 15V converter is required.
 */
void uartTxString(PIC_USART_t* port, uint8_t* outString)
{
	while( *outString )
	{
		uartTx(port, *outString++);
   }
}

/* Usage: uartTxString_P( PSTR("hello!") ); */
/* Send a string which resides in the program memory */
void uartTxString_P(PIC_USART_t* port, PGM_P outString_P)
{
   char c;
   while( (c = (*outString_P++)) )
   {
      uartTx(port, c);    
   }
}

void uartNewLine(PIC_USART_t* port)
{
   uartTx(port, '\r');
   uartTx(port, '\n'); 
}

/* To echo the receiver buffer, write this code in the main.c file */
/* Just remember that the Interrupt is enabled 
ISR(SIG_UART_RECV)
{
	uartTx(UDR);
}
*/




////----------------------------------------------------------------------------
//// Low priority interrupt vector
//
//#pragma code InterruptVectorLow = 0x18
//void InterruptVectorLow (void)
//{
//     _asm
//    goto InterruptHandlerLow //jump to interrupt routine
//     _endasm
//}
//
////----------------------------------------------------------------------------
//// Low priority interrupt routine
//
//#pragma code
//#pragma interrupt InterruptHandlerLow
//
//void InterruptHandlerLow ()
//{
//    if (PIR1bits.TXIF)
//    {                   
//        LATA  ^= 1;
//        PIR1bits.TXIF = 0;            //clear interrupt flag
//        if( !ringbuffer_isEmpty((RINGBUFFER_T*)PrimaryUART->TransmitBuffer) )
//        {
//            *PrimaryUART->TXREGx = ringbuffer_get((RINGBUFFER_T*)PrimaryUART->TransmitBuffer);
//        }
//        else
//        {     
//            PIE1 &= ~(1<<TXIE);
//        }
//    }
//
//    //For Input Echo
//    if (PIR1bits.RCIF)
//    {                   
//        uint8_t i = *PrimaryUART->RCREGx;
//        uartTx(PrimaryUART, i);
//    }
//}
//
////----------------------------------------------------------------------------
//


