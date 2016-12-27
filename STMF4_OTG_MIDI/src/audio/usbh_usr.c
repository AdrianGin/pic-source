/**
  ******************************************************************************
  * @file    usbh_usr.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    09-November-2015
  * @brief   This file includes the usb host library user callbacks
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
#include "usbh_usr.h"

#include "usbh_audio_core.h"

/** @addtogroup USBH_USER
* @{
*/

/** @addtogroup USBH_CDC_DEMO_USER_CALLBACKS
* @{
*/

/** @defgroup USBH_USR
* @brief This file includes the usb host library user callbacks
* @{
*/ 


/** @defgroup USBH_CORE_Exported_TypesDefinitions
* @{
*/ 

/**
* @}
*/ 

/** @addtogroup USER
* @{
*/

/** @defgroup USBH_USR 
* @brief    This file includes the user application layer
* @{
*/ 

/** @defgroup USBH_CORE_Exported_Types
* @{
*/ 



/** @defgroup USBH_USR_Private_Defines
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBH_USR_Private_Macros
* @{
*/ 
/**
* @}
*/ 

/** @defgroup USBH_USR_Private_Variables
* @{
*/
extern USB_OTG_CORE_HANDLE           USB_OTG_Core;
extern USBH_HOST                     USB_Host; 
extern CDC_LineCodingTypeDef         CDC_GetLineCode;
extern CDC_LineCodingTypeDef         CDC_SetLineCode;
extern CDC_Usercb_TypeDef            UserCb;

uint8_t                              prev_select = 0;
uint8_t                              enable_display_received_data = 0;
CDC_DEMO_StateMachine                cdc_demo;
CDC_DEMO_SETTING_StateMachine        cdc_settings_state;
CDC_DEMO_SelectMode                  cdc_select_mode;

static uint32_t i = 0;
#if defined(USE_STM324x9I_EVAL)
static uint16_t refcolumn = 479;
#else
static uint16_t refcolumn = 320;
#endif

static uint8_t  line = 48;

static uint8_t                     Prev_BaudRateIdx = 0;
static uint8_t                     Prev_DataBitsIdx = 0;
static uint8_t                     Prev_ParityIdx = 0;
static uint8_t                     Prev_StopBitsIdx = 0;
static uint8_t                     Prev_Select = 0;  

uint8_t * CDC_main_menu[] = 
{
	(uint8_t*)"      1 - Send Data                                                          ",
	(uint8_t*)"      2 - Receive Data                                                       ",
	(uint8_t*)"      3 - Configuration                                                      ",
};

uint8_t * DEMO_SEND_menu[] = 
{
	(uint8_t*)"      1 - Send File 1                                                        ",
	(uint8_t*)"      2 - Send File 2                                                        ",
	(uint8_t*)"      3 - Send File 3                                                        ",
	(uint8_t*)"      4 - Return                                                             ",
};

uint8_t * DEMO_RECEIVE_menu[] = 
{
	(uint8_t*)"      1 - Return                                                             ",
	(uint8_t*)"                                                                             ",
	(uint8_t*)"                                                                             ",
};

uint8_t * DEMO_CONFIGURATION_menu[] = 
{
	(uint8_t*)"      1 - Apply new settings                                                 ",
	(uint8_t*)"      2 - Return                                                             ",
	(uint8_t*)"                                                                             ",
};

const uint8_t MSG_BITS_PER_SECOND[] = "               : Bit Per Second                          ";
const uint8_t MSG_DATA_BITS[]       = "               : Data width                              ";
const uint8_t MSG_PARITY[]          = "               : Parity                                  ";
const uint8_t MSG_STOP_BITS[]       = "               : Stop Bits                               ";

const uint32_t  BaudRateValue[NB_BAUD_RATES]  = 
{ 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600 };

const uint8_t DataBitsValue[4]  = { 5, 6, 7, 8};
const uint8_t ParityArray[3][5]    = {"NONE", "EVEN", "ODD"};
const uint8_t StopBitsArray[2][2]  = {"1", "2"};

/*  Points to the DEVICE_PROP structure of current device */
/*  The purpose of this register is to speed up the execution */

USBH_Usr_cb_TypeDef USR_Callbacks =
{
  USBH_USR_Init,
  USBH_USR_DeInit,
  USBH_USR_DeviceAttached,
  USBH_USR_ResetDevice,
  USBH_USR_DeviceDisconnected,
  USBH_USR_OverCurrentDetected,
  USBH_USR_DeviceSpeedDetected,
  USBH_USR_Device_DescAvailable,
  USBH_USR_DeviceAddressAssigned,
  USBH_USR_Configuration_DescAvailable,
  USBH_USR_Manufacturer_String,
  USBH_USR_Product_String,
  USBH_USR_SerialNum_String,
  USBH_USR_EnumerationDone,
  USBH_USR_UserInput,
  USBH_USR_Application,
  USBH_USR_DeviceNotSupported,
  USBH_USR_UnrecoveredError
};

/**
* @}
*/

/** @defgroup USBH_USR_Private_Constants
* @{
*/ 
/*--------------- LCD Messages ---------------*/
const uint8_t MSG_HOST_INIT[]          = "> Host Library Initialized\n";
const uint8_t MSG_DEV_ATTACHED[]       = "> Device Attached\n";
const uint8_t MSG_DEV_DISCONNECTED[]   = "> Device Disconnected\n";
const uint8_t MSG_DEV_ENUMERATED[]     = "> Enumeration completed\n";
const uint8_t MSG_DEV_HIGHSPEED[]      = "> High speed device detected\n";
const uint8_t MSG_DEV_FULLSPEED[]      = "> Full speed device detected\n";
const uint8_t MSG_DEV_LOWSPEED[]       = "> Low speed device detected\n";
const uint8_t MSG_DEV_ERROR[]          = "> Device fault\n";
const uint8_t MSG_CDC_CLASS[]          = "> CDC device connected\n";
const uint8_t MSG_UNREC_ERROR[]        = "> Unrecovered error\n";
/**
* @}
*/



/** @defgroup USBH_USR_Private_FunctionPrototypes
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBH_USR_Private_Functions
* @{
*/ 





/**
* @brief  USBH_USR_Init 
*         Displays the message on LCD for host lib initialization
* @param  None
* @retval None
*/
void USBH_USR_Init(void)
{

}

/**
* @brief  USBH_USR_DeviceAttached 
*         Displays the message on LCD on device attached
* @param  None
* @retval None
*/
void USBH_USR_DeviceAttached(void)
{
  

}


/**
* @brief  USBH_USR_UnrecoveredError
* @param  None
* @retval None
*/
void USBH_USR_UnrecoveredError (void)
{

}


/**
* @brief  USBH_DisconnectEvent
*         Device disconnect event
* @param  None
* @retval None
*/
void USBH_USR_DeviceDisconnected (void)
{
  

}

/**
* @brief  USBH_USR_ResetUSBDevice 
*         Reset USB Device
* @param  None
* @retval None
*/
void USBH_USR_ResetDevice(void)
{
  /* Users can do their application actions here for the USB-Reset */
}


/**
* @brief  USBH_USR_DeviceSpeedDetected 
*         Displays the message on LCD for device speed
* @param  Devicespeed : Device Speed
* @retval None
*/
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{

}

/**
* @brief  USBH_USR_Device_DescAvailable 
*         Displays the message on LCD for device descriptor
* @param  DeviceDesc : device descriptor
* @retval None
*/
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{

}

/**
* @brief  USBH_USR_DeviceAddressAssigned 
*         USB device is successfully assigned the Address 
* @param  None
* @retval None
*/
void USBH_USR_DeviceAddressAssigned(void)
{
  
}


/**
* @brief  USBH_USR_Conf_Desc 
*         Displays the message on LCD for configuration descriptor
* @param  ConfDesc : Configuration descriptor
* @retval None
*/
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
{

}

/**
* @brief  USBH_USR_Manufacturer_String 
*         Displays the message on LCD for Manufacturer String 
* @param  ManufacturerString : Manufacturer String of Device
* @retval None
*/
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{

   volatile static void* test;

   test = ManufacturerString;

   return;
}

/**
* @brief  USBH_USR_Product_String 
*         Displays the message on LCD for Product String
* @param  ProductString : Product String of Device
* @retval None
*/
void USBH_USR_Product_String(void *ProductString)
{
   volatile static void* test;

   test = ProductString;

   return;
}

/**
* @brief  USBH_USR_SerialNum_String 
*         Displays the message on LCD for SerialNum_String 
* @param  SerialNumString : SerialNum_String of device
* @retval None
*/
void USBH_USR_SerialNum_String(void *SerialNumString)
{

} 

/**
* @brief  EnumerationDone 
*         User response request is displayed to ask for
*         application jump to class
* @param  None
* @retval None
*/
void USBH_USR_EnumerationDone(void)
{

} 

/**
* @brief  USBH_USR_DeviceNotSupported
*         Device is not supported
* @param  None
* @retval None
*/
void USBH_USR_DeviceNotSupported(void)
{

  
}  


/**
* @brief  USBH_USR_UserInput
*         User Action for application state entry
* @param  None
* @retval USBH_USR_Status : User response for key button
*/
USBH_USR_Status USBH_USR_UserInput(void)
{

} 

/**
* @brief  USBH_USR_OverCurrentDetected
*         Device Overcurrent detection event
* @param  None
* @retval None
*/
void USBH_USR_OverCurrentDetected (void)
{
  
}

/**
* @brief  USBH_USR_DeInit
*         Deint User state and associated variables
* @param  None
* @retval None
*/
void USBH_USR_DeInit(void)
{
}

/**
* @brief  USBH_USR_Application
*         CDC Main application
* @param  None
* @retval None
*/
int USBH_USR_Application(void)
{
  return 0;
}


/**
* @}h
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
