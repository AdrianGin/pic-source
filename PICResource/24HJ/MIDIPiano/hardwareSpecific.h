
#ifndef _HARDWARE_SPECIFIC_H
#define _HARDWARE_SPECIFIC_H

#include <delay.h>
#include <string.h>

#include "p24Hxxxx.h"
#include "SPI/spi.h"
#include "hardUart24H/hardUart.h"
#include "SD_MMC/sddriver.h"
#include "dmaSPI/dmaSPI.h"

#ifndef F_CPU
#define F_CPU   39469000
#endif

#define USE_FAT_FS  (1)

#if USE_FAT_FS==1
#include "FatFS/diskio.h"
#include "FatFS/ff.h"
#else
#include "PetitFS/diskio.h"
#include "PetitFS/pff.h"
#endif


#define SPI_DDR   (TRISB)
#define SPI_PORT  (LATB)
#define SCK       (5)
#define MISO      (7)
#define MOSI      (6)
#define nSS       (4)

#define SD_CS_DDR   TRISB
#define SD_CS_PORT  LATB
#define SD_CS_PIN   nSS

#define WAVE_LEFT_REG    OC1RS
#define WAVE_RIGHT_REG   OC2RS

#define DELAY_MS(x)    Delay_Us(x*100)
#define DELAY_US(x)    Delay(x)

#define SD_RXBYTE()    SPI_RxByte((PIC_SPI_t*)&S1)
//#define SD_RXBLOCK(buffer, bytes)   SPI_RxBlock(&S1, buffer, bytes)
#define SD_RXBLOCK(buffer, bytes)   DMA_SPI_ReceiveBlock(&DMASPI1, &DMASPI1T, buffer, bytes)
#define SD_TXBYTE(x)   SPI_TxByte((PIC_SPI_t*)&S1, x)

#define DEBUG(string)  uartTxString(&U2, string)
#define DEBUGn(string,n) uartTxDump(&U2, string, n)

#define SD_STARTUP()        SD_Startup()
#define SD_SETMAXSPEED()    SD_SetMaxSpeed()


#define uint8toa(num,str)	utoa(str,num, 10)

//#define SD_PWR_DDR  DDRB
//#define SD_PWR_PORT PORTB
//#define SD_PWR_PIN  PB0

extern PIC_SPI_t S1;
extern PIC_USART_t* PrimaryUART;
extern volatile PIC_USART_t U2;

extern PIC_DMA_SPI_t DMASPI1;
extern PIC_DMA_SPI_t DMASPI1T;

extern FATFS filesys;

extern FIL testFIL;
extern uint16_t br;
extern uint8_t buffer[];
extern volatile uint8_t globalFlag;

void myprintf(char* string, uint32_t num);
void myprintfd(char* string, uint32_t num);
void myprintNum(uint32_t num, uint8_t base);
void* local_readbuf(uint32_t position, uint8_t* buf, uint16_t size);
void TimerStart(void);

#endif

