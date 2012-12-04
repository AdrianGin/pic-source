/**
  ******************************************************************************
  * @file    hw_config.h
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H


#include  <stm32f10x.h>

#define USB_DISCONNECT                    GPIOB
#define USB_DISCONNECT_PIN                GPIO_Pin_7 //8
#define RCC_APB2Periph_GPIO_DISCONNECT    RCC_APB2Periph_GPIOB


/**
  * @brief  SDIO Data Transfer Frequency (25MHz max)
  */
#define SDIO_TRANSFER_CLK_DIV            ((uint8_t)0x02) //0x10
#define SDIO_TRANSFER_HSCLK_DIV            ((uint8_t)0x0) //0x10

#define SD_DETECT_PIN                    GPIO_Pin_2                 /* PB.14 */
#define SD_DETECT_GPIO_PORT              GPIOE                       /* GPIOB */
#define SD_DETECT_GPIO_CLK               RCC_APB2Periph_GPIOE

/**
  * @brief  SDIO Intialization Frequency (400KHz max)
  */
#define SDIO_INIT_CLK_DIV                ((uint8_t)0xC2) //B2
#define SDIO_FIFO_ADDRESS                ((uint32_t)0x40018080)

#define DEFAULT_MIDI_TIMER_PRESCALER	(72)
#define MIDI_TIM 			(TIM2)
#define MIDI_TIM_RCC_APB 	RCC_APB1Periph_TIM2



//Must be a power of two
#define RX_BUFFER_SIZE (128)
#define RX_BUFFER_MASK (RX_BUFFER_SIZE - 1)
/* After 200 times to retry sending a message, we assume the USB is
   disconnected */


extern volatile uint8_t globalFlag;



#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

void GPIO_Configuration(void);
void USART_Configuration(void);
void NVIC_Configuration(void);
void TIM_MIDI_Configuration(void);
void AUX_TIM_Configuration(void);



void USB_Config(void);
void Set_USBClock(void);
void USB_Cable_Config (FunctionalState NewState);

void ProcessUSBMIDIBuffer_Loopback(void);
void ProcessUSBMIDIBuffer_LightSys(void);
void ProcessUSBMIDI_Out(uint8_t* data, uint16_t len);
void USBMIDI_PutByte(uint8_t byte, uint8_t cableNo);
uint8_t USBMIDI_GetByte(uint8_t* inByte, uint8_t cableNo);


void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void SD_LowLevel_DeInit(void);
void SD_LowLevel_Init(void);
void SD_LowLevel_DMA_TxConfig(uint32_t *BufferSRC, uint32_t BufferSize);
void SD_LowLevel_DMA_RxConfig(uint32_t *BufferDST, uint32_t BufferSize);
uint32_t SD_DMAEndOfTransferStatus(void);

#endif  /*__HW_CONFIG_H*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
