
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "PS2_Mouse.h"


void PS2_Init(void)
{
    uint8_t dataIn = 0;
    while( dataIn != 0xAA )
    {
        PS2_GetByte(&dataIn);
    }

    while( dataIn != 0x00 )
    {
        PS2_GetByte(&dataIn);
    }

    dataIn = 0xFF;
    PS2_SendByte(0xFF);
    while( dataIn != 0x00 )
    {
        PS2_GetByte(&dataIn);
    }
    dataIn = 0xFF;



    /* Set Report Rate */
    /* Set sample rate to 20 */
    PS2_SendByte(0xF3);

    while( dataIn != 0xFA )
    {
        PS2_GetByte(&dataIn);
    }

    PS2_SendByte(80);
    while( dataIn != 0xFA )
    {
        PS2_GetByte(&dataIn);
    }

	//Set poll mode
	PS2_SendByte(0xF0);
    while( dataIn != 0xFA )
    {
        PS2_GetByte(&dataIn);
    }

	//Begin reporting steaming.
    PS2_SendByte(0xF4);

}




uint8_t PS2_GetPositionPacket(PS2_Position_t* event)
{
    uint8_t inByte;
	uint8_t dataIn = 0;
	uint8_t status = PS2_DATA_ERROR;
	uint8_t i;
    static uint8_t dataCount = 0;

	if( dataCount == 0 )
	{
		PS2_SendByte(0xEB);
		//Wait for ACK
		for(i =0; i < 250; i++)
		{
	        PS2_GetByte(&dataIn);
			if(dataIn == 0xFA)
			{
				break;
			}
	    }
	}



/*
    if( status == PS2_DATA_ERROR )
    {
        dataCount = 0;
        return 0;
    }*/

	for(i =0; i < 250; i++)
	{
    	status = PS2_GetByte(&inByte);

	    if( status == PS2_DATA_READY )
	    {
	        switch(dataCount)
	        {
	            case 0:
	                /* The firstByte must have 0x08 set */
	                if( !(inByte & 0x08) )
	                {
	                    dataCount = 0;
	                    return 0;
	                }
	                event->buttonMap = inByte;

	                break;

	            case 1:
	                event->xPos = inByte;
	                break;

	            case 2:
	                event->yPos = inByte;
	                break;

	        }



	        if( dataCount >= 2 )
	        {
	            dataCount = 0;
	            if( PS2_ValidateData(event) == 0 )
	            {
	                return 0;
	            }
	            return 1;
	        }
	        dataCount++;
	    }
	}


	dataCount = 0;
    return 0;
}


uint8_t PS2_ValidateData(PS2_Position_t* input)
{
 /* Do a final check here, ignore overflows */
    //if( (input->buttonMap & PS2_MOUSE_X_OVF) || (input->buttonMap & PS2_MOUSE_Y_OVF) )
    {
    //    return 0;
    }

    if( (input->buttonMap & PS2_MOUSE_X_SIGN) && (input->xPos <= 127) )
    {
        return 0;
    }

    if( (input->buttonMap & PS2_MOUSE_Y_SIGN) && (input->yPos <= 127) )
    {
        return 0;
    }

    if( !(input->buttonMap & PS2_MOUSE_X_SIGN) && (input->xPos >= 127) )
    {
        return 0;
    }

    if( !(input->buttonMap & PS2_MOUSE_Y_SIGN) && (input->yPos >= 127) )
    {
        return 0;
    }

    return 1;
}




void PS2_SendByte(uint8_t outByte)
{
    /* Send a RTS */
    PS2_OUT &= ~(PS2_CLK);
    PS2_DIR |= (PS2_CLK);
    _delay_us(120);

    PS2_CLR_DATA();
    PS2_DIR |= (PS2_DATA);

    PS2_DIR &= ~(PS2_CLK);
    PS2_OUT |= (PS2_CLK);


    uint8_t i;
    uint8_t parity = 0;
    for( i = 0; i < 8; i++ )
    {
        _delay_us(5);
        PS2_WAIT_CLK_LOW();
        _delay_us(5);

        PS2_CLR_DATA();
        if( outByte & 0x01 )
        {
            parity++;
            PS2_SET_DATA();
        }
        outByte = outByte >> 1;
        
        PS2_WAIT_CLK_HIGH();
        
    }

    /* Send Parity */
    PS2_WAIT_CLK_LOW();
    PS2_SET_DATA();
    if( parity & 0x01 )
    {
        PS2_CLR_DATA();
    }
    PS2_WAIT_CLK_HIGH();
    
    PS2_WAIT_CLK_LOW();
    /* Release Data line */
    PS2_SET_DATA();
    PS2_DIR &= ~(PS2_DATA);
    PS2_WAIT_CLK_HIGH();

    /* Wait until the line is ready */
    PS2_WAIT_DATA_LOW();
    PS2_WAIT_CLK_LOW();

    PS2_WAIT_CLK_HIGH();
    PS2_WAIT_DATA_HIGH();

    //_delay_ms(10);


}

uint8_t PS2_GetByte(uint8_t* inByte)
{
    uint8_t dataIn = 0;
    uint8_t parity = 0;

    PS2_DIR &= ~(PS2_CLK | PS2_DATA);
    if( !(PS2_STARTBIT) )
    {
        return PS2_NO_DATA;
    }

    /* Wait for High */
    PS2_WAIT_CLK_HIGH();

    uint8_t i;
    for( i = 0; i < 8; i++ )
    {
        /* Wait for Low */
        PS2_WAIT_CLK_LOW();
    
        dataIn = dataIn >> 1;

        if( PS2_DATABIT )
        {
            parity++;
            dataIn |= 0x80;
        }
    
        /* Wait for High */
        PS2_WAIT_CLK_HIGH();
    }

    /* Wait for Low */
    PS2_WAIT_CLK_LOW();
    if( !((parity + PS2_DATABIT) & 0x01) )
    {
        /* Parity Bit failed! */
        return PS2_DATA_ERROR;
    }
    PS2_WAIT_CLK_HIGH();

    /* Wait for stop Low */
    PS2_WAIT_CLK_LOW();
    if( !PS2_DATABIT )
    {
        /* Stop bit must be 0! */
        return PS2_DATA_ERROR;
    }

    PS2_WAIT_CLK_HIGH();

    *inByte = dataIn;

    

    return PS2_DATA_READY;
}
