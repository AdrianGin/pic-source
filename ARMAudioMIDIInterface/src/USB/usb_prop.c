/**
  ******************************************************************************
  * @file    usb_prop.c
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    29-June-2012
  * @brief   All processing related to Audio Speaker Demo
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
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t MUTE_DATA = 0;

DEVICE Device_Table =
  {
    EP_NUM,
    1
  };


DEVICE_PROP Device_Property =
  {
    Speaker_init,
    Speaker_Reset,
    Speaker_Status_In,
    Speaker_Status_Out,
    Speaker_Data_Setup,
    Speaker_NoData_Setup,
    Speaker_Get_Interface_Setting,
    Speaker_GetDeviceDescriptor,
    Speaker_GetConfigDescriptor,
    Speaker_GetStringDescriptor,
    0,
    END0_PACKET_SIZE /*MAX PACKET SIZE*/ // Must match the descriptor
  };

USER_STANDARD_REQUESTS User_Standard_Requests =
  {
    Speaker_GetConfiguration,
    Speaker_SetConfiguration,
    Speaker_GetInterface,
    Speaker_SetInterface,
    Speaker_GetStatus,
    Speaker_ClearFeature,
    Speaker_SetEndPointFeature,
    Speaker_SetDeviceFeature,
    Speaker_SetDeviceAddress
  };

ONE_DESCRIPTOR Device_Descriptor =
  {
    (uint8_t*)Speaker_DeviceDescriptor,
    SPEAKER_SIZ_DEVICE_DESC
  };

ONE_DESCRIPTOR Config_Descriptor =
  {
    (uint8_t*)Speaker_ConfigDescriptor,
    COMPOSITE_SIZE_CONFIG_DESC
  };

ONE_DESCRIPTOR String_Descriptor[] =
  {
    {(uint8_t*)Speaker_StringLangID, SPEAKER_SIZ_STRING_LANGID},
    {(uint8_t*)Speaker_StringVendor, SPEAKER_SIZ_STRING_VENDOR},
    {(uint8_t*)Speaker_StringProduct, SPEAKER_SIZ_STRING_PRODUCT},
    {(uint8_t*)Speaker_StringSerial, SPEAKER_SIZ_STRING_SERIAL},
  };


/* LangID = 0x0409: U.S. English */
const uint8_t USBDesc_LangID[] = {0x09, 0x04};
const uint8_t USBDesc_StringVendor[] = "Creative Instruments Team";
const uint8_t USBDesc_StringProduct[] = "BEST MIDI";
const uint8_t USBDesc_StringSerial[] = "MIDI Piano";
const uint8_t USBDesc_StringProduct2[] = "BEST Audio";

const uint8_t* USBDesciptors[] = {	USBDesc_LangID,
									USBDesc_StringVendor,
									USBDesc_StringProduct,
									USBDesc_StringSerial,
									USBDesc_StringProduct2};



/* Extern variables ----------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Speaker_init.
* Description    : Speaker init routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Speaker_init()
{
  /* Update the serial number string descriptor with the data from the unique
  ID*/
  Get_SerialNum();

  /* Initialize the current configuration */
  pInformation->Current_Configuration = 0;

  /* Connect the device */
  PowerOn();

  /* Perform basic device initialization operations */
  USB_SIL_Init();

  bDeviceState = UNCONNECTED;
}


/*******************************************************************************
* Function Name  : Speaker_Reset.
* Description    : Speaker reset routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Speaker_Reset()
{
  /* Set Speaker device as not configured state */
  pInformation->Current_Configuration = 0;

  /* Current Feature initialization */
  pInformation->Current_Feature = Speaker_ConfigDescriptor[7];

  SetBTABLE(BTABLE_ADDRESS);

  /* Initialize Endpoint 0 */
  SetEPType(ENDP0, EP_CONTROL);
  SetEPTxStatus(ENDP0, EP_TX_NAK);
  SetEPRxAddr(ENDP0, ENDP0_RXADDR);
  SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
  SetEPTxAddr(ENDP0, ENDP0_TXADDR);
  Clear_Status_Out(ENDP0);
  SetEPRxValid(ENDP0);

  /* Initialize Endpoint 1 */
  SetEPType(ENDP1, EP_ISOCHRONOUS);
  SetEPDblBuffAddr(ENDP1, ENDP1_BUF0Addr, ENDP1_BUF1Addr);
  SetEPDblBuffCount(ENDP1, EP_DBUF_OUT, USB_ENDP_MAX_PACKET_SIZE);
  ClearDTOG_RX(ENDP1);
  ClearDTOG_TX(ENDP1);
  ToggleDTOG_TX(ENDP1);
  SetEPRxStatus(ENDP1, EP_RX_VALID);
  SetEPTxStatus(ENDP1, EP_TX_DIS);

  SetEPRxValid(ENDP0);


  SetEPType(ENDP2, EP_BULK);
  SetEPTxAddr(ENDP2, ENDP2_BUF1Addr);
  SetEPTxCount(ENDP2, 0x40);
  SetEPTxStatus(ENDP2, EP_TX_NAK);

  SetEPRxAddr(ENDP2, ENDP2_BUF0Addr);
  SetEPRxCount(ENDP2, 0x40);
  SetEPRxStatus(ENDP2, EP_RX_VALID);



  /* Set this device to response on default address */
  SetDeviceAddress(0);

  bDeviceState = ATTACHED;

}
/*******************************************************************************
* Function Name  : Speaker_SetConfiguration.
* Description    : Update the device state to configured.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Speaker_SetConfiguration(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
}
/*******************************************************************************
* Function Name  : Speaker_SetConfiguration.
* Description    : Update the device state to addressed.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Speaker_SetDeviceAddress (void)
{
  bDeviceState = ADDRESSED;
}
/*******************************************************************************
* Function Name  : Speaker_Status_In.
* Description    : Speaker Status In routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Speaker_Status_In(void)
{}

/*******************************************************************************
* Function Name  : Speaker_Status_Out.
* Description    : Speaker Status Out routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Speaker_Status_Out (void)
{}

/*******************************************************************************
* Function Name  : Speaker_Data_Setup
* Description    : Handle the data class specific requests.
* Input          : None.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
RESULT Speaker_Data_Setup(uint8_t RequestNo)
{
  uint8_t *(*CopyRoutine)(uint16_t);
  CopyRoutine = NULL;

  if ((RequestNo == GET_CUR) || (RequestNo == SET_CUR))
  {
    CopyRoutine = Mute_Command;
  }

  else
  {
    return USB_UNSUPPORT;
  }

  pInformation->Ctrl_Info.CopyData = CopyRoutine;
  pInformation->Ctrl_Info.Usb_wOffset = 0;
  (*CopyRoutine)(0);
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Speaker_NoData_Setup
* Description    : Handle the no data class specific requests.
* Input          : None.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
RESULT Speaker_NoData_Setup(uint8_t RequestNo)
{
  return USB_UNSUPPORT;
}

/*******************************************************************************
* Function Name  : Speaker_GetDeviceDescriptor.
* Description    : Get the device descriptor.
* Input          : Length : uint16_t.
* Output         : None.
* Return         : The address of the device descriptor.
*******************************************************************************/
uint8_t *Speaker_GetDeviceDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Device_Descriptor);
}

/*******************************************************************************
* Function Name  : Speaker_GetConfigDescriptor.
* Description    : Get the configuration descriptor.
* Input          : Length : uint16_t.
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
uint8_t *Speaker_GetConfigDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Config_Descriptor);
}

/*******************************************************************************
* Function Name  : Speaker_GetStringDescriptor.
* Description    : Get the string descriptors according to the needed index.
* Input          : Length : uint16_t.
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
uint8_t *Speaker_GetStringDescriptor(uint16_t Length)
{
  uint8_t wValue0 = pInformation->USBwValue0;
	uint8_t buffer[64];
	uint8_t i, len;
	ONE_DESCRIPTOR desc;
	//Buffer used to create the buffer string descriptor.

  if (wValue0 > 4)
  {
    return NULL;
  }
  else
  {

		switch(wValue0)
		{
			case LANGID_STRINGID:
			{
					buffer[2] = USBDesciptors[wValue0][0];
					buffer[3] = USBDesciptors[wValue0][1];
				  len = 4;
					break;
			}

			default:
			{
					for(i=0, len=2; USBDesciptors[wValue0][i] != 0; i++)
					{
							buffer[len++] = USBDesciptors[wValue0][i];
							buffer[len++] = 0;
					}
					break;
			}
		}

		buffer[0] = len;
		buffer[1] = USB_STRING_DESCRIPTOR_TYPE;
		desc.Descriptor = &buffer[0];
	  desc.Descriptor_Size = len;

    return Standard_GetDescriptorData(Length, &desc);
  }
}


/*******************************************************************************
* Function Name  : Speaker_Get_Interface_Setting.
* Description    : test the interface and the alternate setting according to the
*                  supported one.
* Input1         : uint8_t: Interface : interface number.
* Input2         : uint8_t: AlternateSetting : Alternate Setting number.
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
RESULT Speaker_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting)
{
  if (AlternateSetting > 1)
  {
    return USB_UNSUPPORT;
  }
  //else if (Interface > 1)
  else if (Interface > 4)
  {
    return USB_UNSUPPORT;
  }
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Mute_Command
* Description    : Handle the GET MUTE and SET MUTE command.
* Input          : Length : uint16_t.
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
uint8_t *Mute_Command(uint16_t Length)
{

  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = pInformation->USBwLengths.w;
    return NULL;
  }
  else
  {
    return((uint8_t*)(&MUTE_DATA));
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

