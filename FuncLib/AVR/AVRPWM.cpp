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
#include "AVRPWM.h"
#include "AVRGPIO.h"
#include "AVRTIMER.h"
#include "GPIO.h"

#include "Log.h"

using Devices::PWM;

namespace AVR
{

PWM::PWM(GPIO& pin, TIMER16& timer, TIMER16::eChannels ch) noexcept : pin(pin), timer(timer), ch(ch)
{

}

void PWM::Init(uint16_t top, uint16_t compare, uint16_t prescaler)
{
	pin.Init( Devices::GPIO::OUTPUT );
	pin.SetOutput(Devices::GPIO::LOW);

	timer.Init( TIMER16::FAST_PWM_ICR1_TOP);
	timer.SetOutputMode(ch, TIMER16::CLEAR_ON_MATCH);


	timer.SetPrescale((TIMER16::eClockSelect)prescaler);
	timer.SetICR(top);
	timer.SetCompare(ch, compare);

	SetNonInverting();

	Prescaler = prescaler;
}

void PWM::SetNonInverting(void)
{
	timer.SetOutputMode(ch, TIMER16::CLEAR_ON_MATCH);
}

void PWM::SetInverting(void)
{
	timer.SetOutputMode(ch, TIMER16::SET_ON_MATCH);
}

void PWM::SetCompare(uint16_t compare)
{
	timer.ResetCount();
	timer.SetCompare(ch, compare);
}

void PWM::enable(void)
{
	timer.SetPrescale( (TIMER16::eClockSelect)Prescaler);
	timer.Init( TIMER16::FAST_PWM_ICR1_TOP);
}

void PWM::disable(uint8_t logiclevel)
{
	timer.SetPrescale(TIMER16::eClockSelect::NONE);
	timer.Init( TIMER16::NORMAL);

	pin.SetOutput( (Devices::GPIO::LogicLevel)(logiclevel));

}


}
