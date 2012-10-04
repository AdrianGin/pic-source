
#include <stdint.h>
#include "Bitfields.h"

uint8_t unpackbits(uint8_t data, uint8_t startbits, uint8_t length)
{
    uint8_t mask = ((1 << length) - 1);
    return (data >> startbits) & mask;
}



uint32_t unpack128bits(uint8_t* data, uint8_t startbits, uint8_t length)
{
    uint8_t part = 15 - (startbits >> 3);
    uint8_t SubStart = 8 - (startbits - (startbits & 0xF8));
    SubStart &= 0x07;
    uint8_t tempLength;
    uint32_t ret = 0;
    uint8_t shiftCount = 0;

    while( length != 0)
    {
        if( SubStart)
        {
            tempLength = SubStart;
        }
        else
        {
            tempLength = length;
        }

        if( length <= 8 )
        {
            if( tempLength >= length )
            {
                tempLength = length;
            }
        }
        else
        {
            if( tempLength >= length )
            {
                tempLength = 8;
            }
        }


        length = length - tempLength;
        ret = ret | (unpackbits(data[part--], (startbits & 0x07), tempLength) << shiftCount);
        shiftCount = tempLength + shiftCount;

        startbits = startbits + tempLength;
        SubStart = 8 - (startbits - (startbits & 0xF8));
        SubStart &= 0x07;
    }
    return ret;
}

