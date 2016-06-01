/* Filename: 				DS1307.h
 * Author:   				Adrian Gin (amg94@student.canterbury.ac.nz)
 * Created/Modified: 		26-06-06
 *
 * Description:
 *		Functions used to interface with the DS1307 RTC (real time clock).
 *		These functions utilise the hardware TWIregisters
 * 	I2C / TWI serial routines for the AVR mpu's.
 * 	For AVR Core
 */


#ifndef	_DS1307_ROUTINES
#define	_DS1307_ROUTINES

#include "../I2C/gettime.h"

#define	DS1307_ADDRESS		(0xD0)

#define	SECONDS 	(0)
#define	MINUTES 	(1)
#define	HOURS		(2)
#define	DAY		(3)
#define	DATE		(4)
#define	MONTH		(5)
#define	YEAR		(6)

#define	HOURS24	(0x40)

/* BCD to Decimal macro */
/* These are used to convert the tm structure values to 
 * the format accepted by the DS1307 RTC */
#define	DEC2BCD_HI(x)	((x)/10)
#define	DEC2BCD_LO(x)	((x)%10)
#define	DEC2BCD(x)		(((DEC2BCD_HI(x)) << 4) + (DEC2BCD_LO(x)))

#define	BCD_SECONDS		DEC2BCD(TM_SECONDS)
#define	BCD_MINUTES		DEC2BCD(TM_MINUTES)
#define	BCD_HOURS		DEC2BCD(TM_HOURS)
#define	BCD_DAY			DEC2BCD(TM_DAY)
#define	BCD_DATE			DEC2BCD(TM_DATE)
#define	BCD_MONTH		DEC2BCD(TM_MONTH)
#define	BCD_YEAR			DEC2BCD(TM_YEAR)

#define	TENS_MASK	(0x70)
#define	ONES_MASK	(0x0F)
#define	HOUR_MASK	(0x30)



static uint8_t DS1307_TimeDate_config[] = {BCD_SECONDS, 
												   	BCD_MINUTES,
														((BCD_HOURS) | (HOURS24)),
														(((BCD_DAY+6)%7)+1),
												  		BCD_DATE,
												  		BCD_MONTH+1,
												  		BCD_YEAR};		





/* printTime:
 * Outputs to the UART the time in a pre-defined format*/
void printTime(uint8_t* time); 

/* printDate:
 * Outputs to the UART the day and date in a pre-define format*/
void printDate(uint8_t* time);

/* printTIMEDATE: */
/* Print out variables with a separator 
 * Each variable is converted to ASCII before being printed
 *
 */
void printTIMEDATE(uint8_t timeDATA, uint8_t separator);


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
void getTime(uint8_t* time);











#endif
