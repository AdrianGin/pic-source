
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

#include "oddebug.h"
#include "usbdrv.h"
#include "uart.h"


PROGMEM char usbDescriptorConfiguration[] = {   /* USB configuration descriptor */
    9,          /* sizeof(usbDescrConfig): length of descriptor in bytes */
    USBDESCR_CONFIG,    /* descriptor type */
    67,
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
    USB_CFG_HAVE_INTRIN_ENDPOINT,   /* endpoints excl 0: number of endpoint descriptors to follow */
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


static uchar        stopbit, parity, databit;
static usbDWord_t   baud;


/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

uchar usbFunctionSetup(uchar data[8])
{
usbRequest_t    *rq = (void *)data;

    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* class request type */

        if( rq->bRequest==GET_LINE_CODING || rq->bRequest==SET_LINE_CODING ){
            return 0xff;
        /*    GET_LINE_CODING -> usbFunctionRead()    */
        /*    SET_LINE_CODING -> usbFunctionWrite()    */
        }

    }

    return 0;
}


/*---------------------------------------------------------------------------*/
/* usbFunctionRead                                                          */
/*---------------------------------------------------------------------------*/

uchar usbFunctionRead( uchar *data, uchar len )
{

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

    return 1;
}


void usbFunctionWriteOut( uchar *data, uchar len )
{

    /*  usb -> rs232c:  transmit char    */
    while( len--) {
        uchar   uwnxt;

        uwnxt = (USBUART_writePtr+1) & RX_MASK;
        if( uwnxt!=USBUART_readPtr ) {
            USBUART_rxbuffer[USBUART_writePtr] = *data++;
            USBUART_writePtr = (USBUART_writePtr + 1) & RX_MASK;
        }
    }

    /*  postpone receiving next data    */
    if( uartRxBytesFree()<=HW_CDC_BULK_OUT_SIZE )
	{
        usbDisableAllRequests();
	}
}



int main(void)
{

    odDebugInit();
    //hardwareInit();
    usbInit();


    sei();

    for(;;){    /* main event loop */
        usbPoll();

        uartPoll();
    }

    return 0;
}

