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
#include "AVRTIMER2.h"

namespace AVR
{

TIMER2::TIMER2() noexcept
{

}


void TIMER2::Init(void)
{
   Init(NORMAL);
}

void TIMER2::Init(eWaveformGenerationModes mode)
{
	TCCR2A &= ~(0x03<<WGM20);
	TCCR2B &= ~(0x01<<WGM22);

	TCCR2A |=  ((mode & 0x03) << WGM20);
	TCCR2B |=  (((mode & 0x08)>>2) << WGM22);

	TCNT2 = 0;
	prescaler = NONE;
}


void TIMER2::SetPrescale(uint16_t clockSelect)
{
	TCCR2B &= ~(0x07<<CS20);
	TCCR2B |= (clockSelect<<CS20);
	prescaler = clockSelect;
}

void TIMER2::SetOutputMode(uint8_t ch, eOutputModes mode)
{
	uint8_t bitIndex;

	if( ch == CHANNEL_A )
	{
		bitIndex = COM2A0;
	}
	else
	{
		bitIndex = COM2B0;
	}

	TCCR2A &= ~(0x03 << bitIndex);
	TCCR2A |= (mode << bitIndex);
}

void TIMER2::SetCompare(uint8_t ch, uint16_t compare)
{
	if( ch == CHANNEL_A )
	{
		SetOCR2A(compare);
	}
	else
	{
		SetOCR2B(compare);
	}
}

/** The mode must be set correctly otherwise this has no effect / invalid effect */
void TIMER2::SetTop(uint16_t top)
{
   SetOCR2A(top);
}

void TIMER2::start(void)
{
   SetPrescale(prescaler);
}

void TIMER2::stop(void)
{
   SetPrescale(NONE);
}


}
