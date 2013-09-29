/**
  ******************************************************************************
  * @file    usb_desc.h
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    29-June-2012
  * @brief   Descriptor Header for Mass Storage Device
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
#ifndef __USB_DESC_H
#define __USB_DESC_H

/* Includes ------------------------------------------------------------------*/
#include "usb_conf.h"
#include "hw_config.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/


#define LANGID_STRINGID		(0)
#define VENDOR_STRINGID		(1)
#define PRODUCT_STRINGID	(2)
#define SERIAL_STRINGID		(3)



#define SPEAKER_SIZ_DEVICE_DESC                       18
#define SPEAKER_SIZ_CONFIG_DESC                       109
#define SPEAKER_SIZ_INTERFACE_DESC_SIZE               9

#define SPEAKER_SIZ_STRING_LANGID                     0x04
#define SPEAKER_SIZ_STRING_VENDOR                     0x26
#define SPEAKER_SIZ_STRING_PRODUCT                    0x1C
#define SPEAKER_SIZ_STRING_SERIAL                     0x1A

#define AUDIO_STANDARD_ENDPOINT_DESC_SIZE             0x09
#define AUDIO_STREAMING_ENDPOINT_DESC_SIZE            0x07
/* USB Descriptor Types */
#define USB_DEVICE_DESCRIPTOR_TYPE                    0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE             0x02
#define USB_STRING_DESCRIPTOR_TYPE                    0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE                 0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE                  0x05

#define USB_DEVICE_CLASS_AUDIO                        0x01
#define AUDIO_SUBCLASS_AUDIOCONTROL                   0x01
#define AUDIO_SUBCLASS_AUDIOSTREAMING                 0x02
#define AUDIO_PROTOCOL_UNDEFINED                      0x00
#define AUDIO_STREAMING_GENERAL                       0x01
#define AUDIO_STREAMING_FORMAT_TYPE                   0x02

/* Audio Descriptor Types */
#define AUDIO_INTERFACE_DESCRIPTOR_TYPE               0x24
#define AUDIO_ENDPOINT_DESCRIPTOR_TYPE                0x25


/* Audio Control Interface Descriptor Subtypes */
#define AUDIO_CONTROL_HEADER                          0x01
#define AUDIO_CONTROL_INPUT_TERMINAL                  0x02
#define AUDIO_CONTROL_OUTPUT_TERMINAL                 0x03
#define AUDIO_CONTROL_FEATURE_UNIT                    0x06

#define AUDIO_INPUT_TERMINAL_DESC_SIZE                0x0C
#define AUDIO_OUTPUT_TERMINAL_DESC_SIZE               0x09
#define AUDIO_STREAMING_INTERFACE_DESC_SIZE           0x07

#define AUDIO_CONTROL_MUTE                            0x0001

#define 	AUDIO_FEATURE_MUTE   (1 << 0)
#define 	AUDIO_FEATURE_VOLUME   (1 << 1)
#define 	AUDIO_FEATURE_BASS   (1 << 2)
#define 	AUDIO_FEATURE_MID   (1 << 3)
#define 	AUDIO_FEATURE_TREBLE   (1 << 4)
#define 	AUDIO_FEATURE_GRAPHIC_EQUALIZER   (1 << 5)
#define 	AUDIO_FEATURE_AUTOMATIC_GAIN   (1 << 6)
#define 	AUDIO_FEATURE_DELAY   (1 << 7)
#define 	AUDIO_FEATURE_BASS_BOOST   (1 << 8)
#define 	AUDIO_FEATURE_BASS_LOUDNESS   (1 << 9)

//see termt10.pdf
#define 	AUDIO_TERMINAL_UNDEFINED   0x0100
#define 	AUDIO_TERMINAL_STREAMING   0x0101
#define 	AUDIO_TERMINAL_VENDOR   0x01FF
#define 	AUDIO_TERMINAL_IN_UNDEFINED   0x0200
#define 	AUDIO_TERMINAL_IN_MIC   0x0201
#define 	AUDIO_TERMINAL_IN_DESKTOP_MIC   0x0202
#define 	AUDIO_TERMINAL_IN_PERSONAL_MIC   0x0203
#define 	AUDIO_TERMINAL_IN_OMNIDIR_MIC   0x0204
#define 	AUDIO_TERMINAL_IN_MIC_ARRAY   0x0205
#define 	AUDIO_TERMINAL_IN_PROCESSING_MIC   0x0206
#define 	AUDIO_TERMINAL_IN_OUT_UNDEFINED   0x0300
#define 	AUDIO_TERMINAL_OUT_SPEAKER   0x0301
#define 	AUDIO_TERMINAL_OUT_HEADPHONES   0x0302
#define 	AUDIO_TERMINAL_OUT_HEAD_MOUNTED   0x0303
#define 	AUDIO_TERMINAL_OUT_DESKTOP   0x0304
#define 	AUDIO_TERMINAL_OUT_ROOM   0x0305
//Headset earphone
#define 	AUDIO_TERMINAL_OUT_COMMUNICATION   0x0306
#define 	AUDIO_TERMINAL_OUT_LOWFREQ   0x0307

#define 	AUDIO_TERMINAL_TYPE	(0x0603)

#define AUDIO_FORMAT_TYPE_I                           0x01
#define AUDIO_FORMAT_TYPE_III                           0x03

#define USB_ENDPOINT_ATTRIBUTE_ASYNC				 (0x01 << 2)
#define USB_ENDPOINT_ATTRIBUTE_ADAPTIVE				 (0x02 << 2)
#define USB_ENDPOINT_ATTRIBUTE_SYNC				 	 (0x03 << 2)

#define USB_DESC_BYTE(val, n)	(((val) >> (8*n)) & 0xFF)

#define USB_AUDIO_FREQ_BYTE(n) (((AUDIO_FREQ) >> (8*n)) & 0xFF)



#define USB_ENDPOINT_TYPE_ISOCHRONOUS                 0x01 | USB_ENDPOINT_ATTRIBUTE_ASYNC
#define AUDIO_ENDPOINT_GENERAL                        0x01

/* Exported functions ------------------------------------------------------- */
extern const uint8_t Speaker_DeviceDescriptor[SPEAKER_SIZ_DEVICE_DESC];
extern const uint8_t Speaker_ConfigDescriptor[SPEAKER_SIZ_CONFIG_DESC];
extern const uint8_t Speaker_StringLangID[SPEAKER_SIZ_STRING_LANGID];
extern const uint8_t Speaker_StringVendor[SPEAKER_SIZ_STRING_VENDOR];
extern const uint8_t Speaker_StringProduct[SPEAKER_SIZ_STRING_PRODUCT];
extern uint8_t Speaker_StringSerial[SPEAKER_SIZ_STRING_SERIAL];

#endif /* __USB_DESC_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


