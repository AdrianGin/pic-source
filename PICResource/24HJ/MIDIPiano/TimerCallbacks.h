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



#ifndef _TIMER_CALLBACKS_H
#define _TIMER_CALLBACKS_H

#include "SoftTimer/Softtimer.h"
#include "LEDArray/LEDArray.h"

/* CCR1B associated timers */
enum {  
   SC_LEDUpdate = 0,
   SC_LCDUpdate,
   SC_ScanInput,
   TIMER1_COUNT
};

/* CCR2B associated timers */
enum {  
   SC_OneSecond = 0,
   SC_AutoMenuUpdate,
   SC_VUMeterUpdate,
   SC_DigitalVUUpdate,
   SC_VUDecay,
   SC_AboutUpdate,
   /* LCD_BL_Period should be the last T2 SoftTimer */
   SC_LCD_BL_Period,
   SC_MonitorChannel,
   SC_Keypress,
   TIMER2_COUNT
   
};

extern SoftTimer_16  SoftTimer1[];
extern SoftTimer_16  SoftTimer2[];

#define KP_WAIT          (3)
#define KP_REPEAT_RATE   (3)
#define KP_REPEAT_RATE_FAST   (2)
#define KP_REPEAT_RATE_FASTEST   (1)
#define KP_REPEAT_DELAY  (50)

#define SMOOTH_AUTO_MENU_UPDATE  (5)
#define SLOW_AUTO_MENU_UPDATE (30)
#define FAST_AUTO_MENU_UPDATE (20)
#define DEFAULT_AUTO_MENU_UPDATE    (FAST_AUTO_MENU_UPDATE)

/* Critical timers */
void Callback_LEDUpdate(void);
void Callback_LCDUpdate(void);
void Callback_ScanInput(void);


#endif
