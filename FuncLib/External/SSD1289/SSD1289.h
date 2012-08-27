#ifndef _SSD1289_INTERFACE_H
#define _SSD1289_INTERFACE_H

#include <stdint.h>
#include "hardwareSpecific.h"

#define SSD1289_CONTROL_PORT LATB
#define SSD1289_WR           (1<<3)
#define SSD1289_RS_PIN       (14)
#define SSD1289_RS           (1<<SSD1289_RS_PIN)

#define SSD1289_COMMAND     (0)
#define SSD1289_DATA        (1)

void SSD1289_SendCommand(uint16_t command, uint16_t data);
void SSD1289_LatchData(void);
void SSD1289_WriteCommand(uint16_t command);
void SSD1289_WriteData(uint16_t data);
void SSD1289_OutData(uint16_t data);
void SSD1289_Init(void);
void SSD1289_PrepareDataWrite(void);

void SSD1289_DispOneColor(uint16_t Color);
void SSD1289_PrepareNewScreen(void);
void SSD1289_PutPixel(uint16_t colour);

void SSD1289_SetMaxSpeed(void);

#endif

