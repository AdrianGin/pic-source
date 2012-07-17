
#ifndef _HARDWARE_SPECIFIC_H
#define _HARDWARE_SPECIFIC_H

#include <delay.h>
#include <string.h>

#include "p24Hxxxx.h"
#include "SPI/spi.h"
#include "hardUart24H/hardUart.h"
#include "SD_MMC/sddriver.h"
#include "dmaSPI/dmaSPI.h"
#include "LCDSettings.h"
#include "UI_LCD/LCDInterface.h"

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


/* Not used in Direct Mode */
#define UI_LCD_PORT     (0x00)

/* LCD Inputs */
#define UI_LCD_RS_PIN      (6)
#define UI_LCD_RS       (1<<6)
#define UI_LCD_DAT      (1<<6)
#define UI_LCD_CLK      (1<<1)
#define UI_LCD_E        (1<<3)
#define UI_LCD_D4       (1<<0)
#define UI_LCD_D5       (1<<1)
#define UI_LCD_D6       (1<<2)
#define UI_LCD_D7       (1<<3)

/* Use 8bit mode via 74HC164 */
//#define UI_LCD_8BITMODE
#define UI_LCD_4BITMODE
#define UI_LCD_BUFFERED

#define UI_LCD_SERIAL_DATA (UI_LCD_RS)
#define _delay_ms(x)    Delay(x)
#define _delay_us(x)    _delay_us(x)

#define VERSION_WITH_PE	(0x00)
#define VERSION_WITHOUT_PE (0x01)
#define VERSION_CODE (VERSION_WITHOUT_PE)

#define UI_LCD_CONTROL_DIR   (TRISB)
#define UI_LCD_DATA_DIR      (TRISB)

//#define UI_LCD_DATA	(UI_LCD_D4 | UI_LCD_D5 | UI_LCD_D6 | UI_LCD_D7)
#define UI_LCD_CONTROL	(UI_LCD_CLK | UI_LCD_DAT | UI_LCD_E)

#define UI_LCD_CONTROL_PORT	(LATB)
#define UI_LCD_DATA_PORT	(LATB)

#define UI_LCD_GET_FLASHBYTE(x)  (x)
#define LCD_FUNCTION_DEFAULT	((1<<LCD_FUNCTION) | (1<<LCD_FUNCTION_2LINES))
#define LCD_MODE_DEFAULT	((1<<LCD_ENTRY_MODE) | (1<<LCD_ENTRY_INC))
#define LCD_DISPLAY_DEFAULT   ((1<<LCD_DISPLAY) | (1<<LCD_ON_DISPLAY) | (1<<LCD_ON_BLINK))

#define LCD_FUNCTION_RESET   ((1<<LCD_FUNCTION) | (1<<LCD_FUNCTION_8BIT))
#define LCD_DISPLAY_RESET   ((1<<LCD_DISPLAY))

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

#define DEBUG(string)  uartTxString(&U2, string); LCDInterface_Print(&PrimaryDisplay, string, 0)
#define DEBUGn(string,n) uartTxDump(&U2, string, n); LCDInterface_Print(&PrimaryDisplay, string, n)

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
extern volatile uint8_t tempVar;

void myprintf(char* string, uint32_t num);
void myprintfd(char* string, uint32_t num);
void myprintNum(uint32_t num, uint8_t base);
void* local_readbuf(uint32_t position, uint8_t* buf, uint16_t size);
void TimerStart(void);

#endif

