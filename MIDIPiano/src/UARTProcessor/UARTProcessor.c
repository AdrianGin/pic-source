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

#include "ProjectConfig/ProjectConfig.h"
#include "MIDIPlayback/midiplayback.h"

#include "MIDIPianoLogic/MIDIPianoLogic.h"

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
		xprintf("%X\n", LS_ToggleChannel(byte) );
	}

	if( byte == '+')
	{
		MPL_RestartPlay();
		MPL_SetTranspose(MPL_GetTranspose()+1);

		xprintf("%d\n", MPL_GetTranspose());
	}

	if( byte == '-')
	{

		MPL_RestartPlay();
		MPL_SetTranspose(MPL_GetTranspose()-1);
		xprintf("%d\n", MPL_GetTranspose());
	}

	if( byte == 'C')
	{
		LS_IncrementColourMode();
	}



	switch(byte)
	{
		case 'F':
		{
			uint32_t tmasterClock = MIDIHdr.masterClock / (4*MIDIHdr.PPQ);

			tmasterClock = (tmasterClock + 1) * (4*MIDIHdr.PPQ);
			myprintf("tMClock: ", tmasterClock);
			myprintf("MClock: ", MIDIHdr.masterClock);
			myprintf("maxLen: ", MIDIHdr.currentState.maxLength);
			if(tmasterClock > MIDIHdr.currentState.maxLength)
			{
				tmasterClock = MIDIHdr.currentState.maxLength;
			}
			myprintf("MClock: ", tmasterClock);
			//MPB_PlayMIDIFile(&MIDIHdr, filename);
			//TimerStop();
			MPB_PausePlayback(&MIDIHdr);

			MPB_ResetMIDI();
			MPB_RePosition(&MIDIHdr, tmasterClock, MPB_PB_SAVE_MIDI_STATUS);
			MPB_ReplayStatusBuffer();

			MPB_EnablePlayback(&MIDIHdr);

			//TimerStart();

			break;
		}

        case 'R':
        {
            uint32_t tmasterClock = MIDIHdr.masterClock / (4*MIDIHdr.PPQ);
            if( tmasterClock == 0)
            {
                tmasterClock = 1;
            }
            //MPB_PlayMIDIFile(&MIDIHdr, filename);
            MPB_PausePlayback(&MIDIHdr);
            MPB_ResetMIDI();
            tmasterClock = (tmasterClock - 1) * (4*MIDIHdr.PPQ);
            MPB_RePosition(&MIDIHdr, tmasterClock, MPB_PB_SAVE_MIDI_STATUS);
            MPB_ReplayStatusBuffer();
            MPB_EnablePlayback(&MIDIHdr);


            break;
        }

			default:
				break;
	}

}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
