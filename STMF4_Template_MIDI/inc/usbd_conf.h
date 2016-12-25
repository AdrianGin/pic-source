/**
  ******************************************************************************
  * @file    usbd_conf.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    19-March-2012
  * @brief   USB Device configuration file
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
#ifndef __USBD_CONF__H__
#define __USBD_CONF__H__

/* Includes ------------------------------------------------------------------*/
#include "usb_conf.h"

/** @defgroup USB_CONF_Exported_Defines
  * @{
  */ 

/* Audio frequency in Hz */
#ifndef EXTERNAL_CRYSTAL_25MHz
 #define USBD_AUDIO_FREQ                48000 /* Audio frequency in Hz.
                                                 It is advised to set standard frequencies >= 24KHz to get best quality
                                                 except for STM32F10 devices, when the HSE value is 25MHz, it is advised to
                                                 set audio frequencies reachable with this HSE value (refer to RM0008 for
                                                 more details). ie. it is advised to set 16KHz value in this case. 
                                                 Note that maximum allowed audio frequency is 96KHz (this limitation is 
                                                 due to the codec used on the Evaluation board. The STM32 I2S cell allows
                                                 to  reach 192KHz frequency).
                                                 @note
                                                    When modifying this value, make sure that the I2S PLL configuration allows
                                                    to get minimum error on audio frequency. This configuration is set in file
                                                    system_stm32f2xx.c or system_stm32f10x.c.*/
#else
 #define USBD_AUDIO_FREQ                16000  /* Audio frequency in Hz for STM32F10x devices family when 25MHz HSE value
                                                  is used. */
#endif /* EXTERNAL_CRYSTAL_25MHz */

#define DEFAULT_VOLUME                  65    /* Default volume in % (Mute=0%, Max = 100%) in Logarithmic values.
                                                 To get accurate volume variations, it is possible to use a logarithmic
                                                 coversion table to convert from percentage to logarithmic law.
                                                 In order to keep this example code simple, this conversion is not used.*/

/* Use this section to modify the number of supported interfaces and configurations.
   Note that if you modify these parameters, you have to modify the descriptors 
   accordingly in usbd_audio_core.c file */

#define USBD_CFG_MAX_NUM                1

#define USB_MAX_STR_DESC_SIZ            200 

#define USBD_SELF_POWERED               

#define USBD_MIDI_STREAMING_COUNT (1)

#ifdef USE_USB_AUDIO
#define USBD_AUDIO_STREAMING_TO_HOST_COUNT			(1)
#define USBD_AUDIO_STREAMING_TO_DEV_COUNT			(1)
#else
#define USBD_AUDIO_STREAMING_TO_HOST_COUNT			(0)
#define USBD_AUDIO_STREAMING_TO_DEV_COUNT			(0)
#endif

#define USBD_AUDIO_STREAMING_COUNT (USBD_AUDIO_STREAMING_TO_HOST_COUNT+USBD_AUDIO_STREAMING_TO_DEV_COUNT)

/**
  * @}
  */ 

#define AUDIO_CONTROL_INTERFACE_IDX  (0)

#define AUDIO_STREAMING_TODEVICE_INTERFACE_IDX (AUDIO_CONTROL_INTERFACE_IDX+USBD_AUDIO_STREAMING_TO_DEV_COUNT)
#define AUDIO_STREAMING_TOHOST_INTERFACE_IDX (AUDIO_STREAMING_TODEVICE_INTERFACE_IDX+USBD_AUDIO_STREAMING_TO_HOST_COUNT)

#define AUDIO_CONTROL_MIDI_INTERFACE_IDX (AUDIO_STREAMING_TOHOST_INTERFACE_IDX)
#define MIDI_STREAMING_INTERFACE_IDX (AUDIO_CONTROL_MIDI_INTERFACE_IDX+USBD_MIDI_STREAMING_COUNT)

#if USBD_AUDIO_STREAMING_COUNT > 0
#define AUDIO_CONTROL_INTERFACE_COUNT (1)
#define AUDIO_STREAMING_IF_COUNT	  ()
#define AUDIO_TOTAL_IF_NUM              (AUDIO_CONTROL_INTERFACE_COUNT + USBD_AUDIO_STREAMING_COUNT + USBD_MIDI_STREAMING_COUNT)
#else
#define AUDIO_TOTAL_IF_NUM              0x02
#endif

#define USBD_ITF_MAX_NUM                AUDIO_TOTAL_IF_NUM

#define AUDIO_INPUT_TERMINAL_COUNT	(USBD_AUDIO_STREAMING_TO_DEV_COUNT+USBD_AUDIO_STREAMING_TO_HOST_COUNT)
#define AUDIO_OUTPUT_TERMINAL_COUNT	(USBD_AUDIO_STREAMING_TO_DEV_COUNT+USBD_AUDIO_STREAMING_TO_HOST_COUNT)
#define AUDIO_FEATURE_UNIT_COUNT    (USBD_AUDIO_STREAMING_TO_DEV_COUNT+USBD_AUDIO_STREAMING_TO_HOST_COUNT)


enum
{
	  AUDIO_DEVICE_INPUT_ID = 0x01,
	  AUDIO_FEATURE_UNIT_ID,
	  AUDIO_AS_OUTPUT_ID,
	  AUDIO_AS_INPUT_ID,
	  AUDIO_FEATURE_UNIT_ID2,
	  AUDIO_DEVICE_OUTPUT_ID,
};

//IN EP
#define AUDIO_CHANNEL_COUNT		(0x02)
#define AUDIO_FRAME_SIZE		(0x02) //24bit of audio
#define AUDIO_BIT_DEPTH			(16) //24bit of audio

#define AUDIO_OUT_EP_PACKET_SIZE (AUDIO_CHANNEL_COUNT*AUDIO_FRAME_SIZE*USBD_AUDIO_FREQ/1000)

//OUT EP
#define AUDIO_OUTEP_CHANNEL_COUNT (0x02)

/** @defgroup USB_AUDIO_Class_Layer_Parameter
  * @{
  */ 
#define AUDIO_OUT_EP                    0x03
#define AUDIO_IN_EP                     0x82

#define MIDI_OUT_EP                     0x01
#define MIDI_IN_EP                      0x81

#define EMBEDDED_MIDI_IN_COUNT (1)
#define EMBEDDED_MIDI_OUT_COUNT (1)

#define EXTERNAL_MIDI_IN_COUNT (1)
#define EXTERNAL_MIDI_OUT_COUNT (1)

//Represents the Synth/Sequencer
#define MIDI_ELEMENT_COUNT (1)

#define CDC_DATA_IN_PACKET_SIZE (64)
#define APP_RX_DATA_SIZE               64

/**
  * @}
  */ 

/** @defgroup USB_CONF_Exported_Types
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USB_CONF_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USB_CONF_Exported_Variables
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USB_CONF_Exported_FunctionsPrototype
  * @{
  */ 
/**
  * @}
  */ 


#endif //__USBD_CONF__H__

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

