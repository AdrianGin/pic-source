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

#ifndef _DEVICES_GPIO_H
#define _DEVICES_GPIO_H

#include <stdint.h>


namespace Devices
{

class GPIO {

public:
	enum Direction {
		INPUT,
		OUTPUT,
	};

	enum LogicLevel {
		LOW,
		HIGH
	};

	typedef void (*IntCallback)(void* context);

	LogicLevel currentlevel;
	Direction  currentDirection;

	//Initialises the GPIO with passed state
	virtual void Init( Direction state ) = 0;
	virtual void SetOutput( LogicLevel level) = 0;
	virtual uint8_t ReadInput() = 0;

	virtual uint8_t IsInterruptTriggered(void) = 0;

	virtual void EnableInterrupt(IntCallback cb, void* context);
	virtual void DisableInterrupt(void);

   IntCallback callback;
   void* context;

private:



};

}

#endif
