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

#include "LEDArray/LEDArray.h"

#include "SD_MMC/sd.h"

#include "MIDIPlayback/midiplayback.h"

#include "mmculib/uint16toa.h"
#include "mmculib/root.h"
#include "adc/adc.h"

#include "UI_GLCD/UI_GLCD.h"
#include "UI_GLCD/gLCDInterface.h"
#include "UI_GLCD/gLCDSettings.h"

#include "SSD1289/SSD1289.h"
#include "BMPDraw/BMPDraw.h"
#include "MSB2LSB/MSB2LSB.h"


void InterruptHandlerLow();

volatile PIC_USART_t U1 = {&U1STA, &U1MODE, &U1BRG, &U1TXREG, &U1RXREG, &IFS0, TX1IF, &TransmitBuffer, &ReceiveBuffer};
volatile PIC_USART_t U2 = {&U2STA, &U2MODE, &U2BRG, &U2TXREG, &U2RXREG, &IFS1, TX2IF, &TransmitBuffer2, &TransmitBuffer2};
PIC_USART_t* PrimaryUART;
LINKED_LIST_t PrimaryList;

PIC_SPI_t S1 = {&SPI1STAT, &SPI1CON1, &SPI1CON2, &SPI1BUF};

volatile uint8_t globalFlag = 0;
volatile uint8_t byteCount = 0;

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


BMPFile_t BMPFile;


PGM_P ROMSTRING = "ROM STRING\n";
char RAMSTRING[] = "RAM STRING\n";

//FIL testFIL;
//uint16_t br;
//uint8_t buffer[BUFFER_READ_SIZE];


volatile uint8_t newSongFlag = 0;
volatile uint8_t songIndex = 0;
volatile uint8_t ProcessBufferFlag;
//volatile uint8_t ProcessBufferFlag;

volatile char inputString[80];
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
    OC1CONbits.OCTSEL = 0x00; //TMR2 for Period
    OC1RS = 39;
    T1CONbits.TON = 1;

    PR2 = 60;
    OC2RS = 16;
    OC2CONbits.OCM = 0x06;
    OC2CONbits.OCTSEL = 0x00; //TMR2 for Period
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

    //Setup Timer2 too
    IPC1bits.T2IP = 0x01;
    IFS0bits.T2IF = 0x00;
    IEC0bits.T2IE = 0x01;
    T2CONbits.TON = 0x00;
}

void TimerStart(void)
{
    T1CONbits.TON = 0x01;
}

void TimerStop(void)
{
    T1CONbits.TON = 0x00;
}

uint8_t ReadAnalogueInput(void)
{
    uint16_t sample;
    sample = Button_GetInput();
    myprintfd("Input:", sample);
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

    TRISB &= ~(1<<15);
    TRISB &= ~(1<<14);

    TRISB &= ~(1<<8);
    TRISB &= ~(1<<9);

    LATB |= (1<<SD_CS_PIN);
    TRISB &= ~(1<<SD_CS_PIN);

    LEDArray_Init();

    TRISA &= ~((1<<2) | (1<<4) | (1<<3));


    PPSUnLock;
    PPSOutput(OUT_FN_PPS_U2TX, OUT_PIN_PPS_RP8);
    PPSOutput(OUT_FN_PPS_U1TX, OUT_PIN_PPS_RP9);
    PPSInput(PPS_U2RX, PPS_RP10);
    PPSInput(PPS_U1RX, PPS_RP11);


    //PPSOutput(OUT_FN_PPS_OC1, OUT_PIN_PPS_RP15);
    //PPSOutput(OUT_FN_PPS_OC2, OUT_PIN_PPS_RP14);

    PPSOutput(OUT_FN_PPS_SCK1, OUT_PIN_PPS_RP5);
    PPSInput(PPS_SDI1, PPS_RP6);
    PPSOutput(OUT_FN_PPS_SDO1, OUT_PIN_PPS_RP7);
    

    PPSLock;

    uartInit(PrimaryUART);
    uartSetBaud(PrimaryUART, 320);
    uartInit((PIC_USART_t*)&U2);
    uartSetBaud((PIC_USART_t*)&U2, 320);
    
//    PMD1bits.U1MD = 1;
//    PMD1bits.U2MD = 1;
    SET_CPU_IPL(0);

    //Baud rate of 38400 bps for Bluetooth Init
    uartSetBaud(PrimaryUART, 320);
    uartSetBaud((PIC_USART_t*)&U2, 320);

    
    PWMInit();
    
    //TRISA |= (1<<1);
    //LATA = 0;
    Delay(60);
    //SendString("Welcome executive:\n");
    //SendString("My Name\n");
    //SendString("is Adrian123\n");

    //Delay_Us(Delay_1S_Cnt);
    
    //TimerInit();

    ADC_Init();
    ADC_Set10bit();
    ADC_SetClockSpeed(5, 4);
    ADC_SetPin(11);
    //ADC_Off();

    SPI_Init(&S1);
    
    Delay(10);
    uint16_t ADCValue;

    LATB |= SSD1289_WR;
    TRISB &= ~SSD1289_WR;
    TRISB &= ~SSD1289_RS;

    SSD1289_SetMaxSpeed();
    SSD1289_Init();
    
    uint8_t ret;
    DMA_SPI_Init(&DMASPI1, &DMASPI1T, DMA_SPI1);
    DMA_SPI_Enable();
    
    ret = f_mount(0, &filesys);
    myprintfd("Mount:", ret);

    myprintf("SPISPEED:", *S1.SPIXCON1);
    FIL temp;

    BMPFile.pImageFile = &temp;

    ret = f_open( BMPFile.pImageFile, "bride.bmp", FA_OPEN_EXISTING | FA_READ );

    //myprintf("SPISPEED:", *S1.SPIXCON1);

    myprintfd("Open:", ret);
    
    BMP_ReadHeader(&BMPFile);

    myprintfd("BMP Width:", BMPFile.lWidth);
    myprintfd("BMP Length:", BMPFile.lHeight);
    myprintfd("BMP Planes:", BMPFile.bNumOfPlanes);
    myprintfd("BMP Depth:", BMPFile.bBitsPerPixel);
    myprintfd("BMP CompType:", BMPFile.blCompressionType);

    //ret = BMP_Print(&BMPFile);
    myprintfd("PrintSuccess:", ret);

    uint32_t CSDbuf[4];
    memset(&CSDbuf, 0x00, 16);
    uint8_t k;
    disk_ioctl(0, MMC_GET_CID, &CSDbuf);


    CIDStruct_t CIDStruct;
    uint32_t* resp = &CSDbuf[0];
    uint8_t* ptr = &CSDbuf[0];

    for(k = 0; k < sizeof(CSDbuf); k++ )
    {
        ptr[k] = MSB2LSB(ptr[k]);
    }


    for(k = 0; k < sizeof(CSDbuf) / 2; k++ )
    {
        uint8_t  tempint;
        tempint = ptr[k];
        ptr[k] = ptr[sizeof(CSDbuf)-k];
        ptr[sizeof(CSDbuf)-k] = tempint;
    }

    for(k = 0; k < sizeof(CSDbuf) / 4; k++ )
    {
        uint32_t  tempint;
        tempint = CSDbuf[k];
        CSDbuf[k] = CSDbuf[(sizeof(CSDbuf)/4)-k];
        CSDbuf[(sizeof(CSDbuf)/4)-k] = tempint;
    }


    SD_PopulateCID(&CIDStruct, resp);


//    myprintfd("Sizeof", sizeof(CIDStruct_t));
//
    myprintf("MANF ID:", CIDStruct.manfid);
    myprintf("OEM ID:", CIDStruct.oemid);
//
    DEBUG("ProdName:")
    DEBUGn(CIDStruct.prod_name, sizeof(CIDStruct.prod_name) );
//
    myprintf("prodRev:", CIDStruct.prodRev);
//
//    DEBUG("serialNumber:")
//    DEBUGn(CIDStruct.serialNumber, sizeof(CIDStruct.serialNumber) );
//
    myprintf("Year:", CIDStruct.year);
    myprintf("Month:", CIDStruct.month);
//
    for( k = 0; k < 16; k++ )
    {
        myprintf("CID:", ptr[k]);
    }

    CSDStructV1_t CSDStruct;
    disk_ioctl(0, MMC_GET_CSD, &CSDbuf);
    SD_PopulateCSD(&CSDStruct, resp);

    myprintf("CSD:", CSDStruct.CSD_Structure);
    myprintf("TRANS_SPEED:", CSDStruct.TRANS_SPEED);
    myprintf("C_SIZE:", CSDStruct.C_SIZE);
    myprintf("C_SIZE_MULT:", CSDStruct.C_SIZE_MULT);
    myprintf("READ_BL_LEN:", CSDStruct.READ_BL_LEN);


    for( k = 0; k < 16; k++ )
    {
        myprintf("CSD:", ptr[k]);
    }


//
    
//    while(1)
//    {
//        SSD1289_DispOneColor(0xffff);
//        SSD1289_DispOneColor(0x07e0);
//        SSD1289_DispOneColor(0xFFE0);
//        SSD1289_DispOneColor(0xF800);
//    }

    while(1)
    {

        ret = f_open( BMPFile.pImageFile, "bride.bmp", FA_OPEN_EXISTING | FA_READ );
        myprintfd("Open:", ret);
        BMP_ReadHeader(&BMPFile);
        myprintfd("BMP Width:", BMPFile.lWidth);
        myprintfd("BMP Length:", BMPFile.lHeight);
        myprintfd("BMP Planes:", BMPFile.bNumOfPlanes);
        myprintfd("BMP Depth:", BMPFile.bBitsPerPixel);
        myprintfd("BMP CompType:", BMPFile.blCompressionType);
        ret = BMP_Print(&BMPFile);
        myprintfd("PrintSuccess:", ret);

        DELAY_MS(600);

        ret = f_open( BMPFile.pImageFile, "Tic.bmp", FA_OPEN_EXISTING | FA_READ );
        BMP_ReadHeader(&BMPFile);
        BMP_Print(&BMPFile);
        DELAY_MS(600);

        ret = f_open( BMPFile.pImageFile, "fiona.bmp", FA_OPEN_EXISTING | FA_READ );
        BMP_ReadHeader(&BMPFile);
        BMP_Print(&BMPFile);
        DELAY_MS(600);

    }

    LEDArray_SetColumn(15);

    

//    UI_gLCD_HWInit();
//
//    UI_gLCD_Init(&gPrimaryDisplay);
//    UI_gLCD_Char(&gPrimaryDisplay, 0xAA);
//    UI_gLCD_Char(&gPrimaryDisplay, 0x00);
//    UI_gLCD_Char(&gPrimaryDisplay, 0xFF);
//    UI_gLCD_Char(&gPrimaryDisplay, 0xFF);
//    uint8_t i;
//    while(1)
//    {
//        for(i = 0; i < 240; i++)
//        {
//            UI_gLCD_Char(&gPrimaryDisplay, 0xFF);
//            UI_gLCD_MainLoop(&gPrimaryDisplay);
//            UI_gLCD_MainLoop(&gPrimaryDisplay);
//            Delay(200);
//        }
//        for(i = 0; i < 240; i++)
//        {
//            UI_gLCD_Char(&gPrimaryDisplay, 0x00);
//            UI_gLCD_MainLoop(&gPrimaryDisplay);
//            UI_gLCD_MainLoop(&gPrimaryDisplay);
//            Delay(200);
//        }
//    }
//
//    while(1);
//


    
    MPB_SetTickRate(32, 480);
    uint16_t tickCounter = 0;

    TimerStart();
    //For a 100us tick rate
    //PR1 = F_CPU/10000;
    //uint16_t usPerTick = 1;
    //PR1 = (F_CPU / 1000000)*usPerTick;

    uint16_t intCount = 0;
    uint8_t r = 0, g = 0, b = 0;
    uint8_t* varPtr = &r;

    uint8_t mode = 0;


    PR2 = 2400;
    T2CONbits.TCKPS = 0x00;
    T2CONbits.TON = 0x01;

    
    uint16_t counter = 0;
    //LEDArray_SetLED(87, 7,0,0);
    uint8_t readPtr = 0;

    while(1)
    {


        //LATA ^= (1<<4);
        switch( newSongFlag )
        {

            case 'm':
                mode ^= 1;
                newSongFlag = 0;
                break;
                
            case 1:
            {
                if(mode)
                {
                    strcpy(filename, inputString);
                    MPB_ResetMIDI();
                    //MPB_DetermineLength(&MIDIHdr);
                   // MPB_PlayMIDIFile(&MIDIHdr, filename);
                    if( MPB_PlayMIDIFile(&MIDIHdr, filename) == FR_OK )
                    {
                        TimerStart();
                    }
                    else
                    {
                        TimerStop();
                        //File not found!
                    }
                }
                else
                {
                    myprintfd("IntA:", atoi(inputString));
                    intCount = atoi(inputString);
                    LEDArray_SetLED(atoi(inputString), 127, 0, 0);
                    //inputString;
                }
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
                *varPtr = ADC_Sample()/(1024/LA_MAX_BRIGHTNESS);
                //newSongFlag = 0;
                break;

            case 'y':
                myprintfd("Int:", intCount);
                LEDArray_SetLED(intCount++, 127, 0, 0);
                newSongFlag = 0;
                break;

            case 'l':
                UI_LCD_LoadDefaultChars();
                newSongFlag = 0;
                break;


            default:
                break;

        }

        //LEDArray_SetLED(intCount, r, g, b);
        //LEDArray_SetLED(0, r, g, b);
        if( ringbuffer_len((RINGBUFFER_T*)U1.ReceiveBuffer) )
        {
            LCD_SendChar(&PrimaryDisplay, ringbuffer_get((RINGBUFFER_T*)U1.ReceiveBuffer));
            globalFlag &= ~(0x04);
        }

        if( globalFlag & 0x02 )
        {
            RunCriticalTimer();
            globalFlag &= ~(0x02);
        }

        //if(0)
        if (globalFlag & 0x01)
        {
            //if( tickCounter == 4)
            {
                MIDIHdr.masterClock++;
                globalFlag &= ~1;
                if( MPB_ContinuePlay(&MIDIHdr, MPB_PB_ALL_ON) == MPB_FILE_FINISHED )
                {
                    myprintf("End of MIDI File:  ", 1);

                    TimerStop();
                }
            }

            if( (T1CONbits.TON) )
            {
                tickCounter++;

                uint8_t k;
                for(k = 0;(k < MIDI_MAX_KEY+1); k++)
                {
                    if( MIDIHdr.NoteOnTimer[k] )
                    {
                        MIDIHdr.NoteOnTimer[k]--;
                        if(MIDIHdr.NoteOnTimer[k] == 0)
                        {
                            LEDArray_SetLED(0, 0, 0, 0);
                            LEDArray_AppendLED(87, 0, 0, LA_MAX_BRIGHTNESS);
                        }
                    }
                }

                if( (tickCounter == (MIDIHdr.PPQ / 24)) )
                //if(tickCounter == 255)
                {
                    tickCounter = 0;
                    //MIDI_Tx(0xF8);
                    
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
    globalFlag |= 0x01;
    IFS0bits.T1IF = 0; //clear interrupt flag
}

void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void)
{
    globalFlag |= 0x02;
    static uint8_t tempVar;
    tempVar = LEDArray_ReDraw(tempVar);
    IFS0bits.T2IF = 0; //clear interrupt flag
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


    static uint8_t counter = 0;

//    counter ++;
//
//    if( counter == 3)
//    {
//        uartSetBaud((PIC_USART_t*)&U2, 320);
//    }
//    if( counter == 6 )
//    {
//        uartSetBaud((PIC_USART_t*)&U2, 260);
//        counter = 0;
//    }

    uartTx((PIC_USART_t*)&U2, i);


}

void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void)
{
    uint8_t i = *U1.UXRXREG;
    
    //i = SPI_TxByte(&S1, i);
    uartTx((PIC_USART_t*)&U2, i);
    IFS0bits.U1RXIF = 0;
    return;
    

    ringbuffer_put((RINGBUFFER_T*)U1.ReceiveBuffer, i);

    globalFlag |= 0x04;
    //LCD_SendChar(&PrimaryDisplay, i);

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
