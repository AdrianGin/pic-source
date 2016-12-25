/**
 ******************************************************************************
 * @file    usbd_audio_core.c
 * @author  MCD Application Team
 * @version V1.1.0
 * @date    19-March-2012
 * @brief   This file provides the high layer firmware functions to manage the
 *          following functionalities of the USB Audio Class:
 *           - Initialization and Configuration of high and low layer
 *           - Enumeration as Audio Streaming Device
 *           - Audio Streaming data transfer
 *           - AudioControl requests management
 *           - Error management
 *
 *  @verbatim
 *
 *          ===================================================================
 *                                Audio Class Driver Description
 *          ===================================================================
 *           This driver manages the Audio Class 1.0 following the "USB Device Class Definition for
 *           Audio Devices V1.0 Mar 18, 98".
 *           This driver implements the following aspects of the specification:
 *             - Device descriptor management
 *             - Configuration descriptor management
 *             - Standard AC Interface Descriptor management
 *             - 1 Audio Streaming Interface (with single channel, PCM, Stereo mode)
 *             - 1 Audio Streaming Endpoint
 *             - 1 Audio Terminal Input (1 channel)
 *             - Audio Class-Specific AC Interfaces
 *             - Audio Class-Specific AS Interfaces
 *             - AudioControl Requests: only SET_CUR and GET_CUR requests are supported (for Mute)
 *             - Audio Feature Unit (limited to Mute control)
 *             - Audio Synchronization type: Asynchronous
 *             - Single fixed audio sampling rate (configurable in usbd_conf.h file)
 *
 *           @note
 *            The Audio Class 1.0 is based on USB Specification 1.0 and thus supports only
 *            Low and Full speed modes and does not allow High Speed transfers.
 *            Please refer to "USB Device Class Definition for Audio Devices V1.0 Mar 18, 98"
 *            for more details.
 *
 *           These aspects may be enriched or modified for a specific user application.
 *
 *            This driver doesn't implement the following aspects of the specification
 *            (but it is possible to manage these features with some modifications on this driver):
 *             - AudioControl Endpoint management
 *             - AudioControl requsests other than SET_CUR and GET_CUR
 *             - Abstraction layer for AudioControl requests (only Mute functionality is managed)
 *             - Audio Synchronization type: Adaptive
 *             - Audio Compression modules and interfaces
 *             - MIDI interfaces and modules
 *             - Mixer/Selector/Processing/Extension Units (Feature unit is limited to Mute control)
 *             - Any other application-specific modules
 *             - Multiple and Variable audio sampling rates
 *             - Out Streaming Endpoint/Interface (microphone)
 *
 *  @endverbatim
 *
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

#include "usbd_audio_core.h"
#include "usbd_audio_out_if.h"
#include "usbd_conf.h"
#include "USB.h"
//#include "midi_parsers.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
 * @{
 */

/** @defgroup usbd_audio 
 * @brief usbd core module
 * @{
 */

/** @defgroup usbd_audio_Private_TypesDefinitions
 * @{
 */
/**
 * @}
 */

/** @defgroup usbd_audio_Private_Defines
 * @{
 */
/**
 * @}
 */

/** @defgroup usbd_audio_Private_Macros
 * @{
 */
/**
 * @}
 */

/** @defgroup usbd_audio_Private_FunctionPrototypes
 * @{
 */

/*********************************************
 AUDIO Device library callbacks
 *********************************************/
static uint8_t usbd_audio_Init(void *pdev, uint8_t cfgidx);
static uint8_t usbd_audio_DeInit(void *pdev, uint8_t cfgidx);
static uint8_t usbd_audio_Setup(void *pdev, USB_SETUP_REQ *req);
static uint8_t usbd_audio_EP0_RxReady(void *pdev);
static uint8_t usbd_audio_DataIn(void *pdev, uint8_t epnum);
static uint8_t usbd_audio_DataOut(void *pdev, uint8_t epnum);
static uint8_t usbd_audio_SOF(void *pdev);
static uint8_t usbd_audio_OUT_Incplt(void *pdev);

/*********************************************
 AUDIO Requests management functions
 *********************************************/
static void AUDIO_Req_GetCurrent(void *pdev, USB_SETUP_REQ *req);
static void AUDIO_Req_SetCurrent(void *pdev, USB_SETUP_REQ *req);
static uint8_t *USBD_audio_GetCfgDesc(uint8_t speed, uint16_t *length);
/**
 * @}
 */

/** @defgroup usbd_audio_Private_Variables
 * @{
 */

uint8_t IsoOutBuff[2];

/* Main Buffer for Audio Data Out transfers and its relative pointers */
uint8_t IsocOutBuff[2];
uint8_t* IsocOutWrPtr = IsocOutBuff;
uint8_t* IsocOutRdPtr = IsocOutBuff;

//uint8_t  USBMIDI_InBuff[USB_MIDI_IN_BUF_SIZE];
uint8_t USBMIDI_InBuff[USB_MIDI_IN_BUF_SIZE];
uint8_t USBMIDI_OutBuff[USB_MIDI_OUT_BUF_SIZE];

uint8_t USBMIDI_InBuff_HS[USB_MIDI_IN_BUF_SIZE];
uint8_t USBMIDI_OutBuff_HS[USB_MIDI_OUT_BUF_SIZE];


uint8_t USBMIDI_InLen = 0;
uint8_t USBMIDI_InPtr = 0;

uint32_t APP_Rx_ptr_in = 0;
uint32_t APP_Rx_ptr_out = 0;
uint32_t APP_Rx_length = 0;

USBMIDI_Port_t USBMIDI_Tx =
      {
      .buf = USBMIDI_InBuff,
            .bufSize = USB_MIDI_IN_BUF_SIZE,
            .epnum = MIDI_IN_EP,
            .state = USBMIDI_READY,
            .pdev = &USB_OTG_dev,
      };

USBMIDI_Port_t USBMIDI_Rx =
      {
      .buf = USBMIDI_OutBuff,
            .bufSize = USB_MIDI_OUT_BUF_SIZE,
            .epnum = MIDI_OUT_EP,
            .state = USBMIDI_READY,
            .pdev = &USB_OTG_dev,
      };

USBMIDI_Port_t USBMIDI_Tx_HS =
      {
      .buf = USBMIDI_InBuff_HS,
            .bufSize = USB_MIDI_IN_BUF_SIZE,
            .epnum = MIDI_IN_EP,
            .state = USBMIDI_READY,
            .pdev = &USB_OTG_dev_HS,
      };

USBMIDI_Port_t USBMIDI_Rx_HS =
      {
      .buf = USBMIDI_OutBuff_HS,
            .bufSize = USB_MIDI_OUT_BUF_SIZE,
            .epnum = MIDI_OUT_EP,
            .state = USBMIDI_READY,
            .pdev = &USB_OTG_dev_HS,
      };

//uint8_t*  USBMIDI_OutBuff = &MIDIToDev[0].Fifo;

/* Main Buffer for Audio Control Rrequests transfers and its relative variables */
uint8_t AudioCtl[64];
uint8_t AudioCtlCmd = 0;
uint32_t AudioCtlLen = 0;
uint8_t AudioCtlUnit = 0;

static uint32_t PlayFlag = 0;

static __IO uint32_t usbd_audio_AltSet = 0;

/* AUDIO interface class callbacks structure */
USBD_Class_cb_TypeDef AUDIO_cb =
      {
      usbd_audio_Init, usbd_audio_DeInit, usbd_audio_Setup,
      NULL, /* EP0_TxSent */
      usbd_audio_EP0_RxReady, usbd_audio_DataIn, usbd_audio_DataOut, usbd_audio_SOF,
      NULL, usbd_audio_OUT_Incplt, USBD_audio_GetCfgDesc,
            #ifdef USB_OTG_HS_CORE
            USBD_audio_GetCfgDesc,
      /* use same config as per FS */
#endif
#ifdef USB_SUPPORT_USER_STRING_DESC
      USBD_USR_USRStringDesc,
#endif
   };

/* USB AUDIO device Configuration Descriptor */
static const uint8_t usbd_audio_CfgDesc[USBDESC_CONFIG_SIZE] =
      {
      /* Configuration 1 */
      0x09, /* bLength */
      USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType */
      LOBYTE(USBDESC_CONFIG_SIZE), /* wTotalLength  109 bytes*/
      HIBYTE(USBDESC_CONFIG_SIZE), 0x02, /* bNumInterfaces */
      0x01, /* bConfigurationValue */
      0x00, /* iConfiguration */
      0xC0, /* bmAttributes  BUS Powred*/
      0x32, /* bMaxPower = 100 mA*/
      /* 09 byte*/

      /* USB Speaker Standard interface descriptor */
      AUDIO_INTERFACE_DESC_SIZE, /* bLength */
      USB_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType */
      0x00, /* bInterfaceNumber */
      0x00, /* bAlternateSetting */
      0x00, /* bNumEndpoints */
      USB_DEVICE_CLASS_AUDIO, /* bInterfaceClass */
      AUDIO_SUBCLASS_AUDIOCONTROL, /* bInterfaceSubClass */
      AUDIO_PROTOCOL_UNDEFINED, /* bInterfaceProtocol */
      0x02, /* string index for interface */
      /* 09 byte*/

      /* USB Speaker Class-specific AC Interface Descriptor */
      AUDIO_INTERFACE_DESC_SIZE, /* bLength */
      AUDIO_INTERFACE_DESCRIPTOR_TYPE, /* bDescriptorType */
      AUDIO_CONTROL_HEADER, /* bDescriptorSubtype */
      0x00, /* 1.00 *//* bcdADC */
      0x01, 0x9, /* wTotalLength */
      0x00, 0x01, /* bInCollection */
      0x01, /* baInterfaceNr */
      /* 09 byte*/

      // B.4 MIDIStreaming Interface Descriptors
      // B.4.1 Standard MS Interface Descriptor
            /* interface descriptor follows inline: */
            USBDESC_MIDI_INTERFACE_SIZE, /* length of descriptor in bytes */
            USB_INTERFACE_DESCRIPTOR_TYPE, /* descriptor type */
            1, /* index of this interface */
            0, /* alternate setting for this interface */
            2, /* endpoints excl 0: number of endpoint descriptors to follow */
            1, /* AUDIO */
            3, /* MIDI STREAMING */
            0, /* unused */
            2, /* string index for interface */

            // B.4.2 Class-specific MS Interface Descriptor
            /* MS Class-Specific descriptor */
            USBDESC_MIDI_STREAMING_SIZE, /* length of descriptor in bytes */
            AUDIO_INTERFACE_DESCRIPTOR_TYPE, /* descriptor type */
            1, /* header functional descriptor */
            0x0,
            0x01, /* bcdADC */
            (MIDI_CLASS_SPECIFIC_INTERFACE_SIZE & 0xFF),
            ((MIDI_CLASS_SPECIFIC_INTERFACE_SIZE & 0xFF00) >> 8), /* wTotalLength */

#if EMBEDDED_MIDI_IN_COUNT > 0
            // MIDI IN Jack Descriptor (Embedded)
            6,// Descriptor length
            AUDIO_INTERFACE_DESCRIPTOR_TYPE,       // Descriptor type (CS_INTERFACE)
            MIDICLASS_SUBTYPE_MIDI_IN_JACK,             // MIDI_IN_JACK subtype
            MIDI_EMBEDDED_JACK,             // EMBEDDED
            USBDESC_MIDI_IN_EMB_JACKID_BASE,             // ID of this jack
            0x00,             // unused, refers to text string of this.
#endif

#if EMBEDDED_MIDI_IN_COUNT > 1
            // MIDI IN Jack Descriptor (Embedded)
            6,// Descriptor length
            AUDIO_INTERFACE_DESCRIPTOR_TYPE,// Descriptor type (CS_INTERFACE)
            MIDICLASS_SUBTYPE_MIDI_IN_JACK,// MIDI_IN_JACK subtype
            MIDI_EMBEDDED_JACK,// EMBEDDED
            USBDESC_MIDI_IN_EMB_JACKID_BASE+1,// ID of this jack
            0x00,// unused
#endif

#if EXTERNAL_MIDI_IN_COUNT > 0
            // MIDI Adapter MIDI IN Jack Descriptor (External)
            6,// Descriptor length
            AUDIO_INTERFACE_DESCRIPTOR_TYPE,       // Descriptor type (CS_INTERFACE)
            MIDICLASS_SUBTYPE_MIDI_IN_JACK,             // MIDI_IN_JACK subtype
            MIDI_EXTERNAL_JACK,             // EXTERNAL
            USBDESC_MIDI_IN_EXT_JACKID_BASE,             // ID of this jack
            0x00,             // unused
#endif

#if EXTERNAL_MIDI_IN_COUNT > 1
            // MIDI Adapter MIDI IN Jack Descriptor (External)
            6,// Descriptor length
            AUDIO_INTERFACE_DESCRIPTOR_TYPE,// Descriptor type (CS_INTERFACE)
            MIDICLASS_SUBTYPE_MIDI_IN_JACK,// MIDI_IN_JACK subtype
            MIDI_EXTERNAL_JACK,// EXTERNAL
            USBDESC_MIDI_IN_EXT_JACKID_BASE+1,// ID of this jack
            0x00,// unused
#endif

#if EMBEDDED_MIDI_OUT_COUNT > 0
            // MIDI Adapter MIDI OUT Jack Descriptor (Embedded)
            9,// Descriptor length
            AUDIO_INTERFACE_DESCRIPTOR_TYPE,       // Descriptor type (CS_INTERFACE)
            MIDICLASS_SUBTYPE_MIDI_OUT_JACK,             // MIDI_OUT_JACK subtype
            MIDI_EMBEDDED_JACK,             // EMBEDDED
            USBDESC_MIDI_OUT_EMB_JACKID_BASE,             // ID of this jack
            0x01,             // number of input pins of this jack
            USBDESC_MIDI_ELEMENT_ID, // ID of the entity to which this pin is connected
            0x01, // Output Pin number of the entity to which this input pin is connected
            0x00,             // unused
#endif

#if EMBEDDED_MIDI_OUT_COUNT > 1
            // MIDI Adapter MIDI OUT Jack Descriptor (Embedded)
            9,// Descriptor length
            AUDIO_INTERFACE_DESCRIPTOR_TYPE,// Descriptor type (CS_INTERFACE)
            MIDICLASS_SUBTYPE_MIDI_OUT_JACK,// MIDI_OUT_JACK subtype
            MIDI_EMBEDDED_JACK,// EMBEDDED
            USBDESC_MIDI_OUT_EMB_JACKID_BASE + 1,// ID of this jack
            0x01,// number of input pins of this jack
            USBDESC_MIDI_ELEMENT_ID,// ID of the entity to which this pin is connected
            0x01,// Output Pin number of the entity to which this input pin is connected
            0x00,// unused
#endif

#if EXTERNAL_MIDI_OUT_COUNT > 0
            // MIDI Adapter MIDI OUT Jack Descriptor (External)
            9,// Descriptor length
            AUDIO_INTERFACE_DESCRIPTOR_TYPE,       // Descriptor type (CS_INTERFACE)
            MIDICLASS_SUBTYPE_MIDI_OUT_JACK,             // MIDI_OUT_JACK subtype
            MIDI_EXTERNAL_JACK,             // EXTERNAL
            USBDESC_MIDI_OUT_EXT_JACKID_BASE,             // ID of this jack
            0x01,             // number of input pins of this jack
            USBDESC_MIDI_ELEMENT_ID, // ID of the entity to which this pin is connected
            0x01, // Output Pin number of the entity to which this input pin is connected
            0x00,             // unused
#endif

#if EXTERNAL_MIDI_OUT_COUNT > 1
            // MIDI Adapter MIDI OUT Jack Descriptor (External)
            9,// Descriptor length
            AUDIO_INTERFACE_DESCRIPTOR_TYPE,// Descriptor type (CS_INTERFACE)
            MIDICLASS_SUBTYPE_MIDI_OUT_JACK,// MIDI_OUT_JACK subtype
            MIDI_EXTERNAL_JACK,// EXTERNAL
            USBDESC_MIDI_OUT_EXT_JACKID_BASE+1,// ID of this jack
            0x01,// number of input pins of this jack
            USBDESC_MIDI_ELEMENT_ID,// ID of the entity to which this pin is connected
            0x01,// Output Pin number of the entity to which this input pin is connected
            0x00,// unused
#endif

            // MIDI Adapter Element Descriptor
#if MIDI_ELEMENT_COUNT > 0
            USBDESC_MIDI_ELEMENT_SIZE,            //Descriptor length
            AUDIO_INTERFACE_DESCRIPTOR_TYPE,	// Descriptor type (CS_INTERFACE)
            MIDICLASS_SUBTYPE_ELEMENT,		    // ELEMENT subtype
            USBDESC_MIDI_ELEMENT_ID,				// bElementID
            EXTERNAL_MIDI_IN_COUNT + EMBEDDED_MIDI_IN_COUNT, 		// bNrInputPins

#if EMBEDDED_MIDI_IN_COUNT > 0
            USBDESC_MIDI_IN_EMB_JACKID_BASE,  //sourceID
            0x01,								//outputPin
#endif
#if EMBEDDED_MIDI_IN_COUNT > 1
            USBDESC_MIDI_IN_EMB_JACKID_BASE+1,  //sourceID
            0x01,//outputPin
#endif

#if EXTERNAL_MIDI_IN_COUNT > 0
            USBDESC_MIDI_IN_EXT_JACKID_BASE,  //sourceID
            0x01,								//outputPin
#endif
#if EXTERNAL_MIDI_IN_COUNT > 1
            USBDESC_MIDI_IN_EXT_JACKID_BASE+1,  //sourceID
            0x01,//outputPin
#endif

            EXTERNAL_MIDI_OUT_COUNT + EMBEDDED_MIDI_OUT_COUNT,		// bNrOutputPins
            0x00, //bInTerminalLink
            0x00, //bOutTerminalLink

            //Element Capabilities
            0x01,//bElCapsSize,
            MIDI_ELEMENT_CAPABILITY_CUSTOM | MIDI_ELEMENT_CAPABILITY_MIDI_CLOCK,
            0x02,  ///index to string descriptor of this element.
#endif // MIDI Adapter Element Descriptor

// B.5 Bulk OUT Endpoint Descriptors
//B.5.1 Standard Bulk OUT Endpoint Descriptor
            9, /* bLenght */
            USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType = endpoint */
            MIDI_OUT_EP, /* bEndpointAddress OUT endpoint number 1 */
            USB_OTG_EP_BULK, /* bmAttributes: 2:Bulk, 3:Interrupt endpoint */ //This should be bulk for max speed
            USB_MIDI_OUT_BUF_SIZE,
            0, /* wMaxPacketSize */
            0, /* bIntervall in ms */
            0, /* bRefresh */
            0, /* bSyncAddress */

// B.5.2 Class-specific MS Bulk OUT Endpoint Descriptor
            USBDESC_MIDI_CLASS_ENDPOINT_SIZE + EMBEDDED_MIDI_IN_COUNT, //bLength                                             <---------
            AUDIO_ENDPOINT_DESCRIPTOR_TYPE,       //bDescriptorType - CS_ENDPOINT
            0x01,       //bDescriptorSubtype - MS_GENERAL
            EMBEDDED_MIDI_IN_COUNT, //bNumEmbMIDIJack                                     <---------
#if EMBEDDED_MIDI_IN_COUNT > 0
            USBDESC_MIDI_IN_EMB_JACKID_BASE, //BaAssocJackID(1)                                    <---------
#endif
#if EMBEDDED_MIDI_IN_COUNT > 1
            USBDESC_MIDI_IN_EMB_JACKID_BASE+1,       //BaAssocJackID(2)
#endif

//B.6 Bulk IN Endpoint Descriptors

//B.6.1 Standard Bulk IN Endpoint Descriptor
            9, /* bLenght */
            USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType = endpoint */
            MIDI_IN_EP, /* bEndpointAddress IN endpoint number 1 */
            USB_OTG_EP_BULK, /* bmAttributes: 2: Bulk, 3: Interrupt endpoint */ // This should be bulk so that it is faster than the UART
            USB_MIDI_IN_BUF_SIZE,
            0, /* wMaxPacketSize */
            0, /* bIntervall in ms */
            0, /* bRefresh */
            0, /* bSyncAddress */

            USBDESC_MIDI_CLASS_ENDPOINT_SIZE + EMBEDDED_MIDI_OUT_COUNT, //bLength                                             <---------
            AUDIO_ENDPOINT_DESCRIPTOR_TYPE,       //bDescriptorType - CS_ENDPOINT
            0x01,       //bDescriptorSubtype - MS_GENERAL
            EMBEDDED_MIDI_OUT_COUNT, //bNumEmbMIDIJack                                     <---------

#if EMBEDDED_MIDI_OUT_COUNT > 0
            USBDESC_MIDI_OUT_EMB_JACKID_BASE,
      //BaAssocJackID(1)                                    <---------
#endif
#if EMBEDDED_MIDI_OUT_COUNT > 1
      USBDESC_MIDI_OUT_EMB_JACKID_BASE + 1,       //BaAssocJackID(2)
#endif

   };

/**
 * @}
 */

/**
 * @brief  Handle_USBAsynchXfer
 *         Send data to USB
 * @param  pdev: instance
 * @retval None
 */
static void Handle_USBAsynchXfer(void *pdev)
{
   uint16_t USB_Tx_ptr;
   uint32_t USB_Tx_length;

}

/** @defgroup usbd_audio_Private_Functions
 * @{
 */

/**
 * @brief  usbd_audio_Init
 *         Initilaizes the AUDIO interface.
 * @param  pdev: device instance
 * @param  cfgidx: Configuration index
 * @retval status
 */
static uint8_t usbd_audio_Init(void *pdev, uint8_t cfgidx)
{
   DCD_EP_Flush(pdev, MIDI_IN_EP);
   DCD_EP_Flush(pdev, MIDI_OUT_EP);

   /* Open EP IN */
   DCD_EP_Open(pdev,
   MIDI_IN_EP,
   USB_MIDI_IN_BUF_SIZE,
   USB_OTG_EP_BULK);

   /* Open EP OUT */
   DCD_EP_Open(pdev,
   MIDI_OUT_EP,
   USB_MIDI_OUT_BUF_SIZE,
   USB_OTG_EP_BULK);

   USBMIDI_Port_t* rxdev;
   USBMIDI_Port_t* txdev;

   if( pdev == &USB_OTG_dev )
   {
      txdev = &USBMIDI_Tx;
      rxdev = &USBMIDI_Rx;
   }
   else
   {
      txdev = &USBMIDI_Tx_HS;
      rxdev = &USBMIDI_Rx_HS;
   }


   USDB_audio_MIDI_Port_FlushBuffers(txdev);
   USDB_audio_MIDI_Port_FlushBuffers(rxdev);

   USBD_audio_SetState(rxdev, USBMIDI_READY);
   USBD_audio_SetState(txdev, USBMIDI_READY);

   /* Prepare Out endpoint to receive audio data */
   DCD_EP_PrepareRx(pdev,
   MIDI_OUT_EP, (uint8_t*) rxdev->buf,
   USB_MIDI_OUT_BUF_SIZE);

   return USBD_OK;
}

/**
 * @brief  usbd_audio_DeInit
 *         DeInitializes the AUDIO layer.
 * @param  pdev: device instance
 * @param  cfgidx: Configuration index
 * @retval status
 */
static uint8_t usbd_audio_DeInit(void *pdev, uint8_t cfgidx)
{
   DCD_EP_Close(pdev, MIDI_IN_EP);
   DCD_EP_Close(pdev, MIDI_OUT_EP);

   /* DeInitialize the Audio output Hardware layer */
   if (AUDIO_OUT_fops.DeInit(0) != USBD_OK)
   {
      return USBD_FAIL;
   }

   return USBD_OK;
}

/**
 * @brief  usbd_audio_Setup
 *         Handles the Audio control request parsing.
 * @param  pdev: instance
 * @param  req: usb requests
 * @retval status
 */
static uint8_t usbd_audio_Setup(void *pdev, USB_SETUP_REQ *req)
{
   uint16_t len = USB_AUDIO_DESC_SIZ;
   uint8_t *pbuf = (uint8_t*) (usbd_audio_CfgDesc) + 18;

   switch (req->bmRequest & USB_REQ_TYPE_MASK)
   {
      /* AUDIO Class Requests -------------------------------*/
      case USB_REQ_TYPE_CLASS:
         switch (req->bRequest)
         {
            case AUDIO_REQ_GET_CUR:
               AUDIO_Req_GetCurrent(pdev, req);
               break;

            case AUDIO_REQ_SET_CUR:
               AUDIO_Req_SetCurrent(pdev, req);
               break;

            default:
               USBD_CtlError(pdev, req);
               return USBD_FAIL;
         }
         break;

         /* Standard Requests -------------------------------*/
      case USB_REQ_TYPE_STANDARD:
         switch (req->bRequest)
         {
            case USB_REQ_GET_DESCRIPTOR:
               if ((req->wValue >> 8) == AUDIO_DESCRIPTOR_TYPE)
               {
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
                  pbuf = usbd_audio_Desc;
#else
                  pbuf = (uint8_t*) (usbd_audio_CfgDesc) + 18;
#endif 
                  len = MIN(USB_AUDIO_DESC_SIZ, req->wLength);
               }

               USBD_CtlSendData(pdev, pbuf, len);
               break;

            case USB_REQ_GET_INTERFACE:
               USBD_CtlSendData(pdev, (uint8_t *) &usbd_audio_AltSet, 1);
               break;

            case USB_REQ_SET_INTERFACE:
               if ((uint8_t) (req->wValue) < 2)
               {
                  usbd_audio_AltSet = (uint8_t) (req->wValue);
               }
               else
               {
                  /* Call the error management function (command will be nacked */
                  USBD_CtlError(pdev, req);
               }
               break;
         }
   }
   return USBD_OK;
}

/**
 * @brief  usbd_audio_EP0_RxReady
 *         Handles audio control requests data.
 * @param  pdev: device device instance
 * @retval status
 */
static uint8_t usbd_audio_EP0_RxReady(void *pdev)
{
   /* Check if an AudioControl request has been issued */
   if (AudioCtlCmd == AUDIO_REQ_SET_CUR)
   {/* In this driver, to simplify code, only SET_CUR request is managed */
      /* Check for which addressed unit the AudioControl request has been issued */
      if (AudioCtlUnit == AUDIO_FEATURE_UNIT_ID)
      {/* In this driver, to simplify code, only one unit is manage */
         /* Call the audio interface mute function */
         AUDIO_OUT_fops.MuteCtl(AudioCtl[0]);

         /* Reset the AudioCtlCmd variable to prevent re-entering this function */
         AudioCtlCmd = 0;
         AudioCtlLen = 0;
      }
   }

   return USBD_OK;
}

/**
 * @brief  usbd_audio_DataIn
 *         Handles the audio IN data stage.
 * @param  pdev: instance
 * @param  epnum: endpoint number
 * @retval status
 */
static uint8_t usbd_audio_DataIn(void *pdev, uint8_t epnum)
{

   USBMIDI_Port_t* dev;

   if( pdev == &USB_OTG_dev )
   {
      dev = &USBMIDI_Tx;
   }
   else
   {
      dev = &USBMIDI_Tx_HS;
   }

   switch (epnum)
   {
      case MIDI_IN_EP & 0x1F:
         {
         USBD_audio_MIDI_Port_SetReadPtr(dev, USBD_audio_MIDI_Port_GetReadPtr(dev) + ((USB_OTG_CORE_HANDLE*)pdev)->dev.in_ep[epnum].xfer_count);
         if (USBD_audio_MIDI_Port_byteCount(dev) == 0)
         {
            USBD_audio_MIDI_Port_SetReadPtr(dev, 0);
            USBD_audio_SetState(dev, USBMIDI_READY);
         }
      }
         break;

      default:
         break;
   }

   return USBD_OK;
}

/**
 * @brief  usbd_audio_DataOut
 *         Handles the Audio Out data stage.
 * @param  pdev: instance
 * @param  epnum: endpoint number
 * @retval staztus
 */
static uint8_t usbd_audio_DataOut(void *pdev, uint8_t epnum)
{
   uint16_t USB_Rx_Cnt;

   /* Get the received data buffer and update the counter */
   USB_Rx_Cnt = ((USB_OTG_CORE_HANDLE*) pdev)->dev.out_ep[epnum].xfer_count;

   USBMIDI_Port_t* dev;

   if( pdev == &USB_OTG_dev )
   {
      dev = &USBMIDI_Rx;
   }
   else
   {
      dev = &USBMIDI_Rx_HS;
   }


   switch (epnum)
   {
      case MIDI_OUT_EP:
         USBD_audio_MIDI_Port_SetWritePtr(dev, USBD_audio_MIDI_Port_GetWritePtr(dev) + USB_Rx_Cnt);
         //USBD_audio_SetState(&USBMIDI_Rx, USBMIDI_TRANSMITING);
         /* Prepare Out endpoint to receive next audio packet */
         /*DCD_EP_PrepareRx(pdev,
          epnum,
          (uint8_t*)(USBMIDI_OutBuff),
          USB_MIDI_OUT_BUF_SIZE);*/
         break;
   }

   return USBD_OK;
}

/**
 * @brief  usbd_audio_SOF
 *         Handles the SOF event (data buffer update and synchronization).
 * @param  pdev: instance
 * @param  epnum: endpoint number
 * @retval status
 */
static uint8_t usbd_audio_SOF(void *pdev)
{
   /* Check if there are available data in stream buffer.
    In this function, a single variable (PlayFlag) is used to avoid software delays.
    The play operation must be executed as soon as possible after the SOF detection. */

   //Handle_USBAsynchXfer(pdev);
   return USBD_OK;
}

/**
 * @brief  usbd_audio_OUT_Incplt
 *         Handles the iso out incomplete event.
 * @param  pdev: instance
 * @retval status
 */
static uint8_t usbd_audio_OUT_Incplt(void *pdev)
{
   return USBD_OK;
}

/******************************************************************************
 AUDIO Class requests management
 ******************************************************************************/
/**
 * @brief  AUDIO_Req_GetCurrent
 *         Handles the GET_CUR Audio control request.
 * @param  pdev: instance
 * @param  req: setup class request
 * @retval status
 */
static void AUDIO_Req_GetCurrent(void *pdev, USB_SETUP_REQ *req)
{
   /* Send the current mute state */
   USBD_CtlSendData(pdev, AudioCtl, req->wLength);
}

/**
 * @brief  AUDIO_Req_SetCurrent
 *         Handles the SET_CUR Audio control request.
 * @param  pdev: instance
 * @param  req: setup class request
 * @retval status
 */
static void AUDIO_Req_SetCurrent(void *pdev, USB_SETUP_REQ *req)
{
   if (req->wLength)
   {
      /* Prepare the reception of the buffer over EP0 */
      USBD_CtlPrepareRx(pdev, AudioCtl, req->wLength);

      /* Set the global variables indicating current request and its length
       to the function usbd_audio_EP0_RxReady() which will process the request */
      AudioCtlCmd = AUDIO_REQ_SET_CUR; /* Set the request value */
      AudioCtlLen = req->wLength; /* Set the request data length */
      AudioCtlUnit = HIBYTE(req->wIndex); /* Set the request target unit */
   }
}

/**
 * @brief  USBD_audio_GetCfgDesc
 *         Returns configuration descriptor.
 * @param  speed : current device speed
 * @param  length : pointer data length
 * @retval pointer to descriptor buffer
 */
static uint8_t *USBD_audio_GetCfgDesc(uint8_t speed, uint16_t *length)
{
   *length = sizeof(usbd_audio_CfgDesc);
   return (uint8_t*) (usbd_audio_CfgDesc);
}

uint32_t USBD_audio_TxEPStatus(uint8_t epnum)
{
   uint32_t status = DCD_GetEPStatus(&USB_OTG_dev, epnum);
   return status;
}

void USBD_audio_Disconnect(void)
{
   DCD_DevDisconnect(&USB_OTG_dev);
}

/**
 * @}
 */

uint8_t USBD_audio_RxEPReady(USBMIDI_Port_t* port)
{

   uint32_t status = DCD_GetEPStatus(port->pdev, port->epnum);

   if ((status != USB_OTG_EP_RX_VALID))
   {

      switch (port->epnum)
      {
         case MIDI_OUT_EP:

            USBD_audio_MIDI_Port_SetWritePtr(port, 0);
            USBD_audio_MIDI_Port_SetReadPtr(port, 0);

            /* Prepare Out endpoint to receive next audio packet */
            DCD_EP_PrepareRx(port->pdev,
                  port->epnum,
                  (uint8_t*) (port->buf),
                  USB_MIDI_OUT_BUF_SIZE);

            return 0;
            break;
      }

   }
   return 1;

}

//returns the number of bytes of USB MIDI received
uint32_t USB_audio_MIDI_DataReceived(uint8_t epnum)
{
   return USB_OTG_dev.dev.out_ep[epnum].xfer_count;
}

//ep_addr is 0x80 to 0x85
//Returns the number of 32bit words available.
uint16_t USBD_audio_TxFIFO_WordsAvailable(uint8_t ep_addr)
{
   uint8_t inEpAddr = ep_addr & 0x0F;
   return USB_OTG_dev.regs.INEP_REGS[inEpAddr]->DTXFSTS;
}

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
