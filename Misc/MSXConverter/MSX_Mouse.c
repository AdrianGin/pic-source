#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include "MSX_Mouse.h"


uint8_t MSX_SendPosition(uint8_t xPos, uint8_t yPos)
{
    //MSX_ValidateInputs(&xPos, &yPos);

    MSX_WAIT_FOR_HIGH();

    /* Wait for a High */
    if( (MSX_STROBE_PIN & MSX_STROBE) )
    {

        uint8_t outNib = 0;
        xPos = (~xPos) + 1;

        /* Check for a sustained high */
        _delay_us(55);
        if( !(MSX_STROBE_PIN & MSX_STROBE) )
        {
            MSX_POS_PORT &= ~(MSX_POS_PINS);
            return 0;
        }

        /* Transfer MSBs first */
        outNib = xPos;
        MSX_POS_PORT = (MSX_POS_PORT & ~(MSX_POS_PINS)) | (outNib & MSX_POS_PINS);

        /* Wait for a Low */
        MSX_WAIT_FOR_LOW();
        outNib = xPos << 4;
        MSX_POS_PORT = (MSX_POS_PORT & ~(MSX_POS_PINS)) | (outNib & MSX_POS_PINS);

        /* Wait for a High */
        MSX_WAIT_FOR_HIGH();
        outNib = yPos;
        MSX_POS_PORT = (MSX_POS_PORT & ~(MSX_POS_PINS)) | (outNib & MSX_POS_PINS);

        /* Wait for Low */
        MSX_WAIT_FOR_LOW();
        outNib = yPos << 4;
        MSX_POS_PORT = (MSX_POS_PORT & ~(MSX_POS_PINS)) | (outNib & MSX_POS_PINS);
        
        _delay_us(55);
        MSX_POS_PORT &= ~(MSX_POS_PINS);

        return 1;

    }

    return 0;
}



uint8_t MSX_SendButtons(uint8_t buttonMap)
{
    
    uint8_t buttonCount = 0;

    if( buttonMap & MSX_LEFT_TRIGGER )
    {
        MSX_TRIGGER_PORT &= ~MSX_LEFT_BUTTON_PIN;
        buttonCount++;
    }
    
    if( buttonMap & MSX_RIGHT_TRIGGER )
    {
        MSX_TRIGGER_PORT &= ~MSX_RIGHT_BUTTON_PIN;
        buttonCount++;
    }

    if( buttonCount )
    {
        //_delay_ms(20);
    }

    return buttonCount;


}


void MSX_ValidateInputs(int8_t* xPos, int8_t* yPos)
{
    if( *xPos > MSX_MAX_X_DATA )
    {
        *xPos = MSX_MAX_X_DATA;
    }
    
    if( *xPos < MSX_MIN_X_DATA )
    {
        *xPos = MSX_MIN_X_DATA;
    }


    if( *yPos > MSX_MAX_Y_DATA )
    {
        *yPos = MSX_MAX_Y_DATA;
    }
    
    if( *yPos < MSX_MIN_Y_DATA )
    {
        *yPos = MSX_MIN_Y_DATA;
    }
}



