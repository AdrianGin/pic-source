/* Name: uart.c
 * Project: AVR USB driver for CDC interface on Low-Speed USB
 * Author: Osamu Tamura
 * Creation Date: 2006-06-18
 * Tabsize: 4
 * Copyright: (c) 2006 by Recursion Co., Ltd.
 * License: Proprietary, free under certain conditions. See Documentation.
 *
 * 2006-07-08   adapted to higher baudrate by T.Kitazawa
 */
/*
General Description:
    This module implements the UART rx/tx system of the USB-CDC driver.
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>   /* needed by usbdrv.h */
#include "oddebug.h"
#include "usbdrv.h"
#include "uart.h"


/* UART buffer */
uint8_t USBUART_txbuffer[TX_SIZE];
uint8_t USBUART_rxbuffer[RX_SIZE+HW_CDC_BULK_IN_SIZE];
uint8_t USBUART_writePtr;
uint8_t USBUART_readPtr;

void uartPoll(void)
{
	uchar		next;


	/*  postpone receiving next data    */
    if( usbAllRequestsAreDisabled() && uartRxBytesFree() > HW_CDC_BULK_IN_SIZE )
	{
        usbEnableAllRequests();
	}

	/*  USB <= device  */
    if( usbInterruptIsReady() && (USBUART_writePtr!=USBUART_readPtr) ) {
        uchar   bytesRead, i;
		
        bytesRead = (USBUART_writePtr-USBUART_readPtr) & RX_MASK;
        if(bytesRead>=HW_CDC_BULK_IN_SIZE-1)
		{
            bytesRead = HW_CDC_BULK_IN_SIZE-1;
		}
		next	= USBUART_readPtr + bytesRead;
		if( next>=RX_SIZE ) {
			next &= RX_MASK;
			for( i=0; i<next; i++ )
				USBUART_rxbuffer[RX_SIZE+i]	= USBUART_rxbuffer[i];
		}

		usbSetInterrupt(USBUART_rxbuffer+USBUART_readPtr, bytesRead);

        USBUART_readPtr   = next;
    }
}


