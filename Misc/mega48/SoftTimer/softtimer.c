

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "Softtimer.h"
#include "usbdrv.h"
#include "uart.h"

/* Holds functions for the timers and is an example of implementation*/


/* Auxiliary Timers */
SoftTimer_16  SoftTimer1[TIMER0_COUNT] = { {6, 0, 0},  // Second Delay...
										  {5, 0, 0},   // SC_USB_UART_Tx
										  {1, 0, 0}};  //  SC_USB_UART_Poll	 };


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
		USB_UART_TxString("Heya WHAT?");
	}
	

	if( SoftTimerInterrupt(SoftTimer1[SC_USB_UART_Tx]) )
	{
		SoftTimerReset(SoftTimer1[SC_USB_UART_Tx]);
		uartPoll();
	}

	if( SoftTimerInterrupt(SoftTimer1[SC_USB_UART_Poll]) )
	{
		SoftTimerReset(SoftTimer1[SC_USB_UART_Poll]);
		
	}
}







