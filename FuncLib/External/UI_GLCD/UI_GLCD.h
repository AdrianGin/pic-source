/*

Copyright (c) 2011 Adrian Gin (adrian.gin[at]gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/



/** \file UI_gLCD.h
	 \brief LCD Routines
*/

/**	 

	\ingroup avr_external
 	\defgroup UI_gLCD HD44780 LCD Interface
 	\code #include "uartInput.h" \endcode

 	 
	 \par Description
    This module utilises the MAX7300 Port Expander to interface with
    a 20x4 Alphanumeric LCD.
*/
//@{
   

#ifndef	_UI_GLCD_ROUTINES
#define	_UI_GLCD_ROUTINES

//#include "version.h"
#include <stdint.h>
#include "hardwareSpecific.h"

#define UI_gLCD_DISPLAY_ON      (0x3F)


#define UI_gLCD_RS_INSTRUCTION (0)
#define UI_gLCD_RS_DATA        (1)
#define UI_gLCD_BUFFERED
/* To be defined in hardware Specific */
/*
#define LCD_FUNCTION_DEFAULT	((1<<LCD_FUNCTION) | (1<<LCD_FUNCTION_2LINES) | (LCD_FUNCTION_8BIT))
#define LCD_MODE_DEFAULT		((1<<LCD_ENTRY_MODE) | (1<<LCD_ENTRY_INC))
#define LCD_DISPLAY_DEFAULT   ((1<<LCD_DISPLAY) | (1<<LCD_ON_DISPLAY))
*/

/* Not used in Direct Mode */
#define UI_gLCD_PORT     (0x00)

/* LCD Inputs */
#define UI_gLCD_RS_PIN      (6)
#define UI_gLCD_RS       (1<<6)
#define UI_gLCD_DAT      (1<<6)
#define UI_gLCD_CLK      (1<<1)
#define UI_gLCD_E        (1<<3)
#define UI_gLCD_D4       (1<<0)
#define UI_gLCD_D5       (1<<1)
#define UI_gLCD_D6       (1<<2)
#define UI_gLCD_D7       (1<<3)

#define UI_gLCD_CS       (1<<4)

/* Use 8bit mode via 74HC164 */
//#define UI_gLCD_8BITMODE
#define UI_gLCD_BUFFERED

#define UI_gLCD_SERIAL_DATA (UI_gLCD_RS)
#define _delay_ms(x)    Delay(x)
#define _delay_us(x)    _delay_us(x)

#define UI_gLCD_CONTROL_DIR   (TRISB)
#define UI_gLCD_DATA_DIR      (TRISB)

//#define UI_gLCD_DATA	(UI_gLCD_D4 | UI_gLCD_D5 | UI_gLCD_D6 | UI_gLCD_D7)
#define UI_gLCD_CONTROL	(UI_gLCD_CLK | UI_gLCD_DAT | UI_gLCD_E | UI_gLCD_CS)

#define UI_gLCD_CONTROL_PORT	(LATB)
#define UI_gLCD_DATA_PORT	(LATB)

#define UI_gLCD_GET_FLASHBYTE(x)  (x)

typedef struct
{
    uint8_t data;
    uint8_t RSState;
    
} gLCDData_t;

//The fastest execution time of an instruction
#define MIN_EXECUTION_TIME (60)
enum
{
    gNO_WAIT = 0,
    gSTROBE_WAIT,
    gLONG_WAIT = 50,
};

#define LCD_OUTPUT_BUFFER_SIZE  (16)

typedef struct
{   
   /* User to specify the dimensions of the LCD */
   uint8_t MAX_ROW;
   uint8_t MAX_COL;
   
   volatile uint8_t RSStatus;
   volatile uint8_t RowPos;
   volatile uint8_t ColPos;
   
   /* Set up data direction registers */
   void (*HWInit)(void);
   /* Sets the data on the 8/4 bit bus */
   void (*SetRegister)(uint8_t data);
   
   /* Strobes the clock 'E' pin */
   void (*Strobe)(void);
   
   /* Backlight functionality */
   void (*SelectController)(uint8_t index);
   void (*BL_Off)(void);

    uint8_t writePtr;
    uint8_t readPtr;
    gLCDData_t LCDBuffer[LCD_OUTPUT_BUFFER_SIZE];
   
} gLCD0108_t;

extern const uint8_t LcdCustomChar[][8]; 


uint8_t UI_gLCD_MainLoop(gLCD0108_t* lcd);

void UI_gLCD_Init(gLCD0108_t* lcd);
void UI_gLCD_Write(gLCD0108_t* lcd, char code);
void UI_gLCD_SetInstruction(gLCD0108_t* lcd);
void UI_gLCD_SetData(gLCD0108_t* lcd);

void UI_gLCD_Char(gLCD0108_t* lcd, char data);

void UI_gLCD_StringDump(gLCD0108_t* lcd, char* string, uint8_t n);
void UI_gLCD_String(gLCD0108_t* lcd, char* string);
void UI_gLCD_String_P(gLCD0108_t* lcd, const char* string_P);

void UI_gLCD_Char(gLCD0108_t* lcd, char data);

void UI_gLCD_Clear(gLCD0108_t* lcd);
void UI_gLCD_Home(gLCD0108_t* lcd);
void UI_gLCD_Pos(gLCD0108_t* lcd, uint8_t row, uint8_t col);

void UI_gLCD_FlushBuffer(gLCD0108_t* lcd);

void UI_gLCD_SetCursor(gLCD0108_t* lcd);
void UI_gLCD_ClearCursor(gLCD0108_t* lcd);

void UI_gLCD_Activate(void);
void UI_gLCD_Shutdown(void);



/* Accepts a 7 Byte custom char array */
void UI_gLCD_LoadCustomChar(gLCD0108_t* lcd, uint8_t* lcdCustomCharArray, uint8_t lcdCharNum);



#endif
