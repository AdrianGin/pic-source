#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "USBUART\usbdrv\oddebug.h"
#include "USBUART\usbdrv\usbdrv.h"
#include "USBUART\usbuart.h"
#include "SoftTimer\Softtimer.h"

int main(void)
{
    usbInit();
	usbEnableAllRequests();
	SoftTimer_Init();

	DDRC |= (1 << 5);




    sei();

    for(;;){  

		uint8_t rcvByte;
		//
		//USB_UART_Tx('A');
      usbPoll();
      USB_UART_Poll();


        if(intr3Status != 0 && usbInterruptIsReady3()){
            static uchar serialStateNotification[10] = {0xa1, 0x20, 0, 0, 0, 0, 2, 0, 3, 0};

            if(intr3Status == 2){
                usbSetInterrupt3(serialStateNotification, 8);
            }else{
                usbSetInterrupt3(serialStateNotification+8, 2);
            }
            intr3Status--;
        }

		rcvByte = USB_UART_GetByte();
		if( rcvByte == 'Z' )
		{
			USB_UART_TxString("Hey!");
			PORTC |= (1 << 5);
		}

		if( rcvByte == 'B' )
		{
			USB_UART_TxString("You pressed B");
			PORTC &= ~(1 << 5);
		}






    }





    return 0;
}
