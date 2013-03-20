#include <stdint.h>
#include <avr/io.h>
#include <avr/iom8.h>
#include <avr/interrupt.h>

#include <stdlib.h>

#include "adc.h"
#include "..\UART\Tx.h"
#include "..\Pause\pause.h"
#include "..\pwm\pwm.h"

int main(void)
{
	UartSetBaudRate(B9600, PD0, PD1);
	while(1)
	{
	UartTx(SPH);
	UartTx(SPL);	
	}
	GICR |= ( 1<< INT0);
	MCUCR |= (1<<ISC01) | (1<<ISC00) ;

//	sei();

	DDRD &= ~(1<<PD2);
	PORTD &= ~(1<<PD2);
	
	uint16_t fag;
	
	adcSetPin(ADC_PIN5);
   
   
   adcOn();
	adcSetReference(ADC_REF_AVCC);
	adcSetPrescale(ADC_PRESCALE_DIV128);
	
	pwmTimer1Initialise(1023, TIMER1_CLK_DIV1);
	pwmTimer2Initialise(TIMER2_CLK_DIV1);
	




	return 0;
}





