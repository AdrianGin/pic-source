/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    10/15/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/

#include "stm32f10x_it.h"
#include "hw_config.h"

#include "SDIO/sdio_sd.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}


/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}


//Used by FreeRTOS
///**
//  * @brief  This function handles PendSVC exception.
//  * @param  None
//  * @retval None
//  */
//void PendSV_Handler(void)
//{
//}
//
///**
//  * @brief  This function handles SysTick Handler.
//  * @param  None
//  * @retval None
//  */
//void SysTick_Handler(void)
//{
//}
//
//
///**
//  * @brief  This function handles SVCall exception.
//  * @param  None
//  * @retval None
//  */
//void SVC_Handler(void)
//{
//}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/





/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE ) != RESET)
	{
		/* Send the received data to the PC Host*/
	}

	if (USART_GetITStatus(USART1, USART_IT_ORE ) != RESET)
	{

	}
}




/**
  * @}
  */ 
/*******************************************************************************
* Function Name  : SDIO_IRQHandler
* Description    : This function handles SDIO global interrupt request.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void SDIO_IRQHandler(void)
{
  /* Process All SDIO Interrupt Sources */
//  CPU_SR         cpu_sr;
//  OS_ENTER_CRITICAL();
//  OSIntNesting++;
//  OS_EXIT_CRITICAL();
//
  SD_ProcessIRQSrc();
//
//  OSIntExit();
}



void DMA1_Channel3_IRQHandler(void)
{
	//DMA_ClearITPendingBit(DMA1_FLAG_TC3 | DMA1_FLAG_GL3);
}


void TIM2_IRQHandler(void)
{
	static uint8_t state = 0;

	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	globalFlag |= 1;
}

void TIM3_IRQHandler(void)
{
	static uint16_t cntr = 0;
	static uint16_t cntr2 = 0;
	static uint16_t cntr3 = 0;
	static uint16_t cntr4 = 0;
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);


	if( cntr4++ == (277/2))
	{
		cntr4 = 0;
	}


	if( cntr3++ == 500)
	{
		globalFlag |= 0x10;
		cntr3 = 0;
	}

	cntr2++;
	if( cntr2 == 3000)
	{
		globalFlag |= 0x08;
		cntr2 = 0;
	}

	cntr++;
	if( cntr == 1 )
	{
		//GPIO_SetBits(GPIOC, GPIO_Pin_13);
		globalFlag |= 0x04;
		globalFlag &= ~0x02;
	}

	if( cntr == 320*2 )
	{
		//GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		globalFlag |= 0x02;
		globalFlag &= ~0x04;
		cntr = 0;
	}


}




/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
