/**
  ******************************************************************************
  * @file    usbh_cdc_core.h
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    09-November-2015
  * @brief   This file contains all the prototypes for the usbh_cdc_core.c
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

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __USBH_AUDIO_CORE_H
#define __USBH_AUDIO_CORE_H

/* Includes ------------------------------------------------------------------*/
#include "usb_audio_enums.h"
#include "usbh_audio_funct.h"
#include "Ringbuffer.h"

/** @addtogroup USBH_LIB
* @{
*/

/** @addtogroup USBH_CLASS
* @{
*/

/** @addtogroup USBH_CDC_CLASS
* @{
*/

/** @defgroup USBH_CDC_CORE
* @brief This file is the Header file for USBH_CDC_CORE.c
* @{
*/ 




/*Comuncation Calss codes*/
#define COMMUNICATION_DEVICE_CLASS_CODE                         0x02
#define COMMUNICATION_INTERFACE_CLASS_CODE                      0x02

/*Data Interface Class Codes*/
#define DATA_INTERFACE_CLASS_CODE                               0x0A

/*Communcation sub class codes*/
#define RESERVED                                                0x00
#define DIRECT_LINE_CONTROL_MODEL                               0x01
#define ABSTRACT_CONTROL_MODEL                                  0x02
#define TELEPHONE_CONTROL_MODEL                                 0x03
#define MULTICHANNEL_CONTROL_MODEL                              0x04   
#define CAPI_CONTROL_MODEL                                      0x05
#define ETHERNET_NETWORKING_CONTROL_MODEL                       0x06
#define ATM_NETWORKING_CONTROL_MODEL                            0x07


/*Communication Interface Class Control Protocol Codes*/
#define NO_CLASS_SPECIFIC_PROTOCOL_CODE                         0x00
#define COMMON_AT_COMMAND                                       0x01
#define VENDOR_SPECIFIC                                         0xFF


//#define CS_INTERFACE                                            0x24
#define CDC_PAGE_SIZE_64                                        0x40




/** @defgroup USBH_CDC_CORE_Exported_Types
* @{
*/ 




/* States for MS State Machine */
typedef enum
{
  MS_IDLE= 0,
  MS_READ_DATA,
  MS_SEND_DATA,
  MS_DATA_SENT,
  MS_BUSY,
  MS_GET_DATA,
  MS_POLL,
  MS_CTRL_STATE,
  MS_DISCONNECTED,
  MS_CONNECTED,
}
MS_State;

/* MS Transfer State */
typedef struct _MSXfer
{
  volatile MS_State State;
  uint8_t* pRxTxBuff;
  uint32_t BufferLen;
  uint16_t DataLength;
} MS_Xfer_TypeDef;




typedef struct
{
   uint8_t hc_num_in;
   uint8_t hc_num_out;

   uint8_t outEp;
   uint8_t inEp;

   uint8_t inEpType;
   uint8_t outEpType;

   uint16_t itflength;
   uint8_t interface_index;

   MS_Requests ReqState;
   uint8_t state;

   MS_Xfer_TypeDef TxParam;
   MS_Xfer_TypeDef RxParam;

   Ringbuffer_t* TxBuf;
   Ringbuffer_t* RxBuf;

   uint32_t intTimer;
   USB_OTG_CORE_HANDLE* dev;

} MS_Machine_t;



/**
* @}
*/ 

/** @defgroup USBH_CDC_CORE_Exported_Defines
* @{
*/ 

/**
* @}
*/ 

/** @defgroup USBH_CDC_CORE_Exported_Macros
* @{
*/ 
/**
* @}
*/ 

/** @defgroup USBH_CDC_CORE_Exported_Variables
* @{
*/ 
extern USBH_Class_cb_TypeDef  MS_cb;

/**
* @}
*/ 

/** @defgroup USBH_CDC_CORE_Exported_FunctionsPrototype
* @{
*/ 
void MS_InitTxRxParam(USB_OTG_CORE_HANDLE *pdev);
void MS_SendData(USB_OTG_CORE_HANDLE* pdev, uint8_t *data, uint16_t length);

/**
* @}
*/ 


#endif /* __USBH_CDC_CORE_H */

/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/ 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

