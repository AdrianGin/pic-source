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

#ifndef __USB_AUDIO_CORE_H_
#define __USB_AUDIO_CORE_H_

#include "usbd_ioreq.h"
#include "usbd_req.h"
#include "usbd_desc.h"
#include "usbd_conf.h"



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

/* AudioFreq * DataSize (2 bytes) * NumChannels (Stereo: 2) */
#define AUDIO_OUT_PACKET                              (uint32_t)(((USBD_AUDIO_FREQ * 2 * 2) /1000)) 

/* Number of sub-packets in the audio transfer buffer. You can modify this value but always make sure
  that it is an even number and higher than 3 */
#define OUT_PACKET_NUM                                   4
/* Total size of the audio transfer buffer */
#define TOTAL_OUT_BUF_SIZE                           ((uint32_t)(AUDIO_OUT_PACKET * OUT_PACKET_NUM))



#define AUDIO_CONFIG_DESC_SIZE                        0x09
#define AUDIO_INTERFACE_DESC_SIZE                     (0x09)
//#define AUDIO_CLASS_INTERFACE_SIZE		     		  (0x08 + USBD_AUDIO_STREAMING_COUNT + 1) //Extra for MIDI Streaming

#define AUDIO_AS_CLASS_INTERFACE_SIZE		     		  (0x08 + USBD_AUDIO_STREAMING_COUNT + USBD_MIDI_STREAMING_COUNT) //Extra for MIDI Streaming
#define AUDIO_MS_CLASS_INTERFACE_SIZE		     		  (0x08 + 1) //Extra for MIDI Streaming


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


#define AUDIO_CLASS_SPECIFIC_INTERFACE_SIZE			(AUDIO_AS_CLASS_INTERFACE_SIZE + (AUDIO_INPUT_TERMINAL_COUNT*AUDIO_INPUT_TERMINAL_DESC_SIZE) + (AUDIO_OUTPUT_TERMINAL_COUNT*AUDIO_OUTPUT_TERMINAL_DESC_SIZE) + (AUDIO_FEATURE_UNIT_COUNT*AUDIO_FEATURE_UNIT_DESC_SIZE))

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


#define MIDICLASS_SUBTYPE_MIDI_IN_JACK 	(0x02)
#define MIDICLASS_SUBTYPE_MIDI_OUT_JACK (0x03)
#define MIDICLASS_SUBTYPE_ELEMENT 		(0x04)

#define MIDI_EMBEDDED_JACK	(0x01)
#define MIDI_EXTERNAL_JACK	(0x02)
//Allow for upto 16 IN/OUT ports
#define USBDESC_MIDI_ELEMENT_ID						  (0x01)
#define USBDESC_MIDI_IN_EMB_JACKID_BASE					  (0x02)
#define USBDESC_MIDI_IN_EXT_JACKID_BASE					  (USBDESC_MIDI_IN_EMB_JACKID_BASE + EMBEDDED_MIDI_IN_COUNT)

#define USBDESC_MIDI_OUT_EMB_JACKID_BASE					  (USBDESC_MIDI_IN_EXT_JACKID_BASE + 1)
#define USBDESC_MIDI_OUT_EXT_JACKID_BASE					  (USBDESC_MIDI_OUT_EMB_JACKID_BASE + EMBEDDED_MIDI_OUT_COUNT)

//By default External MIDI jacks are connected to the MIDI Element (for capability bit)
#define USBDESC_MIDI_ELEMENT_SIZE (10+2+EXTERNAL_MIDI_OUT_COUNT+EXTERNAL_MIDI_IN_COUNT+1)

#define USBDESC_INTERFACE_SIZE               9

#define MIDI_ELEMENT_CAPABILITY_CUSTOM 		(0x01)
#define MIDI_ELEMENT_CAPABILITY_MIDI_CLOCK  (0x02)
#define MIDI_ELEMENT_CAPABILITY_MTC (0x04)
#define MIDI_ELEMENT_CAPABILITY_MMC (0x08)
#define MIDI_ELEMENT_CAPABILITY_GM1 (0x10)
#define MIDI_ELEMENT_CAPABILITY_GM2 (0x20)
#define MIDI_ELEMENT_CAPABILITY_GS (0x40)
#define MIDI_ELEMENT_CAPABILITY_XG (0x80)


#define USBDESC_CS_INTERFACE_SIZE  (AUDIO_INTERFACE_DESC_SIZE)


#define USBDESC_AUDIO_STREAMING_CONFIG_SIZE (AUDIO_INTERFACE_DESC_SIZE + AUDIO_AS_CLASS_INTERFACE_SIZE + (AUDIO_INPUT_TERMINAL_COUNT*AUDIO_INPUT_TERMINAL_DESC_SIZE) + \
											(AUDIO_OUTPUT_TERMINAL_COUNT*AUDIO_OUTPUT_TERMINAL_DESC_SIZE) + (AUDIO_FEATURE_UNIT_COUNT*AUDIO_FEATURE_UNIT_DESC_SIZE) + \
											( (AUDIO_INTERFACE_DESC_SIZE*2) + \
											  AUDIO_STREAMING_INTERFACE_DESC_SIZE + \
											  AUDIO_FORMAT_INTERFACE_DESC_SIZE + \
											  AUDIO_STANDARD_ENDPOINT_DESC_SIZE + \
											  AUDIO_STREAMING_ENDPOINT_DESC_SIZE) * USBD_AUDIO_STREAMING_COUNT)

#define MIDI_CLASS_SPECIFIC_INTERFACE_SIZE			(USBDESC_MIDI_STREAMING_SIZE + \
													(EMBEDDED_MIDI_OUT_COUNT+EXTERNAL_MIDI_OUT_COUNT)*(USBDESC_MIDI_OUT_SIZE) + \
													(EMBEDDED_MIDI_IN_COUNT+EXTERNAL_MIDI_IN_COUNT)*(USBDESC_MIDI_IN_SIZE) + \
													MIDI_ELEMENT_COUNT*USBDESC_MIDI_ELEMENT_SIZE)
#if USBD_MIDI_STREAMING_COUNT >= 1
#define USBDESC_MIDI_STREAMING_CONFIG_SIZE ( USBDESC_MIDI_INTERFACE_SIZE + MIDI_CLASS_SPECIFIC_INTERFACE_SIZE + \
											(USBDESC_MIDI_ENDPOINT_SIZE*2) + \
											(USBDESC_MIDI_CLASS_ENDPOINT_SIZE+EMBEDDED_MIDI_IN_COUNT) + \
											(USBDESC_MIDI_CLASS_ENDPOINT_SIZE+EMBEDDED_MIDI_OUT_COUNT))
#else
#define USBDESC_MIDI_STREAMING_CONFIG_SIZE (0)
#endif

#define IAD_CONFIG_SIZE (0x00)

#define USBDESC_CONFIG_SIZE          (AUDIO_CONFIG_DESC_SIZE + IAD_CONFIG_SIZE + \
									 USBDESC_MIDI_STREAMING_CONFIG_SIZE + \
									 USBDESC_AUDIO_STREAMING_CONFIG_SIZE)


extern uint8_t IsoOutBuff[];


/**
  * @}
  */ 


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */
typedef struct _Audio_Fops
{
    uint8_t  (*Init)         (uint32_t  AudioFreq, uint32_t Volume, uint32_t options);
    uint8_t  (*DeInit)       (uint32_t options);
    uint8_t  (*AudioCmd)     (uint8_t* pbuf, uint32_t size, uint8_t cmd);
    uint8_t  (*VolumeCtl)    (uint8_t vol);
    uint8_t  (*MuteCtl)      (uint8_t cmd);
    uint8_t  (*PeriodicTC)   (uint8_t cmd);
    uint8_t  (*GetState)     (void);
}AUDIO_FOPS_TypeDef;
/**
  * @}
  */ 


typedef enum
{
	USBMIDI_READY = 0x00,
	USBMIDI_TRANSMITING,
	USBMIDI_PROCESSING,
} USBMIDI_State;


typedef struct
{
	uint8_t* buf;
	uint16_t  bufSize;

	volatile uint16_t  bufWritePtr;
	volatile uint16_t  bufReadPtr;

	//epnum: 0x01, 0x81, 0x02, 0x82...
	uint8_t  epnum;

	volatile USBMIDI_State  state;
} USBMIDI_Port_t;





/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */ 
#define AUDIO_PACKET_SZE(frq)          (uint8_t)(((frq * AUDIO_CHANNEL_COUNT * AUDIO_FRAME_SIZE)/1000) & 0xFF), \
                                       (uint8_t)((((frq * AUDIO_CHANNEL_COUNT * AUDIO_FRAME_SIZE)/1000) >> 8) & 0xFF)


#define SAMPLE_FREQ(frq)               (uint8_t)(frq), (uint8_t)((frq >> 8)), (uint8_t)((frq >> 16))
/**
  * @}
  */ 

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */ 

extern USBD_Class_cb_TypeDef  AUDIO_cb;


extern uint8_t  USBMIDI_InPtr;
extern uint32_t  APP_Rx_ptr_in;

/**
  * @}
  */ 

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */


extern USBMIDI_Port_t USBMIDI_Tx;
extern USBMIDI_Port_t USBMIDI_Rx;

uint16_t USBD_audio_TxFIFO_WordsAvailable(uint8_t ep_addr);
uint8_t USB_audio_RxData(uint8_t  *pbuf, uint32_t buf_len);


static inline void USBD_audio_TxEP(void *pdev, uint8_t ep_addr, uint8_t *pbuf, uint32_t buf_len)
{
	DCD_EP_Tx( (USB_OTG_CORE_HANDLE*)pdev, ep_addr, pbuf, buf_len);
}


void USBD_audio_Disconnect(void);

uint8_t USBD_audio_RxEPReady(uint8_t epnum);

uint32_t USBD_audio_TxEPStatus(uint8_t epnum);

uint32_t USB_audio_MIDI_DataReceived(uint8_t epnum);


/* USB MIDI Port access functions */
static inline void USBD_audio_SetState(USBMIDI_Port_t* port, USBMIDI_State newState)
{
	port->state = newState;
}

static inline USBMIDI_State USBD_audio_GetState(USBMIDI_Port_t* port)
{
	return port->state;
}

static inline uint16_t USBD_audio_MIDI_Port_byteCount(USBMIDI_Port_t* port)
{
	return port->bufWritePtr - port->bufReadPtr;
}

static inline uint16_t USBD_audio_MIDI_Port_GetReadPtr(USBMIDI_Port_t* port)
{
	return port->bufReadPtr;
}

static inline uint16_t USBD_audio_MIDI_Port_GetWritePtr(USBMIDI_Port_t* port)
{
	return port->bufWritePtr;
}

static inline void USBD_audio_MIDI_Port_SetReadPtr(USBMIDI_Port_t* port, uint16_t newVal)
{
	port->bufReadPtr = newVal;
}

static inline void USBD_audio_MIDI_Port_SetWritePtr(USBMIDI_Port_t* port, uint16_t newVal)
{
	port->bufWritePtr = newVal;
}


static inline void USDB_audio_MIDI_Port_FlushBuffers(USBMIDI_Port_t* port)
{
   USBD_audio_MIDI_Port_SetReadPtr(port, 0);
   USBD_audio_MIDI_Port_SetWritePtr(port, 0);
}





/**
  * @}
  */ 

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
