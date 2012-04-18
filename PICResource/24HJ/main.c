#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "p24Hxxxx.h"

#include <pgm.h>
#include <delay.h>
#include <pps.h>
#include <dma.h>

#include "hardUart24H/hardUart.h"
#include "RingBuffer/ringbuffer.h"
#include "linkedlist/linkedlist.h"
#include "stack/stack.h"
#include "dmaUart/dmaUart.h"

#include "mmculib/uint16toa.h"
#include "adc/adc.h"

void InterruptHandlerLow();

volatile PIC_USART_t U1 = {&U1STA, &U1MODE, &U1BRG, &U1TXREG, &U1RXREG, &TransmitBuffer, &ReceiveBuffer};
volatile PIC_USART_t U2 = {&U2STA, &U2MODE, &U2BRG, &U2TXREG, &U2RXREG, &TransmitBuffer2, &TransmitBuffer2};
PIC_USART_t* PrimaryUART;
LINKED_LIST_t PrimaryList;


PIC_DMA_UART_t DMAUART1 = { &DMA0CON,
                            &DMA0REQ,
                            &DMA0STA,
                            &DMA0PAD,
                            &DMA0CNT,
                            &DMAUARTStack,
                            &DMAUARTBuffer,
                            &U1};



PGM_P ROMSTRING = "ROM STRING\n";
char RAMSTRING[] = "RAM STRING\n";

_FOSCSEL(FNOSC_FRCPLL);
//_FOSCSEL(FNOSC_FRC);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF);


//static char BufferB[20] __attribute__((space(dma)));

void SendString(char* string)
{
    //DMA_SendUARTString(&DMAUART1, string);
    uartTxString_P(&U1, string);
    uartTxString_P(&U2, string);
}

void PWMInit(void)
{
    OC1CONbits.OCM = 0x06;
    PR2 = 895;
    OC1RS = 741;
    T2CONbits.TON = 1;
}

void TimerInit(void)
{
    PR1 = 18;
    IPC0bits.T1IP = 0x01;
    IFS0bits.T1IF = 0x00;
    IEC0bits.T1IE = 0x01;
    T1CONbits.TON = 0x01;
}


int main(void)
{
    
    PrimaryUART = &U1;
    //CLKDIVbits.DOZE = 0;
    CLKDIVbits.DOZEN = 0;
    CLKDIVbits.PLLPOST = 0;
    CLKDIVbits.PLLPRE = 0;
    PLLFBDbits.PLLDIV = 41;
    OSCTUN = 0x00;

    while (OSCCONbits.LOCK != 1)
    {
    };

    TRISB &= ~(1 << 8);
    TRISB &= ~(1 << 9);

    PPSUnLock;
    PPSOutput(PPS_U2TX, PPS_RP8);
    PPSOutput(PPS_U1TX, PPS_RP9);
    PPSInput(PPS_U1RX, PPS_RP10);
    PPSOutput(PPS_OC1, PPS_RP15);

    
    PPSOutput(PPS_SCK1, PPS_RP5);
    PPSOutput(PPS_SDO1, PPS_RP6);
    PPSInput(PPS_SDI1,  PPS_RP7);

    PPSLock;

    uartInit(PrimaryUART);
    uartSetBaud(PrimaryUART, 86);
    uartInit(&U2);
    uartSetBaud(&U2, 86);
    SET_CPU_IPL(0);

    TRISA &= ~(1 << 0);
    TRISA |= (1<<1);
    LATA = 0;

    //The UART Interrupt should be disabled.
    //IEC0bits.U1TXIE = 0;
    DMA_UART_Init(&DMAUART1, UART1);
    DMA_UART_Enable();

    IEC0bits.U1TXIE = 1;
    Delay(10);

    SendString("Welcome executive:\n");
    SendString("My Name\n");
    SendString("is Adrian123\n");
//    DMA_SendUARTString(&DMAUART1, "Hello\n");
//    DMA_SendUARTString(&DMAUART1, "My Name\n");
//    DMA_SendUARTString(&DMAUART1, "is Adrian123\n");

    LATA = 1;
    
    Delay_Us(Delay_1S_Cnt);
    PWMInit();
    //TimerInit();

    ADC_Init();
    ADC_Set10bit();
    ADC_SetClockSpeed(5, 2);
    ADC_SetPin(10);
    //ADCStart();
    uartTx(&U2, 0xAA);
    uartTx(&U2, 0xAB);

    Delay(10);

    uint16_t ADCValue;
    char outputString[10];

    while (1)
    {
        //ADCValue = ADC_Sample();


    }
   
    return 0;
}


void __attribute__((__interrupt__, no_auto_psv)) _DMA0Interrupt(void)
{
    IFS0bits.DMA0IF = 0; // Clear the DMA0 Interrupt Flag;
    DMAUART1.DMAFlag = 0;
    DMA_StartUART(&DMAUART1);
    LATA = 1;
}

void __attribute__((__interrupt__, no_auto_psv)) _AddressError(void)
{
    INTCON1bits.ADDRERR = 0;
    LATA = 1;
    while(1);
}

void __attribute__((__interrupt__, no_auto_psv)) _StackError(void)
{
    INTCON1bits.STKERR = 0;
    LATA = 1;
    while(1);
}

void __attribute__((__interrupt__, no_auto_psv)) _DMACError(void)
{
    INTCON1bits.DMACERR = 0;
    LATA = 1;
    while(1);
}

//----------------------------------------------------------------------------
// Low priority interrupt vector
void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{
    IFS0bits.T1IF = 0; //clear interrupt flag
    //LATA ^= 1;
}


void __attribute__((__interrupt__, no_auto_psv)) _ADC1Interrupt(void)
{
    IFS0bits.AD1IF = 0x00;
    LATA ^= 1;
    uint16_t ADCValue = ADC1BUF0;
    char outputString[10];
    uint16toa(ADCValue, outputString, 0);
    uartTxString(&U2, outputString);
    uartTx(&U2, '\n' );
//    ADCValue = ADC1BUF0;
//    uartTx(&U2, ((ADCValue & 0xFF00) >> 8) );
//    uartTx(&U2, ((ADCValue & 0x00FF)) );

}



void __attribute__((__interrupt__, no_auto_psv)) _U1TXInterrupt(void)
{
    IFS0bits.U1TXIF = 0; //clear interrupt flag
    if( !ringbuffer_isEmpty((RINGBUFFER_T*)U1.TransmitBuffer) )
    {
        *U1.UXTXREG = ringbuffer_get((RINGBUFFER_T*)U1.TransmitBuffer);
    }
    
}

void __attribute__((__interrupt__, no_auto_psv)) _U2TXInterrupt(void)
{
    IFS1bits.U2TXIF = 0; //clear interrupt flag
    if( !ringbuffer_isEmpty((RINGBUFFER_T*) U2.TransmitBuffer))
    {
        *U2.UXTXREG = ringbuffer_get((RINGBUFFER_T*) U2.TransmitBuffer);
    }    
}

void __attribute__((__interrupt__, no_auto_psv)) _U2RXInterrupt(void)
{
    uint8_t i = *U2.UXRXREG;
    IFS1bits.U2RXIF = 0;
    LATA ^= 1;
    uartTx(PrimaryUART, i);
}

void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void)
{
    uint8_t i = *U1.UXRXREG;
    IFS0bits.U1RXIF = 0;
    LATA ^= 1;
    uartTx(PrimaryUART, i);
}
//----------------------------------------------------------------------------
