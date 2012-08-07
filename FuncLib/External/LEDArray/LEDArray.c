



#include <stdint.h>
#include <string.h>
#include "hardwareSpecific.h"
#include "SPI/spi.h"
#include "MSB2LSB/MSB2LSB.h"
#include "LEDArray/LEDArray.h"



uint8_t LEDArrayRAM[LEDARRAY_LEDCOUNT*LEDARRAY_COLOURS_PER_LED];

//We need to keep a copy of the current state. To know when to PWM.
uint8_t LEDArrayCurrentStateRAM[LEDARRAY_LEDCOUNT*LEDARRAY_COLOURS_PER_LED];


void _LA_ShiftOut(uint8_t byte)
{
    uint8_t i;
    for( i = 0; i < (BITS_PER_REGISTER); i++)
    {
        LA_OUT_PORT &= ~LA_SR_DAT;
        if(byte & (1<<(BITS_PER_REGISTER-1-i)))
        {
            LA_OUT_PORT |= LA_SR_DAT;
        }
        LA_OUT_PORT |= LA_SR_CLK;
        LA_OUT_PORT &= ~LA_SR_CLK;
    }
}

//Set the selected LED to a particular r,g,b colour
void LEDArray_SetLED(uint8_t index, uint8_t r, uint8_t g, uint8_t b)
{
    LEDArrayRAM[(index*LEDARRAY_COLOURS_PER_LED)+LEDARRAY_RED_LED] = r;
    LEDArrayRAM[(index*LEDARRAY_COLOURS_PER_LED)+LEDARRAY_GREEN_LED] = g;
    LEDArrayRAM[(index*LEDARRAY_COLOURS_PER_LED)+LEDARRAY_BLUE_LED] = b;
}

//Set the selected LED to a particular r,g,b colour
void LEDArray_AppendLED(uint8_t index, uint8_t r, uint8_t g, uint8_t b)
{
    if( r != 0 )
    {
        if(r >= LA_MAX_BRIGHTNESS)
        {
            r = 0;
        }
        LEDArrayRAM[(index*LEDARRAY_COLOURS_PER_LED)+LEDARRAY_RED_LED] = r;
    }

    if( g != 0 )
    {
        if(g >= LA_MAX_BRIGHTNESS)
        {
            g = 0;
        }
        LEDArrayRAM[(index*LEDARRAY_COLOURS_PER_LED)+LEDARRAY_GREEN_LED] = g;
    }

    if( b != 0 )
    {
        if(b >= LA_MAX_BRIGHTNESS)
        {
            b = 0;
        }
        LEDArrayRAM[(index*LEDARRAY_COLOURS_PER_LED)+LEDARRAY_BLUE_LED] = b;
    }
}


void LEDArray_Init(void)
{
    LA_OUT_DDR &= ~(LA_SR_CLK | LA_SR_DAT | LA_SR_LAT);
    LA_COL_DDR &= ~LA_COL_MASK;
    memset(LEDArrayRAM, 1 , sizeof(LEDArrayRAM));
    memset(LEDArrayCurrentStateRAM, 0 , sizeof(LEDArrayCurrentStateRAM));
}

void LEDArray_SetColumn(uint8_t column)
{
    uint16_t outByte = 0;

    outByte =  ((column&0x01)<<LA_COLA) | ((column&0x02)<<(LA_COLB-1))
              |((column&0x04)<<(LA_COLC-2)) | ((column&0x08)<<(LA_COLD-3));

    LA_COL_PORT = (LA_COL_PORT & ~LA_COL_MASK) | outByte;



}

#define LA_ALL_ZERO (1)
#define LA_NON_ZERO (0)
//Checks the ptr of (n length) for any non zero values
//returns true if the ptr is all zero.
uint8_t _check_zero(uint8_t* ptr, uint8_t n)
{
    uint8_t i;
    for( i = 0; i < n; i++ )
    {
        if( ptr[i] )
        {
            return LA_NON_ZERO;
        }
    }
    return LA_ALL_ZERO;
}


//ReDraw all the shift registers.
//We can be smart about this, since most LEDS won't be on,
//A lot of the GFX RAM will be zero, and hence we won't need to display
//them.
//This function should be re=entrant.


uint8_t LEDArray_ReDraw(uint8_t reEntry)
{
    uint8_t outByte[NUMBER_OF_SHIFT_REGISTERS];

    int16_t tempColour;
    uint8_t tempByte;
    uint8_t j;

    static uint8_t col = 0;
    static uint8_t cnt = 0;
    static uint16_t colPrintBitmap = 0;
    
    if( reEntry == LA_REDRAW_FINISHED)
    {
        colPrintBitmap = 0;
        memset(LEDArrayCurrentStateRAM, 0 , sizeof(LEDArrayCurrentStateRAM));
        for( col = 0; col < LA_COLUMN_COUNT; col++)
        {
            if (_check_zero( &LEDArrayRAM[(LA_ROW_COUNT*col)], LA_ROW_COUNT) == LA_NON_ZERO)
            {
                colPrintBitmap |= (1<<col);
            }
        }
        col = 0;
        if( colPrintBitmap == 0 )
        {
            //Update the registers will 0's
            //LEDArray_SetShiftRegister(0);
            //return LA_REDRAW_FINISHED;
        }   
    }

    //Go through each LED in multiples of the the ShiftRegister bits.
    memset(outByte, 0, sizeof(outByte));
    {

        //Here we convert the GFX RAM into Binary On/Offs for the Shift Registers.
        for(j = 0; j < LA_ROW_COUNT; j++)
        {
            uint16_t ledIndex = j + (LA_ROW_COUNT*col);
            tempByte = (LEDArrayRAM[ledIndex] % (LA_MAX_BRIGHTNESS+1));

            tempColour = LEDArrayCurrentStateRAM[ledIndex] + tempByte;
            if( tempColour >=  LA_MAX_BRIGHTNESS)
            {
                tempColour -= LA_MAX_BRIGHTNESS;
                outByte[j>>LA_SR_MASK] |= (1<<(j&LA_SR_BITMAP_MASK) );
            }
            LEDArrayCurrentStateRAM[ledIndex] = tempColour;
        }
    }
    //Update the register
    LEDArray_SetShiftRegister(outByte);
    //See photo 1, gives the last column time to turn off
    //before starting the new display. Otherwise it rolls
    //across! We still need the resistors however!
    LEDArray_OffDisplay();
    LEDArray_OffDisplay();
    //Turn on the column
    LEDArray_SetColumn(col);
    //Refer to picture 2, but there is no chance
    //of LEDs turning on with incorrect data.
    //Turn on time for PNP is much greater
    //than the time it takes to latch.
    LA_OUT_PORT |= LA_SR_LAT;
    LA_OUT_PORT &= ~LA_SR_LAT;


    col++;
    
    if( col >= LA_COLUMN_COUNT)
    {
        cnt++;
        col = 0;
    }

    if( cnt >= LA_MAX_BRIGHTNESS)
    {
        cnt = 0;
        return LA_REDRAW_FINISHED;
    }

    return LA_REDRAW_COL_FINISHED;

}





//ledStates correspondings to each of the shiftRegister bits.
//if LED states is 0, the shift register is cleared.
void LEDArray_SetShiftRegister(uint8_t* ledStates)
{
    uint8_t j;
    uint8_t* sr_state;
    //clear the current states
    //LA_OUT_PORT &= ~(LA_SR_CLK | LA_SR_DAT);

    //We have to output the data starting with the 'last' Shift Register as they
    //are daisy chained through Q7S.
    for( j = 0; j < NUMBER_OF_SHIFT_REGISTERS; j++)
    {
        if( ledStates )
        {
            sr_state = &ledStates[NUMBER_OF_SHIFT_REGISTERS-j-1];
            LA_TxByte(MSB2LSB(*sr_state));
        }
        else
        {
            LA_TxByte(0);
        }
    }
}







