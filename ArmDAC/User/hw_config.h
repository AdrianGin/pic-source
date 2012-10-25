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

/* Includes ------------------------------------------------------------------*/

#include "platform_config.h"
#include <stdio.h>

/* Exported types ------------------------------------------------------------*/
/* Codec Control defines */
#define PLLon             1
#define PLLoff            0

#define VerifData         1
#define NoVerifData       0 

#define Codec_PDN_GPIO    GPIOG
#define Codec_PDN_Pin     GPIO_Pin_11

#define BufferSize        100
#define CodecAddress      0x27

#define USART_RX_DATA_SIZE   2048
#define USB_MIDI_CABLE_COUNT	(1)


/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* External variables --------------------------------------------------------*/
void Set_System(void);
void Set_USBClock(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_Config(void);
void Audio_Config(void);
void USB_Cable_Config (FunctionalState NewState);
void Speaker_Config(void);
void NVIC_Config(void);
void GPIO_Config(void);
uint32_t Sound_release(uint16_t Standard, uint16_t MCLKOutput, uint16_t AudioFreq, uint8_t AudioRepetitions);
void I2S_Config(uint16_t Standard, uint16_t MCLKOutput, uint16_t AudioFreq);
void Codec_PowerDown(void);
uint32_t I2SCodec_WriteRegister(uint32_t RegisterAddr, uint32_t RegisterValue, uint32_t Verify);
uint32_t Codec_SpeakerConfig(uint16_t I2S_Standard, uint8_t volume, uint32_t verif, uint8_t pll);
void Get_SerialNum(void);
void ReceiveUSARTData(void);
void USBMIDI_PutByte(uint8_t byte, uint8_t cableNo);
void ProcessUARTBuffer(void);
void ProcessUSBMIDIBuffer_Loopback(void);
void ProcessUSBMIDIBuffer_LightSys(void);

void ToggleActiveChannel(uint8_t byte);

void ProcessUSBMIDI_Out(uint8_t* data, uint16_t len);
uint8_t USBMIDI_GetByte(uint8_t* inByte, uint8_t cableNo);
void dumpData(void);
#endif  /*__HW_CONFIG_H*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/