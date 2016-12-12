/*

Copyright (c) 2016 Adrian Gin (adrian.gin[at]gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/


#include <avr/pgmspace.h>
#include "AVRUSARTn.h"
#include "AVRGPIO.h"
#include "AVRSPI.h"
#include "AVRTWI.h"
#include "AVRTIMER.h"
#include "AVRTIMER2.h"
#include "AVRPWM.h"
#include "AVRPCINT.h"


#include <stdlib.h>
#include <string.h>

#include "hardwareSpecific.h"
#include "hw_config.h"


#define DEBUG_LED_DDR (DDRB)
#define DEBUG_LED (PORTB)
#define DEBUG_LED_PIN (1<<0)
#define INPUT_SWITCH (PINC)
#define INPUT_SWITCH_PIN (1<<2)
#define INPUT_SWITCH_PIN2 (1<<3)


AVR::PCINT SoftUART_PCINT = AVR::PCINT(PCMSK2, PCIE2, PCINT18);

AVR::GPIO SoftUART_GPIO = AVR::GPIO(DDRD, PORTD, PIND, 2, &SoftUART_PCINT);

AVR::GPIO DebugLED = AVR::GPIO(DDRB, PORTB, PINB, 0);

AVR::TIMER2 TIM2 = AVR::TIMER2();

volatile uint8_t rxChar = 0;
volatile uint8_t rxFlag = 0;
volatile uint8_t M1Speed;

void testme(void* context)
{
   USART0.tx( PSTR("IntCallback\n"));
   return;
}

int main(void)
{
	char outputString[20];

	clock_prescale_set(clock_div_1);

	DebugLED.Init( Devices::GPIO::OUTPUT );
	DebugLED.SetOutput( Devices::GPIO::HIGH );

	USART0.Init(BAUD19200);

   SoftUART_GPIO.Init(Devices::GPIO::INPUT);

   SoftUART_GPIO.SetOutput( Devices::GPIO::HIGH ); //Enable Pullup
   SoftUART_GPIO.EnableInterrupt( (Devices::GPIO::IntCallback)&testme, 0);


	/*Enable interrupts*/
	sei();

	_delay_ms(100);

	uint8_t speed;

	while(1)
	{
	   DebugLED.SetOutput( Devices::GPIO::HIGH );
	   _delay_ms(1000);
	   DebugLED.SetOutput( Devices::GPIO::LOW );
	   _delay_ms(1000);
	   USART0.tx( PSTR("hello\n"));

	}

	return 0;
}



ISR(USART_RX_vect)
{
	rxChar = UDR0;
	rxFlag = 1;
	//uartTxString("Rcvd its working");
	USART0.tx(rxChar);

	if( rxChar == '+' )
	{
		M1Speed++;
	}

}


ISR(PCINT0_vect)
{
   USART0.tx( PSTR("PC0\n"));
}


ISR(PCINT1_vect)
{
   USART0.tx( PSTR("PC1\n"));
}


ISR(PCINT2_vect)
{
   USART0.tx( PSTR("PC2\n"));
   if( SoftUART_GPIO.IsInterruptTriggered() )
   {
      SoftUART_GPIO.callback( SoftUART_GPIO.context );
   }
}
