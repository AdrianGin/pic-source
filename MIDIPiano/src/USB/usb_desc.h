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
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/


#define LANGID_STRINGID		(0)
#define VENDOR_STRINGID		(1)
#define PRODUCT_STRINGID	(2)
#define SERIAL_STRINGID		(3)


#define USB_MIDI_MAX_CABLE_COUNT (16)
#define USBDESC_DEVICE_SIZE                       18
#define USBDESC_CONFIG_SIZE             USBDESC_INTERFACE_SIZE*3 + \
										USBDESC_MIDI_INTERFACE_SIZE + \
										USBDESC_MIDI_STREAMING_SIZE + \
										USB_MIDI_CABLE_COUNT*2*(USBDESC_MIDI_OUT_SIZE+USBDESC_MIDI_IN_SIZE) + \
										USBDESC_MIDI_ENDPOINT_SIZE*2 + \
										(USBDESC_MIDI_CLASS_ENDPOINT_SIZE+USB_MIDI_CABLE_COUNT) * 2



#define USBDESC_INTERFACE_SIZE               9

#define MIDI_EMBEDDED_JACK	(0x01)
#define MIDI_EXTERNAL_JACK	(0x02)
//Allow for upto 16 IN/OUT ports
#define USBDESC_MIDI_IN_JACKID_BASE					  (0x01)
#define USBDESC_MIDI_OUT_JACKID_BASE					(USBDESC_MIDI_IN_JACKID_BASE+USB_MIDI_MAX_CABLE_COUNT*2)

/* Audio Descriptor Types */
#define AUDIO_INTERFACE_DESCRIPTOR_TYPE               0x24
#define AUDIO_ENDPOINT_DESCRIPTOR_TYPE                0x25

#define USBDESC_MIDI_ENDPOINT_SIZE				0x09
#define USBDESC_MIDI_CLASS_ENDPOINT_SIZE	(0x04)
#define USBDESC_MIDI_INTERFACE_SIZE		(0x09)
#define USBDESC_MIDI_STREAMING_SIZE		(0x07)
#define USBDESC_MIDI_IN_SIZE (0x06)
#define USBDESC_MIDI_OUT_SIZE (0x09)

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




/* Audio Control Interface Descriptor Subtypes */
#define AUDIO_CONTROL_HEADER                          0x01
#define AUDIO_CONTROL_INPUT_TERMINAL                  0x02
#define AUDIO_CONTROL_OUTPUT_TERMINAL                 0x03
#define AUDIO_CONTROL_FEATURE_UNIT                    0x06

#define AUDIO_INPUT_TERMINAL_DESC_SIZE                0x0C
#define AUDIO_OUTPUT_TERMINAL_DESC_SIZE               0x09
#define AUDIO_STREAMING_INTERFACE_DESC_SIZE           0x07

#define AUDIO_CONTROL_MUTE                            0x0001

#define AUDIO_FORMAT_TYPE_I                           0x01

#define USB_ENDPOINT_TYPE_ISOCHRONOUS                 0x01
#define AUDIO_ENDPOINT_GENERAL                        0x01

/* Exported functions ------------------------------------------------------- */
extern const uint8_t Speaker_DeviceDescriptor[];
extern const uint8_t Speaker_ConfigDescriptor[];
extern const uint8_t* USBDesciptors[];


#endif /* __USB_DESC_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


