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



void ADC_Init(void)
{
    //Sample Clock Select
    AD1CON1bits.SSRC = ADC_AUTOSAMPLE;
    //Auto sample after the conversion
    AD1CON1bits.ASAM = 0x01;
    //Activate sampling
    AD1CON1bits.SAMP = 0x01;

    //Set the Voltage Reference
    ADC_SetReference(ADC_REF_VDD_VSS);

    //DMA Samples per Interrupt
    AD1CON2bits.SMPI = 0;

    //Use system clock and sample for 10 Tad.
    ADC_SetClockSpeed(10 ,10);

    //Enable Interrupts
    IPC3bits.AD1IP = 0x02;
    ADC_On();
}


void ADC_On(void)
{
    IFS0bits.AD1IF = 0x00;
    //IEC0bits.AD1IE = 0x01;
    AD1CON1bits.ADON = 1;
}


void ADC_Off(void)
{
    IFS0bits.AD1IF = 0x00;
    IEC0bits.AD1IE = 0x00;
    AD1CON1bits.ADON = 0;
}


// Where ADC Pin is ANx, x = 0 to 12.
void ADC_SetPin(uint8_t ADCPin)
{
    AD1PCFGL &= ~((1<<ADCPin));
    AD1CHS0bits.CH0SA = ADCPin;
    AD1CHS0bits.CH0SB = ADCPin;
}

uint16_t ADC_Sample(void)
{
    while( !AD1CON1bits.DONE )
    {
    }
    return ADC1BUF0;
}

void ADC_Set12bit(void)
{
    AD1CON1bits.AD12B = 0x01;
}

void ADC_Set10bit(void)
{
    AD1CON1bits.AD12B = 0x00;
}


// Note that the prescale, max ADC clock speed is 500ksps @ 12bit, 1.1MSPS @ 10bit.
// Refer to datasheet.
// Max Clock Speed is 8.5MHz for the ADC Clock, 76ns @ 10bit, 118ns @ 12bit.
//
// Thus, clock prescale, 40MHz / x = 76ns
// For 10bit, choose 4
// For 12bit, choose 5
void ADC_SetClockSpeed(uint8_t sampletime, uint8_t clockprescale)
{
    AD1CON3bits.ADRC = 0;
    AD1CON3bits.SAMC = sampletime;
    AD1CON3bits.ADCS = clockprescale;
}



void ADC_SetReference(uint8_t reference)
{
    AD1CON2bits.VCFG = reference;
}


