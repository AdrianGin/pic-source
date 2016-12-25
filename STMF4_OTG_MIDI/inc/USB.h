#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _USB_H
#define _USB_H

#include <stdint.h>
#include "usb_core.h"
#include "usb_dcd.h"

//Does not correspond to Official USB states,
//but allows user applications to know if
//the USB is enumerated and ready for EP transactions
typedef enum
{
   USBD_DISCONNECTED = 0x00,
   USBD_CONFIGURED,
} USBDevice_State;

enum
{
   USBD_STATE_CHANGED = 0x01,
};

typedef struct
{
   USBDevice_State laststate;
   USBDevice_State state;
   volatile uint8_t ISR_stateChangeRequestCount;
   uint8_t stateChangeHandledCount;
} USBDevice_t;

extern USB_OTG_CORE_HANDLE USB_OTG_dev;
extern USB_OTG_CORE_HANDLE USB_OTG_dev_HS;

extern USBDevice_t USB_User_Dev;

void USB_Init(void);

static inline uint32_t USBD_GetEPStatus(uint8_t epnum)
{
   return DCD_GetEPStatus(&USB_OTG_dev, epnum);
}

static inline void USBDevice_FlagStateChange(USBDevice_t* dev)
{
   dev->ISR_stateChangeRequestCount++;
}

/* USB MIDI Port access functions */
static inline void USB_Device_SetState(USBDevice_t* dev, USBDevice_State newState)
{
   dev->state = newState;
   //USBDevice_FlagStateChange(&USB_User_Dev);
}

static inline USBDevice_State USB_Device_GetState(USBDevice_t* dev)
{
   return dev->state;
}

static inline uint8_t USBDevice_StateHasChanged(USBDevice_t* dev)
{
   return (dev->laststate != dev->state);
}

static inline void USBDevice_StateChangeHandled(USBDevice_t* dev)
{
   dev->laststate = dev->state;
   //dev->stateChangeHandledCount++;
}

#endif

#ifdef __cplusplus
}
#endif
