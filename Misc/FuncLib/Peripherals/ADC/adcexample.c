/* ADCexample.c
 * ADC Routines for the AVR Mega8
 *
 * Date: 17/04/06
 * Author : Adrian Gin (amg94@student.canterbury.ac.nz)
 * 
 * This C file is an example of how to use adc.c ADC routines.
 *
 */

#include <stdint.h>
#include <avr/io.h>
#include <avr/iom8.h>
#include <avr/interrupt.h>

#include <stdlib.h>

#include "adc.h"
#include "..\UART\Tx.h"
#include "..\UART\Rx.h"
#include "..\Pause\pause.h"
#include "..\pwm\pwm.h"


/* showVoltage 
 *
 * Displays the voltage on ADC Pin5 via the UART.
 * The user measures Vcc and places it into the #define
 * It can only accept a 10bit ADC Result.
 */
void showVoltage(uint16_t Result);



/* main
 *
 * Displays a G followed by a RAW Result. The G is so that
 * a parser can understand and process the Raw result.
 * Windmill software is used to receive the information
 *
 *
 * The voltage outputs on Timer 1A and 1B are also related to the voltage 
 * read on the ADC Input.
 *
 * One is proportional to the voltage while the other is negatively
 * proportional (Timer1B).
 * 
 * The voltage controlled at Timer1A and 1B's outputs are via Timer1's
 * PWM Module.
 */
 




int main(void)
{
	UartSetBaudRate(B9600, PD0, PD1);


	GICR |= ( 1<< INT0);
	MCUCR |= (1<<ISC01) | (1<<ISC00) ;

	sei();


	DDRD &= ~(1<<PD2);
	PORTD &= ~(1<<PD2);
	
	uint16_t Result;
	uint8_t message[10];

	adcSetPin(ADC_PIN5);
   
   
   adcOn();
	adcSetReference(ADC_REF_AVCC);
	adcSetPrescale(ADC_PRESCALE_DIV128);
	
	pwmTimer1Initialise(1023, TIMER1_CLK_DIV1);
	pwmTimer2Initialise(TIMER2_CLK_DIV1);
	


	uint8_t i = 0;
	uint8_t j = 0;
	while(1)
	{
	Result = adc10();
	UartTx('G');

	utoa(Result, message, 10);
	serout(message);
	serout("\r\n");

	pausems(1000);
//	showVoltage(Result);


	
	pwmTimer1AEnable( Result );
	pwmTimer1BEnable( 1023 - Result );
	
	i = (++j > 127) ? 255 - j : j;
	pwmTimer2Enable(i);


	
	}

	return 0;
}


#define VCC 5.18
#define VOLT_SCALE10 ((uint8_t)(1024/VCC))
#define VOLT_SCALE8  ((uint8_t)(256/VCC))


void showVoltage(uint16_t Result)
{
 	uint8_t resolution;
   uint8_t 	ones;
	uint16_t tenths;
	uint16_t hundredths;
   uint8_t message[10];
   
   resolution = VOLT_SCALE10;
   
	ones = Result / resolution;
	tenths = (Result % resolution)/ ((resolution+10)/10);
	hundredths = (Result % resolution) 
					 % ((resolution+10)/10);
 	
	if(resolution == VOLT_SCALE10)
	{
	hundredths = hundredths / ((resolution+10)/100); 
	}
					 
	utoa(ones, message, 10);
	serout(message);
	UartTx('.');
	utoa(tenths, message, 10);
	serout(message);
	utoa(hundredths, message, 10);
	serout(message);
	serout(" Volts\r\n"); 
   
}



ISR(INT0_vect)
{
	pausems(7000);
	serout("Yo wassup dudes? Interrupt 0 was triggered!");
	pausems(7000);
	return;
}





