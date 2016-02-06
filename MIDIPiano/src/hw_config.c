
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "usb_prop.h"

#include "hw_config.h"
//#include "stm32_eval_sdio_sd.h"
#include "SDIO/sdio_sd.h"

#include "USBMIDI\USBMIDI.h"

#include "MIDILightLogic/MIDILightLogic.h"
#include "MIDIPianoLogic/MIDIPianoLogic.h"
#include "MIDIInputProcessor/MIDIInputProcessor.h"
#include "MIDIPlaybackControlLogic/MIDIPlaybackControlLogic.h"
#include "intertaskComm.h"

#ifdef __GNUC__
#include <malloc.h>
#else
#endif /* __GNUC__ */


volatile uint8_t rxReadPtr[USB_MIDI_CABLE_COUNT];
uint8_t USB_Connected;
volatile uint8_t RxBuffer[USB_MIDI_CABLE_COUNT][RX_BUFFER_SIZE];
volatile uint8_t rxWritePtr[USB_MIDI_CABLE_COUNT];



/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configure GPIO Pin
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | SD_DETECT_GPIO_CLK, ENABLE);
/**
 *	BACK LIGHT FOR LCD.
 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB, GPIO_Pin_1);


//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_14;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//  GPIO_SetBits(GPIOB, GPIO_Pin_5);

  //For debugging purposes
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Configure USB pull-up */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_DISCONNECT, ENABLE);
  GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);

/**
 *	Configure SD_CD pin: SD Card detect pin
 */
  GPIO_InitStructure.GPIO_Pin = SD_DETECT_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(SD_DETECT_GPIO_PORT, &GPIO_InitStructure);



  /**
  *  JOY_UP -> PC1 , JOY_DOWN -> PC3 , JOY_LEFT -> PC2 , JOY_RIGHT -> PA0 , JOY_SEL -> PA1
  *  KeyA -> PC13 , KeyB -> PB2
  */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


  USB_Cable_Config(DISABLE);
  USB_Cable_Config(ENABLE);
}


/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures SDIO IRQ channel.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */
  NVIC_SetVectorTable( NVIC_VectTab_FLASH, 0x0 );
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 11;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 11;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 11;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


void TIM_MIDI_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(MIDI_TIM_RCC_APB, ENABLE);
	TIM_DeInit(MIDI_TIM);
	/* TIM2 Configuration */
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 10000;
	//due to the MIDI playback being 64 prescaled.
	TIM_TimeBaseStructure.TIM_Prescaler = DEFAULT_MIDI_TIMER_PRESCALER;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(MIDI_TIM, &TIM_TimeBaseStructure);

	//Enable MIDI Timer now, it should always be enabled.
	TIM_ITConfig(MIDI_TIM, TIM_IT_Update, ENABLE);
	TIM_Cmd(MIDI_TIM, ENABLE);

//	TIM_ITConfig(MIDI_TIM, TIM_IT_Update, DISABLE);
//	TIM_Cmd(MIDI_TIM, DISABLE);
}


void AUX_TIM_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIM3);
	/* TIM2 Configuration */
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 50;
	//due to the MIDI playback being 64 prescaled.
	TIM_TimeBaseStructure.TIM_Prescaler = 72;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);


	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}


void display_mallinfo(void)
{
#ifdef __GNUC__
    struct mallinfo mi;

    mi = mallinfo();

    xprintf("Total non-mmapped bytes (arena):       %d\n", mi.arena);
    xprintf("# of free chunks (ordblks):            %d\n", mi.ordblks);
    xprintf("# of free fastbin blocks (smblks):     %d\n", mi.smblks);
    xprintf("# of mapped regions (hblks):           %d\n", mi.hblks);
    xprintf("Bytes in mapped regions (hblkhd):      %d\n", mi.hblkhd);
    xprintf("Max. total allocated space (usmblks):  %d\n", mi.usmblks);
    xprintf("Free bytes held in fastbins (fsmblks): %d\n", mi.fsmblks);
    xprintf("Total allocated space (uordblks):      %d\n", mi.uordblks);
    xprintf("Total free space (fordblks):           %d\n", mi.fordblks);
    xprintf("Topmost releasable block (keepcost):   %d\n", mi.keepcost);
#endif
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

  /* Enable and configure the priority of the USB_LP IRQ Channel*/
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable and configure the priority of the USB_HP IRQ Channel*/
  NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
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

	//TODO: Link with the common UART MIDI process routine.

	while( USBMIDI_GetByte(&byte, 0) != NO_DATA_BYTE )
	{
		if( ((byte & 0xF0) == MIDI_NOTE_ON) || ((byte & 0xF0) == MIDI_NOTE_OFF) || (bufCount))
		{
			lsBuf[bufCount] = byte;
			bufCount++;
			if( bufCount == 3)
			{
				bufCount = 0;
				MIP_ProcessEvent((MIDI_CHAN_EVENT_t*)&lsBuf);
			}
		}
		else
		{
			bufCount = 0;
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



void USBSetConnected(void)
{
	USB_Connected = 1;
}

void USBSetDisconnected(void)
{
	USB_Connected = 0;
}

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

			while(USB_Connected && GetEPTxStatus(ENDP1) == EP_TX_VALID )
			{
				//xprintf("!");
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





/**
  * @brief  DeInitializes the SDIO interface.
  * @param  None
  * @retval None
  */
void SD_LowLevel_DeInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  /*!< Disable SDIO Clock */
  SDIO_ClockCmd(DISABLE);

  /*!< Set Power State to OFF */
  SDIO_SetPowerState(SDIO_PowerState_OFF);

  /*!< DeInitializes the SDIO peripheral */
  SDIO_DeInit();

  /*!< Disable the SDIO AHB Clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SDIO, DISABLE);

  /*!< Configure PC.08, PC.09, PC.10, PC.11, PC.12 pin: D0, D1, D2, D3, CLK pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /*!< Configure PD.02 CMD line */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/**
  * @brief  Initializes the SD Card and put it into StandBy State (Ready for
  *         data transfer).
  * @param  None
  * @retval None
  */
void SD_LowLevel_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  /*!< GPIOC and GPIOD Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | SD_DETECT_GPIO_CLK, ENABLE);

  /*!< Configure PC.08, PC.09, PC.10, PC.11, PC.12 pin: D0, D1, D2, D3, CLK pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /*!< Configure PD.02 CMD line */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /*!< Configure SD_SPI_DETECT_PIN pin: SD Card detect pin */
  GPIO_InitStructure.GPIO_Pin = SD_DETECT_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(SD_DETECT_GPIO_PORT, &GPIO_InitStructure);

  /*!< Enable the SDIO AHB Clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SDIO, ENABLE);

  /*!< Enable the DMA2 Clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
}

/**
  * @brief  Configures the DMA2 Channel4 for SDIO Tx request.
  * @param  BufferSRC: pointer to the source buffer
  * @param  BufferSize: buffer size
  * @retval None
  */
void SD_LowLevel_DMA_TxConfig(uint32_t *BufferSRC, uint32_t BufferSize)
{

  DMA_InitTypeDef DMA_InitStructure;

  DMA_ClearFlag(DMA2_FLAG_TC4 | DMA2_FLAG_TE4 | DMA2_FLAG_HT4 | DMA2_FLAG_GL4);

  /*!< DMA2 Channel4 disable */
  DMA_Cmd(DMA2_Channel4, DISABLE);

  /*!< DMA2 Channel4 Config */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SDIO_FIFO_ADDRESS;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)BufferSRC;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = BufferSize / 4;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA2_Channel4, &DMA_InitStructure);

  DMA_ITConfig(DMA2_Channel4, DMA_IT_TC, ENABLE);

  /*!< DMA2 Channel4 enable */
  DMA_Cmd(DMA2_Channel4, ENABLE);
}

/**
  * @brief  Configures the DMA2 Channel4 for SDIO Rx request.
  * @param  BufferDST: pointer to the destination buffer
  * @param  BufferSize: buffer size
  * @retval None
  */
void SD_LowLevel_DMA_RxConfig(uint32_t *BufferDST, uint32_t BufferSize)
{
  DMA_InitTypeDef DMA_InitStructure;

  /*!< DMA2 Channel4 disable */
  DMA_Cmd(DMA2_Channel4, DISABLE);

  DMA_ClearFlag(DMA2_FLAG_TC4 | DMA2_FLAG_TE4 | DMA2_FLAG_HT4 | DMA2_FLAG_GL4);

  /*!< DMA2 Channel4 Config */
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SDIO_FIFO_ADDRESS;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)BufferDST;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = BufferSize / 4;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA2_Channel4, &DMA_InitStructure);

  //DMA_ITConfig(DMA2_Channel4, DMA_IT_TC, ENABLE);

  /*!< DMA2 Channel4 enable */
  DMA_Cmd(DMA2_Channel4, ENABLE);
}

/**
  * @brief  Returns the DMA End Of Transfer Status.
  * @param  None
  * @retval DMA SDIO Channel Status.
  */
uint32_t SD_DMAEndOfTransferStatus(void)
{
	//return (uint32_t)SET;
  return (uint32_t)DMA_GetFlagStatus(DMA2_FLAG_TC4);
}

//void vApplicationStackOverflowHook( xTaskHandle xTask, signed portCHAR *pcTaskName )
//{
//
//	printf("STACK OVER FLOW! %s\n", pcTaskName);
//
//}
//

