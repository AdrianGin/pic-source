#ifndef _LCD_SETTINGS
#define _LCD_SETTINGS

#include "UI_LCD/UI_LCD.h"




extern HD44780lcd_t   PrimaryDisplay;

void UI_LCD_HWInit(void);
void UI_LCD_Strobe(void);
void UI_LCD_BL_Off(void);
void UI_LCD_BL_On(void);
void UI_LCD_BL_Toggle(void);
void UI_LCD_BLInit(uint16_t TimerPeriod);


void UI_LCD_LoadDefaultChars(void);
void UI_LCD_SetRegister(uint8_t data);


#endif

































