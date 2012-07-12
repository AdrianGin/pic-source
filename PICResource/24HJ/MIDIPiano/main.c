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

//Number of keys with RGB LEDs
uint8_t LEDArrayRAM[88][3];

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
    DEBUG(string);
    myPrintNum(num, 16);
    DEBUG("\n");
}

void myprintfd(char* string, uint32_t num)
{
    DEBUG(string);
    myPrintNum(num, 10);
    DEBUG("\n");
}

void myPrintNum(uint32_t num, uint8_t base)
{
    char outputString[20];
    utoa(outputString, num, base);
    DEBUG(outputString);
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

void TimerStop(void)
{
    T1CONbits.TON = 0x00;
}

#define LED_CLK (1<<0)
#define LED_DAT (1<<1)

#define LED_BLUE    (0x01)
#define LED_GREEN   (0x02)
#define LED_RED     (0x04)

#define RED_POS     (11)
#define GREEN_POS   (5)
#define BLUE_POS    (0)

#define RGB(r,g,b)   ((r<<RED_POS) | (g<<GREEN_POS) | b)
#define BLUE_MAP     (0x1F)
#define GREEN_MAP    (0x3F << GREEN_POS)
#define RED_MAP      (0x1F << RED_POS)

#define MAX_BRIGHTNESS (127)

void Show8BitColour(uint16_t bitmap, uint8_t ledIndex, uint8_t r, uint8_t g, uint8_t b)
{
    int16_t i;
    uint8_t outByte;

    int16_t rc,gc,bc;

    r = (r % (MAX_BRIGHTNESS+1));
    g = (g % (MAX_BRIGHTNESS+1));
    b = (b % (MAX_BRIGHTNESS+1));
//    uint8_t r,g,b;
//    r = (bitmap & RED_MAP) >> RED_POS;
//    g = (bitmap & GREEN_MAP) >> GREEN_POS;
//    b = (bitmap & BLUE_MAP) >> BLUE_POS;

    for( i=0, rc=0, gc=0,bc=0; i < MAX_BRIGHTNESS ; i ++)
    {

        rc += r;
        gc += g;
        bc += b;

        outByte = 0;
        if( bc >=  MAX_BRIGHTNESS)
        {
            bc -= MAX_BRIGHTNESS;
            outByte |= LED_BLUE;
        }

        if( gc >=  MAX_BRIGHTNESS)
        {
            gc -= MAX_BRIGHTNESS;
            outByte |= LED_GREEN;
        }

        if( rc >=  MAX_BRIGHTNESS)
        {
            rc -= MAX_BRIGHTNESS;
            outByte |= LED_RED;
        }

        SendLED(outByte<<(ledIndex*3), ledIndex);
//        Delay(1);

//        uint16_t scanCount=0;
//        while( scanCount < 1 )
//        {
//            while( globalFlag )
//            {
//                scanCount++;
//                globalFlag = 0;
//            }
//        }


    }


//        globalFlag = 0;
//        while( globalFlag == 0 )
//        {
//        }
        
   

    
}

void SendLED(uint16_t bitmap, uint8_t ledIndex)
{
    uint8_t i;
    uint16_t scanCount=0;

    LATA &= ~(LED_CLK | LED_DAT);
    LATB |= (1<<15);
    for( i = 0; i < 16; i++)
    {
        LATA &= ~LED_DAT;
        if(bitmap & (1<<i))
        {
            LATA |= LED_DAT;
        }
        LATA |= LED_CLK;
        LATA &= ~LED_CLK;
        
    }

    LATB |= (1<<14);
    LATB &= ~(1<<14);
    {
        LATB &= ~(1<<15);
    } 
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
    TRISB &= ~(1<<15);
    TRISB &= ~(1<<14);

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


    //MPB_SetTickRate(32, 480);
    uint8_t tickCounter = 0;

    TimerStart();
    //For a 100us tick rate
    //PR1 = F_CPU/10000;
    uint16_t usPerTick = 7;
    PR1 = (F_CPU / 1000000)*usPerTick;

    uint16_t intCount = 0;
    uint8_t r,g,b,i;
    uint8_t* varPtr = &r;

    while(1)
    {
        //if( globalFlag )


        switch( newSongFlag )
        {
            case 'r':
                varPtr = &r;
                newSongFlag = 0;
                break;

            case 'g':
                varPtr = &g;
                newSongFlag = 0;
                break;

            case 'b':
                varPtr = &b;
                newSongFlag = 0;
                break;

            case '+':
                *varPtr = *varPtr + 1;
                newSongFlag = 0;
                break;

            case '-':
                *varPtr = *varPtr - 1;
                newSongFlag = 0;
                break;

            case 't':
                *varPtr = ADC_Sample()/(1024/MAX_BRIGHTNESS);
                //newSongFlag = 0;
                break;

            default:
                break;
        }

        
        {
            intCount++;
            {
//                for( r = 0; r < 4; r++ )
                {
//                    for( g = 0; g < 8; g++ )
                    {
//                        for( b = 1; b < 8; b++ )
                        {
                            //myprintfd("ADC:", *varPtr);
                            Show8BitColour( RGB(r,g,b) , 0 , r, g, b);
                            for( i = 0; i < 10; i++ )
                            {
                                Show8BitColour( RGB(0,0,0) , 1, 0, 0, 0 );
                            }
                          

//                            Show8BitColour( RGB(3-r,7-g,7-b) , 1 );
                            //Delay(200);
                        }
                    }

                }


//                for( r = 0; r < 4; r++ )
//                {
//                    for( g = 0; g < 8; g++ )
//                    {
//                        for( b = 1; b < 8; b++ )
//                        {
//                            Show8BitColour( RGB(3-r,7-g,7-b) , 0 );
//                            Show8BitColour( RGB(r,g,b) , 1 );
//                        }
//                    }
//                }
                //LATB |= (1<<15);
                intCount = 0;
            }
            
            globalFlag = 0;
        }
        
    }



    while(1)
    {

        switch( newSongFlag )
        {
            case 1:
            {
                strcpy(filename, inputString);
                MPB_ResetMIDI();
                MPB_PlayMIDIFile(&MIDIHdr, filename);
                //MPB_DetermineLength(&MIDIHdr);
               // MPB_PlayMIDIFile(&MIDIHdr, filename);
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
                
                tmasterClock = (tmasterClock + 1) * (4*MIDIHdr.PPQ);
                myprintf("tMClock: ", tmasterClock);
                myprintf("MClock: ", MIDIHdr.masterClock);
                myprintf("maxLen: ", MIDIHdr.currentState.maxLength);
                if(tmasterClock > MIDIHdr.currentState.maxLength)
                {
                    tmasterClock = MIDIHdr.currentState.maxLength;
                }
                myprintf("MClock: ", tmasterClock);
                //MPB_PlayMIDIFile(&MIDIHdr, filename);
                TimerStop();
                MPB_ResetMIDI();
                MPB_RePosition(&MIDIHdr, tmasterClock, MPB_PB_NO_NOTES);
                TimerStart();
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
                //MPB_PlayMIDIFile(&MIDIHdr, filename);
                TimerStop();
                MPB_ResetMIDI();
                tmasterClock = (tmasterClock - 1) * (4*MIDIHdr.PPQ);
                MPB_RePosition(&MIDIHdr, tmasterClock, MPB_PB_NO_NOTES);
                TimerStart();
                newSongFlag = 0;

                break;
            }

            case 'O':
            {
                uint32_t tmasterClock = MIDIHdr.masterClock / MIDIHdr.PPQ;
                MIDIHdr.masterClock = (tmasterClock + 1) * MIDIHdr.PPQ;
                TimerStop();
                MPB_ResetMIDI();
                MPB_RePosition(&MIDIHdr, MIDIHdr.masterClock, MPB_PB_NO_NOTES);
                TimerStart();
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
                newSongFlag = 0;
                break;
            }

            case 'i':
            {
                TimerStop();
                MPB_ResetMIDI();
                MPB_RePositionTime(&MIDIHdr, ADC_Sample()*MIDIHdr.currentState.trackLengthSecs / 1024, MPB_PB_NO_NOTES);
                TimerStart();
                newSongFlag = 0;
                break;
            }

            default:
                break;

        }

        //if(0)
        if (globalFlag)
        {

            
            //if( tickCounter == 4)
            {
                MIDIHdr.masterClock++;
                globalFlag = 0;
                if( MPB_ContinuePlay(&MIDIHdr, MPB_PB_ALL_ON) == MPB_FILE_FINISHED )
                {
                    myprintf("End of MIDI File: ", 1);
                    T1CONbits.TON = 0x00;
                }
            }

            if( (T1CONbits.TON) )
            {
                tickCounter++;
                if(tickCounter == (MIDIHdr.PPQ / 24))
                {
                    MIDI_Tx(0xF8);
                    tickCounter = 0;
                }
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
