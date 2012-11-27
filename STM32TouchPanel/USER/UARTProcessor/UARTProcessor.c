/**
  ******************************************************************************
  * @file    hw_config.c
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    29-June-2012
  * @brief   Hardware Configuration & Setup
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
#include <stdio.h>
#include <string.h>

#include "LightSys\LightSys.h"
#include "UARTProcessor.h"
#include "MIDICodes\MIDICodes.h"

#define USART_RX_DATA_SIZE   2048

uint8_t  USART_Rx_Buffer [USART_RX_DATA_SIZE]; 
uint32_t USART_Rx_ptr_in = 0;
uint32_t USART_Rx_ptr_out = 0;
uint32_t USART_Rx_length  = 0;


/*******************************************************************************
* Function Name  : UART_To_USB_Send_Data.
* Description    : send the received data from UART 0 to USB.
* Input          : None.
* Return         : none.
*******************************************************************************/
void ReceiveUSARTData(void)
{
	//dumpData();
	
  USART_Rx_Buffer[USART_Rx_ptr_in] = USART_ReceiveData(USART1);
	//printf("%c", USART_Rx_Buffer[USART_Rx_ptr_in]);
  USART_Rx_ptr_in++;
  /* To avoid buffer overflow */
  if(USART_Rx_ptr_in == USART_RX_DATA_SIZE)
  {
    USART_Rx_ptr_in = 0;
  }
}

void ProcessUARTBuffer(void)
{
	if(USART_Rx_ptr_in != USART_Rx_ptr_out)
	{
		//This echoes it out.
		//USBMIDI_PutByte(USART_Rx_Buffer[USART_Rx_ptr_out] , 0);
		ToggleActiveChannel(USART_Rx_Buffer[USART_Rx_ptr_out]);
		USART_Rx_ptr_out++;
		
		if(USART_Rx_ptr_out == USART_RX_DATA_SIZE)
		{
			USART_Rx_ptr_out = 0;
		}		
	}
}


void ToggleActiveChannel(uint8_t byte)
{
	uint8_t i;
	if( byte < MIDI_MAX_CHANNELS )
	{
		printf("%X\n", LS_ToggleChannel(byte) );
	}

	if( byte == '+')
	{
		for( i = 0; i < MIDI_MAX_CHANNELS; i++)
		{
			LS_SetTranspose(i, LS_GetTranspose(i) + 1 );
		}
		printf("%d\n", LS_GetTranspose(0));
	}

	if( byte == '-')
	{
		for( i = 0; i < MIDI_MAX_CHANNELS; i++)
		{
			LS_SetTranspose(i, LS_GetTranspose(i) - 1 );
		}
		printf("%d\n", LS_GetTranspose(0));
	}

	if( byte == 'C')
	{
		LS_IncrementColourMode();
	}
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
