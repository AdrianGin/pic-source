/**
  ******************************************************************************
  * @file    usbd_audio_core.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    19-March-2012
  * @brief   header file for the usbd_audio_core.c file.
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

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __USB_AUDIO_ENUMS_H_
#define __USB_AUDIO_ENUMS_H_


/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */
  
/** @defgroup usbd_audio
  * @brief This file is the Header file for USBD_audio.c
  * @{
  */ 


/** @defgroup usbd_audio_Exported_Defines
  * @{
  */ 


#define VENDOR_SPECIFIC_INTERFACE_CLASS (0xFF)

#define AUDIO_CONFIG_DESC_SIZE                        0x09
#define AUDIO_INTERFACE_DESC_SIZE                     (0x09)


#define USB_AUDIO_DESC_SIZ                            0x09
#define AUDIO_STANDARD_ENDPOINT_DESC_SIZE             0x09
#define AUDIO_STREAMING_ENDPOINT_DESC_SIZE            0x07

#define AUDIO_DESCRIPTOR_TYPE                         0x21
#define USB_DEVICE_CLASS_AUDIO                        0x01
#define AUDIO_SUBCLASS_AUDIOCONTROL                   0x01
#define AUDIO_SUBCLASS_AUDIOSTREAMING                 0x02
#define AUDIO_PROTOCOL_UNDEFINED                      0x00
#define AUDIO_STREAMING_GENERAL                       0x01
#define AUDIO_STREAMING_FORMAT_TYPE                   0x02
#define MIDI_STREAMING								  0x03

/* Audio Descriptor Types */
#define AUDIO_INTERFACE_DESCRIPTOR_TYPE               0x24
#define AUDIO_ENDPOINT_DESCRIPTOR_TYPE                0x25

/* Audio Control Interface Descriptor Subtypes */
#define AUDIO_CONTROL_HEADER                          0x01
#define AUDIO_CONTROL_INPUT_TERMINAL                  0x02
#define AUDIO_CONTROL_OUTPUT_TERMINAL                 0x03
#define AUDIO_CONTROL_FEATURE_UNIT                    0x06

#define AUDIO_INPUT_TERMINAL_DESC_SIZE                0x0C
#define AUDIO_FEATURE_UNIT_DESC_SIZE				  (0x09)
#define AUDIO_OUTPUT_TERMINAL_DESC_SIZE               0x09


#define AUDIO_STREAMING_INTERFACE_DESC_SIZE           0x07
#define AUDIO_FORMAT_INTERFACE_DESC_SIZE			  (0x0B)

#define AUDIO_USB_STREAMING_TERMINAL_TYPE			  (0x0101)
//Can use these to get different looking Icons in Windows Mixer
//http://www.usb.org/developers/docs/devclass_docs/termt10.pdf
#define AUDIO_TERMINAL_TYPE_MIC						  (0x0201)
#define AUDIO_TERMINAL_TYPE_ANALOGUE				  (0x0603)
#define AUDIO_TERMINAL_TYPE_SPDIF					  (0x0605)
#define AUDIO_TERMINAL_TYPE_SYNTH					  (0x0713)
#define AUDIO_TERMINAL_TYPE_SPEAKER					  (0x0301)
#define AUDIO_TERMINAL_TYPE_HEADPHONE				  (0x0302)

enum
{
	AUDIO_CONTROL_MUTE = 0x0001,
	AUDIO_CONTROL_VOLUME,
};


enum
{
	AUDIO_SYNC_ASYNC = (0x01 << 2),
	AUDIO_SYNC_ADAPT = (0x02 << 2),
	AUDIO_SYNC_SYNC =  (0x03 << 2),
};
#define AUDIO_SYNC_ASYNC	(0x01 << 2)

#define AUDIO_FORMAT_TYPE_I                           0x01
#define AUDIO_FORMAT_TYPE_III                         0x03

#define USB_ENDPOINT_TYPE_ISOCHRONOUS                 0x01
#define AUDIO_ENDPOINT_GENERAL                        0x01

enum
{
	AUDIO_REQ_SET_CUR = 0x01,
	AUDIO_REQ_GET_CUR = 0x81,

	AUDIO_REQ_SET_MIN = 0x02,
	AUDIO_REQ_GET_MIN = 0x82,

	AUDIO_REQ_SET_MAX = 0x03,
	AUDIO_REQ_GET_MAX = 0x83,

	AUDIO_REQ_SET_RES = 0x04,
	AUDIO_REQ_GET_RES = 0x84,
};



/* USB Audio MIDI Defines */
#define USBDESC_MIDI_ENDPOINT_SIZE				0x09
#define USBDESC_MIDI_CLASS_ENDPOINT_SIZE	(0x04)
#define USBDESC_MIDI_INTERFACE_SIZE		(0x09)
#define USBDESC_MIDI_STREAMING_SIZE		(0x07)
#define USBDESC_MIDI_IN_SIZE (0x06)
#define USBDESC_MIDI_OUT_SIZE (0x09)

#define USB_MIDI_OUT_BUF_SIZE       (64)
#define USB_MIDI_IN_BUF_SIZE       (64)

#define MIDI_STREAMING_INTERFACE_HEADER (0x24)

#define MIDICLASS_SUBTYPE_MIDI_IN_JACK 	(0x02)
#define MIDICLASS_SUBTYPE_MIDI_OUT_JACK (0x03)
#define MIDICLASS_SUBTYPE_ELEMENT 		(0x04)

#define MIDI_EMBEDDED_JACK	(0x01)
#define MIDI_EXTERNAL_JACK	(0x02)

#define USBDESC_INTERFACE_SIZE               9

#define MIDI_ELEMENT_CAPABILITY_CUSTOM 		(0x01)
#define MIDI_ELEMENT_CAPABILITY_MIDI_CLOCK  (0x02)
#define MIDI_ELEMENT_CAPABILITY_MTC (0x04)
#define MIDI_ELEMENT_CAPABILITY_MMC (0x08)
#define MIDI_ELEMENT_CAPABILITY_GM1 (0x10)
#define MIDI_ELEMENT_CAPABILITY_GM2 (0x20)
#define MIDI_ELEMENT_CAPABILITY_GS (0x40)
#define MIDI_ELEMENT_CAPABILITY_XG (0x80)


#endif  // __USB_AUDIO_CORE_H_
/**
  * @}
  */ 

/**
  * @}
  */ 
  
#ifdef __cplusplus
}
#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
