/* Filename: 				serial_ee.c
 * Author:   				Adrian Gin (amg94@student.canterbury.ac.nz)
 * Created/Modified: 		26-06-06
 *
 * Description:
 *		Functions used to interface with the 24XX(XXX) Serial EEPROM IC.
 *		These functions utilise the hardware TWI and UART registers
 * 	I2C / TWI serial routines for the AVR mpu's.
 * 	For AVR Core
 */
 

#include <avr/io.h>

#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>

#include "I2C/i2c.h"
#include "hardUart/hardUart.h"
#include "serial_ee/serial_ee.h"
//#include "Pause/pause.h"


#define	DEBUG 0

static uint16_t	EE_EndBlockAddress[EE_NUMBER_OF_BLOCKS] = {0};
static uint8_t		OutputString[5];

/* printEEPointer:
 * Prints the location of the current position of the EEPROM Pointer
 */
#if DEBUG 
void printEEPointer(EE_AddressStruct* EE_AddressData)
{
	uartTxString("\n\rCurrent location of the EEPROM Pointer: \n\rBlock: ");
	uartTx( (EE_AddressData->EE_Block) + '0' );
	
	utoa( (EE_AddressData->EE_Address / EE_PAGE_SIZE), OutputString, 10);
	uartTxString("\n\rPage: ");
	uartTxString(OutputString);
	
	utoa( EE_AddressData->EE_Address, OutputString, 10);
	uartTxString("\n\rAddress: ");
	uartTxString(OutputString);
}
#endif


#if EE_HAS_BLOCKS == 1


/* serialEE_HandleBlockOverflow:
 *	Check whether the current block is going to overflow when 
 *	writing data to it. And return the to be selected EEPROM Block.
 *
 * If it is, return the block number which is next in line.
 */
 
/* This function is only needed if the EEPROM is partitioned into blocks
 * ie. the 24XX16 and 24XX1025.
 */


void serialEE_HandleBlockOverflow( EE_AddressStruct* EE_AddressData,
											  uint8_t datasize)
{
	
	/* Check that the current memory block is not full
	 * (For 24XX16 & 24XX1025 devices only!)
	 */
	if( ((EE_AddressData->EE_Address) + datasize) > EE_BLOCK_SIZE )
	{		
		/* Record the last entry of the block */	
		EE_EndBlockAddress[(EE_AddressData->EE_Block)] = EE_AddressData->EE_Address;
		/* Select the next block */
		EE_AddressData->EE_Block++;


		if( EE_AddressData->EE_Block >= EE_NUMBER_OF_BLOCKS)
		{
		/*ALL MEMORY BLOCKS HAVE BEEN USED UP!*/
		/* Handle event here: */	
			uartTxString("\n\rWarning : All memory blocks have been used up!");
			EE_AddressData->EE_Block = (EE_NUMBER_OF_BLOCKS - 1);
			
		}
		
		/* Reset Block address pointer */
		EE_AddressData->EE_Address = 0x00;
		
		
		
		/* Print which block is currently selected */				
		uartTxString("\n\rNow writing to memory block:");
		uartTx( (EE_AddressData->EE_Block) + '0');
		uartTxString("\n\r");
			
	}

	
	
}


#endif

/* serialEE_WriteBlock:
 * Send 'datasize' bytes from the array pointed to by 'data'
 * to the block_address and block of the EEPROM
 */
void serialEE_WriteBlock(uint8_t* data,
								 uint8_t	 datasize,  
								 EE_AddressStruct* EE_AddressData)
{
		uint8_t	pageOverflow;
		uint8_t 	overflow;
		uint8_t 	newpage = 1;
		uint8_t 	j;
		uint8_t	i;
				
		/* If writing will overflow a page, then write data until
		 * the end of the page.
		 * Stop the I2C, and then resume writing onto the next page.
		 */
		 
		/* 18/04/2007, and keep writing for to the next page until all data is written */ 
		overflow = ((EE_AddressData->EE_Address) % EE_PAGE_SIZE) + datasize;
		pageOverflow = overflow / EE_PAGE_SIZE;
			for( j = 0, i = 0; j <= pageOverflow; j++)
				{
					/* Address the next page and write the remaining bytes*/
					i2cAddress( (EE_I2C_ADDRESS) 
									| (EE_BLOCKSELECT(EE_AddressData->EE_Block)) 
									, TW_WRITE );
									
#if	EE_WORD_ADDRESS == 1
					i2cTransmit( (uint16_t)((EE_AddressData->EE_Address)+ i) >> 8);
#endif			
					i2cTransmit(((EE_AddressData->EE_Address)+ i)& 0xFF);				

					
									
					for( ; 
						  ((((i + ((EE_AddressData->EE_Address) % EE_PAGE_SIZE)) % (EE_PAGE_SIZE)) != 0)
						   || newpage == 1)
							&& (i < datasize); 
							i++)
					{
						newpage = 0;				
						i2cTransmit(data[i]);
					}					
					i2cStop();
					newpage = 1;
					_delay_ms(5);

				}


}


/* serialEE_getEndBlock:*/
/* Returns the last memory entry for the passed block */
uint16_t serialEE_getEndBlock(uint8_t Block )
{
	
#if	DEBUG == 1	
	
	uartTxString("\n\rThe end of Block ");
	
	utoa(Block , OutputString, 10);
	
	uartTxString(OutputString);
	uartTxString(" is.. : ");
	utoa(EE_EndBlockAddress[Block] , OutputString, 10);
	
	uartTxString(OutputString);

#endif	
	
	
	return EE_EndBlockAddress[Block];
}


/* serialEE_setEndAddress
 * Sets the end address for the passed block */
void serialEE_setEndAddress(uint8_t Block, uint16_t Address)
{
	EE_EndBlockAddress[Block] = Address;
}


/* serialEE_ReadBlock:
 * Read 'datasize' bytes to the array pointed to by 'data'
 * from the block_address and block of the EEPROM
 */
void serialEE_ReadBlock(uint8_t* data,
								 uint8_t	 datasize,  
								 EE_AddressStruct* EE_AddressData)
{
	
		/* Begin reading from the given address and block */
		i2cAddress( (EE_I2C_ADDRESS) 
						| (EE_BLOCKSELECT(EE_AddressData->EE_Block))
						, TW_WRITE );
#if	EE_WORD_ADDRESS == 1
		i2cTransmit((uint16_t)(EE_AddressData->EE_Address) >> 8);
#endif							
		i2cTransmit( (EE_AddressData->EE_Address) & 0xFF );

	
		/* Begin reading */
		i2cAddress( (EE_I2C_ADDRESS) 
						| (EE_BLOCKSELECT(EE_AddressData->EE_Block))
						, TW_READ );
		i2cReadBlock(data , datasize);
		i2cStop();
	
	
}

#if DEBUG
/* serialEE_PrintBlock:
 * Prints out the block data pointed by EE_AddressData.
 */
void serialEE_PrintBlock(uint8_t block, uint8_t datasize)
{
	uint8_t buffer[80];
	
	EE_AddressStruct EE_Current;
	EE_Current.EE_Address = 0x00;
	EE_Current.EE_Block = block;	
	
	uartTxString("\n\rNow reading from memory block:");
	uartTx( (EE_Current.EE_Block) + '0');
	uartTxString("\n\r");
	
	/* Read until the last entry of the current block */
	for(  EE_Current.EE_Address = EE_START_ADDRESS; 
			EE_Current.EE_Address < serialEE_getEndBlock(EE_Current.EE_Block); 
			EE_Current.EE_Address += datasize)
	{
						
		serialEE_ReadBlock(buffer, 
								datasize, 
								&EE_Current);			
								
		/* Parse the relevant information*/
		uartTxString("\r\n");						
		uartTxString(buffer);
	}
	
}



#endif


