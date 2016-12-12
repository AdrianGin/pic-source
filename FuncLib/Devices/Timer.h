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


#ifndef	_DEVICES_TIMER_H
#define	_DEVICES_TIMER_H

#include <stdint.h>

namespace Devices
{

class Timer
{

public:

   /**
    * The type of the callback invoked when the timer times out
    */
   typedef void (*TimeoutCallback)(void* context);

	enum ActiveState {
		DISABLED,
		ENABLED,
	};

	virtual void Init(void) = 0;
	virtual void SetCompare(uint8_t ch, uint16_t compare) = 0;
	virtual void SetPrescale(uint16_t prescaler) = 0;
	virtual void SetTop(uint16_t top) = 0;

	void SetCallback(TimeoutCallback cb, void* context);

	virtual void start(void) = 0;
	virtual void stop(void) = 0;


private:
	TimeoutCallback callback = 0;
	void* context = 0;


};







}













#endif




