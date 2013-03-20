
/* Name: uart.h
 * Project: AVR USB driver for CDC interface on Low-Speed USB
 * Author: Osamu Tamura
 * Creation Date: 2006-06-18
 * Tabsize: 4
 * Copyright: (c) 2006 by Recursion Co., Ltd.
 * License: Proprietary, free under certain conditions. See Documentation.
 */

#ifndef __uart_h_included__
#define __uart_h_included__

#include <avr/io.h> /* for TXEN or TXEN0 resp., if available */

enum {
   SEND_ENCAPSULATED_COMMAND = 0,
   GET_ENCAPSULATED_RESPONSE,
   SET_COMM_FEATURE,
   GET_COMM_FEATURE,
   CLEAR_COMM_FEATURE,
   SET_LINE_CODING = 0x20,
   GET_LINE_CODING,
   SET_CONTROL_LINE_STATE,
   SEND_BREAK
};

enum CDC_DescriptorSubtype {
   HEADER_FUNCTION = 0,
   CALL_MANAGEMENT,
   ABSTRACT_CONTROL,
   DIRECT_LINE,
   TELEPHONE_RINGER,
   TELEPHONE_CALL,
   UNION_FUNCTION,
   
};

#define HW_CDC_BULK_OUT_SIZE     8
#define HW_CDC_BULK_IN_SIZE      8

/* CDC Class Specific Functional Descriptors 5.2.2 of USB CDC 1.1 (usb.org)*/

#define CDC_CS_INTERFACE (0x24)
#define CDC_CS_ENDPOINT  (0x25)


#define	RX_SIZE		64      /* UART receive buffer size (must be 2^n, 16-128)  */
#define	TX_SIZE		64      /* UART transmit buffer size (must be 2^n, 16-256) */
#define	RX_MASK		(RX_SIZE-1)
#define	TX_MASK		(TX_SIZE-1)

typedef union usbDWord {
    uint32_t	dword;
    uint8_t   bytes[4];
} usbDWord_t;

typedef struct usbDescrCDC_HeaderFn
{
   uint8_t FunctionLength;
   uint8_t DescriptorType;
   uint8_t DescriptorSubType;
   uint16_t bcdCDC;
   
} usbDescrCDC_HeaderFn;

typedef struct usbDescrCDC_AcmFn
{
   uint8_t FunctionLength;
   uint8_t DescriptorType;
   uint8_t DescriptorSubType;
   uint8_t Capabilities;
   
} usbDescrCDC_AcmFn;

/* Only allows for one Slave Interface */
typedef struct usbDescrCDC_UnionFn
{
   uint8_t FunctionLength;
   uint8_t DescriptorType;
   uint8_t DescriptorSubType;
   uint8_t MasterInterfaceIndex;
   uint8_t SlaveInterfaceIndex;
   
} usbDescrCDC_UnionFn;


typedef struct usbDescrCDC_CallMgtFn
{
   uint8_t FunctionLength;
   uint8_t DescriptorType;
   uint8_t DescriptorSubType;
   uint8_t Capabilities;
   uint8_t DataInterface;
   
} usbDescrCDC_CallMgtFn;


extern uint8_t USBUART_txbuffer[];
extern uint8_t USBUART_rxbuffer[];
extern uint8_t USBUART_txWritePtr;
extern uint8_t USBUART_txReadPtr;

extern uint8_t USBUART_rxWritePtr;
extern uint8_t USBUART_rxReadPtr;

extern uint8_t intr3Status;

/* The following function returns the amount of bytes available in the TX
 * buffer before we have an overflow.
 */
uint8_t uartRxBytesFree(void);
uint8_t uartTxBytesFree(void);


uint8_t USB_UART_GetByte(void);
void USB_UART_Tx(uint8_t data);
void USB_UART_TxString(uint8_t* data);

void USB_UART_Poll(void);

#endif  /*  __uart_h_included__  */

