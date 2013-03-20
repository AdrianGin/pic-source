/* PWM.c
 * PWM Routines for the AVR Mega8
 *
 * Date: 16/04/06
 * Author : Adrian Gin (amg94@student.canterbury.ac.nz)
 * Description: PWM Routines for the AVRMega 8
 *
 */
/* See pwm.h for a description of operation */


#include <avr/io.h>
#include <stdint.h>
#include "pwm.h"

void pwmTimer1AEnable(uint16_t pwmDutyCycle)
{
	 DDRB    |= ( 1<<(PB1) );
    /* Clear PWM Output Pin on a compare match, but
     * set the pin high upon reaching 'TOP' */
    TCCR1A  = ((TCCR1A & (~(1<< (COM1A0) ))) | (1<< (COM1A1)) ) ;
 	 OCR1A = pwmDutyCycle;
}

/* Set the OC1B Pin as a PWM output, and set Duty Cycle */
void pwmTimer1BEnable(uint16_t pwmDutyCycle)
{
    DDRB    |= (1<<PB2);
    /* Clear PWM Output Pin on a compare match, but
     * set the pin high upon reaching 'TOP' */
    TCCR1A  = ((TCCR1A & (~(1<< (COM1B0) ))) | (1<< (COM1B1)) ) ;
    OCR1B   = pwmDutyCycle;
}

/* Disable the OC1A PWM output */
void pwmTimer1ADisable(void)
{
    TCCR1A  &= ~( (1<<COM1A1) | (1<<COM1A0) );
}

/* Disable the OC1B PWM output */
void pwmTimer1BDisable(void)
{
    TCCR1A  &= ~( (1<<COM1B1) | (1<<COM1B0) );
}

/* Set Timer1's prescaler, set PWM mode to Fast PWM and set PWM frequency */
void pwmTimer1Initialise(uint16_t pwmTopCount, uint8_t pwmPreScaler)
{
    /*Set to Fast PWM with ICR being the Top.*/
    TCCR1A = ((TCCR1A & (~(1<<WGM10))) | (1<<WGM11) ); 

    /* Set the Timer1 Prescaler */
	 TCCR1B = ((TCCR1B & (~TIMER_PRESCALE_MASK)) | pwmPreScaler 
										| (  (1<<WGM12) | (1<<WGM13) ));
   
    ICR1 = pwmTopCount;
}



/* Set Timer2's prescaler, set PWM mode to Fast PWM and set PWM frequency */
/* Timer2's top count does not need to be initialised since it has a fixed
 * TOP of 0xFF*/
void pwmTimer2Initialise(uint8_t pwmPreScaler)
{
    /*Set to Fast PWM with ICR being the Top.*/
    /* Set the Timer2 Prescaler */
	 TCCR2 = ((TCCR2 & (~TIMER_PRESCALE_MASK)) 
				| pwmPreScaler | ( (1<<WGM21) | (1<<WGM20) ));
   
}

/* Set the OC2 Pin as a PWM output, and set Duty Cycle */
/* Dutycycle is pwmDutyCycle / 255)*/
void pwmTimer2Enable(uint8_t pwmDutyCycle)
{
    DDRB    |= (1<<PB3);
    TCCR2   = ((TCCR2 & (~(1<<COM20))) | (1<<COM21) );
    OCR2    = pwmDutyCycle;
}

/* Disable the OC1B PWM output */
void pwmTimer2Disable(void)
{
    TCCR2  &= ~( (1<<COM21) | (1<<COM20) );
}

