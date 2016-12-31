/**
 ******************************************************************************
 * @file    usbh_audio_core.c
 * @author  MCD Application Team
 * @version V2.2.0
 * @date    09-November-2015
 * @brief   This file is the Audio Layer Handlers for USB Host Audio class.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
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
#include "usbh_audio_core.h"
#include <string.h>

#include "USB.h"

/** @addtogroup USBH_LIB
 * @{
 */

/** @addtogroup USBH_CLASS
 * @{
 */

/** @addtogroup CDC_CLASS
 * @{
 */

/** @defgroup CDC_CORE 
 * @brief    This file includes CDC Layer Handlers for USB Host CDC class.
 * @{
 */

/** @defgroup CDC_CORE_Private_TypesDefinitions
 * @{
 */
/**
 * @}
 */

/** @defgroup CDC_CORE_Private_Defines
 * @{
 */
#define USB_MS_BUFFER_SIZE     (512)
#define APP_MS_BUFFER_SIZE     (256)
/**
 * @}
 */

/** @defgroup CDC_CORE_Private_Macros
 * @{
 */
/**
 * @}
 */

/** @defgroup CDC_CORE_Private_Variables
 * @{
 */
enum
{
   MS_ITF_COUNT = 2
};

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4   
#endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

__ALIGN_BEGIN MS_Machine_t MS_Machine[MS_ITF_COUNT] __ALIGN_END;


#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4   
#endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN uint8_t TxBuf[MS_ITF_COUNT][USB_MS_BUFFER_SIZE] __ALIGN_END;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4   
#endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN uint8_t RxBuf[MS_ITF_COUNT][USB_MS_BUFFER_SIZE] __ALIGN_END;

/**
 * @}
 */

uint8_t MS_RxBuffer[MS_ITF_COUNT][APP_MS_BUFFER_SIZE];
uint8_t MS_TxBuffer[MS_ITF_COUNT][APP_MS_BUFFER_SIZE];

Ringbuffer_t MS_RingbufTx[MS_ITF_COUNT] = {{ &MS_TxBuffer[0][0], APP_MS_BUFFER_SIZE, 0, 0 },
                                             { &MS_TxBuffer[1][0], APP_MS_BUFFER_SIZE, 0, 0 }};



Ringbuffer_t MS_RingbufRx[MS_ITF_COUNT] = {{ &MS_RxBuffer[0][0], APP_MS_BUFFER_SIZE, 0, 0 },
                                             { &MS_RxBuffer[1][0], APP_MS_BUFFER_SIZE, 0, 0 }};


MS_Machine_t* MS_GetMachineInstance(USB_OTG_CORE_HANDLE *pdev)
{
   if (pdev == &USB_OTG_dev)
   {
      return &MS_Machine[0];
   }
   else
   {
      return &MS_Machine[1];
   }
}


uint8_t MS_GetMachineIndex(USB_OTG_CORE_HANDLE *pdev)
{
   if (pdev == &USB_OTG_dev)
   {
      return 0;
   }
   else
   {
      return 1;
   }
}

/** @defgroup CDC_CORE_Private_FunctionPrototypes
 * @{
 */


static void MS_ReceiveData(USB_OTG_CORE_HANDLE *pdev, Ringbuffer_t* buf);

void MS_ProcessTransmission(USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost);

static void MS_ProcessReception(USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost);

static USBH_Status AudioMIDIHost_InterfaceInit(USB_OTG_CORE_HANDLE *pdev, void *phost);

void AudioMIDIHost_InterfaceDeInit(USB_OTG_CORE_HANDLE *pdev, void *phost);

static USBH_Status AudioMIDIHost_Handle(USB_OTG_CORE_HANDLE *pdev, void *phost);

static USBH_Status AudioMIDIHost_ClassRequest(USB_OTG_CORE_HANDLE *pdev, void *phost);

USBH_Class_cb_TypeDef MS_cb = { AudioMIDIHost_InterfaceInit, AudioMIDIHost_InterfaceDeInit, AudioMIDIHost_ClassRequest, AudioMIDIHost_Handle };
/**
 * @}
 */

/** @defgroup CDC_CORE_Private_Functions
 * @{
 */

static USBH_Status AudioMIDIHost_InterfaceInit(USB_OTG_CORE_HANDLE *pdev, void *phost)
{
   USBH_HOST *pphost = phost;
   USBH_Status status = USBH_NOT_SUPPORTED;

   MS_Machine_t* ms = MS_GetMachineInstance(pdev);

   /* Find the MIDI streaming interface */
   for (uint8_t i = 0; i < pphost->device_prop.Cfg_Desc.bNumInterfaces; i++)
   {
      /* Audio Interface or Vendor Specific */
      if (((pphost->device_prop.Itf_Desc[i].bInterfaceClass == USB_DEVICE_CLASS_AUDIO) || (pphost->device_prop.Itf_Desc[i].bInterfaceClass == VENDOR_SPECIFIC_INTERFACE_CLASS)))
      {

         ms->interface_index = i;

         /* Assume a MIDI port here */
         if ((pphost->device_prop.Itf_Desc[i].bNumEndpoints == 2))
         {

            /*Collect the notification endpoint address and length*/
            ms->itflength = pphost->device_prop.Ep_Desc[i][0].wMaxPacketSize;

            if (pphost->device_prop.Ep_Desc[i][0].bEndpointAddress & 0x80)
            {
               ms->inEp = pphost->device_prop.Ep_Desc[i][0].bEndpointAddress;
               ms->outEp = pphost->device_prop.Ep_Desc[i][1].bEndpointAddress;

               ms->inEpType = pphost->device_prop.Ep_Desc[i][0].bmAttributes & EP_TYPE_MSK;
               ms->outEpType = pphost->device_prop.Ep_Desc[i][1].bmAttributes & EP_TYPE_MSK;

            }

            if (pphost->device_prop.Ep_Desc[i][1].bEndpointAddress & 0x80)
            {
               ms->inEp = pphost->device_prop.Ep_Desc[i][1].bEndpointAddress;
               ms->outEp = pphost->device_prop.Ep_Desc[i][0].bEndpointAddress;

               ms->inEpType = pphost->device_prop.Ep_Desc[i][1].bmAttributes & EP_TYPE_MSK;
               ms->outEpType = pphost->device_prop.Ep_Desc[i][0].bmAttributes & EP_TYPE_MSK;
            }

            /*Allocate the length for host channel number in*/
            ms->hc_num_in = USBH_Alloc_Channel(pdev, ms->inEp);
            ms->hc_num_out = USBH_Alloc_Channel(pdev, ms->outEp);

            /* Open channel for OUT endpoint */
            USBH_Open_Channel(pdev, ms->hc_num_out, pphost->device_prop.address, pphost->device_prop.speed, ms->outEpType, ms->itflength);

            /* Open channel for IN endpoint */
            USBH_Open_Channel(pdev, ms->hc_num_in, pphost->device_prop.address, pphost->device_prop.speed, ms->inEpType, ms->itflength);

            MS_InitTxRxParam(pdev);

            status = USBH_OK;
            ms->ReqState = MS_GET_CLASS_FUNCTIONAL_DESC;
            ms->state = MS_CONNECTED;

         }

      }
   }

   if (status == USBH_NOT_SUPPORTED)
   {
      pphost->usr_cb->DeviceNotSupported();
   }

   return status;

}

/**
 * @brief  CDC_InterfaceDeInit
 *         The function DeInit the Host Channels used for the CDC class.
 * @param  pdev: Selected device
 * @param  hdev: Selected device property
 * @retval None
 */
void AudioMIDIHost_InterfaceDeInit(USB_OTG_CORE_HANDLE *pdev, void *phost)
{
   MS_Machine_t* ms = MS_GetMachineInstance(pdev);

   if (ms->hc_num_in)
   {
      USB_OTG_HC_Halt(pdev, ms->hc_num_in);
      USBH_Free_Channel(pdev, ms->hc_num_in);
      ms->hc_num_in = 0; /* Reset the Channel as Free */
   }

   if (ms->hc_num_out)
   {
      USB_OTG_HC_Halt(pdev, ms->hc_num_out);
      USBH_Free_Channel(pdev, ms->hc_num_out);
      ms->hc_num_out = 0; /* Reset the Channel as Free */
   }

   ms->state = MS_DISCONNECTED;

   USBHostInit(pdev);

//  return USBH_OK;
}

/**
 * @brief  CDC_ClassRequest
 *         The function is responsible for handling CDC Class requests
 *         for CDC class.
 * @param  pdev: Selected device
 * @param  hdev: Selected device property
 * @retval  USBH_Status :Response for USB Set Protocol request
 */
static USBH_Status AudioMIDIHost_ClassRequest(USB_OTG_CORE_HANDLE *pdev, void *phost)
{
   USBH_HOST *pphost = phost;

   USBH_Status status = USBH_BUSY;
   USBH_Status ClassReqStatus = USBH_BUSY;

   MSInterfaceDesc_t itfdesc;
   MS_Machine_t* ms = MS_GetMachineInstance(pdev);


   switch (ms->ReqState)
   {

   case MS_GET_CLASS_FUNCTIONAL_DESC:
      /*Issue the get line coding request*/
      status = USBH_GetDescriptor(pdev, phost,
      USB_REQ_RECIPIENT_DEVICE | USB_REQ_TYPE_STANDARD,
      USB_DESC_CONFIGURATION, pdev->host.Rx_Buffer, pphost->device_prop.Cfg_Desc.wTotalLength);

      if (status == USBH_OK)
      {
         MSInterfaceDesc_t* desc = MS_FindInterfaceIndex(pdev->host.Rx_Buffer, pphost->device_prop.Cfg_Desc.wTotalLength, ms->interface_index);
         if (desc)
         {
            memcpy(&itfdesc, desc, sizeof(MSInterfaceDesc_t));
         }
         status = USBH_OK;
         //ClassReqStatus = MS_GetClassSpecificInterfaceHeader(pdev, phost, &itfdesc);
      }
   }

   return status;
}

/**
 * @brief  CDC_Handle
 *         The function is for managing state machine for CDC data transfers
 * @param  pdev: Selected device
 * @param  hdev: Selected device property
 * @retval USBH_Status
 */
static USBH_Status AudioMIDIHost_Handle(USB_OTG_CORE_HANDLE *pdev, void *phost)
{
   USBH_Status status = USBH_OK;
   USBH_HOST *pphost = phost;

   /* Call Application process */
   pphost->usr_cb->UserApplication();

   /*Handle the transmission */
   MS_ProcessTransmission(pdev, pphost);

   /*Always send in packet to device*/
   MS_ProcessReception(pdev, pphost);

   return status;
}

/**
 * @brief  The function is responsible for sending data to the device
 * @param  pdev: Selected device
 * @retval None
 */
void MS_ProcessTransmission(USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost)
{
   uint32_t len;
   URB_STATE URB_StatusTx = URB_IDLE;

   MS_Machine_t* ms = MS_GetMachineInstance(pdev);

   URB_StatusTx = HCD_GetURB_State(pdev, ms->hc_num_out);

   switch (ms->TxParam.State)
   {
   case MS_IDLE:
      if (RingBuffer_GetSpaceUsed(ms->TxBuf) >= 4)
      {
         //Fall through to MS_SEND_DATA;
      }
      else
      {
         break;
      }

   case MS_SEND_DATA:

      if ((URB_StatusTx == URB_DONE) || (URB_StatusTx == URB_IDLE))
      {
         len = RingBuffer_GetSpaceUsed(ms->TxBuf);
         if (len > ms->itflength)
         {
            len = ms->itflength;
         }

         RingBuffer_ReadBuffer(ms->TxBuf, ms->TxParam.pRxTxBuff, len);

         /*Send the data */
         if( ms->outEpType == EP_TYPE_INTR)
         {
            USBH_InterruptSendData(pdev, ms->TxParam.pRxTxBuff, len, ms->hc_num_out);
         }
         else
         {
            USBH_BulkSendData(pdev, ms->TxParam.pRxTxBuff, len, ms->hc_num_out);
         }

         ms->TxParam.State = MS_DATA_SENT;

      }

      break;

   case MS_DATA_SENT:
      /*Check the status done for transmssion*/
      if ((URB_StatusTx == URB_DONE) || (URB_StatusTx == URB_IDLE))
      {
         ms->TxParam.State = MS_IDLE;
      }
      else if (URB_StatusTx == URB_NOTREADY)
      {
         /*Send the same data */
         if( ms->outEpType == EP_TYPE_INTR)
         {
            USBH_InterruptSendData(pdev, ms->TxParam.pRxTxBuff, len, ms->hc_num_out);
         }
         else
         {
            USBH_BulkSendData(pdev, ms->TxParam.pRxTxBuff, len, ms->hc_num_out);
         }
      }

      break;

   }
}
/**
 * @brief  This function responsible for reception of data from the device
 * @param  pdev: Selected device
 * @retval None
 */
static void MS_ProcessReception(USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost)
{
   MS_Machine_t* ms = MS_GetMachineInstance(pdev);
   URB_STATE URB_StatusRx = HCD_GetURB_State(pdev, ms->hc_num_in);
   switch (ms->RxParam.State)
   {
   case MS_IDLE:
      /*check the received length lesser then the remaining space available in the
       buffer */
      if (RingBuffer_GetSpaceAvailable(ms->RxBuf) >= ms->itflength)
      {
         /*Receive the data */
         if( ms->inEpType == EP_TYPE_INTR)
         {
            if( ( HCD_GetCurrentFrame(pdev) - ms->intTimer) >= 1)
            {
               /* Interrupts we have to keep polling */
               ms->intTimer = HCD_GetCurrentFrame(pdev);
               if (URB_StatusRx == URB_DONE)
               {
                  ms->RxParam.State = MS_GET_DATA;
               }
               else
               {
                  USBH_InterruptReceiveData(pdev, ms->RxParam.pRxTxBuff, ms->itflength, ms->hc_num_in);
               }
            }
         }
         else
         {
            /*change the MS state to USBH_MS_GET_DATA*/
            USBH_BulkReceiveData(pdev, ms->RxParam.pRxTxBuff, ms->itflength, ms->hc_num_in);
            ms->RxParam.State = MS_GET_DATA;
         }
      }
      break;


   case MS_GET_DATA:
      /*Check the last state of the device is URB_DONE */
      if (URB_StatusRx == URB_DONE)
      {
         /* Move the pointer as well as datalength */
         RingBuffer_WriteBuffer(ms->RxBuf, ms->RxParam.pRxTxBuff, pdev->host.hc[ms->hc_num_in].xfer_count);

         /* Process the recived data */
         MS_ReceiveData(pdev, ms->RxBuf);

         /*change the state to the MS state*/
         ms->RxParam.State = MS_IDLE;

      }
      break;
   }
}

/**
 * @brief  Initialize the transmit and receive buffer and its parameter
 * @param  None
 * @retval None
 */
void MS_InitTxRxParam(USB_OTG_CORE_HANDLE *pdev)
{
   MS_Machine_t* ms = MS_GetMachineInstance(pdev);
   uint8_t idx = MS_GetMachineIndex(pdev);

   /*Initialize the Transmit buffer and its parameter*/
   ms->TxParam.State = MS_IDLE;
   ms->TxParam.DataLength = 0;
   ms->TxParam.pRxTxBuff = &TxBuf[idx][0];

   /*Initialize the Receive buffer and its parameter*/
   ms->RxParam.State = MS_IDLE;
   ms->RxParam.DataLength = 0;
   ms->RxParam.pRxTxBuff = &RxBuf[idx][0];
   ms->RxParam.BufferLen = USB_MS_BUFFER_SIZE;

   ms->TxBuf = &MS_RingbufTx[idx];
   ms->RxBuf = &MS_RingbufRx[idx];

   ms->dev = pdev;

   RingBuffer_Clear(ms->TxBuf);
   RingBuffer_Clear(ms->RxBuf);
}

/**
 * @brief  This is a call back function from cdc core layer to redirect the
 *         received data on the user out put system
 * @param  cdc_Data: type of USBH_CDCXfer_TypeDef
 * @retval None
 */
static void MS_ReceiveData(USB_OTG_CORE_HANDLE *pdev, Ringbuffer_t* buf)
{
   uint8_t msIdx = (MS_GetMachineIndex(pdev) == 0) ? 1 : 0;
   MS_Machine_t* ms = &MS_Machine[msIdx];
   uint8_t byte[64];
   uint16_t length = RingBuffer_GetSpaceUsed(buf);

   /* Process actual data */
   RingBuffer_ReadBuffer(buf, &byte[0], length);
   MS_SendData(ms->dev, &byte[0], length);

}

void MS_SendData(USB_OTG_CORE_HANDLE* pdev, uint8_t *data, uint16_t length)
{
   MS_Machine_t* ms = MS_GetMachineInstance(pdev);
   if (ms->state == MS_CONNECTED)
   {
      RingBuffer_WriteBuffer(ms->TxBuf, data, length);
   }
}

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

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
