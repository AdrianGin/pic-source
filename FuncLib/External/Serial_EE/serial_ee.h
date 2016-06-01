/* Filename: 				serial_ee.h
 * Author:   				Adrian Gin (amg94@student.canterbury.ac.nz)
 * Created/Modified: 		26-06-07
 *
 * Description:
 *		Functions used to interface with the 24XX(XXX) Serial EEPROM IC.
 *		These functions utilise the hardware TWI and UART registers
 * 	I2C / TWI serial routines for the AVR mpu's.
 * 	For AVR Core
 */


/** \file serial_ee.h
	 \brief I2C Serial EEPROM Drivers
*/

/**	 

	\ingroup avr_external
 	\defgroup serial_ee I2C Serial EEPROM Drivers
 	\code #include "serial_ee.h" \endcode

 	 
	 \par Description
	Written mainly for Microchip's line of Serial EEPROMs,
	this library maybe used to perform read and write operations.
 	 
*/
//@{
 
 
 
#ifndef	_SERIAL_EEPROM
#define	_SERIAL_EEPROM

/* Each block contains 256 bytes */
/* There are 8 blocks making a total of 2048 bytes on the 24XX16*/
#define	EE_I2C_ADDRESS			(0xA0)
#define	EE_I2C_BLOCK_MASK		(0x0E)
#define	EE24AA16_BLOCK(x)		(x<<1)
#define	EE24AA256_BLOCK(x)	(0)
#define	EE24AA512_BLOCK(x)	(0)
#define	EE24AA1025_BLOCK(x)	(x<<3)

/* The size of each block in bytes */
#define	EE24AA16_BLOCK_SIZE		((0xFF)+1)
#define	EE24AA256_BLOCK_SIZE		((0x1FFF)+1)
#define	EE24AA512_BLOCK_SIZE		((0xFFFF)+1)
#define	EE24AA1025_BLOCK_SIZE	((0xFFFF)+1)


/** The size of each page in the EEPROM (in bytes) */
#define  EE24AA16_PAGE_SIZE				(16)
#define  EE24AA256_PAGE_SIZE				(64)
#define  EE24AA512_PAGE_SIZE				(128)
#define  EE24AA1025_PAGE_SIZE				(128)

#define	EE24AA16_NUMBER_OF_BLOCKS		(8)
#define	EE24AA256_NUMBER_OF_BLOCKS		(1)
#define	EE24AA512_NUMBER_OF_BLOCKS		(1)
#define	EE24AA1025_NUMBER_OF_BLOCKS	(2)

#define  EE_ADDRESS	(0)
#define	EE_BLOCK		(1)


/** The size of each block in bytes */

#define	EE_HAS_BLOCKS				0

/** Define EE_WORD_ADDRESS if the IC has a word address, some only have a byte
	addresss */
#define	EE_WORD_ADDRESS			1

#define	EE_BLOCK_SIZE				(EE24AA512_BLOCK_SIZE)
#define	EE_NUMBER_OF_BLOCKS	 	(EE24AA512_NUMBER_OF_BLOCKS)
#define	EE_PAGE_SIZE				(EE24AA512_PAGE_SIZE)
#define	EE_BLOCKSELECT(x)			(EE24AA512_BLOCK(x))

#define	EE_START_ADDRESS		(0x00)
#define  EE_START_BLOCK			(0x00)

/** Memory Address Structure*/
typedef struct 
{
	uint16_t	EE_Address;
	uint8_t	EE_Block;	
} EE_AddressStruct;



/** printEEPointer:
 * Prints the current addresss and block/chip_select of the I2C EEPROM to the 
 * UART */
void printEEPointer(EE_AddressStruct* EE_AddressData);



/** serialEE_HandleBlockOverflow:
 *	Check whether the current block is going to overflow when 
 *	writing data to it. And return the to be selected EEPROM Block.
 *
 * If it is, return the block number which is next in line.
 */
 
/** This function is only needed if the EEPROM is partitioned into blocks
 * ie. the 24XX16 and 24XX1025.
 */


void	serialEE_HandleBlockOverflow( EE_AddressStruct* EE_AddressData,
												uint8_t datasize);


/** serialEE_WriteBlock:
 * Send 'datasize' bytes from the array pointed to by 'data'
 * to the address and block of the EEPROM
 */
void serialEE_WriteBlock(uint8_t* data,
								 uint8_t	 datasize,  
								 EE_AddressStruct*	EE_AddressData);




/** serialEE_ReadBlock:
 * Read 'datasize' bytes to the array pointed to by 'data'
 * from the block_address and block of the EEPROM
 */
void serialEE_ReadBlock(uint8_t* data,
								 uint8_t	 datasize,  
								 EE_AddressStruct* EE_AddressData);



/** serialEE_setEndAddress
 * Sets the end address for the passed block */
void serialEE_setEndAddress(uint8_t Block, uint16_t Address);


/** serialEE_getEndBlock:*/
/** Returns the last memory entry for the passed block */
uint16_t serialEE_getEndBlock(uint8_t Block );

/** serialEE_PrintBlock:
 * Prints out the block data pointed by EE_AddressData.
 */
void serialEE_PrintBlock(uint8_t block, uint8_t datasize);










#endif
 
