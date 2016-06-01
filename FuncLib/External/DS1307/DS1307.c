/* Filename: 				DS1307.c
 * Author:   				Adrian Gin (amg94@student.canterbury.ac.nz)
 * Created/Modified: 		26-06-06
 *
 * Description:
 *		Functions used to interface with the DS1307 RTC (real time clock).
 *		These functions utilise the hardware TWIregisters
 * 	I2C / TWI serial routines for the AVR mpu's.
 * 	For AVR Core
 */

#include <avr/io.h>
#include <avr/iom8.h>

#include <stdint.h>
#include	<stdlib.h> 
#include <string.h>

#define	F_CPU 8000000UL
#include <util/delay.h>

#include "../I2C/i2c.h"
#include "../DS1307/DS1307.h"
#include "../hardUart/hardUart.h"
#include "../Pause/pause.h"

static	uint8_t	OutputString[10];

static const char* WEEKDAYS[7] = { "Monday",
									 "Tuesday",
									 "Wednesday",
									 "Thursday",
									 "Friday",
									 "Saturday",
									 "Sunday" };
									 
									 
							 


/* printTIMEDATE: */
/* Print out variables with a separator 
 * Each variable is converted to ASCII before being printed
 *
 */
void printTIMEDATE(uint8_t timeDATA, uint8_t separator)
{
		utoa(timeDATA, OutputString, 10);
		if(timeDATA < 10)
		{
			uartTx('0');
		}
		
		pausems(2);
				
		uartTxString(OutputString);
		uartTx(separator);

}


/* getTime:
 * Obtain the time and date data from the DS1307
 * The data is to be stored in time.
 * Where time[] = {SECONDS,
 *						 MINUTES,
 *						 HOURS,
 *						 DAY,
 *						 DATE,
 *						 MONTH,
 * 					 YEAR};
 */
void getTime(uint8_t* time)
{
		/* Set the address of the DS1307 to 0x00 */
		/* Receive the bytes of the RTC */
		i2cAddress(DS1307_ADDRESS, TW_WRITE);
		i2cTransmit(0x00);
		i2cStop();
		
		/* Get the bytes */
		i2cAddress(DS1307_ADDRESS, TW_READ);
		i2cReadBlock(time, 7);
		i2cStop();
}

/* printTime:
 * Print out the time in the format HH:MM:SS
 * when passed the time array pointer
 */
void printTime(uint8_t* time)
{

	uint8_t temp_time[3];
	uint8_t i;
	
	/*Convert all times from BCD to decimal */
	for( i = SECONDS; i <= MINUTES; i++)
	{
		temp_time[i] = ((time[i] & TENS_MASK) >> 3)*5;
		temp_time[i] = temp_time[i] + (time[i] & ONES_MASK);
	}

	temp_time[HOURS] = ((time[HOURS] & HOUR_MASK) >> 3)*5;
	temp_time[HOURS] = temp_time[HOURS] + (time[HOURS] & ONES_MASK);

	/* Print out the time in the format HH:MM:SS */
	uartTxString("\n\rThe current time is: ");
	for( i = HOURS; i <= HOURS; i--)
	{		
		printTIMEDATE(temp_time[i], ':');
	}
	
}


/* printDate:
 * Print out the Day and Date in the format <DAY OF WEEK> DD:MM:YY
 * when passed the time array pointer
 */
void printDate(uint8_t* time)
{
	
	uint8_t temp_time[4];
	uint8_t i;
	
	/*Convert all dates from BCD to decimal */
	for( i = DAY; i <= YEAR; i++)
	{
		temp_time[i-DAY] = ((time[i] & TENS_MASK) >> 3)*5;
		temp_time[i-DAY] = temp_time[i-DAY] + (time[i] & ONES_MASK);
	}
	
	/* Print out the day of the week*/
	uartTxString("\n\rThe current date is: ");
	
	uartTxString(WEEKDAYS[temp_time[DAY-DAY]-1]);	
	uartTx(' ');
	
	/* Print out the Date in the format DD/MM/YY*/
	for( i = DATE; i <= YEAR; i++)
	{		
		printTIMEDATE(temp_time[i-DAY], '/');
	}

}



