/* Adrian Gin */

#ifndef _USB_UART
#define _USB_UART

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

/* CDC Class Specific Functional Descriptors 5.2.2 of USB CDC 1.1 (usb.org)*/

#define CDC_CS_INTERFACE (0x24)
#define CDC_CS_ENDPOINT  (0x25)

#define HW_CDC_BULK_OUT_SIZE     8
#define HW_CDC_BULK_IN_SIZE      8

#define	RX_SIZE		16      /* UART receive buffer size (must be 2^n, 16-128)  */
#define	TX_SIZE		16      /* UART transmit buffer size (must be 2^n, 16-256) */
#define	RX_MASK		(RX_SIZE-1)
#define	TX_MASK		(TX_SIZE-1)


/* allow ATmega8 compatibility */
#ifdef URSEL
#define UBRR0H    UBRRH
#define UBRR0L    UBRRL
#define UCSR0A    UCSRA
#define UCSR0B    UCSRB
#define UCSR0C    UCSRC
#define UDR0      UDR

/* UCSR0A */
#define RXC0      RXC
#define TXC0      TXC
#define UDRE0     UDRE
#define FE0       FE
#define DOR0      DOR
#define UPE0      PE
#define U2X0      U2X

/* UCSR0B */
#define RXCIE0    RXCIE
#define TXCIE0    TXCIE
#define UDRIE0    UDRIE
#define RXEN0     RXEN
#define TXEN0     TXEN

/* UCSR0C */
#define UPM00     UPM0
#define USBS0     USBS
#define UPBS0     UPBS
#define UCSZ00    UCSZ0

/* Interrupts */

#define USART0_RX_vect USART_RXC_vect
#define USART0_TX_vect USART_TXC_vect


#endif

typedef union usbDWord {
    uint64_t	dword;
    uint8_t    bytes[4];
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
extern uint8_t USBUART_writePtr;
extern uint8_t USBUART_readPtr;

extern uint8_t sendEmptyFrame;

#endif
