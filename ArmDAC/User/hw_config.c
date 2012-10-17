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
#include "stm32f10x_it.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "usb_prop.h"

#include "USBMIDI\\USBMIDI.h"
#include "SysTick\\systick.h"
#include "LightSys\\LightSys.h"

#include <stdio.h>
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TIM2ARRValue    3273 /* 22KHz = 72MHz / 3273 */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;

uint8_t  USART_Rx_Buffer [USART_RX_DATA_SIZE]; 
uint32_t USART_Rx_ptr_in = 0;
uint32_t USART_Rx_ptr_out = 0;
uint32_t USART_Rx_length  = 0;

//Must be a power of two
#define RX_BUFFER_SIZE (128)
#define RX_BUFFER_MASK (RX_BUFFER_SIZE - 1)
/* After 200 times to retry sending a message, we assume the USB is
   disconnected */
#define USB_CONNECT_TIMEOUT (1500)
#define CABLE_NO_COUNT (2)

volatile uint8_t rxReadPtr[CABLE_NO_COUNT];
uint8_t USB_Connected;
volatile uint8_t RxBuffer[CABLE_NO_COUNT][RX_BUFFER_SIZE];
volatile uint8_t rxWritePtr[CABLE_NO_COUNT];


/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len);
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_System(void)
{
#if !defined(STM32L1XX_MD) 
  GPIO_InitTypeDef GPIO_InitStructure;
#endif /* STM32L1XX_MD */  
#if defined(USB_USE_EXTERNAL_PULLUP)
  GPIO_InitTypeDef  GPIO_InitStructure;
#endif /* USB_USE_EXTERNAL_PULLUP */ 
 
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32xxx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32xxx.c file
     */ 
  
#ifdef STM32L1XX_MD 
  /* Enable the SYSCFG module clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
#endif /* STM32L1XX_MD */   
  
  
#ifdef USE_STM3210B_EVAL
  /* Enable GPIOB, TIM2 & TIM4 clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM4 , ENABLE);
#endif /* USE_STM3210B_EVAL */

#if !defined(STM32L1XX_MD)
  /* Configure USB pull-up */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_DISCONNECT, ENABLE);

  /* Configure USB pull-up */
  GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);
#endif /* STM32L1XX_MD */

#if defined(USB_USE_EXTERNAL_PULLUP)
  /* Enable the USB disconnect GPIO clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIO_DISCONNECT, ENABLE);

  /* USB_DISCONNECT used as USB pull-up */
  GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);  
#endif /* USB_USE_EXTERNAL_PULLUP */
  
  USB_Cable_Config(DISABLE);

  USB_Cable_Config(ENABLE);
}

/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz)
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_USBClock(void)
{
  /* Select USBCLK source */
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
  
  /* Enable the USB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

/*******************************************************************************
* Function Name  : Enter_LowPowerMode
* Description    : Power-off system clocks and power while entering suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode
* Description    : Restores system clocks and power while exiting suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else
  {
    bDeviceState = ATTACHED;
  }
}

/*******************************************************************************
* Function Name  : USB_Interrupts_Config
* Description    : Configures the USB interrupts
* Input          : None.
* Return         : None.
*******************************************************************************/
void USB_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  /* Enable and configure the priority of the USB_LP IRQ Channel*/
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable and configure the priority of the USB_HP IRQ Channel*/
  NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  
  /* Audio Components Interrupt configuration */
  Audio_Config();
}
/*******************************************************************************
* Function Name  : USB_Interrupts_Config
* Description    : Configures the USB interrupts
* Input          : None.
* Return         : None.
*******************************************************************************/
void Audio_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  

}
/*******************************************************************************
* Function Name  : USB_Cable_Config
* Description    : Software Connection/Disconnection of USB Cable
* Input          : None.
* Return         : Status
*******************************************************************************/
void USB_Cable_Config (FunctionalState NewState)
{

  if (NewState != DISABLE)
  {
    GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
  else
  {
    GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
}


void dumpData(void)
{
		usbMIDIMessage_t temp; 
	temp.header = 0x05;
	temp.MIDIData[0] = 0xf8;
	temp.MIDIData[1] = 0;
	temp.MIDIData[2] = 0;	

	
	 			while( GetEPTxStatus(ENDP1) != EP_TX_NAK )
 			{
 			}
 		USB_SIL_Write(EP1_IN, (uint8_t*)&temp, 0x04); 
 		SetEPTxValid(ENDP1); 	
}
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
	while( GetEPTxStatus(ENDP1) == EP_TX_VALID )
	{
		//printf("!");
	}
	

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
	printf("%X\n", LS_ToggleChannel(byte) );

}

//For Loopback
void ProcessUSBMIDIBuffer_Loopback(void)
{
	uint8_t byte;
	uint16_t ret;
	
	while( USBMIDI_GetByte(&byte, 0) != NO_DATA_BYTE )
	{
		USBMIDI_PutByte(byte, 0);
	}
	ret = GetEPRxStatus(ENDP1);
	if( ret != EP_RX_VALID )
	{
		SetEPRxValid(ENDP1);
	}
}

void ProcessUSBMIDIBuffer_LightSys(void)
{
	uint8_t byte;
	uint16_t ret;
	static uint8_t lsBuf[3];
	static uint8_t bufCount = 0;

	while( USBMIDI_GetByte(&byte, 0) != NO_DATA_BYTE )
	{
		if( ((byte & 0xF0) == MIDI_NOTE_ON) || ((byte & 0xF0) == MIDI_NOTE_OFF) || (bufCount))
		{
			lsBuf[bufCount] = byte;
			bufCount++;
		}
		else
		{
			bufCount = 0;
		}

		if( bufCount == 3)
		{
			bufCount = 0;
			LS_ProcessMIDINote(lsBuf[0], lsBuf[1], lsBuf[2]);
		}
	}



	ret = GetEPRxStatus(ENDP1);
	if( ret != EP_RX_VALID )
	{
		SetEPRxValid(ENDP1);
	}
}




//call this to put all the data into the USB->MIDI rxbuffer
void ProcessUSBMIDI_Out(uint8_t* data, uint16_t len)
{
   /* Route it all to the UART port at 31250 baud */
   uint8_t codeIndexNumber;
   uint8_t messageSize = 0;
   uint8_t cableNo = 0;
   uint8_t i = 0;
   uint8_t j = 0;

   for( i = 0 ; i < len ; i = i + sizeof(usbMIDIMessage_t) )
   {
      if( data[i] )
      {
         codeIndexNumber = data[i] & (0x0F);
         messageSize = (MIDIResponseMap[codeIndexNumber]);
         cableNo = data[i] >> 4;

         for(j = 0; j < messageSize; j++)
         {
            uint8_t buffer = data[i+j+1];
            RxBuffer[cableNo][rxWritePtr[cableNo]] = buffer;
            rxWritePtr[cableNo] = ((rxWritePtr[cableNo] + 1) & RX_BUFFER_MASK);
         }
      }
   }
}


/* This reads the MIDI data received from USB */
uint8_t USBMIDI_GetByte(uint8_t* inByte, uint8_t cableNo)
{
   /* Process messages in the UART Rx buffer is there are any */
   if( rxReadPtr[cableNo] != rxWritePtr[cableNo] )
   {
      *inByte = RxBuffer[cableNo][rxReadPtr[cableNo]];
      rxReadPtr[cableNo] = ((rxReadPtr[cableNo] + 1) & RX_BUFFER_MASK);
      return 1;
   }
   return NO_DATA_BYTE;
}



usbMIDIcable_t MIDICable[2];

/* This here makes the process Buffer redundant */
void USBMIDI_PutByte(uint8_t byte, uint8_t cableNo)
{
   uint8_t midiReady;
   uint16_t retry;

   midiReady = MIDIDataReady(byte, &MIDICable[cableNo]);
   /* Copy it out, so the tempbuffer is ready again */	
   if( midiReady )
   {
		 
		 if( midiReady == MIDI_DATA_READY)
		{
			memcpy(&MIDImsgComplete[wMIDImsgCount], &MIDICable[cableNo].msg, sizeof(usbMIDIMessage_t));
		}
	    MIDImsgComplete[wMIDImsgCount].header = MIDImsgComplete[wMIDImsgCount].header | (cableNo << 4);
		  wMIDImsgCount = (wMIDImsgCount + 1) & MIDI_OUT_MASK;
   
			while( GetEPTxStatus(ENDP1) == EP_TX_VALID )
			{
				//printf("!");
			}
	
       if( usbMIDI_bufferLen() == 1 )
       {
          /* Send 1 byte */
				  USB_SIL_Write(EP1_IN, (uint8_t*)&MIDImsgComplete[rMIDImsgCount], sizeof(usbMIDIMessage_t)); 
				  retry = GetEPTxStatus(ENDP1);
					SetEPTxValid(ENDP1); 
          rMIDImsgCount = (rMIDImsgCount + 1) & MIDI_OUT_MASK;
       }
       else
       {
          /* Send 2 bytes */
				 	USB_SIL_Write(EP1_IN, (uint8_t*)&MIDImsgComplete[rMIDImsgCount], sizeof(usbMIDIMessage_t)*2); 
					SetEPTxValid(ENDP1); 
          rMIDImsgCount = (rMIDImsgCount + 2) & MIDI_OUT_MASK;

       }	 
	 }
	 
}


/*******************************************************************************
* Function Name  : Speaker_Timer_Config
* Description    : Configure and enable the timer
* Input          : None.
* Return         : None.
*******************************************************************************/
void Speaker_Config(void)
{

}
  

/*******************************************************************************
* Function Name  : Get_SerialNum.
* Description    : Create the serial number string descriptor.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Get_SerialNum(void)
{
  uint32_t Device_Serial0, Device_Serial1, Device_Serial2;

#ifdef STM32L1XX_MD
  Device_Serial0 = *(uint32_t*)(0x1FF80050);
  Device_Serial1 = *(uint32_t*)(0x1FF80054);
  Device_Serial2 = *(uint32_t*)(0x1FF80064);
#else   
  Device_Serial0 = *(__IO uint32_t*)(0x1FFFF7E8);
  Device_Serial1 = *(__IO uint32_t*)(0x1FFFF7EC);
  Device_Serial2 = *(__IO uint32_t*)(0x1FFFF7F0);
#endif /* STM32L1XX_MD */ 

  Device_Serial0 += Device_Serial2;

  if (Device_Serial0 != 0)
  {
    IntToUnicode (Device_Serial0, (uint8_t*)&USBDesciptors[3][2] , 8);
    IntToUnicode (Device_Serial1, (uint8_t*)&USBDesciptors[3][18], 4);
  }
}

/*******************************************************************************
* Function Name  : HexToChar.
* Description    : Convert Hex 32Bits value into char.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len)
{
  uint8_t idx = 0;
  
  for( idx = 0 ; idx < len ; idx ++)
  {
    if( ((value >> 28)) < 0xA )
    {
      pbuf[ 2* idx] = (value >> 28) + '0';
    }
    else
    {
      pbuf[2* idx] = (value >> 28) + 'A' - 10; 
    }
    
    value = value << 4;
    
    pbuf[ 2* idx + 1] = 0;
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
