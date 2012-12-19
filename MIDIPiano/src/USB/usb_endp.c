/**
 ******************************************************************************
 * @file    usb_endp.c
 * @author  MCD Application Team
 * @version V3.4.0
 * @date    29-June-2012
 * @brief   Endpoint routines
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
#include "usb_lib.h"
#include "usb_istr.h"
#include "hw_config.h"
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t Stream_Buff[24];
uint16_t In_Data_Offset;

uint8_t Bulk_Data_Buff[0x80]; /* data buffer*/

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
 * Function Name  : EP1_OUT_Callback
 * Description    : Endpoint 1 out callback routine.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void EP1_OUT_Callback(void)
{
	uint16_t Data_Len; /* data length*/
	uint16_t i;

	Data_Len = USB_SIL_Read(EP1_OUT, Bulk_Data_Buff);
	ProcessUSBMIDI_Out(Bulk_Data_Buff, Data_Len);

	/* Enable the receive of data on EP1 */
	SetEPRxValid(ENDP1 );
	In_Data_Offset += Data_Len;
}

/*******************************************************************************
 * Function Name  : EP1_IN_Callback
 * Description    :
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void EP1_IN_Callback(void)
{
	uint16_t USB_Tx_ptr;
	uint16_t USB_Tx_length;

	//UserToPMABufferCopy(&USART_Rx_Buffer[USB_Tx_ptr], ENDP1_TXADDR, USB_Tx_length);

}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

