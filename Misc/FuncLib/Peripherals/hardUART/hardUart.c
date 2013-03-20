/* Serial Port routines for the AVR mpu's.
*  These functions utilise the hardware UART registers
*
* Filename: harUart.c
* By Adrian Gin (amg94@student.canterbury.ac.nz)
* Created: 16-06-06
* 
* For AVR Core
* Since the UART has true ouputs, a MAX232 is required to interface
* with a computer's serial port.
*/

#include <avr/io.h>
#include <avr/pgmspace.h>
#include	<util/delay.h>
#include	"hardUart.h"
#include "RingBuffer/ringbuffer.h"

static char txbuffer[TXBUFFER_SIZE];
static RINGBUFFER_T TransmitBuffer = {txbuffer, sizeof(txbuffer)};

static char rxbuffer[RXBUFFER_SIZE];
RINGBUFFER_T ReceiveBuffer = {rxbuffer, sizeof(rxbuffer)};


/* uartInit:
 * Initialises the baudrate, parity, stop bit generation and 8bit mode
 * It must be called before any hardUart function is used 
 *
 * Interrupts are not set by default.
 *
 */
void uartInit(uint8_t baudrate, uint8_t U2Xvalue)
{
	/*Setup the U2X Bit*/
	UCSRA	=	(UCSRA & (~(1<<U2X))) | (U2Xvalue << U2X);
	
	UCSRB |= (1<<RXEN) | (1<<TXEN) | (1<<TXCIE) | (1<<RXCIE);	/*Enable Rx and Tx modules*/
	UCSRB &= ~(1<<UCSZ2);				/*Set to 8bit mode*/
	

	/*Select UCSRC to be written to*/	
	/* Set to Asynchronous Mode
	 *			 1 Stop-bit
	 *			 No Parity
	 *			 8-bit char mode
	 */
	UCSRC = (UCSRC & ~( UCSRCMASK ))
				|  (NOPARITY<<UPM0)
				|	(BIT8 << UCSZ0) 
				|  (1<<URSEL);

	/*Set the baud rate to the desired rate*/
	uartSetBaud(baudrate, 0);
}

/* uartSetBaud:
 * Changes the baudrate to the specified value.
 * See the datasheet for more details on what the
 * Baudrate generation registers should be.
 */
void uartSetBaud(uint8_t baudrateL, uint8_t baudrateH)
{
	UBRRH = 	baudrateH;
	/* The lower 8bits must be written last as the baudrate generator
	 * is updated as soon as UBRRL is written to*/
	UBRRL	=	baudrateL; 

}


/* Disables the Receiver and Transmitter modules*/
void uartDisable(void)
{
	UCSRB &= ~((1<<RXEN) | (1<<TXEN));	/*Disable Rx and Tx modules*/
	
}

/* uartTx:
 *
 * Transmits the passed byte to the Uart.Tx pin.
 *
 */
//void uartTx(uint8_t outbyte)
//{
	/*Wait until output shift register is empty*/	
//	while( (UCSRA & (1<<UDRE)) == 0 );
		
	/*Send byte to output buffer*/
//	UDR	= outbyte;
//}



void uartTx(uint8_t byte)
{
   /* If the buffer is full, then we have to wait until we have to send the data
    * to prevent data loss */

   while(ringbuffer_put((RINGBUFFER_T*)&TransmitBuffer, byte) == BUFFER_OVERFLOW)
   {
      PORTC |= (1<<4);
      if( (UCSRA & (1<<UDRE)) )
      {
         if( ringbuffer_len((RINGBUFFER_T*)&TransmitBuffer) )
      	{
         	UDR = ringbuffer_get((RINGBUFFER_T*)&TransmitBuffer); 
      	}	
   	}
   }

   /* If this is the first byte */
   if( (UCSRA & (1<<UDRE)) )
   {
      if( ringbuffer_len((RINGBUFFER_T*)&TransmitBuffer) )
   	{
      	UDR = ringbuffer_get((RINGBUFFER_T*)&TransmitBuffer); 
   	}	
	}
}

/* Once a tx has completed, this is called */
ISR(SIG_UART_TRANS)
{

   sei();
   /* Tx the next byte if there are still bytes in the buffer */
   if( ringbuffer_len((RINGBUFFER_T*)&TransmitBuffer) )
   {
      UDR = ringbuffer_get((RINGBUFFER_T*)&TransmitBuffer);
   }
}





/** Writes nbytes of buffer to the UART */
void uartTxDump(uint8_t* buffer, uint8_t nbytes )
{
	uint16_t i = 0;
	while( i++ < nbytes )
	{
		uartTx(*buffer++);
	}
}


/* uartTxString:
 * Outputs the passed string to the UART.Tx pin
 * The output is true ouput, not inverted, so a MAX232 or some sort of
 * TTL -> +/- 15V converter is required.
 */
void uartTxString(uint8_t* outString)
{
	while( *outString )
	{
		uartTx(*outString++);
   }
	
}

/* Usage: uartTxString_P( PSTR("hello!") ); */
/* Send a string which resides in the program memory */
void uartTxString_P(const char* outString_P)
{

   char c;

   while( (c = pgm_read_byte(outString_P++)) )
   {
      uartTx(c);    
   }
}

void uartNewLine(void)
{
   uartTx('\r');
   uartTx('\n'); 
}

/* To echo the receiver buffer, write this code in the main.c file */
/* Just remember that the Interrupt is enabled 
ISR(SIG_UART_RECV)
{
	uartTx(UDR);
}
*/

