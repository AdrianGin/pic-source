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
#include "mmculib/root.h"
#include "adc/adc.h"

#include "Waveplayer/waveplayerFS.h"

void InterruptHandlerLow();

volatile PIC_USART_t U1 = {&U1STA, &U1MODE, &U1BRG, &U1TXREG, &U1RXREG, &IFS0, TX1IF, &TransmitBuffer, &ReceiveBuffer};
volatile PIC_USART_t U2 = {&U2STA, &U2MODE, &U2BRG, &U2TXREG, &U2RXREG, &IFS1, TX2IF, &TransmitBuffer2, &TransmitBuffer2};
PIC_USART_t* PrimaryUART;
LINKED_LIST_t PrimaryList;

PIC_SPI_t S1 = {&SPI1STAT, &SPI1CON1, &SPI1CON2, &SPI1BUF};



PIC_DMA_UART_t DMAUART1 = {&DMA0CON,
    &DMA0REQ,
    &DMA0STA,
    &DMA0PAD,
    &DMA0CNT,
    &DMAUARTStack,
    (uint8_t*)&DMABuffer,
    &U1};

PIC_DMA_SPI_t DMASPI1 = {&DMA1CON,
    &DMA1REQ,
    &DMA1STA,
    &DMA1PAD,
    &DMA1CNT,
    &DMAUARTStack,
    (uint8_t*)&DMABuffer[DMA_TX_SIZE],
    &S1};

PIC_DMA_SPI_t DMASPI1T = {&DMA2CON,
    &DMA2REQ,
    &DMA2STA,
    &DMA2PAD,
    &DMA2CNT,
    &DMAUARTStack,
    (uint8_t*)&DMABuffer[0],
    &S1};


PGM_P ROMSTRING = "ROM STRING\n";
char RAMSTRING[] = "RAM STRING\n";



waveFile_t wav[2];
volatile uint8_t newSongFlag = 0;
volatile uint8_t songIndex = 0;
volatile uint8_t ProcessBufferFlag;
//volatile uint8_t ProcessBufferFlag;

volatile char inputString[20];

FATFS filesys;

void LoadWaveFile(waveFile_t* waveFile, char* filename);

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
    ADC_SetClockSpeed(5, 4);
    ADC_SetPin(11);
    //ADC_Off();

    Delay(10);
    uint16_t ADCValue;
    char outputString[10];

    SPI_Init(&S1);
    uint8_t ret;

    DMA_SPI_Init(&DMASPI1, &DMASPI1T, DMA_SPI1);
    DMA_SPI_Enable();

    ret = f_mount(0, &filesys);
    uartTx(PrimaryUART, ret);

//    FIL testFIL;
//
//    ret = f_open(&testFIL, "hello.txt", FA_OPEN_EXISTING | FA_READ);
//    uartTx(PrimaryUART, ret);
//
//    uint16_t br;
//    uint8_t buffer[30];
//    memset(buffer, 0, 30);
//    ret = f_read(&testFIL, buffer, 20, &br);
//
//    DEBUG(buffer);

    //while(1);

    strcpy(outputString, "4816s.wav");
    //ret = pf_open("1.wav");
    //SPI_TxByte(&S1, 'A');
    while (1)
    {


        if (newSongFlag)
        {
            LoadWaveFile(&wav[songIndex], inputString);
            songIndex = (songIndex + 1) % 2;
            newSongFlag = 0;

            //waveSetSampleRate(wavefile[songIndex].waveHeader.sampleRate);
        }
        

        ProcessBufferFlag = 1;


        if ((waveIsPlaying(&wav[0]) & WAVE_AUDIO_STATUS_PLAYING)&&(waveContinuePlaying(&wav[0])==0))
        {
            //waveAudioOff(&wavefile[0]);
            DEBUG(("\nWave Finished! 1"));
            newSongFlag = 1;
        }

        if ((waveIsPlaying(&wav[1])& WAVE_AUDIO_STATUS_PLAYING)&&(waveContinuePlaying(&wav[1])==0))
        {
            //waveAudioOff();
            DEBUG(("\nWave Finished! 2 "));
            //newSongFlag = 1;
        }
        ProcessBufferFlag = 0;
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
    
    int16_t Z, Y, A, B;
    uint16_t X, G;
    uint8_t a,b;
    
    a = wav[0].Buffer[(wav[0].audioReadptr)];
    b = wav[1].Buffer[(wav[1].audioReadptr)];

    //b = 128;

    //Y = ((a*b)/128);
    //Z = ((2*(a+b)) - ((a*b)/128)) - 256;
//    Z = (a - 128);
//    B = b - 128;
//
//    B = (B * 5) / 20;
//
//    //Z = (Z * 8) / 10;
//    Y = (ADC_Sample()/50);
//    Y = Z * Y / 20;
//    Z = Y + B;
//    Z += 128;

    A = a - 128;
    B = b - 128;
    X = (A*A) + (B*B);
    X = X << 1;

    if( X < 127)
    {
        X = 127;
    }
    else
    {
        X = root(X);
    }
    
    A  = (A * A) / X;
    if( a < 128 )
    {
        A = -A;
    }
    B  = (B * B) / X;
    if( b < 128 )
    {
        B = -B;
    }
    Z = (A + B) + 128;
    if ( Z > 255)
    {
        LATA ^= 1;
    }

    WAVE_LEFT_REG = Z;
    //WAVE_LEFT_REG = wav[0].Buffer[(wav[0].audioReadptr)];

    /* Right is second */
    /* This will not do anything if WAVE_STEREO_ENABLED is not set to 1 */
    WAVE_RIGHT_REG = wav[1].Buffer[(wav[1].audioReadptr)];// + ((wav[1].waveHeader.channelCount) >> 1)];
    //WAVE_RIGHT_REG = Buff[(audioReadptr+isStereo)];
    //WAVE_RIGHT_REG = wavefile[1].Buffer[(wavefile[1].audioReadptr)];
    if( waveBufferedBytes(&wav[0]) >= 0x10)
    {
        wav[0].audioReadptr = (wav[0].audioReadptr + (wav[0].waveHeader.channelCount))&WAVE_OUTMASK;
    }

    if( waveBufferedBytes(&wav[1]) >= 0x10)
    {
        wav[1].audioReadptr = (wav[1].audioReadptr + (wav[1].waveHeader.channelCount))&WAVE_OUTMASK;
    }





    //ADCValue = ;
    //waveSetSampleRate((ADC_Sample()*86)+8000);
    //wavefile[0].audioReadptr++;
    //uartTx(PrimaryUART, wavefile[0].audioReadptr);

    //wavefile[1].audioReadptr = (wavefile[1].audioReadptr + (wavefile[1].waveHeader.channelCount))&WAVE_OUTMASK;
    
}

void __attribute__((__interrupt__, no_auto_psv)) _ADC1Interrupt(void)
{
    IFS0bits.AD1IF = 0x00;
    uint16_t ADCValue = ADC1BUF0;

    

//    char outputString[10];
//    uint16toa(ADCValue, outputString, 0);
//    uartTx((PIC_USART_t*)&U2, 'A');
//    LATA = 1;
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
    while( !(*U2.UXSTA & (1<<UTXBF)) && !ringbuffer_isEmpty((RINGBUFFER_T*)U2.TransmitBuffer) )
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

void LoadWaveFile(waveFile_t* waveFile, char* filename)
{
    char outputString[20];
    waveHeader_t* waveheader = &waveFile->waveHeader;
    
    strncpy(outputString, filename, strlen(filename));

    strncpy(&outputString[strlen(filename)], ".wav", 5);
    //pf_mount(&filesys);
    DEBUG(("Playing\n"));
    DEBUG(outputString);

    while( ProcessBufferFlag == 1);
    
    wavePlayFile(waveFile, outputString);

    uint16toa(waveheader->channelCount, outputString, 0);
    DEBUG(("\nChannel Count: "));
    DEBUG(outputString);

    uint16toa(waveheader->resolution, outputString, 0);
    DEBUG(("\nRes: "));
    DEBUG(outputString);

    uint16toa(waveheader->sampleRate, outputString, 0);
    DEBUG(("\nSampleRate: "));
    DEBUG(outputString);

    DEBUG(("\nDataSize: "));
    uint16toa(((uint32_t)(waveheader->dataSize)>>16), outputString, 0);
    DEBUG(outputString);
    uint16toa(waveheader->dataSize, outputString, 0);
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
        //songIndex = (songIndex) % 2;
    }

}
//----------------------------------------------------------------------------
