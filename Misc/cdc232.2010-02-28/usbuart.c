/* Name: USBUART.c
 * Project: AVR USB driver for CDC interface on Low-Speed USB
 * Author: Adrian Gin
 * Creation Date: 18/05/2010
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
#include "usbuart.h"

#include "SoftTimer/Softtimer.h"

uint8_t USBUART_txbuffer[TX_SIZE];
uint8_t USBUART_writePtr = 0;
uint8_t USBUART_readPtr = 0;

uint8_t USBUART_rxbuffer[RX_SIZE+HW_CDC_BULK_IN_SIZE];


PROGMEM char usbDescriptorConfiguration[] = {   /* USB configuration descriptor */
    9,          /* sizeof(usbDescrConfig): length of descriptor in bytes */
    USBDESCR_CONFIG,    /* descriptor type */
    9 + (9 * 2) + (7 * (1+USB_CFG_HAVE_INTRIN_ENDPOINT + USB_CFG_HAVE_INTRIN_ENDPOINT3)) +
    sizeof(usbDescrCDC_HeaderFn) + sizeof(usbDescrCDC_AcmFn) +
    sizeof(usbDescrCDC_UnionFn) + sizeof(usbDescrCDC_CallMgtFn)
    , 0,        /* total length of data returned (including inlined descriptors) */          
    2,          /* number of interfaces in this configuration , there are two see AN1247
                   communications and data interface */
    1,          /* index of this configuration */
    0,          /* configuration name string index */
    (1 << 7) | USBATTR_SELFPOWER,       /* attributes */
    USB_CFG_MAX_BUS_POWER/2,            /* max USB current in 2mA units */

    /* COMMUNICATIONS INTERFACE */
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

    /* DATA INTERFACE */
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




uint8_t               sendEmptyFrame;
static uint8_t        intr3Status;    /* used to control interrupt endpoint transmissions */
static usbDWord_t     baud;
static uint8_t        stopbit, parity, databit;

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
        
        
           /*  Prepare bulk-in endpoint to respond to early termination   */
         //if((rq->bmRequestType & USBRQ_DIR_MASK) == USBRQ_DIR_HOST_TO_DEVICE)
           // sendEmptyFrame  = 1;
        
        
    }
    return 0;
}


/*---------------------------------------------------------------------------*/
/* usbFunctionRead                                                          */
/*---------------------------------------------------------------------------*/

uchar usbFunctionRead( uchar *data, uchar len )
{

    /* GET_LINE_CODING */
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

    /*    SET_LINE_CODING (UNUSED)   */
    
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

/* Sends a character to the USB port */
void USB_UART_Tx(uint8_t data)
{
   USBUART_txbuffer[USBUART_writePtr++] = data;
}

/* Once USB data is received trigger an interrupt */
void usbFunctionWriteOut( uchar *data, uchar len )
{

	/*  device <= RS-232C  */
	while( len-- ) {
	    /* Loop around the buffer */
	    USBUART_rxbuffer[USBUART_writePtr] = *data++;
	    USBUART_writePtr = (USBUART_writePtr+1) & RX_MASK;
    }
}




static void hardwareInit(void)
{

    /* activate pull-ups except on USB lines */
    USB_CFG_IOPORT   = (uchar)~((1<<USB_CFG_DMINUS_BIT)|(1<<USB_CFG_DPLUS_BIT));
    /* all pins input except USB (-> USB reset) */
#ifdef USB_CFG_PULLUP_IOPORT    /* use usbDeviceConnect()/usbDeviceDisconnect() if available */
    USBDDR    = 0;    /* we do RESET by deactivating pullup */
    usbDeviceDisconnect();
#else
    USBDDR    = (1<<USB_CFG_DMINUS_BIT)|(1<<USB_CFG_DPLUS_BIT);
#endif

    /* 250 ms disconnect */
    _delay_ms(250);

#ifdef USB_CFG_PULLUP_IOPORT
    usbDeviceConnect();
#else
    USBDDR    = 0;      /*  remove USB reset condition */
#endif

}



int main(void)
{
   
   
    //hardwareInit();
    
    usbInit();
    //USB_UART_Init();
   
  
    intr3Status = 0;
    sendEmptyFrame  = 0;


    DDRD |= (1<<1);
    
    sei();
    
    //SofTimer_Init();
    
  
    
    while(1)
    {
       usbPoll();


uchar		next;


	/*  USB <= device  */
    if( usbInterruptIsReady() && (USBUART_writePtr!=USBUART_readPtr) ) {
        uchar   bytesRead, i;

        bytesRead = (USBUART_writePtr-USBUART_readPtr) & RX_MASK;
        if(bytesRead>7)
            bytesRead = 7;
		next	= USBUART_readPtr + bytesRead;
		if( next>=RX_SIZE ) {
			next &= RX_MASK;
			
			/* Get the looped around the buffer bytes*/
			for( i=0; i<next; i++ )
				USBUART_rxbuffer[RX_SIZE+i]	= USBUART_rxbuffer[i];
		}
        usbSetInterrupt(USBUART_rxbuffer+USBUART_readPtr, bytesRead);
        USBUART_readPtr   = next;
    }




#if 0
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
#endif
         
    }
    

    return 0;
}







