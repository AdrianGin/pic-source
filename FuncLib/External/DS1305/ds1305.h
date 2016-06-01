/* DS1305 Routines for the ATMEGA Micro
 * Author: Adrian Gin
 * Date: 27/06/07
 *
 */

/** \file ds1305.h
	 \brief Dallas DS1305 RTC Drivers
*/

/**	 

	\ingroup avr_external
 	\defgroup ds1305 Dallas DS1305 RTC Drivers
 	\code #include "ds1305.h" \endcode

 	 
	 \par Description
	Drivers for the DS1305 RTC over SPI bus.
 	 
*/
//@{

#ifndef	_DS1305_ROUTINES
#define	_DS1305_ROUTINES



#include "RTC/RTCPrint.h"

/* To do, use similar structure to the DS1307 data array*/

#define DS1305_READ     (0)
#define DS1305_WRITE    (1)

#define DS1305_CE_DDR   (DDRD)
#define DS1305_CE_PORT  (PORTD)
#define DS1305_CE_PIN   (PD6)



/* The 7th bit in the Address is the R/W Bit
 * 1 for Write 0 for Read */
#define DS1305_RW       (7)

/* Control Register */
#define DS1305_EOSC     (7)
#define DS1305_WP       (6)
#define DS1305_INTCN    (2)
#define DS1305_AIE1     (1)
#define DS1305_AIE0     (0)

/* Status Register */
#define DS1305_IRQF1     (1)
#define DS1305_IRQF0     (0)

/* DS1305 Register Addresses */
#define DS1305_SECS     (0x00)
#define DS1305_MINS     (0x01)
#define DS1305_HOURS    (0x02)
#define DS1305_DAY      (0x03)
#define DS1305_DATE     (0x04)
#define DS1305_MONTH    (0x05)
#define DS1305_YEAR     (0x06)

/* Number of bytes in each alarm register */
#define DS1305_SIZEOFALARM (0x04)
#define DS1305_ALARMMASK	(0x80);

/* Alarm 0 Registers */
#define DS1305_A0_SECS     (0x07)
#define DS1305_A0_MINS     (0x08)
#define DS1305_A0_HOURS    (0x09)
#define DS1305_A0_DAY      (0x0A)
 
/* Alarm 1 Registers */
#define DS1305_A1_SECS     (0x0B)
#define DS1305_A1_MINS     (0x0C)
#define DS1305_A1_HOURS    (0x0D)
#define DS1305_A1_DAY      (0x0E)

/* Control and Status Registers */
#define DS1305_CTRL        (0x0F)
#define DS1305_STATUS      (0x10)
#define DS1305_TRICKLE     (0x11)

/* Interrupt Names */
#define DS1305_RTC0     (0)
#define DS1305_RTC1     (1)

extern uint8_t DS1305_TimeDate_config[];


void DS1305_SetAlarm(uint8_t* alarmConfig, uint8_t alarmIndex);
void DS1305_GetAlarm(uint8_t* alarmConfig, uint8_t alarmIndex);
void DS1305_AlarmControl(uint8_t alarmIndex, uint8_t enable);

void DS1305_GetTime(uint8_t* time);
void DS1305_SetTime(uint8_t* DS1305_config);
void DS1305_Init(void);
void DS1305_WriteByte(uint8_t address, uint8_t data);
uint8_t DS1305_ReadByte(uint8_t address);







#endif



