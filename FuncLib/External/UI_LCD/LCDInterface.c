


#include <stdint.h>
#include <string.h>
#include "UI_LCD/UI_LCD.h"
#include "UI_LCD/LCDInterface.h"
#include "hardwareSpecific.h"


void LCDInterface_Print(HD44780lcd_t* lcd, uint8_t* string, uint8_t n)
{
    if( n != 0 )
    {
    }
    else
    {
        n = strlen( (char*)string);
    }

    //If the string goes on for more than line,
    //start at the next line.
    if( (n + lcd->ColPos) > (lcd->MAX_COL+1) && (lcd->ColPos != 0) )
    {
        LCD_ClearLine(lcd, lcd->RowPos+1);
    }
    
    while (n)
    {
        LCD_SendChar(lcd, *string++);
        n = n - 1;
    }
}

void LCD_ClearLine(HD44780lcd_t* lcd, uint8_t row)
{
    UI_LCD_Pos(lcd, row, 0);
    uint8_t i;
    for( i = 0; i <= lcd->MAX_COL; i++ )
    {
        UI_LCD_Char(lcd, ' ');
    }
    UI_LCD_Pos(lcd, row, 0);
}

void LCD_SendChar(HD44780lcd_t* lcd, uint8_t byte)
{
    static uint8_t pendingClear = 0;
    if( pendingClear )
    {
        //LCD_ClearLine(lcd, 0);
        //LCD_ClearLine(lcd, 2);
        //LCD_ClearLine(lcd, 1);
        //LCD_ClearLine(lcd, 0);
        UI_LCD_Clear(lcd);
        pendingClear = 0;
    }

    if( (byte == 0x0D) || (byte == 0x0A) )
    {
        pendingClear = 1;
    }
    else
    {
        if( lcd->ColPos > lcd->MAX_COL )
        {
            LCD_ClearLine(lcd, lcd->RowPos+1);
        }
        UI_LCD_Char(lcd, byte);
        lcd->ColPos++;
    }
}






















