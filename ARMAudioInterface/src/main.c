/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    29-June-2012
  * @brief   Audio Speaker Demo main file
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
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_prop.h"

#include "DAC_DMA/DAC_DMA.h"
#include "AudioBuffer/audiobuffer.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
extern uint32_t MUTE_DATA;

extern uint8_t IT_Clock_Sent;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int main(void)
{
  Set_System();

  USART_Configuration();
  Set_USBClock();
  USB_Config();
  USB_Init();

  //Audio_Config();
  DAC_DMA_Init();
  DAC_DMA_NVIC_Init();

  xprintf("Hi Dr. Nick!\n");

  //Speaker_Config();
//  DAC_DMA_Configuration(0, 10);
//
//  {
//	  uint16_t test[10] = {0, 255, 512, 1024, 2048, 4096, 8192, 16000, 32000, 64000};
//
//	  DAC_DMA_SendToDMABuffer((uint8_t*)&test, 10*2);
//  }

  DAC_DMA_Configuration(0, (2*2*MAX_AUDIO_FREQ) / (MAX_AUDIO_CHANNELS * MAX_AUDIO_BIT_RESOLUTION / 8) );

  while (1)
  {
		static uint8_t bufFlag = 0;
		AudioBuffer_t* streamBuf;
		uint16_t* buffer;
		uint16_t* inData;
		uint16_t* outData;

		extern volatile AudioBuffer_t streambuffers[];
	    streamBuf = &streambuffers[bufFlag];
	    buffer = (uint16_t*)&streambuffers[bufFlag].buffer;
	    inData =  &streambuffers[bufFlag].dataLen;
	    outData =  &streambuffers[bufFlag].dataPtr;



	    if( *inData )
	    {
		    if( (Audio_buffer_fill & LOW_EMPTY) )
		    {
		    	DAC_DMA_SendToDMABuffer((uint8_t*)buffer, (*inData), 0 );

				*inData = 0;
				*outData = 0;

				bufFlag++;
				if( bufFlag >= 10 )
				{
					bufFlag = 0;
				}
		    }
		    else if( (Audio_buffer_fill & HIGH_EMPTY) )
		    {
		    	DAC_DMA_SendToDMABuffer((uint8_t*)buffer, (*inData), 1 );
		    	//DAC_DMA_Configuration(0, (*inData) / (MAX_AUDIO_CHANNELS * MAX_AUDIO_BIT_RESOLUTION / 8) );
				*inData = 0;
				*outData = 0;

				bufFlag++;
				if( bufFlag >= 10 )
				{
					bufFlag = 0;
				}
		    }
	    }
	    else
	    {
	    	if( (Audio_buffer_fill & HIGH_EMPTY) )
	    	{
	    		//DAC_DMA_Stop();
	    	}
	    }




  }
}

#ifdef  USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
