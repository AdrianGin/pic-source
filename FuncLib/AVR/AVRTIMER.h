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

#ifndef _AVR_TIMER_H
#define _AVR_TIMER_H


#include <stdint.h>

namespace AVR
{

class TIMER16
{
public:

	enum eWaveformGenerationModes
	{
		NORMAL,
		PWM_PC_8BIT,
		PWM_PC_9BIT,
		PWM_PC_10BIT,
		CTC_OCR1A_TOP,
		FAST_PWM_8BIT,
		FAST_PWM_9BIT,
		FAST_PWM_10BIT,
		PWM_PFC_ICR1_TOP,
		PWM_PFC_OCR1A_TOP,
		PWM_PC_ICR1_TOP,
		PWM_PC_OCR1A_TOP,
		CTC_ICR1_TOP,
		RESERVED,
		FAST_PWM_ICR1_TOP,
		FAST_PWM_OCR1A_TOP,
	};

	enum eChannels
	{
		CHANNEL_A,
		CHANNEL_B,
	};

	enum eOutputModes
	{
		DISCONNECTED,
		TOGGLE_ON_MATCH,
		CLEAR_ON_MATCH,
		SET_ON_MATCH,
	};

	enum eClockSelect
	{
		NONE,
		CLK_DIV1,
		CLK_DIV8,
		CLK_DIV64,
		CLK_DIV256,
		CLK_DIV1024,
		EXT_FALLING,
		EXT_RISING,
	};

	TIMER16() noexcept;

	void Init( eWaveformGenerationModes mode);
	void SetPrescale(eClockSelect clockSelect);

	void SetOutputMode(eChannels ch, eOutputModes outmode);

	void SetCompare(eChannels ch, uint16_t compare);

	inline void ResetCount(void) {TCNT1 = 0;}
	inline void SetOCRA(uint16_t val) {OCR1A = val;}
	inline void SetOCRB(uint16_t val) {OCR1B = val;}
	inline void SetICR(uint16_t val) {ICR1 = val;}

private:



};


}
#endif
