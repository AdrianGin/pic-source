/* PWM.h
 * PWM Routines for the AVR Mega8
 *
 * Date: 16/04/06
 * Author : Adrian Gin (amg94@student.canterbury.ac.nz)
 * Description: PWM Routines for the AVRMega 8
 *
 */

#ifndef	_PWM_ROUTINES
#define  _PWM_ROUTINES

#include <avr/io.h>

#include <stdint.h>





/* pwmInitialise:
 * Sets the Waveform Generator to operate at 'fastPWM'
 * also set the TimerPrescaler.
 *
 * PWM Frequency = fclk / (N * (1+TOP))
 * where N is the timer prescaler, either 1,8,64,256 or 1024
 * function assumes fclk = 8MHz 
 * Output frequency is in Hz 
 *
 */
#define TIMER1_CLK_STOP			0x00	///< Timer Stopped
#define TIMER1_CLK_DIV1			0x01	///< Timer clocked at F_CPU
#define TIMER1_CLK_DIV8			0x02	///< Timer clocked at F_CPU/8
#define TIMER1_CLK_DIV64		0x03	///< Timer clocked at F_CPU/64
#define TIMER1_CLK_DIV256		0x04	///< Timer clocked at F_CPU/256
#define TIMER1_CLK_DIV1024		0x05	///< Timer clocked at F_CPU/1024
#define TIMER1_CLK_T_FALL		0x06	///< Timer clocked at T falling edge
#define TIMER1_CLK_T_RISE		0x07	///< Timer clocked at T rising edge

#define TIMER2_CLK_STOP			0x00	///< Timer Stopped
#define TIMER2_CLK_DIV1			0x01	///< Timer clocked at F_CPU
#define TIMER2_CLK_DIV8			0x02	///< Timer clocked at F_CPU/8
#define TIMER2_CLK_DIV32		0x03	///< Timer clocked at F_CPU/32
#define TIMER2_CLK_DIV64		0x04	///< Timer clocked at F_CPU/64
#define TIMER2_CLK_DIV128		0x05	///< Timer clocked at F_CPU/128
#define TIMER2_CLK_DIV256		0x06	///< Timer clocked at F_CPU/256
#define TIMER2_CLK_DIV1024 	0x07	///< Timer clocked at F_CPU/1024

#define TIMER_PRESCALE_MASK	0x07	///< Timer Prescaler Bit-Mask



void pwmTimer1Initialise(uint16_t pwmTopCount, uint8_t pwmPreScaler);

void pwmTimer2Initialise(uint8_t pwmPreScaler);


/* pwm1xEnable:
 * 1. Turns the selected pin to an output.
 * 2. Changes the passed Duty Cycle in percent
 * 
 * pwm1xDisable:
 * 1. Disables the PWM on the selected output
 */
void pwmTimer1AEnable(uint16_t pwmDutyCycle);
void pwmTimer1BEnable(uint16_t pwmDutyCycle);
void pwmTimer1ADisable(void);
void pwmTimer1BDisable(void);

void pwmTimer2Enable(uint8_t pwmDutyCycle);
void pwmTimer2Disable(void);



#endif


