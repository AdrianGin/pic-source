
#ifndef _LED_ARRAY_H
#define _LED_ARRAY_H

#include <stdint.h>


//Number of keys with RGB LEDs
#define LEDARRAY_LEDCOUNT   (88)
#define LEDARRAY_COLOURS_PER_LED (3)

#define LEDARRAY_BLUE_LED   (0)
#define LEDARRAY_RED_LED    (1)
#define LEDARRAY_GREEN_LED  (2)

#define LA_COL_DDR  TRISA
#define LA_COL_PORT LATA

#define LA_COL_MASK ((1<<LA_COLA) | (1<<LA_COLB) | (1<<LA_COLC) | (1<<LA_COLD))
#define LA_COLA     (1)
#define LA_COLB     (2)
#define LA_COLC     (3)
#define LA_COLD     (4)

#define LA_OUT_DDR  TRISB
#define LA_OUT_PORT LATB
#define LA_SR_CLK  (1<<0)
#define LA_SR_DAT  (1<<1)
#define LA_SR_LAT  (1<<2)

//#define LA_SR_MR_PORT LATB
//#define LA_SR_OE      (1<<LA_COL0)



#define BITS_PER_LED  (24)

#define BITS_PER_REGISTER         (8)
#define NUMBER_OF_SHIFT_REGISTERS (3)

//First 5 bits (bit of a cheat mask, to avoid a divide
#define LA_SR_MASK          (3)
//Last 3 bits
#define LA_SR_BITMAP_MASK   (0x07)

#define LA_MAX_BRIGHTNESS   (8)
#define LA_COLUMN_COUNT     (11)
#define LA_ROW_COUNT        (BITS_PER_REGISTER*NUMBER_OF_SHIFT_REGISTERS)

//Note that Data needs to be shifted out LSB first.
//#define LA_TxByte(x)    SPI_TxByte((PIC_SPI_t*)&S1, x)
#define LA_TxByte(x)    _LA_ShiftOut(x)

#define LEDArray_OffDisplay() LEDArray_SetColumn(LA_COLUMN_COUNT+1)

#define LA_REDRAW_NOT_FINISHED (0)
#define LA_REDRAW_COL_FINISHED (4)
#define LA_REDRAW_FINISHED (2)


void LEDArray_AppendLED(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
void LEDArray_Init(void);
void LEDArray_SetShiftRegister(uint8_t* ledStates);
uint8_t LEDArray_ReDraw(uint8_t reEntry);
void LEDArray_SetLED(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
void LEDArray_SetColumn(uint8_t column);

uint8_t _check_zero(uint8_t* ptr, uint8_t n);



#endif


