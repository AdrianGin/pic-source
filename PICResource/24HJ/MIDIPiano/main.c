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
#include "MIDIParser/midiparser.h"

#include "SD_MMC/sd.h"

#include "MIDIPlayback/midiplayback.h"

#include "mmculib/uint16toa.h"
#include "mmculib/root.h"
#include "adc/adc.h"


void InterruptHandlerLow();

volatile PIC_USART_t U1 = {&U1STA, &U1MODE, &U1BRG, &U1TXREG, &U1RXREG, &IFS0, TX1IF, &TransmitBuffer, &ReceiveBuffer};
volatile PIC_USART_t U2 = {&U2STA, &U2MODE, &U2BRG, &U2TXREG, &U2RXREG, &IFS1, TX2IF, &TransmitBuffer2, &TransmitBuffer2};
PIC_USART_t* PrimaryUART;
LINKED_LIST_t PrimaryList;

PIC_SPI_t S1 = {&SPI1STAT, &SPI1CON1, &SPI1CON2, &SPI1BUF};

volatile uint8_t globalFlag = 0;


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

//FIL testFIL;
//uint16_t br;
//uint8_t buffer[BUFFER_READ_SIZE];


volatile uint8_t newSongFlag = 0;
volatile uint8_t songIndex = 0;
volatile uint8_t ProcessBufferFlag;
//volatile uint8_t ProcessBufferFlag;

volatile char inputString[20];
char filename[20];

MIDI_HEADER_CHUNK_t MIDIHdr;
FATFS filesys;

void LoadMIDIFile(FIL* waveFile, char* filename);

void LoadMIDIFile(FIL* waveFile, char* filename)
{
    char outputString[20];
    strcpy(outputString, filename);
}



//static char BufferB[20] __attribute__((space(dma)));

void myprintf(char* string, uint32_t num)
{
    char outputString[20];
    utoa(outputString, num, 16);
    DEBUG(string);
    DEBUG(outputString);
    DEBUG("\n");
}

void myprintfd(char* string, uint32_t num)
{
    char outputString[20];
    utoa(outputString, num, 10);
    DEBUG(string);
    DEBUG(outputString);
    DEBUG("\n");
}

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
    //For a 100us tick rate
    //PR1 = F_CPU/10000;
    PR1 = F_CPU/10000;
    IPC0bits.T1IP = 0x01;
    IFS0bits.T1IF = 0x00;
    IEC0bits.T1IE = 0x01;
    T1CONbits.TON = 0x00;
}

void TimerStart(void)
{
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
    uartSetBaud(PrimaryUART, 320);
    uartInit((PIC_USART_t*)&U2);
    uartSetBaud((PIC_USART_t*)&U2, 320);
    SET_CPU_IPL(0);

    //TRISA |= (1<<1);
    //LATA = 0;
    Delay(10);

    //SendString("Welcome executive:\n");
    //SendString("My Name\n");
    //SendString("is Adrian123\n");

    //Delay_Us(Delay_1S_Cnt);
    //PWMInit();
    TimerInit();

    ADC_Init();
    ADC_Set10bit();
    ADC_SetClockSpeed(5, 4);
    ADC_SetPin(11);
    //ADC_Off();

    Delay(10);
    uint16_t ADCValue;

    SPI_Init(&S1);
    uint8_t ret;

    DMA_SPI_Init(&DMASPI1, &DMASPI1T, DMA_SPI1);
    DMA_SPI_Enable();

    ret = f_mount(0, &filesys);

    MPB_SetTickRate(32, 480);
    while(1)
    {

        switch( newSongFlag )
        {
            case 1:
            {
                strcpy(filename, inputString);
                MPB_PlayMIDIFile(&MIDIHdr, filename);
                MPB_DetermineLength(&MIDIHdr);
                MPB_PlayMIDIFile(&MIDIHdr, filename);
                TimerStart();
                newSongFlag = 0;
                break;
            }

            case 'T':
            {
                MPB_SetTickRate(ADC_Sample(), MIDIHdr.PPQ);
                newSongFlag = 0;
                break;
            }

            case 'F':
            {
                uint32_t tmasterClock = MIDIHdr.masterClock / (4*MIDIHdr.PPQ);
                MPB_PlayMIDIFile(&MIDIHdr, filename);
                TimerStart();
                MIDIHdr.masterClock = (tmasterClock + 1) * (4*MIDIHdr.PPQ);

                if(MIDIHdr.masterClock > MIDIHdr.currentState.maxLength)
                {
                    MIDIHdr.masterClock = MIDIHdr.currentState.maxLength;
                }

                MPB_RePosition(&MIDIHdr, MIDIHdr.masterClock, MPB_PB_NO_NOTES);
                newSongFlag = 0;

                break;
            }

            case 'R':
            {
                uint32_t tmasterClock = MIDIHdr.masterClock / (4*MIDIHdr.PPQ);
                if( tmasterClock == 0)
                {
                    tmasterClock = 1;
                }
                MPB_PlayMIDIFile(&MIDIHdr, filename);
                TimerStart();
                MIDIHdr.masterClock = (tmasterClock - 1) * (4*MIDIHdr.PPQ);
                MPB_RePosition(&MIDIHdr, MIDIHdr.masterClock, MPB_PB_NO_NOTES);
                newSongFlag = 0;

                break;
            }

            case 'O':
            {
                uint32_t tmasterClock = MIDIHdr.masterClock / MIDIHdr.PPQ;
                MPB_PlayMIDIFile(&MIDIHdr, filename);
                MIDIHdr.masterClock = (tmasterClock + 1) * MIDIHdr.PPQ;
                MPB_RePosition(&MIDIHdr, MIDIHdr.masterClock, MPB_PB_NO_NOTES);
                newSongFlag = 0;
                break;
            }

            case 'L':
            {
                //MPB_PlayMIDIFile(&MIDIHdr, filename);
                MPB_DetermineLength(&MIDIHdr);
                newSongFlag = 0;
                break;
            }

            case 'P':
            {
                MPB_CurrentTimePosition(&MIDIHdr);
            }

            default:
                break;

        }

        //if(0)
        if (globalFlag)
        {
            MIDIHdr.masterClock++;
            globalFlag = 0;
            if( MPB_RePosition(&MIDIHdr, MIDIHdr.masterClock, MPB_PB_ALL_ON) == MPB_FILE_FINISHED )
            {
                myprintf("End of MIDI File: ", 1);
                T1CONbits.TON = 0x00;
            }
        }

    }

    //MPB_PlayTrack(&MIDIHdr, &MIDIHdr.Track[11]);
    while(1);
    

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
    globalFlag = 1;
    IFS0bits.T1IF = 0; //clear interrupt flag
}

void __attribute__((__interrupt__, no_auto_psv)) _ADC1Interrupt(void)
{
    IFS0bits.AD1IF = 0x00;
    uint16_t ADCValue = ADC1BUF0;

    LATA ^= 1;
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
    while (!(*U1.UXSTA&(1<<UTXBF))&& !ringbuffer_isEmpty((RINGBUFFER_T*)U1.TransmitBuffer))
    {
        *U1.UXTXREG = ringbuffer_get((RINGBUFFER_T*)U1.TransmitBuffer);
    }

}

void __attribute__((__interrupt__, no_auto_psv)) _U2TXInterrupt(void)
{
    IFS1bits.U2TXIF = 0; //clear interrupt flag
    while (!(*U2.UXSTA&(1<<UTXBF))&& !ringbuffer_isEmpty((RINGBUFFER_T*)U2.TransmitBuffer))
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

void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void)
{
    uint8_t i = *U1.UXRXREG;
    IFS0bits.U1RXIF = 0;
    //i = SPI_TxByte(&S1, i);
    //uartTx((PIC_USART_t*)&U1, i);
    static uint8_t byteCount = 0;


    if (i!=0x0D)
    {
        inputString[byteCount++] = i;
    }
    else
    {
        inputString[byteCount] = '\0';
        

        if( byteCount == 1)
        {
            newSongFlag = inputString[0];
        }
        else
        {
            newSongFlag = 1;
        }
        byteCount = 0;    
        //songIndex = (songIndex) % 2;
    }

}
//----------------------------------------------------------------------------
