/*********************************************************************************************************
*
* File                : systick.c
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : V1.0
* By                  : 
*
*                                  (c) Copyright 2005-2011, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "systick.h"
#include <core_cm3.h>

/* Private variables ---------------------------------------------------------*/	 
static __IO uint32_t TimingDelay;
static __IO uint32_t NonBlockTimingDelay;

void Delay_Init(void)
{
  if (SysTick_Config(SystemCoreClock / 1000))
  { 
    /* Capture error */ 
    while (1);
  }
}


uint32_t Delay_GetDelta(void)
{
	return NonBlockTimingDelay;
}

void Delay_DeltaTime(__IO uint32_t nTime)
{
	NonBlockTimingDelay = nTime;
}
/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }

  if (NonBlockTimingDelay != 0x00)
  {
	  NonBlockTimingDelay--;
  }

}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
















