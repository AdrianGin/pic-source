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


#include "p24Hxxxx.h"
#include "hardUart.h"
#include "RingBuffer/ringbuffer.h"
#include <pgm.h>
#include <stdint.h>

static char txbuffer[TXBUFFER_SIZE];
volatile RINGBUFFER_T TransmitBuffer = {txbuffer, sizeof (txbuffer)};
static char rxbuffer[RXBUFFER_SIZE];
volatile RINGBUFFER_T ReceiveBuffer = {rxbuffer, sizeof (rxbuffer)};

static char txbuffer2[TXBUFFER_SIZE];
volatile RINGBUFFER_T TransmitBuffer2 = {txbuffer2, sizeof (txbuffer2)};
static char rxbuffer2[RXBUFFER_SIZE];
volatile RINGBUFFER_T ReceiveBuffer2 = {rxbuffer2, sizeof (rxbuffer2)};


/* Example
volatile PIC_USART_t U1 = {&U1STA, &U1MODE, &U1BRG, &U1TXREG, &U1RXREG, &IFS0, TX1IF, &TransmitBuffer, &ReceiveBuffer};
volatile PIC_USART_t U2 = {&U2STA, &U2MODE, &U2BRG, &U2TXREG, &U2RXREG, &IFS1, TX2IF, &TransmitBuffer2, &TransmitBuffer2};
 *
 *
 *
 *
 *
void __attribute__((__interrupt__, no_auto_psv)) _U1TXInterrupt(void)
{
    IFS0bits.U1TXIF = 0;
    //Send the next byte
    if( !ringbuffer_isEmpty((RINGBUFFER_T*) U1.TransmitBuffer))
    {
        *U1.UXTXREG = ringbuffer_get((RINGBUFFER_T*) U1.TransmitBuffer);
    }
}
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
    uint16_t i;
    /*Setup the U2X Bit*/
    *port->UXMODE |= ((1 << UARTEN) | (1 << BGRH)); /*Enable Rx and Tx modules*/
    *port->UXSTA |= ((1 << UTXEN) | (1 << TXISEL0)); /*Enable Rx and Tx modules*/

    //Enable Interrupts & Set Priority
    IFS0bits.U1TXIF = 0;
    IFS0bits.U1RXIF = 0;
    IFS1bits.U2TXIF = 0;
    IFS1bits.U2RXIF = 0;

    IPC3bits.U1TXIP = 5;
    IPC2bits.U1RXIP = 3;
    IEC0bits.U1TXIE = 1;
    IEC0bits.U1RXIE = 1;

    IPC7bits.U2TXIP = 5;
    IPC7bits.U2RXIP = 4;

    IEC1bits.U2TXIE = 1;
    IEC1bits.U2RXIE = 1;
}

/* uartSetBaud:
 * Changes the baudrate to the specified value.
 * See the datasheet for more details on what the
 * Baudrate generation registers should be.
 */
void uartSetBaud(PIC_USART_t* port, uint16_t baudrate)
{
    *port->UXBRG = baudrate;
}

/* Disables the Receiver and Transmitter modules*/
void uartDisable(PIC_USART_t* port)
{
    *port->UXSTA &= ~((1 << UTXEN)); /*Enable Rx and Tx modules*/
    *port->UXMODE &= ~((1 << UARTEN) | (1 << BGRH)); /*Enable Rx and Tx modules*/

    IEC0bits.U1TXIE = 0;
    IEC0bits.U1RXIE = 0;

    IEC1bits.U2TXIE = 0;
    IEC1bits.U2RXIE = 0;
}

/* uartTx:
 *
 * Transmits the passed byte to the Uart.Tx pin.
 *
 */
//void uartTx(PIC_USART_t* port, uint8_t outbyte)
//{
//	/*Wait until output shift register is empty*/	
//	while( (*port->UXSTA & (1 << UTXBF)) == 0 );
//		
//	/*Send byte to output buffer*/
//	*port->UXTXREG	= outbyte;
//}
//

//Total interrupt immersion.
void _internalTx(PIC_USART_t* port, uint8_t byte)
{
    if ( (*port->UXSTA & (1 << TXMT)) && !ringbuffer_isEmpty((RINGBUFFER_T*) port->TransmitBuffer))
    {
        *port->UXIFSREG |= (1<<(port->IFSBit));
        //*port->UXTXREG = ringbuffer_get((RINGBUFFER_T*) port->TransmitBuffer);
    }

}

void uartTx(PIC_USART_t* port, uint8_t byte)
{
    //IEC0bits.U1TXIE = 1;
    //*port->IEREG |= ((1<<TXIE));
    //* If the buffer is full, then we have to wait until we have to send the data
    // * to prevent data loss
    while (ringbuffer_put((RINGBUFFER_T*) port->TransmitBuffer, byte) == BUFFER_OVERFLOW)
    {
        LATA ^= (1<<4);
        _internalTx(port, byte);
    }
    _internalTx(port, byte);
}

/** Writes nbytes of buffer to the UART */
void uartTxDump(PIC_USART_t* port, uint8_t* buffer, uint16_t nbytes)
{
    uint16_t i = 0;
    while (i++ < nbytes)
    {
        uartTx(port, *buffer++);
    }
}

/** Writes nbytes of buffer to the UART */
void uartTxDump_P(PIC_USART_t* port, PGM_P buffer, uint16_t nbytes)
{
    uint16_t i = 0;
    while (i++ < nbytes)
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
    while (*outString)
    {
        uartTx(port, *outString++);
    }
}

/* Usage: uartTxString_P( PSTR("hello!") ); */

/* Send a string which resides in the program memory */
void uartTxString_P(PIC_USART_t* port, PGM_P outString_P)
{
    char c;
    while ((c = (*outString_P++)))
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
//        if( !atomic_ringbuffer_isEmpty((RINGBUFFER_T*)PrimaryUART->TransmitBuffer) )
//        {
//            *PrimaryUART->TXREGx = atomic_ringbuffer_get((RINGBUFFER_T*)PrimaryUART->TransmitBuffer);
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


