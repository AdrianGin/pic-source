/*

Copyright (c) 2011 Adrian Gin (adrian.gin[at]gmail.com)

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

#include "KeyboardEvents.h"

KeyboardEvent_t KeyboardMsgArray[KEYBOARD_EVENT_MSG_COUNT];


VoidBuffer_t KeyboardMsgQueue =
{
		.memPtrArray = (void*)KeyboardMsgArray,
		.bufferSize = KEYBOARD_EVENT_MSG_COUNT,
		.elementSize = sizeof(KeyboardEvent_t),
};

uint8_t KeyboardEvents_AddEvent(KeyboardEvent_t* event)
{
	return VoidBuffer_PushData(&KeyboardMsgQueue, (void*) event);
}

KeyboardEvent_t KeyboardEvents_GetEvent(void)
{
	KeyboardEvent_t* event;
	event = VoidBuffer_PopData(&KeyboardMsgQueue);

	if( event != 0 )
	{
		return *event;
	}

	return 0;
}

















