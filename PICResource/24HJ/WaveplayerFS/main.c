#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#include <pgm.h>

#include <pps.h>
#include <dma.h>


#include "hardwareSpecific.h"
#include "RingBuffer/ringbuffer.h"
#include "linkedlist/linkedlist.h"
#include "stack/stack.h"
#include "dmaUart/dmaUart.h"
#include "dmaSPI/dmaSPI.h"

#include "SD_MMC/sd.h"

#include "mmculib/uint16toa.h"
#include "adc/adc.h"

#include "Waveplayer/waveplayer.h"

void InterruptHandlerLow();

volatile PIC_USART_t U1 = {&U1STA, &U1MODE, &U1BRG, &U1TXREG, &U1RXREG, &IFS0, TX1IF, &TransmitBuffer, &ReceiveBuffer};
volatile PIC_USART_t U2 = {&U2STA, &U2MODE, &U2BRG, &U2TXREG, &U2RXREG, &IFS1, TX2IF, &TransmitBuffer2, &TransmitBuffer2};
PIC_USART_t* PrimaryUART;
LINKED_LIST_t PrimaryList;

volatile PIC_SPI_t S1 = {&SPI1STAT, &SPI1CON1, &SPI1CON2, &SPI1BUF};


PIC_DMA_UART_t DMAUART1 = {&DMA0CON,
    &DMA0REQ,
    &DMA0STA,
    &DMA0PAD,
    &DMA0CNT,
    &DMAUARTStack,
    (uint8_t*)&DMABuffer,
    &U1};

volatile PIC_DMA_SPI_t DMASPI1 = {&DMA1CON,
    &DMA1REQ,
    &DMA1STA,
    &DMA1PAD,
    &DMA1CNT,
    &DMAUARTStack,
    (uint8_t*)&DMABuffer[DMA_TX_SIZE],
    &S1};

volatile PIC_DMA_SPI_t DMASPI1T = {&DMA2CON,
    &DMA2REQ,
    &DMA2STA,
    &DMA2PAD,
    &DMA2CNT,
    &DMAUARTStack,
    (uint8_t*)&DMABuffer[0],
    &S1};


PGM_P ROMSTRING = "ROM STRING\n";
char RAMSTRING[] = "RAM STRING\n";



volatile waveHeader_t wavefile;
volatile uint8_t newSongFlag = 0;
volatile uint8_t ProcessBufferFlag;
//volatile uint8_t ProcessBufferFlag;

volatile char inputString[20];

FATFS filesys;

void LoadWaveFile(char* filename);

//static char BufferB[20] __attribute__((space(dma)));

void SendString(char* string)
{
    DEBUG(string);
    //uartTxString_P((PIC_USART_t*)&U2, string);
}

void PWMInit(void)
{
    OC1CONbits.OCM = 0x06;
    PR2 = 4;
    OC1RS = 1;
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

    PrimaryUART = (PIC_USART_t*)&U1;
    //CLKDIVbits.DOZE = 0;
    CLKDIVbits.DOZEN = 0;
    CLKDIVbits.PLLPOST = 0;
    CLKDIVbits.PLLPRE = 0;
    PLLFBDbits.PLLDIV = 41;

    OSCTUN = 0x00;

    while (OSCCONbits.LOCK!=1)
    {
    };

    TRISB &= ~(1<<8);
    TRISB &= ~(1<<9);

    TRISA &= ~((1<<1)|(1<<0));

    PPSUnLock;
    PPSOutput(PPS_U2TX, PPS_RP8);
    PPSOutput(PPS_U1TX, PPS_RP9);
    PPSInput(PPS_U2RX, PPS_RP10);
    PPSInput(PPS_U1RX, PPS_RP11);


    PPSOutput(PPS_OC1, PPS_RP15);
    PPSOutput(PPS_OC2, PPS_RP14);

    PPSOutput(PPS_SCK1, PPS_RP5);
    PPSOutput(PPS_SDO1, PPS_RP6);
    PPSInput(PPS_SDI1, PPS_RP7);

    PPSLock;

    uartInit(PrimaryUART);
    uartSetBaud(PrimaryUART, 86);
    uartInit((PIC_USART_t*)&U2);
    uartSetBaud((PIC_USART_t*)&U2, 86);
    SET_CPU_IPL(0);

    //TRISA |= (1<<1);
    //LATA = 0;
    Delay(10);

    SendString("Welcome executive:\n");
    SendString("My Name\n");
    SendString("is Adrian123\n");

    //Delay_Us(Delay_1S_Cnt);
    //PWMInit();
    //TimerInit();

    ADC_Init();
    ADC_Set10bit();
    ADC_SetClockSpeed(5, 2);
    ADC_SetPin(11);
    ADC_Off();
    //ADCStart();

    Delay(10);
    uint16_t ADCValue;
    char outputString[10];

    SPI_Init(&S1);
    uint8_t ret;

    DMA_SPI_Init(&DMASPI1, &DMASPI1T, DMA_SPI1);
    DMA_SPI_Enable();

    ret = pf_mount(&filesys);
    uartTx(PrimaryUART, ret);
    uartTx(PrimaryUART, 'A');
    uartTx(PrimaryUART, 'B');
    uartTx(PrimaryUART, 'C');
    uartTx(PrimaryUART, 'D');

    strcpy(outputString, "4816s.wav");
    //ret = pf_open("1.wav");
    //SPI_TxByte(&S1, 'A');
    while (1)
    {
        //ADCValue = ADC_Sample();
        ProcessBufferFlag = 1;
        if ((waveIsPlaying())&&(waveContinuePlaying((waveHeader_t*)&wavefile)==0))
        {
            waveAudioOff();
            DEBUG(("\nWave Finished!"));
            newSongFlag = 1;
        }
        ProcessBufferFlag = 0;


        if (newSongFlag)
        {
            LoadWaveFile(inputString);
            newSongFlag = 0;
        }
    }

    return 0;
}

void __attribute__((__interrupt__, no_auto_psv)) _AddressError(void)
{
    INTCON1bits.ADDRERR = 0;
    LATA = 1;
    while (1);
}

void __attribute__((__interrupt__, no_auto_psv)) _StackError(void)
{
    INTCON1bits.STKERR = 0;
    LATA = 1;
    while (1);
}

void __attribute__((__interrupt__, no_auto_psv)) _DMACError(void)
{
    INTCON1bits.DMACERR = 0;
    LATA = 1;
    while (1);
}

//----------------------------------------------------------------------------
// Low priority interrupt vector

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{
    IFS0bits.T1IF = 0; //clear interrupt flag
    WAVE_LEFT_REG = Buff[(audioReadptr)];
    /* Right is second */
    /* This will not do anything if WAVE_STEREO_ENABLED is not set to 1 */
    WAVE_RIGHT_REG = Buff[(audioReadptr+isStereo)];
    audioReadptr = (audioReadptr+1+isStereo)&WAVE_OUTMASK;
    //LATA ^= 1;
}

void __attribute__((__interrupt__, no_auto_psv)) _ADC1Interrupt(void)
{
    IFS0bits.AD1IF = 0x00;
    uint16_t ADCValue = ADC1BUF0;
    char outputString[10];
    uint16toa(ADCValue, outputString, 0);
    uartTx((PIC_USART_t*)&U2, 'A');
    LATA = 1;
    //    ADCValue = ADC1BUF0;
    //    uartTx(&U2, ((ADCValue & 0xFF00) >> 8) );
    //    uartTx(&U2, ((ADCValue & 0x00FF)) );

}

void __attribute__((__interrupt__, no_auto_psv)) _SPI1Interrupt(void)
{
    IFS0bits.SPI1IF = 0; //clear interrupt flag
    //uint16_t buf = SPI1BUF;
}

void __attribute__((__interrupt__, no_auto_psv)) _SPI2Interrupt(void)
{
    IFS2bits.SPI2IF = 0; //clear interrupt flag
}

void __attribute__((__interrupt__, no_auto_psv)) _DMA0Interrupt(void)
{
    IFS0bits.DMA0IF = 0; // Clear the DMA0 Interrupt Flag;
    //__ManagedUART_DMA_ISR(&MUART1);
}

void __attribute__((__interrupt__, no_auto_psv)) _DMA1Interrupt(void)
{
    IFS0bits.DMA1IF = 0; // Clear the DMA0 Interrupt Flag;
    DMASPI1.DMAFlag = 0;
    //Turn off the infinite transmitter
    //*DMASPI1T.DMACON &= ~DMA_CHEN;
    //__ManagedUART_DMA_ISR(&MUART1);
}

void __attribute__((__interrupt__, no_auto_psv)) _DMA2Interrupt(void)
{
    IFS1bits.DMA2IF = 0; // Clear the DMA0 Interrupt Flag;
    DMASPI1T.DMAFlag = 0;

    //uartTxDump(PrimaryUART, &DMASPI1.DMABuffer[0], 16);
    //__ManagedUART_DMA_ISR(&MUART1);
}




void __attribute__((__interrupt__, no_auto_psv)) _U1TXInterrupt(void)
{
    IFS0bits.U1TXIF = 0;
    //Send the next byte
    //if (!ringbuffer_isEmpty((RINGBUFFER_T*)U1.TransmitBuffer))
    while( !(*U1.UXSTA & (1<<UTXBF)) && !ringbuffer_isEmpty((RINGBUFFER_T*)U1.TransmitBuffer) )
    {
        *U1.UXTXREG = ringbuffer_get((RINGBUFFER_T*)U1.TransmitBuffer);
    }
    
}

void __attribute__((__interrupt__, no_auto_psv)) _U2TXInterrupt(void)
{
    IFS1bits.U2TXIF = 0; //clear interrupt flag
    if (!ringbuffer_isEmpty((RINGBUFFER_T*)U2.TransmitBuffer))
    {
        *U2.UXTXREG = ringbuffer_get((RINGBUFFER_T*)U2.TransmitBuffer);
    }
}

void __attribute__((__interrupt__, no_auto_psv)) _U2RXInterrupt(void)
{
    uint8_t i = *U2.UXRXREG;
    IFS1bits.U2RXIF = 0;
    //i = SPI_TxByte(&S1, i);
    uartTx((PIC_USART_t*)&U2, i);

}

void LoadWaveFile(char* filename)
{
    char outputString[20];
    strncpy(outputString, filename, strlen(filename));

    strncpy(&outputString[strlen(filename)], ".wav", 5);
    //pf_mount(&filesys);
    DEBUG(("Playing\n"));
    DEBUG(outputString);

    while( ProcessBufferFlag == 1);

    wavePlayFile((waveHeader_t*)&wavefile, outputString);

    uint16toa(wavefile.channelCount, outputString, 0);
    DEBUG(("\nChannel Count: "));
    DEBUG(outputString);

    uint16toa(wavefile.resolution, outputString, 0);
    DEBUG(("\nRes: "));
    DEBUG(outputString);

    uint16toa(wavefile.sampleRate, outputString, 0);
    DEBUG(("\nSampleRate: "));
    DEBUG(outputString);

    DEBUG(("\nDataSize: "));
    uint16toa(((uint32_t)(wavefile.dataSize)>>16), outputString, 0);
    DEBUG(outputString);
    uint16toa(wavefile.dataSize, outputString, 0);
    DEBUG(outputString);
}

void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void)
{
    uint8_t i = *U1.UXRXREG;
    IFS0bits.U1RXIF = 0;
    //i = SPI_TxByte(&S1, i);
    uartTx((PIC_USART_t*)&U1, i);
    static uint8_t byteCount = 0;
    

    if (i!=0x0D)
    {
        inputString[byteCount++] = i;
    }
    else
    {
        inputString[byteCount] = '\0';
        byteCount = 0;
        newSongFlag = 1;
        
    }

}
//----------------------------------------------------------------------------
