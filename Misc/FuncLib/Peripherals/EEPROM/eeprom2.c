#include <avr/io.h>
#include <avr/iom8.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "..\UART\Tx.h"
#include "..\Pause\pause.h"

int main(void)
{
	uint8 i;
	uint8 j;
	uint8 OutputString[10];
	
	/* Write the letter 'H' to address 0 of 511' */
	EEAR = 0x01;
	EEDR = 'H';

	while( (EECR & (1 << EEWE) ) == 1)
	{;
	
	}

//	/* Enable Master write of the EEPROM*/
//	EECR = (EECR & (~(1 << EEWE)) ) | (1  << EEMWE);
//	
//	/* Perform a write to the eeprom */
//	EECR |= (1 << EEWE);


	/* Wait until the EEPROM write has completed */
	while( (EECR & (1 << EEWE) ) == 1)
	{;
	
	}

	pausems(100);

	EEAR = 0x01;
	EECR |= (1 << EERE);

	i = EEDR;


	utoa(i, OutputString, 10);

	UartSetBaudRate( B2400, PD1, PD0 );
	j = EEAR;

	while(1)
	{
	serout(OutputString);
	}

}

