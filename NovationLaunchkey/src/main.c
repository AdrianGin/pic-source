

#include "HardwareSpecific.h"

#include "ADC.h"
#include "LED.h"
#include "MultiplexControl.h"
#include "UART.h"
#include "Timer.h"

#include "Switch.h"
#include "Keyboard.h"

#include "TimerCallbacks.h"


int main(void)
{

	//Initialise Hardware
	SystemClockInit();
	NVIC_Config();
	
	MUX_GPIO_Init();
	LED_GPIO_Init();
	Switch_GPIO_Init();
	Keyboard_GPIO_Init();


	UART_Init();

	STR_UART_T sParam;
	/* UART Setting */
	sParam.u32BaudRate 		= 31250;
	sParam.u8cDataBits 		= DRVUART_DATABITS_8;
	sParam.u8cStopBits 		= DRVUART_STOPBITS_1;
	sParam.u8cParity 		= DRVUART_PARITY_NONE;
	sParam.u8cRxTriggerLevel= DRVUART_FIFO_1BYTES;

	/* Set UART Configuration */
	if( DrvUART_Open(UART_PORT1,&sParam) == E_SUCCESS)
	{
		DrvUART_EnableInt(UART_PORT1, DRVUART_THREINT, UART_INT_HANDLE);
	}


	LED_Init();
   
	//Timer_Init(76800);
	//Divide by (11+1) = 12 for a 1MHz prescale
	//Count up to (2nd Param) for 50kHz timer.
	Timer_InitNoBSP(11, 20);

	ADC_Init();

   LED_SetLEDBrightness(0, LED_PAD0_G, MAX_LED_BRIGHTNESS/200);
   LED_SetLEDBrightness(0, LED_PAD1_G, MAX_LED_BRIGHTNESS/150);
   LED_SetLEDBrightness(0, LED_PAD2_G, MAX_LED_BRIGHTNESS/110);
   LED_SetLEDBrightness(0, LED_PAD3_G, MAX_LED_BRIGHTNESS/100);

   LED_SetLEDBrightness(0, LED_PAD4_G, MAX_LED_BRIGHTNESS/90);
   LED_SetLEDBrightness(0, LED_PAD5_G, MAX_LED_BRIGHTNESS/80);
//   LED_SetLEDBrightness(0, LED_PAD6_G, MAX_LED_BRIGHTNESS/70);


   LED_SetLEDBrightness(0, LED_PAD8_G, MAX_LED_BRIGHTNESS/50);
   LED_SetLEDBrightness(0, LED_PAD9_G, MAX_LED_BRIGHTNESS/40);
   LED_SetLEDBrightness(0, LED_PAD10_G, MAX_LED_BRIGHTNESS/30);
   LED_SetLEDBrightness(0, LED_PAD11_G, MAX_LED_BRIGHTNESS/20);

   LED_SetLEDBrightness(0, LED_PAD12_G, 1);
   LED_SetLEDBrightness(0, LED_PAD13_G, MAX_LED_BRIGHTNESS/5);
   LED_SetLEDBrightness(0, LED_PAD14_G, MAX_LED_BRIGHTNESS/2);
   LED_SetLEDBrightness(0, LED_PAD15_G, MAX_LED_BRIGHTNESS);


   SoftTimerStart(SoftTimer2[SC_ADC]);
   SoftTimerStart(SoftTimer2[SC_UPDATE_DISPLAY]);
   SoftTimerStart(SoftTimer1[SC_COLUMN_MUX]);

   while(1)
   {
	   if( TIM_IsMasterTickTriggered() )
	   {
			
			
		   RunAndExecuteTimers( (SoftTimer_16*)SoftTimer2, TIMER2_COUNT);
		   //RunAuxTimers();
		   TIM_ResetMasterTick();
	   }


   }

	return 0;
}



















