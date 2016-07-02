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

#ifndef _AVR_PWM_H
#define _AVR_PWM_H


#include "PWM.h"
#include "AVRGPIO.h"
#include "AVRTIMER.h"
#include <stdint.h>

using Devices::PWM;

namespace AVR
{

class PWM : public Devices::PWM
{
public:
	PWM(GPIO& pin, TIMER16& timer, TIMER16::eChannels ch) noexcept;

	void Init(uint16_t top, uint16_t compare, uint16_t prescaler);
	void SetCompare(uint16_t compare);

	void enable(void);
	void disable(void);

private:
	GPIO& pin;
	TIMER16& timer;
	TIMER16::eChannels ch;

	uint16_t Prescaler;
};


}
#endif
