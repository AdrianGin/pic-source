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
#define USB_MS_BUFFER_SIZE     (1024)
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
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4   
#endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN CDC_Machine_TypeDef   CDC_Machine __ALIGN_END ;


__ALIGN_BEGIN MIDI_Machine_t   MS_Machine __ALIGN_END ;



MS_Requests                         MS_ReqState;


CDC_Requests                        CDC_ReqState;
CDC_Xfer_TypeDef                    CDC_TxParam;
CDC_Xfer_TypeDef                    CDC_RxParam;

MS_Xfer_TypeDef   MS_TxParam;
MS_Xfer_TypeDef   MS_RxParam;



#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4   
#endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN uint8_t               TxBuf [USB_MS_BUFFER_SIZE] __ALIGN_END ;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma data_alignment=4   
#endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */
__ALIGN_BEGIN uint8_t               RxBuf [USB_MS_BUFFER_SIZE] __ALIGN_END ;

CDC_Usercb_TypeDef                  UserCb;
uint8_t                             RX_Enabled = 0;
/**
* @}
*/ 

uint8_t MS_RxBuffer[APP_MS_BUFFER_SIZE];
uint8_t MS_TxBuffer[APP_MS_BUFFER_SIZE];

Ringbuffer_t MS_TxBuf = {&MS_TxBuffer[0], APP_MS_BUFFER_SIZE, 0, 0};
Ringbuffer_t MS_RxBuf = {&MS_RxBuffer[0], APP_MS_BUFFER_SIZE, 0, 0};


/** @defgroup CDC_CORE_Private_FunctionPrototypes
* @{
*/ 
void MS_InitTxRxParam(void);

static void MS_ReceiveData(Ringbuffer_t* buf);

void MS_ProcessTransmission(USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost);

static void MS_ProcessReception(USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost);

static USBH_Status AudioMIDIHost_InterfaceInit ( USB_OTG_CORE_HANDLE *pdev, void *phost);


void CDC_InterfaceDeInit  (USB_OTG_CORE_HANDLE *pdev , 
                                  void *phost);

static USBH_Status CDC_Handle(USB_OTG_CORE_HANDLE *pdev , 
                              void *phost);

static USBH_Status CDC_ClassRequest(USB_OTG_CORE_HANDLE *pdev , 
                                    void *phost);


USBH_Class_cb_TypeDef  CDC_cb = 
{
  AudioMIDIHost_InterfaceInit,
  CDC_InterfaceDeInit,
  CDC_ClassRequest,
  CDC_Handle
};
/**
* @}
*/ 


/** @defgroup CDC_CORE_Private_Functions
* @{
*/ 


static USBH_Status AudioMIDIHost_InterfaceInit ( USB_OTG_CORE_HANDLE *pdev, void *phost)
{
   USBH_HOST *pphost = phost;
   USBH_Status status = USBH_NOT_SUPPORTED;

   /* Find the MIDI streaming interface */
   for( uint8_t i = 0; i < pphost->device_prop.Cfg_Desc.bNumInterfaces; i++)
   {
      /* Audio Interface or Vendor Specific */
      if( ((pphost->device_prop.Itf_Desc[i].bInterfaceClass  == USB_DEVICE_CLASS_AUDIO) || \
          (pphost->device_prop.Itf_Desc[i].bInterfaceClass  == VENDOR_SPECIFIC_INTERFACE_CLASS)) )
      {

         MS_Machine.interface_index = i;

         /* Assume a MIDI port here */
         if( (pphost->device_prop.Itf_Desc[i].bNumEndpoints  == 2) )
         {

            /*Collect the notification endpoint address and length*/
            MS_Machine.ep_addr = pphost->device_prop.Ep_Desc[i][0].bEndpointAddress;
            MS_Machine.itflength  = pphost->device_prop.Ep_Desc[i][0].wMaxPacketSize;

            if(pphost->device_prop.Ep_Desc[i][0].bEndpointAddress & 0x80)
            {
               MS_Machine.inEp = pphost->device_prop.Ep_Desc[i][0].bEndpointAddress;
               MS_Machine.outEp = pphost->device_prop.Ep_Desc[i][1].bEndpointAddress;

               MS_Machine.inEpType = pphost->device_prop.Ep_Desc[i][0].bmAttributes & EP_TYPE_MSK;
               MS_Machine.outEpType = pphost->device_prop.Ep_Desc[i][1].bmAttributes & EP_TYPE_MSK;

            }

            if(pphost->device_prop.Ep_Desc[i][1].bEndpointAddress & 0x80)
            {
               MS_Machine.inEp = pphost->device_prop.Ep_Desc[i][1].bEndpointAddress;
               MS_Machine.outEp = pphost->device_prop.Ep_Desc[i][0].bEndpointAddress;

               MS_Machine.inEpType = pphost->device_prop.Ep_Desc[i][1].bmAttributes & EP_TYPE_MSK;
               MS_Machine.outEpType = pphost->device_prop.Ep_Desc[i][0].bmAttributes & EP_TYPE_MSK;
            }

              /*Allocate the length for host channel number in*/
            MS_Machine.hc_num_in = USBH_Alloc_Channel(pdev, MS_Machine.inEp );
            MS_Machine.hc_num_out = USBH_Alloc_Channel(pdev, MS_Machine.outEp );


            /* Open channel for OUT endpoint */
            USBH_Open_Channel  (pdev,
                                MS_Machine.hc_num_out,
                                pphost->device_prop.address,
                                pphost->device_prop.speed,
                                MS_Machine.outEpType,
                                MS_Machine.itflength);

            /* Open channel for IN endpoint */
            USBH_Open_Channel  (pdev,
                                MS_Machine.hc_num_in,
                                pphost->device_prop.address,
                                pphost->device_prop.speed,
                                MS_Machine.inEpType,
                                MS_Machine.itflength);


            MS_InitTxRxParam();

            status = USBH_OK;
            MS_ReqState = MS_GET_CLASS_FUNCTIONAL_DESC;
            MS_Machine.state = MS_CONNECTED;

         }

      }
   }

   if( status == USBH_NOT_SUPPORTED )
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
void CDC_InterfaceDeInit ( USB_OTG_CORE_HANDLE *pdev,
                          void *phost)
{
  if ( MS_Machine.hc_num_in)
  {
    USB_OTG_HC_Halt(pdev, MS_Machine.hc_num_in);
    USBH_Free_Channel  (pdev,MS_Machine.hc_num_in);
    MS_Machine.hc_num_in = 0;     /* Reset the Channel as Free */
  }
  
  if ( MS_Machine.hc_num_out)
  {
    USB_OTG_HC_Halt(pdev, MS_Machine.hc_num_out);
    USBH_Free_Channel  (pdev, MS_Machine.hc_num_out);
    MS_Machine.hc_num_out = 0;     /* Reset the Channel as Free */
  }
  
  MS_Machine.state = MS_DISCONNECTED;

  USBHostInit();


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
static USBH_Status CDC_ClassRequest(USB_OTG_CORE_HANDLE *pdev , 
                                    void *phost)
{   
  USBH_HOST *pphost = phost;
  
  USBH_Status status         = USBH_BUSY;
  USBH_Status ClassReqStatus = USBH_BUSY;
  
  MSInterfaceDesc_t itfdesc;

  switch(MS_ReqState)
  {
    
  case MS_GET_CLASS_FUNCTIONAL_DESC:
    /*Issue the get line coding request*/
     status = USBH_GetDescriptor(pdev,
                                 phost,
                                 USB_REQ_RECIPIENT_DEVICE | USB_REQ_TYPE_STANDARD,
                                 USB_DESC_CONFIGURATION,
                                 pdev->host.Rx_Buffer, pphost->device_prop.Cfg_Desc.wTotalLength);


     if( status == USBH_OK )
     {
        MSInterfaceDesc_t* desc = MS_FindInterfaceIndex(pdev->host.Rx_Buffer, pphost->device_prop.Cfg_Desc.wTotalLength, MS_Machine.interface_index);
        if( desc )
        {
           memcpy(&itfdesc, desc, sizeof(MSInterfaceDesc_t) );
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
static USBH_Status CDC_Handle(USB_OTG_CORE_HANDLE *pdev , 
                              void   *phost)
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
  static uint32_t len ;
  URB_STATE URB_StatusTx = URB_IDLE;
  
  URB_StatusTx =   HCD_GetURB_State(pdev , MS_Machine.hc_num_out);
  
  switch(MS_TxParam.State)
  {
  case MS_IDLE:
     if( RingBuffer_GetSpaceUsed(&MS_TxBuf) )
     {
        //Fall through to MS_SEND_DATA;
     }
     else
     {
        break;
     }
    
  case MS_SEND_DATA:
    
    if(( URB_StatusTx == URB_DONE ) || (URB_StatusTx == URB_IDLE))
    {
       len = RingBuffer_GetSpaceUsed(&MS_TxBuf);
       if( len > MS_Machine.itflength)
       {
          len = MS_Machine.itflength;
       }

       RingBuffer_ReadBuffer(&MS_TxBuf, MS_TxParam.pRxTxBuff, len);

       /*Send the data */
       USBH_BulkSendData (pdev,
                          MS_TxParam.pRxTxBuff,
                          len ,
                          MS_Machine.hc_num_out);

      MS_TxParam.State = MS_DATA_SENT;
      
    }
    
    break;
    
  case MS_DATA_SENT:
    /*Check the status done for transmssion*/
    if(URB_StatusTx == URB_DONE )
    {         
       MS_TxParam.State = MS_IDLE;
    }
    else if( URB_StatusTx == URB_NOTREADY )
    {
      /*Send the same data */
      USBH_BulkSendData (pdev,
                         (MS_TxParam.pRxTxBuff),
                         len, 
                         MS_Machine.hc_num_out);
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

   URB_STATE URB_StatusRx =   HCD_GetURB_State(pdev , MS_Machine.hc_num_in);
   switch(MS_RxParam.State)
   {
   case MS_IDLE:
   /*check the received length lesser then the remaining space available in the
   buffer */
   if(RingBuffer_GetSpaceAvailable(&MS_RxBuf) >= MS_Machine.itflength)
   {
     /*Receive the data */
     USBH_BulkReceiveData(pdev,
                          MS_RxParam.pRxTxBuff,
                          MS_Machine.itflength,
                          MS_Machine.hc_num_in);

     /*change the MS state to USBH_MS_GET_DATA*/
     MS_RxParam.State = MS_GET_DATA;
   }
   break;

   case MS_GET_DATA:
   /*Check the last state of the device is URB_DONE */
   if(URB_StatusRx == URB_DONE)
   {
     /* Move the pointer as well as datalength */
     RingBuffer_WriteBuffer(&MS_RxBuf, MS_RxParam.pRxTxBuff, pdev->host.hc[MS_Machine.hc_num_in].xfer_count);

     /* Process the recived data */
     MS_ReceiveData(&MS_RxBuf);

     /*change the state od the MS state*/
     MS_RxParam.State = MS_IDLE;

   }
   break;
   }
}

/**
  * @brief  Initialize the transmit and receive buffer and its parameter
  * @param  None
  * @retval None
  */
void MS_InitTxRxParam(void)
{
  /*Initialize the Transmit buffer and its parameter*/
  MS_TxParam.State = MS_IDLE;
  MS_TxParam.DataLength = 0;
  MS_TxParam.pRxTxBuff = TxBuf;
  
  /*Initialize the Receive buffer and its parameter*/
  MS_RxParam.State = MS_IDLE;
  MS_RxParam.DataLength = 0;
  MS_RxParam.pRxTxBuff = RxBuf;
  MS_RxParam.pFillBuff = RxBuf;
  MS_RxParam.pEmptyBuff = RxBuf;
  MS_RxParam.BufferLen = sizeof(RxBuf);
}

/**
  * @brief  This is a call back function from cdc core layer to redirect the 
  *         received data on the user out put system
  * @param  cdc_Data: type of USBH_CDCXfer_TypeDef
  * @retval None
  */
static void MS_ReceiveData(Ringbuffer_t* buf)
{
   uint8_t byte;
  /* Process actual data */
   while( RingBuffer_Read(buf, &byte ))
   {
      RingBuffer_Write(&MS_TxBuf , byte);
   }



}


void  MS_SendData(uint8_t *data, uint16_t length)
{
   if( MS_Machine.state == MS_CONNECTED)
   {
      RingBuffer_WriteBuffer(&MS_TxBuf, data, length);
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
