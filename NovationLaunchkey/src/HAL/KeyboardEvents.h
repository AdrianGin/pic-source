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



#ifndef _KEYBOARD_EVENTS
#define _KEYBOARD_EVENTS

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "voidbuffer.h"

typedef struct
{
	uint8_t status; //so we can use Note On / Off
	uint8_t note;
	uint8_t velocity;
} KeyboardEvent_t;

#define HAS_EVENT (1)
#define KEYBOARD_EVENT_MSG_COUNT (16)

extern volatile VoidBuffer_t KeyboardMsgQueue;

uint8_t KeyboardEvents_AddEvent(KeyboardEvent_t* event);
uint8_t KeyboardEvents_GetEvent(KeyboardEvent_t* event);

#ifdef __cplusplus
}
#endif

#endif
