

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "USBUART/usbuart.h"
#include "Softtimer.h"
#include "USBUART/usbdrv/usbdrv.h"

/* Holds functions for the timers and is an example of implementation*/


/* Auxiliary Timers */
SoftTimer_8  SoftTimer1[TIMER0_COUNT] = { {100, 0, 0},  // Second Delay...
										  {20, 0, 1},   // SC_USB_UART_Tx
										  {10, 0, 1}};  // Retrigger Reset	 }; 


void SoftTimer_Init(void)
{
   
   /* 1024 prescaler */
   TCCR0 = 0x03;
   
   /* Enable Overflow Interrupt */
   TIMSK |= (1 << TOIE0);
      
}


ISR(TIMER0_OVF_vect)
{

	if( SoftTimerInterrupt(SoftTimer1[SC_USB_UART_Tx]) )
	{
		USB_UART_Poll();
	}

}







