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
#include "usb_conf.h"
#include <string.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

#include "AudioBuffer/audiobuffer.h"


#define AUDIO_BUFFER_COUNT	(10)
volatile AudioBuffer_t streambuffers[AUDIO_BUFFER_COUNT];
uint8_t bufferCount;


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
  uint16_t Data_Len;       /* data length*/
  
	if (GetENDPOINT(ENDP1) & EP_DTOG_TX)
	{
		/*read from ENDP1_BUF0Addr buffer*/
		Data_Len = GetEPDblBuf0Count(ENDP1);
		PMAToUserBufferCopy( (uint8_t*)&streambuffers[bufferCount].buffer, ENDP1_BUF0Addr, Data_Len);
		//memcpy( (uint8_t*)&streambuffers[bufferCount].buffer, (uint8_t*)(PMAAddr + ENDP1_BUF0Addr * 2), Data_Len);
		streambuffers[bufferCount].dataLen = Data_Len;
		streambuffers[bufferCount].dataPtr = 0;
	}
	else
	{
		/*read from ENDP1_BUF1Addr buffer*/
		Data_Len = GetEPDblBuf1Count(ENDP1);
		//memcpy( (uint8_t*)&streambuffers[bufferCount].buffer, (uint8_t*)(PMAAddr + ENDP1_BUF1Addr * 2), Data_Len);
		PMAToUserBufferCopy( (uint8_t*)&streambuffers[bufferCount].buffer, ENDP1_BUF1Addr, Data_Len);
		streambuffers[bufferCount].dataLen = Data_Len;
		streambuffers[bufferCount].dataPtr = 0;
	}

	xprintf("%d\n",Data_Len);
	streambuffers[bufferCount].dataLen = Data_Len;
	streambuffers[bufferCount].dataPtr = 0;
	bufferCount++;

	if( bufferCount >= AUDIO_BUFFER_COUNT )
	{
		bufferCount = 0;
	}

	FreeUserBuffer(ENDP1, EP_DBUF_OUT);
	//In_Data_Offset += Data_Len;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

