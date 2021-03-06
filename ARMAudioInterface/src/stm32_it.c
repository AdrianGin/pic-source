/**
  ******************************************************************************
  * @file    stm32_it.c
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    29-June-2012
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"
#include "stm32_it.h"
#include "usb_istr.h"
#include "usb_lib.h"
#include "usb_pwr.h"

#include "AudioBuffer/audiobuffer.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DUMMYDATA  0x1111

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define STREAM_BUF_SIZE (256)

extern uint32_t MUTE_DATA;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M Processor Exceptions Handlers                         */
/******************************************************************************/

/*******************************************************************************
* Function Name  : NMI_Handler
* Description    : This function handles NMI exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NMI_Handler(void)
{
}

/*******************************************************************************
* Function Name  : HardFault_Handler
* Description    : This function handles Hard Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : MemManage_Handler
* Description    : This function handles Memory Manage exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : BusFault_Handler
* Description    : This function handles Bus Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : UsageFault_Handler
* Description    : This function handles Usage Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : SVC_Handler
* Description    : This function handles SVCall exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SVC_Handler(void)
{
}

/*******************************************************************************
* Function Name  : DebugMon_Handler
* Description    : This function handles Debug Monitor exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DebugMon_Handler(void)
{
}

/*******************************************************************************
* Function Name  : PendSV_Handler
* Description    : This function handles PendSV_Handler exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PendSV_Handler(void)
{
}

/*******************************************************************************
* Function Name  : SysTick_Handler
* Description    : This function handles SysTick Handler.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*            STM32 Peripherals Interrupt Handlers                        */
/******************************************************************************/

/*******************************************************************************
* Function Name  : USB_HP_CAN1_TX_IRQHandler
* Description    : This function handles USB High Priority or CAN1 TX interrupts
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_HP_CAN1_TX_IRQHandler(void)
{
  CTR_HP();
}

/*******************************************************************************
* Function Name  : USB_LP_CAN1_RX0_IRQHandler
* Description    : This function handles USB Low Priority or CAN RX0 interrupts
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  USB_Istr();
}


/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : This function handles TIM2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_IRQHandler(void)
{

	static uint8_t bufFlag = 0;
	AudioBuffer_t* streamBuf;
	uint16_t* buffer;
	uint16_t* inData;
	uint16_t* outData;

	extern AudioBuffer_t streambuffers[];



  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
    /* Clear TIM2 update interrupt */
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

    streamBuf = &streambuffers[bufFlag];
    buffer = (uint16_t*)&streambuffers[bufFlag].buffer;
    inData =  &streambuffers[bufFlag].dataLen;
    outData =  &streambuffers[bufFlag].dataPtr;

    if( *inData  && ((uint8_t)(MUTE_DATA) == 0) )
    {


//    	For 8 bits
//    	uint8_t eightBit;
//    	uint8_t* eightBitBuffer;
//
//    	eightBitBuffer = (uint8_t*)buffer;
//    	eightBit = (uint8_t)eightBitBuffer[*outData];
//        //TIM_SetCompare3(TIM4, Stream_Buff[Out_Data_Offset]);
//  			DAC_SetChannel1Data(DAC_Align_8b_R, eightBit);  //for 8bit.
//  			DAC_SetChannel2Data(DAC_Align_8b_R, eightBit);
//    	*outData = *outData + 1;

    	    	uint16_t sixteenBit;
    	    	uint16_t* sixteenBitBuffer;

    	    	sixteenBitBuffer = (uint16_t*)buffer;
    	    	sixteenBit = (uint16_t)sixteenBitBuffer[*outData];


  			DAC_SetChannel1Data(DAC_Align_12b_L, sixteenBit + 32768); //for mono 16bit
  			DAC_SetChannel2Data(DAC_Align_12b_L, sixteenBit + 32768);
  			*outData = *outData + 1;

  			//DAC_SetChannel1Data(DAC_Align_12b_L, (uint16_t)buffer[Out_Data_Offset] + 32768); //for stereo 16bit
  			//DAC_SetChannel2Data(DAC_Align_12b_L, (uint16_t)buffer[Out_Data_Offset] + 32768); //for stereo 16bit
  			//DAC_SetChannel2Data(DAC_Align_12b_L, (uint16_t)Stream_Buff[Out_Data_Offset+1] + 32768);
    }

    if( *outData * 2 >= *inData )
    {
    	*inData = 0;
    	*outData = 0;

    	bufFlag++;
    	if( bufFlag >= 10 )
    	{
    		bufFlag = 0;
    	}
    }


  }
}

void TIM6_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
  {
    /* Clear TIM6 update interrupt */
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);

    //if ((Out_Data_Offset < In_Data_Offset) && ((uint8_t)(MUTE_DATA) == 0))
    {
      /* Set DAC Channel1 DHR register */
      //DAC_SetChannel1Data(DAC_Align_8b_R, Stream_Buff[Out_Data_Offset]);
		//	DAC_SetChannel2Data(DAC_Align_8b_R, Stream_Buff[Out_Data_Offset]);
    }
  }
}


#if defined (USE_STM3210E_EVAL)
/*******************************************************************************
* Function Name  : SPI2_IRQHandler
* Description    : This function handles SPI2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI2_IRQHandler(void)
{
  static uint8_t channel = 0;

  if ((SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_TXE) == SET))
  {
    /* Audio codec configuration section */
    if (GetVar_SendDummyData() == 1)
    {
      /* Send a dummy data just to generate the I2S clock */
      SPI_I2S_SendData(SPI2, DUMMYDATA);
    }
    
   else if ((Out_Data_Offset < In_Data_Offset) && ((uint8_t)(MUTE_DATA) == 0))
    {
      if ((channel++) & 1)
      {
        SPI_I2S_SendData(SPI2, (uint16_t)Stream_Buff[Out_Data_Offset++]);
      }
      else
      {
        SPI_I2S_SendData(SPI2, (uint16_t)Stream_Buff[Out_Data_Offset]);
      }
    }
  }
}

#endif /* USE_STM3210E_EVAL */

#if defined(STM32L1XX_MD)

/**
  * @brief  This function handles TIM6 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM6_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
  {
    /* Clear TIM6 update interrupt */
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);

    if ((Out_Data_Offset < In_Data_Offset) && ((uint8_t)(MUTE_DATA) == 0))
    {
      /* Set DAC Channel1 DHR register */
      DAC_SetChannel1Data(DAC_Align_8b_R, Stream_Buff[Out_Data_Offset]);      
      Out_Data_Offset++;
    }
  }
}

/*******************************************************************************
* Function Name  : USB_LP_IRQHandler
* Description    : This function handles USB Low Priority interrupts  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_LP_IRQHandler(void)
{
  USB_Istr();
}

#endif /* STM32L1XX_XD */

/*******************************************************************************
* Function Name  : USB_HP_IRQHandler
* Description    : This function handles USB High Priority interrupts  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_HP_IRQHandler(void)
{
  CTR_HP();
}

/******************************************************************************/
/*                 STM32 Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32xxx.s).                                            */
/******************************************************************************/

/*******************************************************************************
* Function Name  : PPP_IRQHandler
* Description    : This function handles PPP interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*void PPP_IRQHandler(void)
{
}*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
