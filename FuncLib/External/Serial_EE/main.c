/* I2C routines for the AVR mpu's.
*  These functions utilise the hardware TWI registers
*
* By Adrian Gin (amg94@student.canterbury.ac.nz)
* Created: 16-06-06
* For AVR Core
*/


/* This is an exmaple program which utilises the UART, I2C / TWI module
 * and interfaces with a DS1624 temperature sensor*/

/* This can also be used to interface with the DS1307 RTC (real-time-clock)
 */

#include <avr/io.h>
#include <avr/iom8.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


#include <avr/interrupt.h>


#define F_CPU 8000000UL

#include <util/delay.h>

#include "../hardUart/hardUart.h"
#include "../Pause/pause.h"
#include "../I2C/i2c.h"

#include "../DS1624/DS1624.h"
#include "../DS1307/DS1307.h"
#include "../serial_ee/serial_ee.h"

/* Obsolete */


char message[] = "SHIT";

#define 	DATA_SIZE			 sizeof(message)

int main(void)
{
	int8_t* temperature;
	
	uint8_t buffer[10];
	uint8_t time[7];
	uint8_t outString[25];
	uint8_t datasize = DATA_SIZE;
	

	EE_AddressStruct	EE_AddressData;

	EE_AddressData.EE_Address = EE_START_ADDRESS;
	EE_AddressData.EE_Block	 = EE_START_BLOCK;

/* Data needs to be converted to BCD */
/* TM_DAY needs to be normalised */
/* TM_HOURS needs to be in 24Hr Mode*/	
	sei();
	/*Enable receive complete interrupt*/
	UCSRB |=	(1<<RXCIE);
	
	uartInit(BAUD9600, FAST);
	
	i2cInit(TWI_100KHZ , TWI_PRESCALE_3);
	 	
	/* Write to the DS1307 The time and date */	
	i2cAddress(DS1307_ADDRESS, TW_WRITE);

	/* Set the address to 0x00 */
	/* Then write to the RTC all the TIME and DATE */
	i2cTransmit(0x00);
	i2cTransmitBlock(DS1307_TimeDate_config, 7);
	i2cStop();

	/* Access DS1624 Config */
	/* Set the config to continuous conversion */
	i2cAddress(DS1624_ADDRESS, TW_WRITE);
	i2cTransmitBlock( DS1624config, 2 );	
	i2cStop();

	/* Start Temperature conversions */
	pausems(100);
	i2cAddress(DS1624_ADDRESS, TW_WRITE);
	i2cTransmit(0xEE); 	/* Start Temperature conversions */
	i2cStop();
		
	pausems(1000);	
	

	

	while(1)
	{	
			 
#if	EE_HAS_BLOCKS == 1
		serialEE_HandleBlockOverflow(&EE_AddressData, DATA_SIZE); 
#endif			
		serialEE_WriteBlock( message, 
									DATA_SIZE, 
									&EE_AddressData);
		
 		/* Increment Address Pointer */
		EE_AddressData.EE_Address = EE_AddressData.EE_Address + DATA_SIZE;
		serialEE_setEndAddress(EE_AddressData.EE_Block, EE_AddressData.EE_Address);
		
		/* For devices which are divided up into 'blocks' */
		printEEPointer(&EE_AddressData);


		pausems(500);
		

			
		}
		
	
	return 0;	
}






ISR(SIG_UART_RECV)
{
	uint8_t		buffer[DATA_SIZE];
	uint16_t temp;
	uint16_t	EE_EndBlockAddress;
	EE_AddressStruct	EE_Current;
	
	EE_Current.EE_Address 	= 0x00;
	EE_Current.EE_Block	 	= 0x00;
		
	temp = UDR;

	
	switch (temp)
	{
		/* (G)et, to print out all the data collected since turning on the
		 * AVR*/
		
		/* Obtain the data from the 24XX16 EEPROM IC*/ 
		
		case 'G' :

			/* Read data from all blocks */
			for( EE_Current.EE_Block = EE_START_BLOCK; 
			     EE_Current.EE_Block <= (EE_NUMBER_OF_BLOCKS-1); 
			     EE_Current.EE_Block++)

			{     			
					serialEE_PrintBlock(EE_Current.EE_Block, DATA_SIZE);
			}

			
			
			break;
			
		case 'D':

				serialEE_ReadBlock( buffer, 1, &EE_Current);
				uartTx(buffer[0]);
			
			break;	
			
			/* Read from 'x' block */
			
		default:
			
				serialEE_PrintBlock(temp - '0', DATA_SIZE);
			

				break;
				
			
				
				
		
		
	}
	
	//uartTx(UDR);
	
}





