#ifndef _GLCD_SETTINGS
#define _GLCD_SETTINGS


extern gLCD0108_t   gPrimaryDisplay;

void UI_gLCD_HWInit(void);
void UI_gLCD_Strobe(void);
void UI_gLCD_SelectController(uint8_t index);
void UI_gLCD_LoadDefaultChars(void);
void UI_gLCD_SetRegister(uint8_t reg, uint8_t data);



#endif

































