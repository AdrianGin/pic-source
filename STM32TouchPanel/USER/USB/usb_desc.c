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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants----------------------------------------------------------*/
/* USB Standard Device Descriptor */
const uint8_t Speaker_DeviceDescriptor[] =
  {
    0x12,          /* bLength */
    0x01,           /* bDescriptorType */
    0x00,          /* 2.00 */             /* bcdUSB */
    0x02,
    0x00,                                 /* bDeviceClass */
    0x00,                                 /* bDeviceSubClass */
    0x00,                                 /* bDeviceProtocol */
    0x08,                                 /* bMaxPacketSize 40 */
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
/* USB configuration descriptor */
	USBDESC_INTERFACE_SIZE,			/* sizeof(usbDescrConfig): length of descriptor in bytes */
	USB_CONFIGURATION_DESCRIPTOR_TYPE,	/* descriptor type */
	(USBDESC_CONFIG_SIZE&0xFF), (USBDESC_CONFIG_SIZE>>8),			/* total length of data returned (including inlined descriptors) */
	2,			/* number of interfaces in this configuration */
	1,			/* index of this configuration */
	0,			/* configuration name string index */
	0x80,	/* attributes */
	0x32,	/* max USB current in 2mA units */

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
	0,			/* index of this interface */
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
	1,			/* */
	1,			/* */

// B.4 MIDIStreaming Interface Descriptors

// B.4.1 Standard MS Interface Descriptor
	/* interface descriptor follows inline: */
	USBDESC_MIDI_INTERFACE_SIZE,			/* length of descriptor in bytes */
	USB_INTERFACE_DESCRIPTOR_TYPE,	/* descriptor type */
	1,			/* index of this interface */
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
	0x1,			/* bEndpointAddress OUT endpoint number 1 */
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
	0x81,			/* bEndpointAddress IN endpoint number 1 */
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

/* LangID = 0x0409: U.S. English */
const uint8_t USBDesc_LangID[] = {0x09, 0x04};
const uint8_t USBDesc_StringVendor[] = "AG Funds Ltd";
const uint8_t USBDesc_StringProduct[] = "AlexW MIDI";
const uint8_t USBDesc_StringSerial[] = "MIDI Piano";

const uint8_t* USBDesciptors[] = {USBDesc_LangID, USBDesc_StringVendor, USBDesc_StringProduct, USBDesc_StringSerial};

\
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
