


#include <stdint.h>
#include <string.h>
#include "hardwareSpecific.h"

#include "SPI/spi.h"
#include "SSD1289/SSD1289.h"




typedef struct
{
} SSD1289_t;

uint8_t RS;

void SSD1289_SendCommand(uint16_t command, uint16_t data)
{
    SSD1289_WriteCommand(command);
    SSD1289_PrepareDataWrite();
    SSD1289_WriteData(data);
}

void SSD1289_LatchData(void)
{
    SSD1289_CONTROL_PORT &= ~SSD1289_WR;
    //This needs to be slightly longer.
    SSD1289_CONTROL_PORT |= SSD1289_WR;
}

void SSD1289_WriteCommand(uint16_t command)
{
    SSD1289_OutData(command);
    SSD1289_CONTROL_PORT &= ~SSD1289_RS;
    SSD1289_LatchData();
}

void SSD1289_PrepareDataWrite(void)
{
    SSD1289_CONTROL_PORT |= (SSD1289_DATA<<SSD1289_RS_PIN);
}

void SSD1289_WriteData(uint16_t data)
{
    SSD1289_OutData(data);
    SSD1289_LatchData();
}

void SSD1289_OutData(uint16_t data)
{
    //SSD1289_SetMaxSpeed();
    //SSD1289_SendByte((data >> 8) & 0xFF);
    SSD1289_SendByte(data);
}

void SSD1289_Init(void)
{
    SSD1289_SendCommand(0x0000,0x0001);    DELAY_US(1);
    SSD1289_SendCommand(0x0003,0xA8A4);    DELAY_US(1);
    SSD1289_SendCommand(0x000C,0x0000);    DELAY_US(1);
    SSD1289_SendCommand(0x000D,0x080C);    DELAY_US(1);
    SSD1289_SendCommand(0x000E,0x2B00);    DELAY_US(1);
    SSD1289_SendCommand(0x001E,0x00B0);    DELAY_US(1);
    SSD1289_SendCommand(0x0001,0x2B3F);    DELAY_US(1);
    SSD1289_SendCommand(0x0002,0x0600);    DELAY_US(1);
    SSD1289_SendCommand(0x0010,0x0000);    DELAY_US(1);
    SSD1289_SendCommand(0x0011,0x6070);    DELAY_US(1);
    SSD1289_SendCommand(0x0005,0x0000);    DELAY_US(1);
    SSD1289_SendCommand(0x0006,0x0000);    DELAY_US(1);
    SSD1289_SendCommand(0x0016,0xEF1C);    DELAY_US(1);
    SSD1289_SendCommand(0x0017,0x0003);    DELAY_US(1);
    SSD1289_SendCommand(0x0007,0x0233);    DELAY_US(1);
    SSD1289_SendCommand(0x000B,0x0000);    DELAY_US(1);
    SSD1289_SendCommand(0x000F,0x0000);    DELAY_US(1);
    SSD1289_SendCommand(0x0041,0x0000);    DELAY_US(1);
    SSD1289_SendCommand(0x0042,0x0000);    DELAY_US(1);
    SSD1289_SendCommand(0x0048,0x0000);    DELAY_US(1);
    SSD1289_SendCommand(0x0049,0x013F);    DELAY_US(1);
    SSD1289_SendCommand(0x004A,0x0000);    DELAY_US(1);
    SSD1289_SendCommand(0x004B,0x0000);    DELAY_US(1);
    SSD1289_SendCommand(0x0044,0xEF00);    DELAY_US(1);
    SSD1289_SendCommand(0x0045,0x0000);    DELAY_US(1);
    SSD1289_SendCommand(0x0046,0x013F);    DELAY_US(1);
    SSD1289_SendCommand(0x0030,0x0707);    DELAY_US(1);
    SSD1289_SendCommand(0x0031,0x0204);    DELAY_US(1);
    SSD1289_SendCommand(0x0032,0x0204);    DELAY_US(1);
    SSD1289_SendCommand(0x0033,0x0502);    DELAY_US(1);
    SSD1289_SendCommand(0x0034,0x0507);    DELAY_US(1);
    SSD1289_SendCommand(0x0035,0x0204);    DELAY_US(1);
    SSD1289_SendCommand(0x0036,0x0204);    DELAY_US(1);
    SSD1289_SendCommand(0x0037,0x0502);    DELAY_US(1);
    SSD1289_SendCommand(0x003A,0x0302);    DELAY_US(1);
    SSD1289_SendCommand(0x003B,0x0302);    DELAY_US(1);
    SSD1289_SendCommand(0x0023,0x0000);    DELAY_US(1);
    SSD1289_SendCommand(0x0024,0x0000);    DELAY_US(1);
    SSD1289_SendCommand(0x0025,0x8000);    DELAY_US(1);
    SSD1289_SendCommand(0x004f,0);
    SSD1289_SendCommand(0x004e,0);
    
}

void SSD1289_SetMaxSpeed(void)
{
    SPI_SetSpeed(&S1, PRESCALE_DIV3);
    SPI_SetDataMode(&S1, MODE_16BIT);
}

void SSD1289_PrepareNewScreen(void)
{
    SSD1289_SendCommand(0x004f,0);
    SSD1289_SendCommand(0x004e,0);
    SSD1289_WriteCommand(0x22);
    SSD1289_PrepareDataWrite();
}

void SSD1289_PutPixel(uint16_t colour)
{
    SSD1289_WriteData(colour);
}

void SSD1289_DispOneColor(uint16_t Color)
{
    int i,j,k;
    SSD1289_SendCommand(0x004f,0);
    SSD1289_SendCommand(0x004e,0);
    SSD1289_WriteCommand(0x22);
    SSD1289_PrepareDataWrite();

    for(j=0;j<320;j++)
        for(i=0;i<240;i++)
            SSD1289_WriteData(Color);

    LATB ^= (1<<15);

    
}












