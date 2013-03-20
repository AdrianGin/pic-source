/*****************************************************************************
*
* Atmel Corporation
*
* File              : serial.c
* Compiler          : IAR C 3.10C Kickstart, AVR-GCC/avr-libc(>= 1.2.5)
* Revision          : $Revision: 1.7 $
* Date              : $Date: Tuesday, June 07, 200 $
* Updated by        : $Author: raapeland $
*
* Support mail      : avr@atmel.com
*
* Target platform   : All AVRs with bootloader support
*
* AppNote           : AVR109 - Self-programming
*
* Description       : UART communication routines
****************************************************************************/
#include "defines.h"
#include "USBUART\usbuart.h"

void sendchar(uint8_t c)
{
	USB_UART_Tx(c);
}


uint8_t recchar(void)
{
	return USB_UART_GetByte();
}
