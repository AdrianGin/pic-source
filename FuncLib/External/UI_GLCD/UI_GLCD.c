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

#include "UI_GLCD.h"
#include "hardwareSpecific.h"

#ifdef UI_gLCD_BUFFERED
#include "stack/stack.h"
#include "RingBuffer/ringbuffer.h"
#endif



/* Internal Functions */
void UI_gLCD_SetInstruction(gLCD0108_t* lcd)
{
    lcd->RSStatus = UI_gLCD_RS_INSTRUCTION;
}

void UI_gLCD_SetData(gLCD0108_t* lcd)
{
    lcd->RSStatus = UI_gLCD_RS_DATA;
}


/* Assumes 4-bit Mode, 4 MSB are sent first */
void UI_gLCD_Write(gLCD0108_t* lcd, char code)
{
    uint8_t bufLen;
    uint8_t rsStatus = lcd->RSStatus;
    uint8_t* writeAddr = &lcd->writePtr;
    uint8_t* readAddr = &lcd->readPtr;
    
    if( *writeAddr >= *readAddr )
    {
        bufLen = *writeAddr - *readAddr;
    }
    else
    {
        bufLen = *writeAddr + LCD_OUTPUT_BUFFER_SIZE - *readAddr;
    }

    if( (bufLen+1) >= LCD_OUTPUT_BUFFER_SIZE )
    {
        UI_gLCD_FlushBuffer(lcd);
    }

    uint8_t bufMask = LCD_OUTPUT_BUFFER_SIZE-1;

    //adds it to the output buffer.
    lcd->LCDBuffer[*writeAddr&bufMask].data = (code);
    //The RS Status can change sometimes when flushing the buffer.
    lcd->LCDBuffer[*writeAddr&bufMask].RSState = rsStatus;
    *writeAddr = (*writeAddr + 1) & (bufMask);
}

#define UPPERNIBBLE_FLAG (0x02)
#define RS_STATUS_FLAG   (0x01)
//Call this function once per about 3-milliseconds.
uint8_t UI_gLCD_MainLoop(gLCD0108_t* lcd)
{
    //for 4bit modes
    //stange that this needs to be volatile??? not sure why
    static uint8_t mainLoopCounter = 0;
    uint8_t fullByte;
    LCDData_t* lcdData;
    uint8_t* writeAddr = &lcd->writePtr;
    uint8_t* readAddr = &lcd->readPtr;
    
    if( *readAddr != *writeAddr || (mainLoopCounter != 0))
    {

        if( mainLoopCounter == 0)
        {
            lcdData = &lcd->LCDBuffer[*readAddr];
            fullByte = lcdData->data;
            lcd->RSStatus = lcdData->RSState;
            mainLoopCounter = STROBE_WAIT;
                
            lcd->SetRegister(fullByte);
            lcd->Strobe();
            *readAddr = (*readAddr + 1) & (LCD_OUTPUT_BUFFER_SIZE-1);
            return LCD_OUTPUT_SENT_ONE_BYTE;
        }
        else
        {
            mainLoopCounter--;
        }
        return LCD_OUTPUT_BUFFER_NOT_EMPTY;
    }
    return LCD_OUTPUT_BUFFER_EMPTY;
}


void UI_gLCD_FlushBuffer(gLCD0108_t* lcd)
{
    while( UI_gLCD_MainLoop(lcd) != LCD_OUTPUT_BUFFER_EMPTY)
    {
        _delay_us(MIN_EXECUTION_TIME);
    }
}

void UI_gLCD_Char(gLCD0108_t* lcd, char data)
{
    UI_gLCD_SetData(lcd);
    UI_gLCD_Write(lcd, data);
}

/* Clears the screen, sets up the LCD to accept commands */
void UI_gLCD_Init(gLCD0108_t* lcd)
{
    UI_gLCD_SetInstruction(lcd);
    /* Set to 8 - bit mode */
    UI_gLCD_Write(lcd, UI_gLCD_DISPLAY_ON);

}

/* Public Functions */
void UI_gLCD_Home(gLCD0108_t* lcd)
{
    UI_gLCD_SetInstruction(lcd);
    UI_gLCD_Write(lcd, (1<<LCD_HOME));
#ifndef UI_gLCD_BUFFERED
    _delay_ms(2);
#endif
    lcd->RowPos = 0;
    lcd->ColPos = 0;
}


void UI_gLCD_Clear(gLCD0108_t* lcd)
{
    UI_gLCD_SetInstruction(lcd);
    UI_gLCD_Write(lcd, (1<<LCD_CLR));
#ifndef UI_gLCD_BUFFERED
    _delay_ms(2);
#endif
    lcd->RowPos = 0;
    lcd->ColPos = 0;

}

void UI_gLCD_StringDump(gLCD0108_t* lcd, char* string, uint8_t n)
{
    UI_gLCD_SetData(lcd);
    while (n--)
    {
        UI_gLCD_Write(lcd, *string++);
    }
}

/* Prints a string to the LCD at the current position 
 * If the line overflows, the data continues to the next address */
void UI_gLCD_String(gLCD0108_t* lcd, char* string)
{
    UI_gLCD_SetData(lcd);
    while (*string)
    {
        UI_gLCD_Write(lcd, *string++);
    }
}

/* New Lines are done by using UI_gLCD_Pos( row+1, 0); function */

/* Prints a string to the LCD at the current position 
 * If the line overflows, the data continues to the next address */
void UI_gLCD_String_P(gLCD0108_t* lcd, const char* string_P)
{
    uint8_t c;
    UI_gLCD_SetData(lcd);

    while ((c = (uint8_t)UI_gLCD_GET_FLASHBYTE(string_P++)))
    {
        UI_gLCD_Write(lcd, c);
    }
}

/* Max Row is 3 and Max Col is 0 -> 19 (Dec) */
void UI_gLCD_Pos(gLCD0108_t* lcd, uint8_t row, uint8_t col)
{
    uint8_t DDRAMAddr = 0;
    lcd->RowPos = row;
    lcd->ColPos = col;

    // remap lines into proper order
    switch (row)
    {
        case 0:
            DDRAMAddr = LCD_LINE0_DDRAMADDR+col;
            break;

        case 1:
            DDRAMAddr = LCD_LINE1_DDRAMADDR+col;
            break;

        case 2:
            DDRAMAddr = LCD_LINE2_DDRAMADDR+col;
            break;

        case 3:
            DDRAMAddr = LCD_LINE3_DDRAMADDR+col;
            break;
        default:
            DDRAMAddr = LCD_LINE0_DDRAMADDR + col;
            lcd->RowPos = 0;
            break;
    }
    // set data address
    UI_gLCD_SetInstruction(lcd);
    UI_gLCD_Write(lcd, (1<<LCD_DDRAM)|DDRAMAddr);

}



void UI_gLCD_SetCursor(gLCD0108_t* lcd)
{
    UI_gLCD_SetInstruction(lcd);

    /** Activates the display, cursor and cursor blink */
    UI_gLCD_Write(lcd, (1<<LCD_DISPLAY)
            |(1<<LCD_ON_DISPLAY)
            |(1<<LCD_ON_CURSOR));
}

void UI_gLCD_ClearCursor(gLCD0108_t* lcd)
{
    UI_gLCD_SetInstruction(lcd);

    /** Activates the display, cursor and cursor blink */
    UI_gLCD_Write(lcd, (1<<LCD_DISPLAY)|(1<<LCD_ON_DISPLAY));
}

/* Accepts a 8 Byte custom char array */
//UI_gLCD_LoadCustomChar(&PrimaryDisplay, (uint8_t*)LcdCustomChar[0], 0);
//UI_gLCD_LoadCustomChar(&PrimaryDisplay, (uint8_t*)LcdCustomChar[1], 1);
//UI_gLCD_LoadCustomChar(&PrimaryDisplay, (uint8_t*)LcdCustomChar[2], 2);
void UI_gLCD_LoadCustomChar(gLCD0108_t* lcd, uint8_t* lcdCustomCharArray, uint8_t lcdCharNum)
{
    uint8_t i;

    // multiply the character index by 8
    lcdCharNum = (lcdCharNum<<3); // each character occupies 8 bytes
    // copy the 8 bytes into CG (character generator) RAM
    UI_gLCD_SetInstruction(lcd);
    // set CG RAM address
    UI_gLCD_Write(lcd, (1<<LCD_CGRAM)|(lcdCharNum));

    /* Assumes auto increment of cursor position */
    for (i = 0; i<8; i++)
    {
#ifndef UI_gLCD_BUFFERED
        _delay_us(200);
#endif
        UI_gLCD_SetData(lcd);
        // write character data
        UI_gLCD_Write(lcd, *(lcdCustomCharArray++));
    }
}

