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
#include "AVRGPIO.h"
#include "GPIO.h"

using Devices::GPIO;

namespace AVR
{

GPIO::GPIO(volatile uint8_t& DDR, volatile uint8_t& PORT, volatile uint8_t& PIN, uint8_t pn) noexcept:
		DDR(DDR), PORT(PORT), PIN(PIN), pinNumber(pn)
{

}

void GPIO::Init( Direction dir )
{
	if( dir == INPUT )
	{

		DDR &= ~(1 << pinNumber);
	}
	else
	{
		DDR |= (1 << pinNumber);
	}
	currentDirection = dir;
}

void GPIO::SetOutput( LogicLevel level)
{
	if( level == currentlevel )
	{
		return;
	}
	PIN = (1<<pinNumber);
	currentlevel = level;
}


uint8_t GPIO::ReadInput(void)
{

	return (PIN & (1<<pinNumber)) ? 1 : 0;
}

}
