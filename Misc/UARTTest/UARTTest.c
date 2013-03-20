#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "hardUart/hardUart.h"

/* SPI Defines */
#define SPI_DDR   (DDRB)
#define SPI_PORT  (PORTB)
#define SCK       (PB7)
#define MISO      (PB6)
#define MOSI      (PB5)
#define nSS       (PB4)

#define	ENABLE_TRANSMITTER_BIT	TXEN	
#define	ENABLE_RECEIVER_BIT	RXEN	
#define	UART_STATUS_REG	UCSRA	
#define	TRANSMIT_COMPLETE_BIT	TXC	
#define	RECEIVE_COMPLETE_BIT	RXC	
#define	UART_DATA_REG	UDR	


int main(void)
{
   MCUCSR = (1 << JTD);
   MCUCSR = (1 << JTD);

   SPI_DDR |= ((1 << nSS) | (1 << MOSI) );
   /* Initiate as Master and Use CPHA = 1*/
   SPCR |= ((1 << SPE) | (1 << MSTR));
   /* set CLK speed to fclk/32 */

	/*Setup the U2X Bit*/
	UCSRA	=	(UCSRA & (~(1<<U2X)));
	
	UCSRB |= ((1<<RXEN) | (1<<TXEN));	/*Enable Rx and Tx modules*/
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

   /*Enable the pull up on RXD */
   PORTD |= (1 << PD0);

   UBRRL = 39;

   //sei();

   uint8_t c;
   while(1)
   {
      while(!(UART_STATUS_REG & (1 << RECEIVE_COMPLETE_BIT)));
      c = UART_DATA_REG;

      UART_DATA_REG = c;                                   // prepare transmission
      while (!(UART_STATUS_REG & (1 << TRANSMIT_COMPLETE_BIT)));// wait until byte sendt
      UART_STATUS_REG |= (1 << TRANSMIT_COMPLETE_BIT);          // delete TXCflag

   }

}
