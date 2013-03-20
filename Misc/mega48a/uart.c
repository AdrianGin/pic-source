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

extern uchar    sendEmptyFrame;

/* UART buffer */
uchar    urptr, uwptr, irptr, iwptr;
uchar    rx_buf[RX_SIZE+HW_CDC_BULK_IN_SIZE], tx_buf[TX_SIZE];


uint8_t USBUART_txbuffer[TX_SIZE+HW_CDC_BULK_IN_SIZE];
uint8_t USBUART_rxbuffer[RX_SIZE+HW_CDC_BULK_IN_SIZE];

uint8_t USBUART_txReadPtr;
uint8_t USBUART_rxReadPtr;

uint8_t USBUART_txWritePtr;
uint8_t USBUART_rxWritePtr;

uint8_t sendEmptyFrame = 0;



uint8_t USB_UART_GetByte(void)
{
	uint8_t returnValue;

	if( iwptr != urptr)
	{
		returnValue = rx_buf[urptr];
		urptr = (urptr + 1) & RX_MASK;

		USB_UART_Tx(returnValue);
	}

	return returnValue;
}

/* Puts one byte onto the transmit buffer */
void USB_UART_Tx(uint8_t data)
{
	
	/*while( ((USBUART_txWritePtr+1)&TX_MASK) == USBUART_txReadPtr)
	{
		//usbPoll();
		//USB_UART_Poll();
	}*/

	if( ((USBUART_txWritePtr+1)&TX_MASK) != USBUART_txReadPtr)
	{	
		USBUART_txbuffer[USBUART_txWritePtr] = data;
		USBUART_txWritePtr = (USBUART_txWritePtr + 1) & TX_MASK;
	}
	

	/*  If the Transmit buffer is not ready, disable comms*/
    if( uartTxBytesFree()<=HW_CDC_BULK_OUT_SIZE )
	{
        //usbDisableAllRequests();
	}
}


/* This should be a timer call to 'poll' whether there is any data to be sent */
void USB_UART_Poll(void)
{
	uchar		next;


	/*  If the TxBuffer is ready to accept data again, enable it  */
    if( usbAllRequestsAreDisabled() && uartTxBytesFree() > HW_CDC_BULK_IN_SIZE )
	{
        usbEnableAllRequests();
	}

	/*  USB <= device  */
    if( usbInterruptIsReady() && (USBUART_txWritePtr!=USBUART_txReadPtr || sendEmptyFrame) ) {
        uchar   bytesRead, i;
		
        bytesRead = (USBUART_txWritePtr-USBUART_txReadPtr) & TX_MASK;
        if(bytesRead>=HW_CDC_BULK_IN_SIZE)
		{
            bytesRead = HW_CDC_BULK_IN_SIZE;
		}
		next	= USBUART_txReadPtr + bytesRead;
		if( next>=TX_SIZE ) {
			next &= TX_MASK;
			for( i=0; i<next; i++ )
				USBUART_txbuffer[TX_SIZE+i]	= USBUART_txbuffer[i];
		}

		usbSetInterrupt(USBUART_txbuffer+USBUART_txReadPtr, bytesRead);
		USBUART_txReadPtr   = next;		

		if( (USBUART_txWritePtr == USBUART_txReadPtr) && (bytesRead == HW_CDC_BULK_IN_SIZE) )
		{
			sendEmptyFrame = 1;
		}
		else
		{
			sendEmptyFrame = 0;
		}

        
    }
}


uint8_t uartTxBytesFree(void)
{
    return (USBUART_txReadPtr - USBUART_txWritePtr - 1) & TX_MASK;
}
