/* Filename: 				DS1624.h
 * Author:   				Adrian Gin (amg94@student.canterbury.ac.nz)
 * Created/Modified: 		26-06-06
 *
 * Description:
 *		Functions used to interface with the DS1624 Temperature Sensor IC.
 *		These functions utilise the hardware TWIregisters
 * 	I2C / TWI serial routines for the AVR mpu's.
 * 	For AVR Core
 */


#ifndef	_DS1624_ROUTINES
#define	_DS1624_ROUTINES

#define	DS1624_ADDRESS 	(0x90)

static uint8_t DS1624config[] = {0x4C, 0x48};

/* signed_temperature:
 * Calculate the temperature returned from a DS1624 Temperature sensor
 * See the DS1624 Datasheet for more information.
 *
 * temperature[0] = upper byte returned from DS1624
 * temperature[1] = lower byte from DS1624.
 *
 * sign_temperature(temperature);
 *
 * The altered values are stored in 'temperature'
 */
void sign_temperature(uint8_t* unsigned_temperature);

/* printlower8bits:
 * Prints the lower eight bits of the temperature sensor
 * The passed parameter is to be the lower 8bits obtained from the DS1624.
 */
void printlower8Bits(uint8_t temperature);


/* getTemp:
 * Obtain the temperature from the DS1624 IC
 * and return the pointer to where it is stored
 */
int8_t* getTemp(void);

/* printTemperature:
 * Prints the temperature via the hardware UART.
 */
void printTemperature(uint8_t* temperature);



#endif
