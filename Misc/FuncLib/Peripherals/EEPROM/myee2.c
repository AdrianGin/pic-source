#include <avr/io.h>
#include <avr/iom8.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <avr/eeprom.h>

#include "..\UART\Tx.h"
#include "..\Pause\pause.h"

int main(void)
{
	uint8 i;
	uint8 j;
	uint8 OutputString[10];
	
	pausems(5000);	
	UartSetBaudRate( B2400, PD1, PD0 );

	EEAR = 0x01;
	

	i = 'C';

	EEDR = i;	

	/* Write the letter 'H' to address 0 of 511' */
	asm volatile ("sbi %2, %0\n\t"
		  			  "sbi %2, %1\n\t"
						:
						:"I" (EEMWE), "I" (EEWE), "I" (0x1C)
		  				);

//	utoa(EEDR, OutputString, 10);


	/* Wait until the EEPROM write has completed */
	while( (EECR & (1 << EEWE) ) == 1)
	{;
	
	}

	while(1)
	{	
	EECR |= (1<<EERE);
	pausems(10);
	utoa(EEDR, OutputString, 10);
	serout(OutputString);
	}



}

