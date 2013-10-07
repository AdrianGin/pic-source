/**
  ******************************************************************************
  * @file    usb_desc.c
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    29-June-2012
  * @brief   Descriptors for Audio Speaker Demo
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
#include "usb_lib.h"
#include "usb_desc.h"

#include "hw_config.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants----------------------------------------------------------*/
/* USB Standard Device Descriptor */

const uint8_t Speaker_DeviceDescriptor[] =
  {
    SPEAKER_SIZ_DEVICE_DESC,          /* bLength */
    USB_DEVICE_DESCRIPTOR_TYPE,           /* bDescriptorType */
    0x00,          /* 2.00 */             /* bcdUSB */
    0x02,
    0x00,                                 /* bDeviceClass */
    0x00,                                 /* bDeviceSubClass */
    0x00,                                 /* bDeviceProtocol */
    END0_PACKET_SIZE,                                 /* bMaxPacketSize 40 */
    0x83,                                 /* idVendor */
    0x04,
    0x30,                                 /* idProduct  = 0x5730*/
    0x57,
    0x00,          /* 2.00 */             /* bcdDevice */
    0x02,
    1,                                    /* iManufacturer */
    2,                                    /* iProduct */
    3,                                    /* iSerialNumber */
    0x01                                  /* bNumConfigurations */
  };


/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
const uint8_t Speaker_ConfigDescriptor[] =
  {
    /* Configuration 1 */
    0x09,                                 /* bLength */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,    /* bDescriptorType */
    (COMPOSITE_SIZE_CONFIG_DESC&0xFF), (COMPOSITE_SIZE_CONFIG_DESC>>8),			/* total length of data returned (including inlined descriptors) */
    //0x02,                                 /* bNumInterfaces */
    0x04,                                 /* bNumInterfaces */
    0x01,                                 /* bConfigurationValue */
    0x00,                                 /* iConfiguration */
    0xC0,                                 /* bmAttributes  BUS Powred*/
    0x32,                                 /* bMaxPower = 100 mA*/
    /* 09 byte*/



    /* USB Speaker Standard interface descriptor */
    SPEAKER_SIZ_INTERFACE_DESC_SIZE,      /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
    0x00,                                 /* bInterfaceNumber */
    0x00,                                 /* bAlternateSetting */
    0x00,                                 /* bNumEndpoints */
    USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass */
    AUDIO_SUBCLASS_AUDIOCONTROL,          /* bInterfaceSubClass */
    AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol */
    0x04,                                 /* iInterface */
    /* 09 byte*/

    /* USB Speaker Class-specific AC Interface Descriptor */
    SPEAKER_SIZ_INTERFACE_DESC_SIZE,   /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    AUDIO_CONTROL_HEADER,                 /* bDescriptorSubtype */
    0x00,          /* 1.00 */             /* bcdADC */
    0x01,
    0x27,                                 /* wTotalLength = 39*/
    0x00,
    0x01,                                 /* bInCollection */
    0x01,                                 /* baInterfaceNr */
    /* 09 byte*/

    /* USB Speaker Input Terminal Descriptor */
    AUDIO_INPUT_TERMINAL_DESC_SIZE,       /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    AUDIO_CONTROL_INPUT_TERMINAL,         /* bDescriptorSubtype */
    0x01,                                 /* bTerminalID */
    0x01,                                 /* wTerminalType AUDIO_TERMINAL_USB_STREAMING   0x0101 */
    0x01,
    0x00,                                 /* bAssocTerminal */
    0x01,                                 /* bNrChannels */
    0x00,                                 /* wChannelConfig 0x0000  Mono */
    0x00,
    0x00,                                 /* iChannelNames */
    0x00,                                 /* iTerminal */
    /* 12 byte*/

    /* USB Speaker Audio Feature Unit Descriptor */
    0x09,                                 /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    AUDIO_CONTROL_FEATURE_UNIT,           /* bDescriptorSubtype */
    0x02,                                 /* bUnitID */
    0x01,                                 /* bSourceID */
    0x01,                                 /* bControlSize */
    AUDIO_CONTROL_MUTE,                   /* bmaControls(0) */
    0x00,                                 /* bmaControls(1) */
    0x00,                                 /* iTerminal */
    /* 09 byte*/

    /*USB Speaker Output Terminal Descriptor */
    0x09,      /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    AUDIO_CONTROL_OUTPUT_TERMINAL,        /* bDescriptorSubtype */
    0x03,                                 /* bTerminalID */
    //0x03,                                 /* wTerminalType  0x0301*/
    //0x03,
    USB_DESC_BYTE(AUDIO_TERMINAL_TYPE, 0),
    USB_DESC_BYTE(AUDIO_TERMINAL_TYPE, 1),
    0x00,                                 /* bAssocTerminal */
    0x02,                                 /* bSourceID */
    0x00,                                 /* iTerminal */
    /* 09 byte*/

    /* USB Speaker Standard AS Interface Descriptor - Audio Streaming Zero Bandwith */
    /* Interface 1, Alternate Setting 0                                             */
    SPEAKER_SIZ_INTERFACE_DESC_SIZE,  /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
    0x01,                                 /* bInterfaceNumber */
    0x00,                                 /* bAlternateSetting */
    0x00,                                 /* bNumEndpoints */
    USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass */
    AUDIO_SUBCLASS_AUDIOSTREAMING,        /* bInterfaceSubClass */
    AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol */
    0x04,                                 /* iInterface */
    /* 09 byte*/

    /* USB Speaker Standard AS Interface Descriptor - Audio Streaming Operational */
    /* Interface 1, Alternate Setting 1                                           */
    SPEAKER_SIZ_INTERFACE_DESC_SIZE,  /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,        /* bDescriptorType */
    0x01,                                 /* bInterfaceNumber */
    0x01,                                 /* bAlternateSetting */
    0x01,                                 /* bNumEndpoints */
    USB_DEVICE_CLASS_AUDIO,               /* bInterfaceClass */
    AUDIO_SUBCLASS_AUDIOSTREAMING,        /* bInterfaceSubClass */
    AUDIO_PROTOCOL_UNDEFINED,             /* bInterfaceProtocol */
    0x04,                                 /* iInterface */
    /* 09 byte*/

    /* USB Speaker Audio Streaming Interface Descriptor */
    AUDIO_STREAMING_INTERFACE_DESC_SIZE,  /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    AUDIO_STREAMING_GENERAL,              /* bDescriptorSubtype */
    0x01,                                 /* bTerminalLink */
    0x01,                                 /* bDelay */
    0x01,                                 /* wFormatTag AUDIO_FORMAT_PCM8  0x0002*/
    //0x02,
    0x00,
    /* 07 byte*/

    /* USB Speaker Audio Type I Format Interface Descriptor */
    0x0B,                                 /* bLength */
    AUDIO_INTERFACE_DESCRIPTOR_TYPE,      /* bDescriptorType */
    AUDIO_STREAMING_FORMAT_TYPE,          /* bDescriptorSubtype */
    AUDIO_FORMAT_TYPE_I,                  /* bFormatType */
    MAX_AUDIO_CHANNELS,                                 /* bNrChannels */
    MAX_AUDIO_BIT_RESOLUTION/8,                                 /* bSubFrameSize */
    MAX_AUDIO_BIT_RESOLUTION,                                    /* bBitResolution */
    0x01,                                 /* bSamFreqType */
    //0xF0,                                 /* tSamFreq 22000 = 0x55F0 */
    //0x55,
    USB_AUDIO_FREQ_BYTE(0),
	USB_AUDIO_FREQ_BYTE(1),
	USB_AUDIO_FREQ_BYTE(2),

    /* 11 byte*/

    /* Endpoint 1 - Standard Descriptor */
    AUDIO_STANDARD_ENDPOINT_DESC_SIZE,    /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,         /* bDescriptorType */
    0x01,                                 /* bEndpointAddress 1 out endpoint*/
    USB_ENDPOINT_TYPE_ISOCHRONOUS,        /* bmAttributes */

    //44*2,                                 /* wMaxPacketSize 22 bytes*/
		//0x16
    USB_DESC_BYTE(USB_ENDP_MAX_PACKET_SIZE, 0),
    USB_DESC_BYTE(USB_ENDP_MAX_PACKET_SIZE, 1),
    0x01,                                 /* bInterval */
    0x00,                                 /* bRefresh */
    0x00,                                 /* bSynchAddress */
    /* 09 byte*/

    /* Endpoint - Audio Streaming Descriptor*/
    AUDIO_STREAMING_ENDPOINT_DESC_SIZE,   /* bLength */
    AUDIO_ENDPOINT_DESCRIPTOR_TYPE,       /* bDescriptorType */
    AUDIO_ENDPOINT_GENERAL,               /* bDescriptor */
    0x00,                                 /* bmAttributes */
    0x00,                                 /* bLockDelayUnits */
    0x00,                                 /* wLockDelay */
    0x00,
    /* 07 byte*/



    // B.3 AudioControl Interface Descriptors
    // The AudioControl interface describes the device structure (audio function topology)
    // and is used to manipulate the Audio Controls. This device has no audio function
    // incorporated. However, the AudioControl interface is mandatory and therefore both
    // the standard AC interface descriptor and the classspecific AC interface descriptor
    // must be present. The class-specific AC interface descriptor only contains the header
    // descriptor.

    // B.3.1 Standard AC Interface Descriptor
    // The AudioControl interface has no dedicated endpoints associated with it. It uses the
    // default pipe (endpoint 0) for all communication purposes. Class-specific AudioControl
    // Requests are sent using the default pipe. There is no Status Interrupt endpoint provided.
    	/* AC interface descriptor follows inline: */
    	USBDESC_INTERFACE_SIZE,			/* sizeof(usbDescrInterface): length of descriptor in bytes */
    	USB_INTERFACE_DESCRIPTOR_TYPE,	/* descriptor type */
    	0x02,			/* index of this interface */
    	0,			/* alternate setting for this interface */
    	0,			/* endpoints excl 0: number of endpoint descriptors to follow */
    	1,			/* Audio, */
    	1,			/* Audio Control */
    	0,			/* */
    	2,			/* string index for interface */

    // B.3.2 Class-specific AC Interface Descriptor
    // The Class-specific AC interface descriptor is always headed by a Header descriptor
    // that contains general information about the AudioControl interface. It contains all
    // the pointers needed to describe the Audio Interface Collection, associated with the
    // described audio function. Only the Header descriptor is present in this device
    // because it does not contain any audio functionality as such.
    	/* AC Class-Specific descriptor */
    	USBDESC_INTERFACE_SIZE,			/* sizeof(usbDescrCDC_HeaderFn): length of descriptor in bytes */
    	36,			/* descriptor type */
    	1,			/* header functional descriptor */
    	0x0, 0x01,		/* bcdADC */
    	9, 0,			/* wTotalLength */
    	1,			/* inCollection */
    	3,			/* baInterfaceNr */

    // B.4 MIDIStreaming Interface Descriptors

    // B.4.1 Standard MS Interface Descriptor
    	/* interface descriptor follows inline: */
    	USBDESC_MIDI_INTERFACE_SIZE,			/* length of descriptor in bytes */
    	USB_INTERFACE_DESCRIPTOR_TYPE,	/* descriptor type */
    	0x03,			/* index of this interface */
    	0,			/* alternate setting for this interface */
    	2,			/* endpoints excl 0: number of endpoint descriptors to follow */
    	1,			/* AUDIO */
    	3,			/* MIDI STREAMING */
    	0,			/* unused */
    	2,			/* string index for interface */

    // B.4.2 Class-specific MS Interface Descriptor
    	/* MS Class-Specific descriptor */
    	USBDESC_MIDI_STREAMING_SIZE,			/* length of descriptor in bytes */
    	AUDIO_INTERFACE_DESCRIPTOR_TYPE,			/* descriptor type */
    	1,			/* header functional descriptor */
    	0x0, 0x01,		/* bcdADC */
    	((USBDESC_CONFIG_SIZE-(USBDESC_MIDI_INTERFACE_SIZE*4))&0xFF), ((USBDESC_CONFIG_SIZE-(USBDESC_MIDI_INTERFACE_SIZE*4))>>8),			/* wTotalLength */


    #if USB_MIDI_CABLE_COUNT > 0
      // MIDI IN Jack Descriptor (Embedded)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_IN_JACKID_BASE,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI IN Jack Descriptor (External)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_IN_JACKID_BASE+1,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (Embedded)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_OUT_JACKID_BASE,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+1,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (External)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_OUT_JACKID_BASE+1,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused
    #endif

    #if USB_MIDI_CABLE_COUNT > 1

      // MIDI IN Jack Descriptor (Embedded)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_IN_JACKID_BASE+2,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI IN Jack Descriptor (External)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_IN_JACKID_BASE+3,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (Embedded)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_OUT_JACKID_BASE+2,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+3,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (External)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_OUT_JACKID_BASE+3,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+2,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused
    #endif

    #if USB_MIDI_CABLE_COUNT > 2
      // MIDI IN Jack Descriptor (Embedded)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_IN_JACKID_BASE+4,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI IN Jack Descriptor (External)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_IN_JACKID_BASE+5,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (Embedded)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_OUT_JACKID_BASE+4,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+5,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (External)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_OUT_JACKID_BASE+5,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+4,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused
    #endif


    #if USB_MIDI_CABLE_COUNT > 3
      // MIDI IN Jack Descriptor (Embedded)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_IN_JACKID_BASE+6,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI IN Jack Descriptor (External)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_IN_JACKID_BASE+7,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (Embedded)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_OUT_JACKID_BASE+6,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+7,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (External)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_OUT_JACKID_BASE+7,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+6,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused
    #endif

    #if USB_MIDI_CABLE_COUNT > 4
      // MIDI IN Jack Descriptor (Embedded)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_IN_JACKID_BASE+8,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI IN Jack Descriptor (External)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_IN_JACKID_BASE+9,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (Embedded)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_OUT_JACKID_BASE+8,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+9,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (External)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_OUT_JACKID_BASE+9,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+8,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused
    #endif
    #if USB_MIDI_CABLE_COUNT > 5
      // MIDI IN Jack Descriptor (Embedded)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_IN_JACKID_BASE+10,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI IN Jack Descriptor (External)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_IN_JACKID_BASE+11,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (Embedded)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_OUT_JACKID_BASE+10,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+11,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (External)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_OUT_JACKID_BASE+11,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+10,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused
    #endif
    #if USB_MIDI_CABLE_COUNT > 6
      // MIDI IN Jack Descriptor (Embedded)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_IN_JACKID_BASE+12,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI IN Jack Descriptor (External)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_IN_JACKID_BASE+13,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (Embedded)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_OUT_JACKID_BASE+12,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+13,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (External)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_OUT_JACKID_BASE+13,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+12,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused
    #endif
    #if USB_MIDI_CABLE_COUNT > 7
      // MIDI IN Jack Descriptor (Embedded)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_IN_JACKID_BASE+14,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI IN Jack Descriptor (External)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_IN_JACKID_BASE+15,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (Embedded)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_OUT_JACKID_BASE+14,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+15,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (External)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_OUT_JACKID_BASE+15,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+14,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused
    #endif
    #if USB_MIDI_CABLE_COUNT > 8
      // MIDI IN Jack Descriptor (Embedded)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_IN_JACKID_BASE+16,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI IN Jack Descriptor (External)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_IN_JACKID_BASE+17,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (Embedded)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_OUT_JACKID_BASE+16,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+17,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (External)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_OUT_JACKID_BASE+17,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+16,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused
    #endif

    #if USB_MIDI_CABLE_COUNT > 9
      // MIDI IN Jack Descriptor (Embedded)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_IN_JACKID_BASE+18,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI IN Jack Descriptor (External)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_IN_JACKID_BASE+19,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (Embedded)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_OUT_JACKID_BASE+18,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+19,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (External)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_OUT_JACKID_BASE+19,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+18,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused
    #endif
    #if USB_MIDI_CABLE_COUNT > 10
      // MIDI IN Jack Descriptor (Embedded)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_IN_JACKID_BASE+20,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI IN Jack Descriptor (External)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_IN_JACKID_BASE+21,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (Embedded)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_OUT_JACKID_BASE+20,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+21,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (External)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_OUT_JACKID_BASE+21,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+20,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused
    #endif
    #if USB_MIDI_CABLE_COUNT > 11
      // MIDI IN Jack Descriptor (Embedded)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_IN_JACKID_BASE+22,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI IN Jack Descriptor (External)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_IN_JACKID_BASE+23,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (Embedded)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_OUT_JACKID_BASE+22,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+23,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (External)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_OUT_JACKID_BASE+23,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+22,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused
    #endif
    #if USB_MIDI_CABLE_COUNT > 12
      // MIDI IN Jack Descriptor (Embedded)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_IN_JACKID_BASE+24,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI IN Jack Descriptor (External)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_IN_JACKID_BASE+25,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (Embedded)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_OUT_JACKID_BASE+24,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+25,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (External)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_OUT_JACKID_BASE+25,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+24,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused
    #endif
    #if USB_MIDI_CABLE_COUNT > 13
      // MIDI IN Jack Descriptor (Embedded)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_IN_JACKID_BASE+26,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI IN Jack Descriptor (External)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_IN_JACKID_BASE+27,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (Embedded)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_OUT_JACKID_BASE+26,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+27,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (External)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_OUT_JACKID_BASE+27,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+26,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused
    #endif
    #if USB_MIDI_CABLE_COUNT > 14
      // MIDI IN Jack Descriptor (Embedded)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_IN_JACKID_BASE+28,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI IN Jack Descriptor (External)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_IN_JACKID_BASE+29,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (Embedded)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_OUT_JACKID_BASE+28,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+29,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (External)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_OUT_JACKID_BASE+29,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+28,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused
    #endif
    #if USB_MIDI_CABLE_COUNT > 15
      // MIDI IN Jack Descriptor (Embedded)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_IN_JACKID_BASE+30,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI IN Jack Descriptor (External)
      6,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x02,             // MIDI_IN_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_IN_JACKID_BASE+21,             // ID of this jack
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (Embedded)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EMBEDDED_JACK,             // EMBEDDED
      USBDESC_MIDI_OUT_JACKID_BASE+30,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+31,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused

      // MIDI Adapter MIDI OUT Jack Descriptor (External)
      9,                // Descriptor length
      AUDIO_INTERFACE_DESCRIPTOR_TYPE,         // Descriptor type (CS_INTERFACE)
      0x03,             // MIDI_OUT_JACK subtype
      MIDI_EXTERNAL_JACK,             // EXTERNAL
      USBDESC_MIDI_OUT_JACKID_BASE+31,             // ID of this jack
      0x01,             // number of input pins of this jack
      USBDESC_MIDI_IN_JACKID_BASE+30,             // ID of the entity to which this pin is connected
      0x01,             // Output Pin number of the entity to which this input pin is connected
      0x00,             // unused
    #endif
    // B.5 Bulk OUT Endpoint Descriptors

    //B.5.1 Standard Bulk OUT Endpoint Descriptor
    	9,			/* bLenght */
    	USB_ENDPOINT_DESCRIPTOR_TYPE,	/* bDescriptorType = endpoint */
    	0x2,			/* bEndpointAddress OUT endpoint number 2 */
    	2,			/* bmAttributes: 2:Bulk, 3:Interrupt endpoint */ //This should be bulk for max speed
    	64, 0,			/* wMaxPacketSize */
    	0,			/* bIntervall in ms */
    	0,			/* bRefresh */
    	0,			/* bSyncAddress */

    // B.5.2 Class-specific MS Bulk OUT Endpoint Descriptor
     USBDESC_MIDI_CLASS_ENDPOINT_SIZE+USB_MIDI_CABLE_COUNT,       //bLength                                             <---------
     AUDIO_ENDPOINT_DESCRIPTOR_TYPE,       //bDescriptorType - CS_ENDPOINT
     0x01,       //bDescriptorSubtype - MS_GENERAL
     USB_MIDI_CABLE_COUNT,       //bNumEmbMIDIJack                                     <---------
    #if USB_MIDI_CABLE_COUNT > 0
     USBDESC_MIDI_IN_JACKID_BASE,       //BaAssocJackID(1)                                    <---------
    #endif
    #if USB_MIDI_CABLE_COUNT > 1
     USBDESC_MIDI_IN_JACKID_BASE+2,       //BaAssocJackID(2)
    #endif
    #if USB_MIDI_CABLE_COUNT > 2
     USBDESC_MIDI_IN_JACKID_BASE+4,       //BaAssocJackID(2)
    #endif
    #if USB_MIDI_CABLE_COUNT > 3
     USBDESC_MIDI_IN_JACKID_BASE+6,       //BaAssocJackID(2)
    #endif
    #if USB_MIDI_CABLE_COUNT > 4
     USBDESC_MIDI_IN_JACKID_BASE+8,       //BaAssocJackID(2)
    #endif
    #if USB_MIDI_CABLE_COUNT > 5
     USBDESC_MIDI_IN_JACKID_BASE+10,       //BaAssocJackID(2)
    #endif
    #if USB_MIDI_CABLE_COUNT > 6
     USBDESC_MIDI_IN_JACKID_BASE+12,       //BaAssocJackID(2)
    #endif
    #if USB_MIDI_CABLE_COUNT > 7
     USBDESC_MIDI_IN_JACKID_BASE+14,       //BaAssocJackID(2)
    #endif
    #if USB_MIDI_CABLE_COUNT > 8
     USBDESC_MIDI_IN_JACKID_BASE+16,       //BaAssocJackID(2)
    #endif
    #if USB_MIDI_CABLE_COUNT > 9
     USBDESC_MIDI_IN_JACKID_BASE+18,       //BaAssocJackID(2)
    #endif
    #if USB_MIDI_CABLE_COUNT > 10
     USBDESC_MIDI_IN_JACKID_BASE+20,       //BaAssocJackID(2)
    #endif
    #if USB_MIDI_CABLE_COUNT > 11
     USBDESC_MIDI_IN_JACKID_BASE+22,       //BaAssocJackID(2)
    #endif
    #if USB_MIDI_CABLE_COUNT > 12
     USBDESC_MIDI_IN_JACKID_BASE+24,       //BaAssocJackID(2)
    #endif
    #if USB_MIDI_CABLE_COUNT > 13
     USBDESC_MIDI_IN_JACKID_BASE+26,       //BaAssocJackID(2)
    #endif
    #if USB_MIDI_CABLE_COUNT > 14
     USBDESC_MIDI_IN_JACKID_BASE+28,       //BaAssocJackID(2)
    #endif
    #if USB_MIDI_CABLE_COUNT > 15
     USBDESC_MIDI_IN_JACKID_BASE+30,       //BaAssocJackID(2)
    #endif

    //B.6 Bulk IN Endpoint Descriptors

    //B.6.1 Standard Bulk IN Endpoint Descriptor
    	9,			/* bLenght */
    	USB_ENDPOINT_DESCRIPTOR_TYPE,	/* bDescriptorType = endpoint */
    	0x82,			/* bEndpointAddress IN endpoint number 2 */
    	2,			/* bmAttributes: 2: Bulk, 3: Interrupt endpoint */ // This should be bulk so that it is faster than the UART
    	64, 0,			/* wMaxPacketSize */
    	0,			/* bIntervall in ms */
    	0,			/* bRefresh */
    	0,			/* bSyncAddress */

     USBDESC_MIDI_CLASS_ENDPOINT_SIZE+USB_MIDI_CABLE_COUNT,       //bLength                                             <---------
     AUDIO_ENDPOINT_DESCRIPTOR_TYPE,       //bDescriptorType - CS_ENDPOINT
     0x01,       //bDescriptorSubtype - MS_GENERAL
     USB_MIDI_CABLE_COUNT,       //bNumEmbMIDIJack                                     <---------

    #if USB_MIDI_CABLE_COUNT > 0
     USBDESC_MIDI_OUT_JACKID_BASE,       //BaAssocJackID(1)                                    <---------
    #endif
    #if USB_MIDI_CABLE_COUNT > 1
     USBDESC_MIDI_OUT_JACKID_BASE+2,       //BaAssocJackID(2)
    #endif
    #if USB_MIDI_CABLE_COUNT > 2
     USBDESC_MIDI_OUT_JACKID_BASE+4,       //BaAssocJackID(2)
    #endif
    #if USB_MIDI_CABLE_COUNT > 3
     USBDESC_MIDI_OUT_JACKID_BASE+6,       //BaAssocJackID(2)
    #endif
    #if USB_MIDI_CABLE_COUNT > 4
     USBDESC_MIDI_OUT_JACKID_BASE+8,       //BaAssocJackID(2)
    #endif
    #if USB_MIDI_CABLE_COUNT > 5
     USBDESC_MIDI_OUT_JACKID_BASE+10,       //BaAssocJackID(2)
    #endif
    #if USB_MIDI_CABLE_COUNT > 6
     USBDESC_MIDI_OUT_JACKID_BASE+12,       //BaAssocJackID(2)
    #endif
    #if USB_MIDI_CABLE_COUNT > 7
     USBDESC_MIDI_OUT_JACKID_BASE+14,       //BaAssocJackID(2)
    #endif
    #if USB_MIDI_CABLE_COUNT > 8
     USBDESC_MIDI_OUT_JACKID_BASE+16,       //BaAssocJackID(2)
    #endif
    #if USB_MIDI_CABLE_COUNT > 9
     USBDESC_MIDI_OUT_JACKID_BASE+18,       //BaAssocJackID(2)
    #endif
    #if USB_MIDI_CABLE_COUNT > 10
     USBDESC_MIDI_OUT_JACKID_BASE+20,       //BaAssocJackID(2)
    #endif
    #if USB_MIDI_CABLE_COUNT > 11
     USBDESC_MIDI_OUT_JACKID_BASE+22,       //BaAssocJackID(2)
    #endif
    #if USB_MIDI_CABLE_COUNT > 12
     USBDESC_MIDI_OUT_JACKID_BASE+24,       //BaAssocJackID(2)
    #endif
    #if USB_MIDI_CABLE_COUNT > 13
     USBDESC_MIDI_OUT_JACKID_BASE+26,       //BaAssocJackID(2)
    #endif
    #if USB_MIDI_CABLE_COUNT > 14
     USBDESC_MIDI_OUT_JACKID_BASE+28,       //BaAssocJackID(2)
    #endif
    #if USB_MIDI_CABLE_COUNT > 15
     USBDESC_MIDI_OUT_JACKID_BASE+30,       //BaAssocJackID(2)
    #endif



  };


/* USB String Descriptor (optional) */
const uint8_t Speaker_StringLangID[SPEAKER_SIZ_STRING_LANGID] =
  {
    0x04,
    0x03,
    0x09,
    0x04
  }
  ; /* LangID = 0x0409: U.S. English */

const uint8_t Speaker_StringVendor[SPEAKER_SIZ_STRING_VENDOR] =
  {
    SPEAKER_SIZ_STRING_VENDOR, /* Size of manufacturer string */
    USB_STRING_DESCRIPTOR_TYPE,  /* bDescriptorType*/
    /* Manufacturer: "STMicroelectronics" */
    'S', 0, 'T', 0, 'M', 0, 'i', 0, 'c', 0, 'r', 0, 'o', 0, 'e', 0,
    'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0,
    'c', 0, 's', 0
  };

const uint8_t Speaker_StringProduct[SPEAKER_SIZ_STRING_PRODUCT] =
  {
    SPEAKER_SIZ_STRING_PRODUCT,  /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, ' ', 0,
    'S', 0, 'p', 0, 'e', 0, 'a', 0, 'k', 0, 'e', 0, 'r', 0
  };

uint8_t Speaker_StringSerial[SPEAKER_SIZ_STRING_SERIAL] =
  {
    SPEAKER_SIZ_STRING_SERIAL,  /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, '1', 0, '0', 0
  };
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
