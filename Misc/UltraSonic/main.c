
/* Name: main.c
 * Project: AVR USB driver for CDC interface on Low-Speed USB
 * Author: Osamu Tamura
 * Creation Date: 2006-05-12
 * Tabsize: 4
 * Copyright: (c) 2006 by Recursion Co., Ltd.
 * License: Proprietary, free under certain conditions. See Documentation.
 *
 * 2006-07-08   removed zero-sized receive block
 * 2006-07-08   adapted to higher baud rate by T.Kitazawa
 *
 */

#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>


#include "SoftTimer/Softtimer.h"
#include "UltraSonic.h"
#include "hardUart/hardUART.h"

int main(void)
{

	uint8_t i;

	DDRC |= (1<<4);
	PORTC |= (1<<4);

	DDRD &= ~(1<<3);
	PORTD |= (1 << 3);

	uartInit(BAUD(38400), 0 );

	UltraS_Init();
	SoftTimer_Init();

	UltraS_Outpulse();

	sei();


	while(1)
	{
		PORTC &= ~(1 << 4);
		
		UltraS_GetDistance();
	}

    while(1)
	{    /* main event loop */

		PORTC |= (1 << 4);

		for( i = 0 ; i <  200 ; i++)
		{
			_delay_ms(1);
		}

		PORTC &= ~(1 << 4);

		for( i = 0 ; i <  200 ; i++)
		{
			_delay_ms(1);
		}
    }

    return 0;
}

