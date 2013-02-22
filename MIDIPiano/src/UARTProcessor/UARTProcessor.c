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
#include "MIDILightLogic/MIDILightLogic.h"

#include "Semaphore/osa_semaphore.h"
#include "UserGUI.h"
#include "intertaskComm.h"

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
	static uint8_t SelectedCHMap = MLL_LIGHTS;

	if( byte < MIDI_MAX_CHANNELS )
	{

		xprintf("SelectedCHMap = %x\n", SelectedCHMap);
		xprintf("MAPState =      %X\n", MLL_ToggleChannel(SelectedCHMap, byte) );
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
			LS_ClearLights();
			MLL_ClearHaltList();


			MPB_RePosition(&MIDIHdr, tmasterClock, MPB_PB_SAVE_MIDI_STATUS);
			MPB_ReplayStatusBuffer();

			MPB_EnablePlayback(&MIDIHdr);
			UG_UpdateSeekSlider();

			//TimerStart();

			break;
		}

		//TODO: Fix bug where seeks take up lots of time, and selecting a song will crash it
		//TODO: Function to preview tracks.
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
    		LS_ClearLights();
    		MLL_ClearHaltList();

            tmasterClock = (tmasterClock - 1) * (4*MIDIHdr.PPQ);
            MPB_RePosition(&MIDIHdr, tmasterClock, MPB_PB_SAVE_MIDI_STATUS);
            MPB_ReplayStatusBuffer();
            MPB_EnablePlayback(&MIDIHdr);

            UG_UpdateSeekSlider();

            break;
        }

        case 'M':
        {
        	SelectedCHMap++;
        	if( SelectedCHMap >= MLL_MAP_COUNT )
        	{
        		SelectedCHMap = 0;
        	}
        	xprintf("SelectedCHMap = %x\n", SelectedCHMap);
        }
        break;


        case 'm':
        {
        	MLL_SetMatchMode(MLL_GetMatchMode()+1);
        	if(MLL_GetMatchMode() >= MATCH_MODE_COUNT )
        	{
        		MLL_SetMatchMode(EXACT_MATCH);
        	}
        	xprintf("MatchMode = %x\n", MLL_GetMatchMode());
        }
        break;

        case 'r':
        {
        	MLL_SetMatchFlags(MLL_GetMatchFlags()+0x40);
        	//if(MLL_GetMatchFlags() >= MATCH_FLAG_MASK )
        	{
        		//MLL_SetMatchFlags(0x00);
        	}
        	xprintf("MatchFlags = %x\n", MLL_GetMatchFlags());
        }
        break;

        case 'l':
        {
        	//Setup a flag to say we're waiting on a MIDI Note;
        	WaitForMIDIInputValue = WAITING_FOR_LO_RANGE;
        }
        break;

        case 'h':
        {
        	//Setup a flag to say we're waiting on a MIDI Note;
        	WaitForMIDIInputValue = WAITING_FOR_HI_RANGE;
        }
        break;

        case 'f':
        {
        	//Setup a flag to say we're waiting on a MIDI Note;
        	WaitForMIDIInputValue = WAITING_FOR_FASTFWD_EVENT;
        }
        break;

		default:
		break;
	}

}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
