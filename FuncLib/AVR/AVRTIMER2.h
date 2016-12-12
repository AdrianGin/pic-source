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

#ifndef _AVR_TIMER2_H
#define _AVR_TIMER2_H


#include <stdint.h>
#include "Timer.h"


/** Timer2 is 8 bit */
namespace AVR
{

class TIMER2 : public Devices::Timer
{
public:

   enum eWaveformGenerationModes
   {
      NORMAL,
      PWM_PC_8BIT,
      CTC_OCRA_TOP,
      FAST_PWM_8BIT,
      RESERVED0,
      PWM_PC_OCRA_TOP,
      RESERVED1,
      FAST_PWM_OCRA_TOP,
   };

	enum eOutputModes
	{
		DISCONNECTED,
		TOGGLE_ON_MATCH,
		CLEAR_ON_MATCH,
		SET_ON_MATCH,
	};


   enum eChannels
   {
      CHANNEL_A,
      CHANNEL_B,
   };


	enum eClockSelect
	{
		NONE,
		CLK_DIV1,
		CLK_DIV8,
		CLK_DIV32,
		CLK_DIV64,
		CLK_DIV128,
		CLK_DIV256,
		CLK_DIV1024,
	};

	TIMER2() noexcept;

	void Init(void);
	void Init( eWaveformGenerationModes mode);
	void SetPrescale(uint16_t /*eClockSelect*/ clockSelect);

	void SetOutputMode(uint8_t ch, eOutputModes outmode);

	void SetTop(uint16_t top);
	void SetCompare(uint8_t ch, uint16_t compare);

	void start(void);
	void stop(void);

	inline void ResetCount(void) {TCNT2 = 0;}
	inline void SetOCR2A(uint16_t val) {OCR2A = val;}
	inline void SetOCR2B(uint16_t val) {OCR2B = val;}

private:
	uint16_t prescaler;


};


}
#endif
