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

#include "UI_LCD.h"
#include "hardwareSpecific.h"

#ifdef UI_LCD_BUFFERED
#include "stack/stack.h"
#include "RingBuffer/ringbuffer.h"
#endif





//uint8_t LCDOutputBuffer[LCD_OUTPUT_BUFFER_SIZE];
//RINGBUFFER_T LCDBuffer = {LCDOutputBuffer, sizeof (LCDOutputBuffer)};


/* Internal Functions */
void UI_LCD_SetInstruction(HD44780lcd_t* lcd)
{
    lcd->RSStatus = UI_LCD_RS_INSTRUCTION;
}

void UI_LCD_SetData(HD44780lcd_t* lcd)
{
    lcd->RSStatus = UI_LCD_RS_DATA;
}


#if VERSION_CODE == VERSION_WITH_PE	

/* Shutdown and Activate only used in special cases (ie. not edrum) */
void UI_LCD_Activate(void)
{
    UI_LCD_PowerStatus = LCD_ON;
    /* Set UI_LCD pins to to output LOW except for LCD Power which is high*/
    UI_SetRegister(UI_LCD_POWER, 0x01);
    /* Need to wait 40ms after applying power */
    _delay_ms(40);
}

void UI_LCD_Shutdown(void)
{
    /* Set UI_LCD pins to to output LOW */
    UI_SetRegister(UI_LCD_PORT, 0x00);
    UI_SetRegister(UI_LCD_POWER, 0x00);
    UI_LCD_PowerStatus = LCD_OFF;
}
#endif

/* Assumes 4-bit Mode, 4 MSB are sent first */
void UI_LCD_Write(HD44780lcd_t* lcd, char code)
{
#ifdef UI_LCD_BUFFERED
#ifdef UI_LCD_4BITMODE

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

    if( (bufLen + 1) >= LCD_OUTPUT_BUFFER_SIZE )
    {
        UI_LCD_FlushBuffer(lcd);
    }

    uint8_t bufMask = LCD_OUTPUT_BUFFER_SIZE-1;

    //adds it to the output buffer.
    lcd->LCDBuffer[*writeAddr&bufMask].data = (code);
    //The RS Status can change sometimes when flushing the buffer.
    lcd->LCDBuffer[*writeAddr&bufMask].RSState = rsStatus;
    *writeAddr = (*writeAddr + 1) & (bufMask);
#else
    LCDBuffer[LCDStack.writePtr].data = code;
    LCDBuffer[LCDStack.writePtr].RSState = lcd->RSStatus;
    STACK_PushData(&LCDStack, &LCDBuffer[LCDStack.writePtr]);
#endif

#else
#ifdef UI_LCD_8BITMODE
    lcd->SetRegister(code);
    lcd->Strobe();
#endif

#ifdef UI_LCD_4BITMODE
    lcd->SetRegister((code>>4) & (0x0F));
    lcd->Strobe();
    _delay_us(50);
    lcd->SetRegister((code) & (0x0F));
    lcd->Strobe();
    _delay_us(50);
#endif
#endif


}

#ifdef UI_LCD_BUFFERED

#define UPPERNIBBLE_FLAG (0x02)
#define RS_STATUS_FLAG   (0x01)
//Call this function once per about 3-milliseconds.


uint8_t UI_LCD_MainLoop(HD44780lcd_t* lcd)
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
            if( (lcd->RSStatus & RS_STATUS_FLAG) == UI_LCD_RS_INSTRUCTION)
            {
                uint8_t instructionType = 0;
                uint8_t i;
                for( i = 0; i < LCD_INSTRUCTION_COUNT; i++)
                {
                    if(fullByte & (1<<(LCD_INSTRUCTION_COUNT-1-i)))
                    {
                        instructionType = 1<<(LCD_INSTRUCTION_COUNT-1-i);
                        break;
                    }
                }

                switch(instructionType)
                {
                    case (1<<LCD_ENTRY_MODE):
                    case (1<<LCD_MOVE):
                    case (1<<LCD_FUNCTION):
                    case (1<<LCD_DISPLAY):
                    case (1<<LCD_CLR):
                    case (1<<LCD_HOME):
                        mainLoopCounter = LONG_WAIT;
                        break;

                    case (1<<LCD_DDRAM):
                    case (1<<LCD_CGRAM):
                        mainLoopCounter = STROBE_WAIT;
                        break;

                    default:
                        mainLoopCounter = STROBE_WAIT;
                        break;
                }
            }
#ifdef UI_LCD_4BITMODE
            lcd->SetRegister(fullByte >> 4);
            lcd->Strobe();
#endif                
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
#endif

void UI_LCD_FlushBuffer(HD44780lcd_t* lcd)
{
    while( UI_LCD_MainLoop(lcd) != LCD_OUTPUT_BUFFER_EMPTY)
    {
        _delay_us(MIN_EXECUTION_TIME);
    }
}

void UI_LCD_Char(HD44780lcd_t* lcd, char data)
{
    UI_LCD_SetData(lcd);
    UI_LCD_Write(lcd, data);
}

/* Clears the screen, sets up the LCD to accept commands */
void UI_LCD_Init(HD44780lcd_t* lcd)
{

#ifdef UI_LCD_8BITMODE
    UI_LCD_SetInstruction(lcd);
    UI_LCD_Write(lcd, LCD_FUNCTION_DEFAULT);
    UI_LCD_Write(lcd, LCD_FUNCTION_DEFAULT);
    UI_LCD_Write(lcd, LCD_FUNCTION_DEFAULT);
    UI_LCD_Write(lcd, LCD_FUNCTION_DEFAULT);
    UI_LCD_Write(lcd, LCD_DISPLAY_RESET);
    UI_LCD_Clear(lcd);
    UI_LCD_Write(lcd, LCD_MODE_DEFAULT);
    
    UI_LCD_Write(lcd, LCD_DISPLAY_DEFAULT);
    UI_LCD_Write(lcd, LCD_MODE_DEFAULT);
    UI_LCD_Clear(lcd);
    UI_LCD_Home(lcd);
#else

    UI_LCD_SetInstruction(lcd);
    /* Set to 8 - bit mode */

    //See initialisation by instructions.
    lcd->SetRegister(((LCD_FUNCTION_RESET)>>4) & (0x0F));
    lcd->Strobe();
    _delay_ms(10);

    lcd->SetRegister(((LCD_FUNCTION_RESET)>>4) & (0x0F));
    lcd->Strobe();
    _delay_ms(10);

    lcd->SetRegister(((LCD_FUNCTION_RESET)>>4) & (0x0F));
    lcd->Strobe();
    _delay_ms(10);

    lcd->SetRegister(((LCD_FUNCTION_DEFAULT)>>4) & (0x0F));
    lcd->Strobe();
    _delay_ms(10);
    
    UI_LCD_Write(lcd, LCD_FUNCTION_DEFAULT);
    UI_LCD_Write(lcd, LCD_DISPLAY_RESET);
    UI_LCD_Clear(lcd);
    UI_LCD_Write(lcd, LCD_MODE_DEFAULT);

    //End of intialisation

    //Some 'strange' reason, cannot do any Read/Writes without calling an extra:
    //however, this needs to be done AFTER a Data write? Strange as!
    UI_LCD_Clear(lcd);
    UI_LCD_Write(lcd, LCD_DISPLAY_DEFAULT);

#endif

}

/* Public Functions */
void UI_LCD_Home(HD44780lcd_t* lcd)
{
    UI_LCD_SetInstruction(lcd);
    UI_LCD_Write(lcd, (1<<LCD_HOME));
#ifndef UI_LCD_BUFFERED
    _delay_ms(2);
#endif
    lcd->RowPos = 0;
    lcd->ColPos = 0;
}


void UI_LCD_Clear(HD44780lcd_t* lcd)
{
    UI_LCD_SetInstruction(lcd);
    UI_LCD_Write(lcd, (1<<LCD_CLR));
#ifndef UI_LCD_BUFFERED
    _delay_ms(2);
#endif
    lcd->RowPos = 0;
    lcd->ColPos = 0;

}

void UI_LCD_StringDump(HD44780lcd_t* lcd, char* string, uint8_t n)
{
    UI_LCD_SetData(lcd);
    while (n--)
    {
        UI_LCD_Write(lcd, *string++);
    }
}

/* Prints a string to the LCD at the current position 
 * If the line overflows, the data continues to the next address */
void UI_LCD_String(HD44780lcd_t* lcd, char* string)
{
    UI_LCD_SetData(lcd);
    while (*string)
    {
        UI_LCD_Write(lcd, *string++);
    }
}

/* New Lines are done by using UI_LCD_Pos( row+1, 0); function */

/* Prints a string to the LCD at the current position 
 * If the line overflows, the data continues to the next address */
void UI_LCD_String_P(HD44780lcd_t* lcd, const char* string_P)
{
    uint8_t c;
    UI_LCD_SetData(lcd);

    while ((c = (uint8_t)UI_LCD_GET_FLASHBYTE(string_P++)))
    {
        UI_LCD_Write(lcd, c);
    }
}

/* Max Row is 3 and Max Col is 0 -> 19 (Dec) */
void UI_LCD_Pos(HD44780lcd_t* lcd, uint8_t row, uint8_t col)
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
    UI_LCD_SetInstruction(lcd);
    UI_LCD_Write(lcd, (1<<LCD_DDRAM)|DDRAMAddr);

}



void UI_LCD_SetCursor(HD44780lcd_t* lcd)
{
    UI_LCD_SetInstruction(lcd);

    /** Activates the display, cursor and cursor blink */
    UI_LCD_Write(lcd, (1<<LCD_DISPLAY)
            |(1<<LCD_ON_DISPLAY)
            |(1<<LCD_ON_CURSOR));
}

void UI_LCD_ClearCursor(HD44780lcd_t* lcd)
{
    UI_LCD_SetInstruction(lcd);

    /** Activates the display, cursor and cursor blink */
    UI_LCD_Write(lcd, (1<<LCD_DISPLAY)|(1<<LCD_ON_DISPLAY));
}

/* Accepts a 8 Byte custom char array */
//UI_LCD_LoadCustomChar(&PrimaryDisplay, (uint8_t*)LcdCustomChar[0], 0);
//UI_LCD_LoadCustomChar(&PrimaryDisplay, (uint8_t*)LcdCustomChar[1], 1);
//UI_LCD_LoadCustomChar(&PrimaryDisplay, (uint8_t*)LcdCustomChar[2], 2);
void UI_LCD_LoadCustomChar(HD44780lcd_t* lcd, uint8_t* lcdCustomCharArray, uint8_t lcdCharNum)
{
    uint8_t i;

    // multiply the character index by 8
    lcdCharNum = (lcdCharNum<<3); // each character occupies 8 bytes
    // copy the 8 bytes into CG (character generator) RAM
    UI_LCD_SetInstruction(lcd);
    // set CG RAM address
    UI_LCD_Write(lcd, (1<<LCD_CGRAM)|(lcdCharNum));

    /* Assumes auto increment of cursor position */
    for (i = 0; i<8; i++)
    {
#ifndef UI_LCD_BUFFERED
        _delay_us(200);
#endif
        UI_LCD_SetData(lcd);
        // write character data
        UI_LCD_Write(lcd, *(lcdCustomCharArray++));
    }
}

