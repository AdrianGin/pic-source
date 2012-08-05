/* Project specific file for LCD Settings */
#include <stdint.h>
#include "UI_GLCD/UI_GLCD.h"
#include "gLCDSettings.h"


gLCD0108_t gPrimaryDisplay ={
    128, 64, 0, 0, 0,
    UI_gLCD_HWInit,
    UI_gLCD_SetRegister,
    UI_gLCD_Strobe,
    UI_gLCD_SelectController,
    0,0
};


void UI_gLCD_SelectController(uint8_t index)
{
    
    if(index == 0)
    {
        UI_gLCD_CONTROL_PORT |= UI_gLCD_CS;
    }
    else
    {
        UI_gLCD_CONTROL_PORT &= ~UI_gLCD_CS;
    }
}


void UI_gLCD_Strobe(void)
{
    /* Need to disable the SPI for a moment as we clock in the LCD data */
    SPI_DisablePort(&S1);
    UI_gLCD_CONTROL_PORT &= ~(1<<UI_gLCD_RS_PIN);
    UI_gLCD_CONTROL_PORT |= (gPrimaryDisplay.RSStatus<<UI_gLCD_RS_PIN);
    UI_gLCD_CONTROL_PORT |= UI_gLCD_E;
    //This needs to be slightly longer.
    _delay_us(1);
    UI_gLCD_CONTROL_PORT |= UI_gLCD_E;
    UI_gLCD_CONTROL_PORT |= UI_gLCD_E;
    UI_gLCD_CONTROL_PORT &= ~UI_gLCD_E;
    SPI_EnablePort(&S1);
}

/* Use a wrapper for the UI_MAX7300 interface to ensure LCD_Power is enabled
 * if any write commands are used */
void UI_gLCD_SetRegister(uint8_t data)
{
    SPI_SetSpeed((PIC_SPI_t*)&S1,PRESCALE_DIV3);
    SPI_TxByte((PIC_SPI_t*)&S1, data);
}

void UI_gLCD_HWInit(void)
{
    UI_gLCD_CONTROL_PORT &= ~(UI_gLCD_CONTROL);
    UI_gLCD_CONTROL_DIR  &= ~(UI_gLCD_CONTROL);
}
