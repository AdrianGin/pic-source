
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdlib.h>

#include "hardUart/hardUART.h"
#include "UltraSonic.h"


#define ULTRASONIC_DDR (DDRB)
#define ULTRASONIC_PIN (1)


/* Number of samples to take when calibrating */
#define CALIBRATION_LIMIT (10)

enum {
	UltraS_INACTIVE = 0,
	UltraS_ACTIVE,
	UltraS_CALIBRATE
};


uint8_t ZeroValue = 0;
uint8_t CalibrateCount = 0;
uint8_t UltraSonicMode = UltraS_INACTIVE;


/*Set up PWM for 40kHz */
void UltraS_Init(void)
{

	ULTRASONIC_DDR |= (1<<ULTRASONIC_PIN);


	TCCR1A |= (1<<COM1A1) | (0x02 << WGM10);
	TCCR1B = (0x03 << WGM12) | (1 << CS10);
	
	/* 400kHz */
	ICR1 = 399; 


	/* 50% DC*/
	OCR1A = 200;



/* ADC Init */
	DDRC &= ~(1<<0);
	ADMUX = (1 << REFS0) | (1 << ADLAR);
	ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADIE) | (0x00);
	
	UltraS_GetAverage();

}


ISR(ADC_vect)
{

	uint8_t ADCResult = ADCH;
	uint8_t  US_Signal = 0;


	switch( UltraSonicMode )
	{
		case UltraS_CALIBRATE:
			ZeroValue = (ADCResult + ZeroValue) >> 1;

			if( ++CalibrateCount == CALIBRATION_LIMIT )
			{
				CalibrateCount = 0;
				UltraSonicMode = UltraS_INACTIVE;
				uartTx( ZeroValue );
			}
		break;


		case UltraS_ACTIVE:
			
			if( (ADCResult > ZeroValue) )
			{
			
				US_Signal = ADCResult - ZeroValue;

				//uartTx( abs(US_Signal) );

				if( abs(US_Signal) > 10 )
				{

				
					PORTC |= (1 << 4);
					//Stop Timer
					//TCCR1B &= ~(1 << CS10);
					//ADCSRA &= ~(1 << ADFR);
					//TIMSK &= ~(1 << TOIE1);
		
					//uartTx(TCNT1 >> 8);
					//uartTx(TCNT1 & 0xFF);
					//TCNT1 = 0;
				
					uartTxString_P(PSTR("Detect!"));
				}
			}
		break;

		default:

		break;
		

	}


}

ISR(TIMER1_OVF_vect)
{
	//ADCSRA &= ~(1 << ADFR);
	//TCCR1B &= ~(1 << CS10);
	//TIMSK &= ~(1 << TOIE1);
	//TCNT1 = 0;
}

void UltraS_Outpulse(void)
{
	TCCR1A |= (1<<COM1A1) | (0x02 << WGM10);
	TCCR1B = (0x03 << WGM12) | (1 << CS10);
}

void UltraS_Killpulse(void)
{
	TCCR1A = 0;
	TCCR1B = 0;
}


/* Updates the Zero Value */
void UltraS_GetAverage(void)
{

	//Deactivate 40kHz Signal
	UltraS_Killpulse();
	ADCSRA |= (1 << ADFR) | (1 << ADSC);
	UltraSonicMode = UltraS_CALIBRATE;

}

void UltraS_GetDistance(void)
{
	uint8_t i;

	if( UltraSonicMode != UltraS_CALIBRATE)
	{
		UltraSonicMode = UltraS_ACTIVE;

		ADCSRA |= (1 << ADFR) | (1 << ADSC);

		//Activate 40kHz Signal
		UltraS_Outpulse();
	
		//uartTx('O');	

		for( i = 0; i < 255; i++ )
		{
			_delay_us(29);
		}

		//Deactivate 40kHz Signal
		UltraS_Killpulse();

		//Only catch echo 
		/*for( i = 0; i < 2; i++ )
		{
			_delay_us(29);
		}*/
	

		/* Start Timer */
		//TCNT1 = 0;
		//TIMSK |= (1 << TOIE1);
		//TCCR1B = (1 << CS10);
		
	}
	else
	{
		//uartTx('I');
	}


}
