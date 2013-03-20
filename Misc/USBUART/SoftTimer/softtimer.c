

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "USBUART/usbuart.h"
#include "Softtimer.h"
#include "USBUART/usbdrv/usbdrv.h"

/* Holds functions for the timers and is an example of implementation*/


/* Auxiliary Timers */
SoftTimer_8  SoftTimer1[TIMER0_COUNT] = { {100, 0, 0},  // Second Delay...
										  {255, 0, 0},   // SC_USB_UART_Tx
										  {1, 0, 1}};  //  SC_USB_UART_Poll	 };


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
		SoftTimerReset(SoftTimer1[SC_USB_UART_Tx]);
		USB_UART_Poll();
	}

	if( SoftTimerInterrupt(SoftTimer1[SC_USB_UART_Poll]) )
	{
		SoftTimerReset(SoftTimer1[SC_USB_UART_Poll]);
		usbPoll();

		/* We need to report rx and tx carrier after open attempt */
        if(intr3Status != 0 && usbInterruptIsReady3()){
            static uchar serialStateNotification[10] = {0xa1, 0x20, 0, 0, 0, 0, 2, 0, 3, 0};

            if(intr3Status == 2){
                usbSetInterrupt3(serialStateNotification, 8);
            }else{
                usbSetInterrupt3(serialStateNotification+8, 2);
            }
            intr3Status--;
        }
	}
}







