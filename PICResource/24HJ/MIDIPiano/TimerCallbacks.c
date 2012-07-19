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

#include "hardwareSpecific.h"
#include "SoftTimer/Softtimer.h"
#include "LEDArray/LEDArray.h"
#include "LCDSettings.h"
#include "TimerCallbacks.h"

/* These are the critical timers, 10ms resolution */
SoftTimer_8 SoftTimer1[TIMER1_COUNT] = {
    {2, 0, 1, Callback_LEDUpdate}, // LED Output
    {1, 0, 1, Callback_LCDUpdate},
};


void Callback_LEDUpdate(void)
{
    static uint8_t tempVar = LA_REDRAW_FINISHED;
    tempVar = LEDArray_ReDraw(tempVar);
}

void Callback_LCDUpdate(void)
{
    UI_LCD_MainLoop(&PrimaryDisplay);
}
