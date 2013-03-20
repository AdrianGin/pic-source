
/* Name: USB_UART;
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

#include "USBUART\usbdrv\oddebug.h"
#include "USBUART\usbdrv\usbdrv.h"
#include "USBUART\usbuart.h"
#include "SoftTimer\Softtimer.h"


PROGMEM char usbDescriptorConfiguration[] = {   /* USB configuration descriptor */
    9,          /* sizeof(usbDescrConfig): length of descriptor in bytes */
    USBDESCR_CONFIG,    /* descriptor type */
    (9 * 3) + sizeof(usbDescrCDC_HeaderFn) + sizeof(usbDescrCDC_AcmFn) + \
	sizeof(usbDescrCDC_CallMgtFn) + sizeof(usbDescrCDC_UnionFn) + (7 * 3) ,
    0,          /* total length of data returned (including inlined descriptors) */
    2,          /* number of interfaces in this configuration */
    1,          /* index of this configuration */
    0,          /* configuration name string index */
#if USB_CFG_IS_SELF_POWERED
    (1 << 7) | USBATTR_SELFPOWER,       /* attributes */
#else
    (1 << 7),                           /* attributes */
#endif
    USB_CFG_MAX_BUS_POWER/2,            /* max USB current in 2mA units */

    /* interface descriptor follows inline: */
    9,          /* sizeof(usbDescrInterface): length of descriptor in bytes */
    USBDESCR_INTERFACE, /* descriptor type */
    0,          /* index of this interface */
    0,          /* alternate setting for this interface */
    1,   /* endpoints excl 0: number of endpoint descriptors to follow */
    USB_CFG_INTERFACE_CLASS,
    USB_CFG_INTERFACE_SUBCLASS,
    USB_CFG_INTERFACE_PROTOCOL,
    0,          /* string index for interface */

    /* CDC Class-Specific descriptor */
    sizeof(usbDescrCDC_HeaderFn),   /* sizeof(usbDescrCDC_HeaderFn): length of descriptor in bytes */
    CDC_CS_INTERFACE,        /* descriptor type */
    HEADER_FUNCTION,           /* header functional descriptor */
    0x10, 0x01,            /* BCD USB VERSION */

    sizeof(usbDescrCDC_AcmFn),           /* sizeof(usbDescrCDC_AcmFn): length of descriptor in bytes */
    CDC_CS_INTERFACE,        /* descriptor type */
    ABSTRACT_CONTROL,        /* abstract control management functional descriptor */
    (1 << 1),                /* SET_LINE_CODING,    GET_LINE_CODING, SET_CONTROL_LINE_STATE    */


    sizeof(usbDescrCDC_CallMgtFn),           /* sizeof(usbDescrCDC_CallMgtFn): length of descriptor in bytes */
    CDC_CS_INTERFACE,        /* descriptor type */
    CALL_MANAGEMENT,           /* call management functional descriptor */
    0x03,           /* allow management on data interface, handles call management by itself */
    1,              /* CDC_DATA_INTF_ID */

    sizeof(usbDescrCDC_UnionFn),           /* sizeof(usbDescrCDC_UnionFn): length of descriptor in bytes */
    CDC_CS_INTERFACE,        /* descriptor type */
    UNION_FUNCTION,           /* union functional descriptor */
    0x00,           /* CDC_COMM_INTF_ID */
    0x01,           /* CDC_DATA_INTF_ID */

	/* Endpoint Descriptor */
    7,           /* sizeof(usbDescrEndpoint) */
    USBDESCR_ENDPOINT,  /* descriptor type = endpoint */
    0x80|USB_CFG_EP3_NUMBER,        /* IN endpoint number */
    0x03,        /* attrib: Interrupt endpoint */
    8, 0,        /* maximum packet size */
    USB_CFG_INTR_POLL_INTERVAL,        /* in ms */

    /* Interface Descriptor  */
    9,           /* sizeof(usbDescrInterface): length of descriptor in bytes */
    USBDESCR_INTERFACE,           /* descriptor type */
    1,           /* index of this interface */
    0,           /* alternate setting for this interface */
    2,           /* endpoints excl 0: number of endpoint descriptors to follow */
    0x0A,        /* Data Interface Class Codes */
    0,
    0,           /* Data Interface Class Protocol Codes */
    0,           /* string index for interface */

    /* Endpoint Descriptor */
    7,           /* sizeof(usbDescrEndpoint) */
    USBDESCR_ENDPOINT,  /* descriptor type = endpoint */
    0x01,        /* OUT endpoint number 1 */
    0x02,        /* attrib: Bulk endpoint */
    8, 0,        /* maximum packet size */
    0,           /* in ms */

    /* Endpoint Descriptor */
    7,           /* sizeof(usbDescrEndpoint) */
    USBDESCR_ENDPOINT,  /* descriptor type = endpoint */
    0x81,        /* IN endpoint number 1 */
    0x02,        /* attrib: Bulk endpoint */
    8, 0,        /* maximum packet size */
    0,           /* in ms */
};


/* UART buffer */
uint8_t USBUART_txbuffer[TX_SIZE+HW_CDC_BULK_IN_SIZE];
uint8_t USBUART_rxbuffer[RX_SIZE+HW_CDC_BULK_IN_SIZE];

uint8_t USBUART_txReadPtr;
uint8_t USBUART_rxReadPtr;

uint8_t USBUART_txWritePtr;
uint8_t USBUART_rxWritePtr;

uint8_t sendEmptyFrame = 0;


/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */
uchar usbFunctionDescriptor(usbRequest_t *rq)
{

    if(rq->wValue.bytes[1] == USBDESCR_DEVICE){
        usbMsgPtr = (uchar *)usbDescriptorDevice;
        return usbDescriptorDevice[0];
    }else{  /* must be config descriptor */
        usbMsgPtr = (uchar *)usbDescriptorConfiguration;
        return sizeof(usbDescriptorConfiguration);
    }
}


uint8_t USB_UART_GetByte(void)
{
	uint8_t returnValue;

	if( USBUART_rxWritePtr != USBUART_rxReadPtr)
	{
		returnValue = USBUART_rxbuffer[USBUART_rxReadPtr];
		USBUART_rxReadPtr = (USBUART_rxReadPtr + 1) & RX_MASK;
	}

	return returnValue;
}

/* Puts one byte onto the transmit buffer */
void USB_UART_Tx(uint8_t data)
{
	
	while( ((USBUART_txWritePtr+1)&TX_MASK) == USBUART_txReadPtr)
	{
		usbPoll();
		USB_UART_Poll();
		PORTC |= (1 << 5);
	}

	if( ((USBUART_txWritePtr+1)&TX_MASK) != USBUART_txReadPtr)
	{	
		PORTC &= ~(1 << 5);
		USBUART_txbuffer[USBUART_txWritePtr] = data;
		USBUART_txWritePtr = (USBUART_txWritePtr + 1) & TX_MASK;
	}
	

	/*  If the Transmit buffer is not ready, disable comms*/
    if( uartTxBytesFree()<=HW_CDC_BULK_OUT_SIZE )
	{
        //usbDisableAllRequests();
	}
}

void USB_UART_TxString(uint8_t* data)
{
	while( *data )
	{
		USB_UART_Tx(*data++);
	}
}

uint8_t        intr3Status;    /* used to control interrupt endpoint transmissions */
static uint8_t        stopbit, parity, databit;
static usbDWord_t   baud;

uchar usbFunctionSetup(uchar data[8])
{
	usbRequest_t    *rq = (void *)data;

    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* class request type */

        if( rq->bRequest==GET_LINE_CODING || rq->bRequest==SET_LINE_CODING ){
            return 0xFF;
        /*    GET_LINE_CODING -> usbFunctionRead()    */
        /*    SET_LINE_CODING -> usbFunctionWrite()    */
        }


		if(rq->bRequest == SET_CONTROL_LINE_STATE){
        	if( intr3Status==0 )
            	intr3Status = 2;
		}

        /*  Prepare bulk-in endpoint to respond to early termination   */
        if((rq->bmRequestType & USBRQ_DIR_MASK) == USBRQ_DIR_HOST_TO_DEVICE)
            sendEmptyFrame  = 1;

    }
    return 0;
}


uchar usbFunctionRead( uchar *data, uchar len )
{

    /*    USART configuration    */
    baud.dword  = 9600;
    stopbit = 0;
    parity  = 0;
    databit = 8;

    data[0] = baud.bytes[0];
    data[1] = baud.bytes[1];
    data[2] = baud.bytes[2];
    data[3] = baud.bytes[3];
    data[4] = stopbit;
    data[5] = parity;
    data[6] = databit;

    return 7;
}


/*---------------------------------------------------------------------------*/
/* usbFunctionWrite                                                          */
/*---------------------------------------------------------------------------*/

uchar usbFunctionWrite( uchar *data, uchar len )
{

    /*    SET_LINE_CODING    */
    baud.bytes[0] = data[0];
    baud.bytes[1] = data[1];
    baud.bytes[2] = data[2];
    baud.bytes[3] = data[3];

    stopbit    = data[4];
    parity     = data[5];
    databit    = data[6];

    if( parity>2 )
        parity    = 0;
    if( stopbit==1 )
        stopbit    = 0;

//    resetUart();

    return 1;
}




/* Receiving Data from USB, use this as the UART Interrupt Routine */
void usbFunctionWriteOut( uchar *data, uchar len )
{
    /*  usb -> rs232c:  transmit char    */
    while( len--) 
	{
        USBUART_rxbuffer[USBUART_rxWritePtr] = *data++;
		USBUART_rxWritePtr = (USBUART_rxWritePtr + 1) & RX_MASK;
		//USB_UART_Tx(USB_UART_GetByte());
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
		
		if( (USBUART_txWritePtr == USBUART_txReadPtr) && (bytesRead == HW_CDC_BULK_IN_SIZE) )
		{
			sendEmptyFrame = 1;
		}
		else
		{
			sendEmptyFrame = 0;
		}

        USBUART_txReadPtr   = next;
    }
}



uint8_t uartRxBytesFree(void)
{
    return (USBUART_rxWritePtr - USBUART_rxReadPtr - 1) & RX_MASK;
}

uint8_t uartTxBytesFree(void)
{
    return (USBUART_txWritePtr - USBUART_txReadPtr - 1) & TX_MASK;
}



