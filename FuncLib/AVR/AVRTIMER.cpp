/*

Copyright (c) 2016 Adrian Gin (adrian.gin[at]gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/



#include <avr/io.h>
#include "AVRTIMER.h"
#include "Log.h"

namespace AVR
{

TIMER16::TIMER16() noexcept
{

}


void TIMER16::Init(eWaveformGenerationModes mode)
{
	TCCR1A &= ~(0x03<<WGM10);
	TCCR1B &= ~(0x03<<WGM12);

	TCCR1A |=  ((mode & 0x03) << WGM10);
	TCCR1B |=  (((mode & 0x0C)>>2) << WGM12);

	TCNT1 = 0;
}

void TIMER16::SetPrescale(eClockSelect clockSelect)
{
	TCCR1B &= ~(0x07<<CS10);
	TCCR1B |= (clockSelect<<CS10);
}

void TIMER16::SetOutputMode(eChannels ch, eOutputModes mode)
{
	uint8_t bitIndex;

	if( ch == CHANNEL_A )
	{
		bitIndex = COM1A0;
	}
	else
	{
		bitIndex = COM1B0;
	}

	TCCR1A &= ~(0x03 << bitIndex);
	TCCR1A |= (mode << bitIndex);
}

void TIMER16::SetCompare(eChannels ch, uint16_t compare)
{
	if( ch == CHANNEL_A )
	{
		SetOCRA(compare);
	}
	else
	{
		SetOCRB(compare);
	}

}

}
