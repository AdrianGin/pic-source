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

static void CDC_ReceiveData(CDC_Xfer_TypeDef *cdc_Data);

static void CDC_ProcessTransmission(USB_OTG_CORE_HANDLE *pdev, USBH_HOST  *phost);

static void CDC_ProcessReception(USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost);

static USBH_Status AudioMIDIHost_InterfaceInit ( USB_OTG_CORE_HANDLE *pdev, void *phost);


static USBH_Status CDC_InterfaceInit  (USB_OTG_CORE_HANDLE *pdev , 
                                       void *phost);

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
  * @brief  CDC_InterfaceInit 
  *         The function init the CDC class.
  * @param  pdev: Selected device
  * @param  hdev: Selected device property
  * @retval  USBH_Status :Response for USB CDC driver intialization
  */
static USBH_Status CDC_InterfaceInit ( USB_OTG_CORE_HANDLE *pdev, 
                                      void *phost)
{	
  USBH_HOST *pphost = phost;
  USBH_Status status = USBH_OK ;
  

  /* Communication Interface */
  if((pphost->device_prop.Itf_Desc[0].bInterfaceClass  == COMMUNICATION_DEVICE_CLASS_CODE)&& \
    (pphost->device_prop.Itf_Desc[0].bInterfaceSubClass  == ABSTRACT_CONTROL_MODEL) && \
      (pphost->device_prop.Itf_Desc[0].bInterfaceProtocol == COMMON_AT_COMMAND))
  {
    /*Collect the notification endpoint address and length*/
    CDC_Machine.CDC_CommItf.ep_addr = pphost->device_prop.Ep_Desc[0][0].bEndpointAddress;
    CDC_Machine.CDC_CommItf.length  = pphost->device_prop.Ep_Desc[0][0].wMaxPacketSize;
    
    if(pphost->device_prop.Ep_Desc[0][0].bEndpointAddress & 0x80)
    {
      CDC_Machine.CDC_CommItf.notificationEp =\
        (pphost->device_prop.Ep_Desc[0][0].bEndpointAddress);
    }
    /*Allocate the length for host channel number in*/
    CDC_Machine.CDC_CommItf.hc_num_in = USBH_Alloc_Channel(pdev, 
                                                           CDC_Machine.CDC_CommItf.notificationEp );
    
    /* Open channel for IN endpoint */
    USBH_Open_Channel  (pdev,
                        CDC_Machine.CDC_CommItf.hc_num_in,
                        pphost->device_prop.address,
                        pphost->device_prop.speed,
                        EP_TYPE_INTR,
                        CDC_Machine.CDC_CommItf.length); 
  }
  else
  {
    pphost->usr_cb->DeviceNotSupported();   
  }
  
  
  /* Data Interface */
  if((pphost->device_prop.Itf_Desc[1].bInterfaceClass  == DATA_INTERFACE_CLASS_CODE)&& \
    (pphost->device_prop.Itf_Desc[1].bInterfaceSubClass  == RESERVED) && \
      (pphost->device_prop.Itf_Desc[1].bInterfaceProtocol == NO_CLASS_SPECIFIC_PROTOCOL_CODE))
  {
    /*Collect the class specific endpoint address and length*/
    CDC_Machine.CDC_DataItf.ep_addr = pphost->device_prop.Ep_Desc[1][0].bEndpointAddress;
    CDC_Machine.CDC_DataItf.length  = pphost->device_prop.Ep_Desc[1][0].wMaxPacketSize;
    
    if(pphost->device_prop.Ep_Desc[1][0].bEndpointAddress & 0x80)
    {      
      CDC_Machine.CDC_DataItf.cdcInEp = (pphost->device_prop.Ep_Desc[1][0].bEndpointAddress);
    }
    else
    {
      CDC_Machine.CDC_DataItf.cdcOutEp = (pphost->device_prop.Ep_Desc[1][0].bEndpointAddress);
    }
    
    if(pphost->device_prop.Ep_Desc[1][1].bEndpointAddress & 0x80)
    {
      CDC_Machine.CDC_DataItf.cdcInEp = (pphost->device_prop.Ep_Desc[1][1].bEndpointAddress);
    }
    else
    {
      CDC_Machine.CDC_DataItf.cdcOutEp = (pphost->device_prop.Ep_Desc[1][1].bEndpointAddress);
    }
    
    /*Allocate the length for host channel number out*/
    CDC_Machine.CDC_DataItf.hc_num_out = USBH_Alloc_Channel(pdev, 
                                                            CDC_Machine.CDC_DataItf.cdcOutEp);
    /*Allocate the length for host channel number in*/
    CDC_Machine.CDC_DataItf.hc_num_in = USBH_Alloc_Channel(pdev, 
                                                           CDC_Machine.CDC_DataItf.cdcInEp);  
    
    /* Open channel for OUT endpoint */
    USBH_Open_Channel  (pdev,
                        CDC_Machine.CDC_DataItf.hc_num_out,
                        pphost->device_prop.address,
                        pphost->device_prop.speed,
                        EP_TYPE_BULK,
                        CDC_Machine.CDC_DataItf.length);  
    /* Open channel for IN endpoint */
    USBH_Open_Channel  (pdev,
                        CDC_Machine.CDC_DataItf.hc_num_in,
                        pphost->device_prop.address,
                        pphost->device_prop.speed,
                        EP_TYPE_BULK,
                        CDC_Machine.CDC_DataItf.length);
    
    /*Initilise the Tx/Rx Params*/
    MS_InitTxRxParam();
    
    
    /*Initialize the class specific request with "GET_LINE_CODING"*/
    CDC_ReqState = CDC_GET_LINE_CODING_RQUEST ;
  }
  else
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
  if ( CDC_Machine.CDC_CommItf.hc_num_in)
  {
    USB_OTG_HC_Halt(pdev, CDC_Machine.CDC_CommItf.hc_num_in);
    USBH_Free_Channel  (pdev,CDC_Machine.CDC_CommItf.hc_num_in);
    CDC_Machine.CDC_CommItf.hc_num_in = 0;     /* Reset the Channel as Free */
  }
  
  if ( CDC_Machine.CDC_DataItf.hc_num_out)
  {
    USB_OTG_HC_Halt(pdev, CDC_Machine.CDC_DataItf.hc_num_out);
    USBH_Free_Channel  (pdev,CDC_Machine.CDC_DataItf.hc_num_out);
    CDC_Machine.CDC_DataItf.hc_num_out = 0;     /* Reset the Channel as Free */
  }
  
  if ( CDC_Machine.CDC_DataItf.hc_num_in)
  {
    USB_OTG_HC_Halt(pdev, CDC_Machine.CDC_DataItf.hc_num_in);
    USBH_Free_Channel  (pdev,CDC_Machine.CDC_DataItf.hc_num_in);
    CDC_Machine.CDC_DataItf.hc_num_in = 0;     /* Reset the Channel as Free */
  } 
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




    if( ClassReqStatus == USBH_OK )
    {          /*Change the state */
      CDC_ReqState = CDC_SET_CONTROL_LINE_STATE_REQUEST;
    }
    break;
#if 0
  case CDC_SET_LINE_CODING_RQUEST: 
    
    /*Issue the set line coding request*/
    ClassReqStatus = CDC_SETLineCoding(pdev, phost);
    if( ClassReqStatus == USBH_OK )
    {
      /*Change the state */
      CDC_ReqState = CDC_GET_LINE_CODING_RQUEST ;
    }
    if(ClassReqStatus == USBH_NOT_SUPPORTED )
    {
      /* a Clear Feature should be issued here */
      CDC_ReqState = CDC_ERROR_STATE;
    }
    break;
    
  case CDC_SET_CONTROL_LINE_STATE_REQUEST:
    /*Issue the set control line coding */
    ClassReqStatus = CDC_SETControlLineState(pdev, phost);
    if( ClassReqStatus == USBH_OK )
    {
      /*Change the state */
      CDC_ReqState = CDC_SET_CONTROL_LINE_STATE_REQUEST;
      /*Also set the state of receive CDCRxParam to IDLE*/
      CDC_RxParam.CDCState = CDC_IDLE; 
      
      status = USBH_OK; /*This return from class specific routinues request*/
    }
    break;
    
  case CDC_ERROR_STATE:
    
    ClassReqStatus = USBH_ClrFeature(pdev,
                                     phost,
                                     0x00,
                                     pphost->Control.hc_num_out);
    
    if(ClassReqStatus == USBH_OK )
    {        
      /*Change the state to waiting*/
      CDC_ReqState = CDC_GET_LINE_CODING_RQUEST ;
    }
    break;
#endif
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
  CDC_ProcessTransmission(pdev, pphost);
  
  /*Always send in packet to device*/    
  CDC_ProcessReception(pdev, pphost);
  
  return status;
}


/**
  * @brief  The function is responsible for sending data to the device
  * @param  pdev: Selected device
  * @retval None
  */
void CDC_ProcessTransmission(USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost)
{
  static uint32_t len ;
  URB_STATE URB_StatusTx = URB_IDLE;
  
  URB_StatusTx =   HCD_GetURB_State(pdev , MS_Machine.hc_num_out);
  
  switch(MS_TxParam.State)
  {
  case MS_IDLE:
     if( RingBuffer_GetSpaceUsed(&MS_TxBuf) )
     {
        //Fall through ti MS_SEND_DATA;
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
static void CDC_ProcessReception(USB_OTG_CORE_HANDLE *pdev, USBH_HOST *phost)
{
  
  if(RX_Enabled == 1)
  {
    URB_STATE URB_StatusRx =   HCD_GetURB_State(pdev , CDC_Machine.CDC_DataItf.hc_num_in);  
    
    switch(CDC_RxParam.CDCState)
    {
      
    case CDC_IDLE:
      
      
      /*check the received length lesser then the remaining space available in the 
      buffer */
      if(CDC_RxParam.DataLength < (CDC_RxParam.BufferLen - CDC_Machine.CDC_DataItf.length))
      {
        /*Receive the data */
        USBH_BulkReceiveData(pdev,
                             CDC_RxParam.pFillBuff,
                             CDC_Machine.CDC_DataItf.length, 
                             CDC_Machine.CDC_DataItf.hc_num_in);
        
        /*change the cdc state to USBH_CDC_GET_DATA*/
        CDC_RxParam.CDCState = CDC_GET_DATA;
      }
      break;
      
    case CDC_GET_DATA:
      /*Check the last state of the device is URB_DONE */
      if(URB_StatusRx == URB_DONE)
      {
        /* Move the pointer as well as datalength */
        CDC_RxParam.DataLength += pdev->host.hc[CDC_Machine.CDC_DataItf.hc_num_in].xfer_count ;
        CDC_RxParam.pFillBuff += pdev->host.hc[CDC_Machine.CDC_DataItf.hc_num_in].xfer_count ;
        
        
        /* Process the recived data */
        CDC_ReceiveData(&CDC_RxParam);
        
        /*change the state od the CDC state*/
        CDC_RxParam.CDCState = CDC_IDLE;
        
      }
      break;
    }
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
static void CDC_ReceiveData(CDC_Xfer_TypeDef *cdc_Data)
{
  uint8_t *ptr; 
  
  if(cdc_Data->pEmptyBuff < cdc_Data->pFillBuff)
  {
    ptr = cdc_Data->pFillBuff;
    *ptr = 0x00;
    
    /* redirect the received data on the user out put system */
    UserCb.Receive(cdc_Data->pEmptyBuff);
    
    cdc_Data->pFillBuff  = cdc_Data->pEmptyBuff ; 
    cdc_Data->DataLength = 0;    /*Reset the data length to zero*/
  }
}


void  MS_SendData(uint8_t *data, uint16_t length)
{
   RingBuffer_WriteBuffer(&MS_TxBuf, data, length);
}


/**
  * @brief  This function send data to the device.
  * @param  fileName : name of the file 
  * @retval the filestate will be returned 
  * FS_SUCCESS : returned to the parent function when the file length become to zero
  */
void  CDC_SendData(uint8_t *data, uint16_t length)
{
  
  if(CDC_TxParam.CDCState == CDC_IDLE)
  {
    CDC_TxParam.pRxTxBuff = data; 
    CDC_TxParam.DataLength = length;
    CDC_TxParam.CDCState = CDC_SEND_DATA;  
  }    
}

/**
  * @brief  This function send data to the device.
  * @param  fileName : name of the file 
  * @retval the filestate will be returned 
  * FS_SUCCESS : returned to the parent function when the file length become to zero
  */
void  CDC_StartReception( USB_OTG_CORE_HANDLE *pdev)
{
  RX_Enabled = 1;
}

/**
  * @brief  This function send data to the device.
  * @param  fileName : name of the file 
  * @retval the filestate will be returned 
  * FS_SUCCESS : returned to the parent function when the file length become to zero
  */
void  CDC_StopReception( USB_OTG_CORE_HANDLE *pdev)
{
  RX_Enabled = 0; 
  USB_OTG_HC_Halt(pdev, CDC_Machine.CDC_DataItf.hc_num_in);
  USBH_Free_Channel  (pdev,CDC_Machine.CDC_DataItf.hc_num_in);
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