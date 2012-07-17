#ifndef _LCD_INTERFACE_H
#define _LCD_INTERFACE_H


#include "LCDSettings.h"
#include "UI_LCD/UI_LCD.h"

void LCDInterface_Print(HD44780lcd_t* lcd, uint8_t* string, uint8_t n);
void LCD_SendChar(HD44780lcd_t* lcd, uint8_t byte);
void LCD_ClearLine(HD44780lcd_t* lcd, uint8_t row);

#endif

