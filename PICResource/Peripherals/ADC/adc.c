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

#include <stdint.h>
#include "p24Hxxxx.h"
#include "adc.h"



void ADCInit(uint16_t inputBitmap)
{
    AD1PCFGL &= ~((1<<inputBitmap) | (1<<0));
    AD1CHS0 = 0x0101;
    //AD1CHS0bits.CH0SB = 0x01;


    AD1CON1bits.SSRC = 0x00;
    AD1CON1bits.ASAM = 0x01;
    AD1CON1bits.SAMP = 0x01;

    AD1CON2bits.VCFG = 0x00;
    AD1CON2bits.SMPI = 0;

    AD1CON3bits.ADRC = 0x01;
    AD1CON3bits.SAMC = 0;
    AD1CON3bits.ADCS = 0;

    //Enable Interrupts
    IPC3bits.AD1IP = 0x07;
    IFS0bits.AD1IF = 0x00;
    IEC0bits.AD1IE = 0x01;

    AD1CON1bits.ADON = 1;
}