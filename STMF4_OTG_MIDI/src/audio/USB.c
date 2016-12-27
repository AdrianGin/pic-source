

#include "USB.h"
#include "usbd_audio_core.h"
#include "usbd_desc.h"
#include "usbd_usr.h"

#include "usbh_conf.h"
#include "usbh_core.h"
#include "usbh_usr.h"
#include "usbh_audio_core.h"

enum
{
#ifdef ISDEVBOARD
   USB_CORE_ID = USB_OTG_HS_CORE_ID,
#else
   USB_CORE_ID  = USB_OTG_FS_CORE_ID,
#endif
};

USB_OTG_CORE_HANDLE           USB_OTG_dev;
USB_OTG_CORE_HANDLE           USB_OTG_dev_HS;

USBH_HOST USB_Host;

USBDevice_t	USB_User_Dev =
{
      .laststate = USBD_DISCONNECTED,
      .state = USBD_DISCONNECTED,
};

void USB_Init(void)
{
   USBD_Init(&USB_OTG_dev,
             USB_OTG_FS_CORE_ID,
             &USR_desc,
             &AUDIO_cb,
             &USR_cb);


   /*USBD_Init(&USB_OTG_dev_HS,
              USB_OTG_HS_CORE_ID,
              &USR_desc_HS,
              &AUDIO_cb,
              &USR_cb);*/

   USBH_Init(&USB_OTG_dev_HS,
             USB_OTG_HS_CORE_ID,
             &USB_Host,
             &CDC_cb,
             &USR_Callbacks);


}
