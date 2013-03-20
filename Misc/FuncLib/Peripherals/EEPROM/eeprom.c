#include <avr/io.h>
#include <avr/iom8.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/eeprom.h>

#include "..\UART\Tx.h"
#include "..\Pause\pause.h"

#define MAX_STR_LEN 55

typedef struct
{
	size_t length;
	uint16* address;
	uint8  string[MAX_STR_LEN];
} message;

int main(void)
{
	message OutMessage;
//	uint8 OutputString[MAX_STR_LEN];

	pausems(5000);	
	UartSetBaudRate( B2400, PD1, PD0 );

	strcpy(OutMessage.string, "Yo Wassup, Adrian in da House!\r\n");

   /* Need to +1 to include the NULL character */
 
//	OutMessage.length = strlen(OutMessage.string) + 1;
//	OutMessage.address = 0x00;


//   eeprom_write_block(OutMessage.string, OutMessage.address, OutMessage.length);

	while(1)
	{	
//	eeprom_read_block(OutputString, OutMessage.address, OutMessage.length);
	serout(OutMessage.string);
	pausems(200);

	}



}

