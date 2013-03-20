

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "Softtimer.h"
#include "UltraSonic.h"
#include "hardUart/hardUart.h"

/* Holds functions for the timers and is an example of implementation*/


/* Auxiliary Timers */
SoftTimer_16  SoftTimer1[TIMER0_COUNT] = { {3, 0, 1},  // Second Delay...
										  {200, 0, 0},   // SC_USB_UART_Tx
										  {200, 0, 1}};  //  SC_USB_UART_Poll	 };


void SoftTimer_Init(void)
{
   
   /* 1024 prescaler */
   TCCR0 = 0x05;
   
   /* Enable Overflow Interrupt */
   TIMSK |= (1 << TOIE0);
      
}


ISR(TIMER0_OVF_vect)
{
	if( SoftTimerInterrupt(SoftTimer1[SC_SecondDelay]) )
	{
		SoftTimerReset(SoftTimer1[SC_SecondDelay]);
		//PORTC ^= (1 << 4);
		//UltraS_GetDistance();
	}
	

	if( SoftTimerInterrupt(SoftTimer1[SC_USB_UART_Tx]) )
	{
		SoftTimerReset(SoftTimer1[SC_USB_UART_Tx]);
		
		//uartTx('A');
	}

	if( SoftTimerInterrupt(SoftTimer1[SC_USB_UART_Poll]) )
	{
		SoftTimerReset(SoftTimer1[SC_USB_UART_Poll]);
		UltraS_GetAverage();
	}
}







