/**
 ******************************************************************************
 * @file    usb_bsp.c
 * @author  MCD Application Team
 * @version V1.1.0
 * @date    19-March-2012
 * @brief   This file is responsible to offer board support package and is
 *          configurable by user.
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
#include "usb_bsp.h"
#include "usbd_conf.h"

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
 * @{
 */

/** @defgroup USB_BSP
 * @brief This file is responsible to offer board support package
 * @{
 */

/** @defgroup USB_BSP_Private_Defines
 * @{
 */
/**
 * @}
 */

/** @defgroup USB_BSP_Private_TypesDefinitions
 * @{
 */
/**
 * @}
 */

/** @defgroup USB_BSP_Private_Macros
 * @{
 */
/**
 * @}
 */

/** @defgroup USBH_BSP_Private_Variables
 * @{
 */

/**
 * @}
 */

/** @defgroup USBH_BSP_Private_FunctionPrototypes
 * @{
 */
/**
 * @}
 */

/** @defgroup USB_BSP_Private_Functions
 * @{
 */

/**
 * @brief  USB_OTG_BSP_Init
 *         Initilizes BSP configurations
 * @param  None
 * @retval None
 */

void USB_OTG_BSP_Init(USB_OTG_CORE_HANDLE *pdev)
{

   //USB Configure
   GPIO_InitTypeDef GPIO_InitStructure;
   GPIO_StructInit(&GPIO_InitStructure);

   RCC_AHB1PeriphClockLPModeCmd( RCC_AHB1Periph_OTG_HS_ULPI, DISABLE);
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_OTG_HS, ENABLE);
   RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);

   /*Set PA11,12 as IN - USB_DM,DP*/
   /* Configure SOF ID DM DP Pins */

   GPIO_InitStructure.GPIO_Pin = (USB_DN | USB_DP);
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_Init(USB_GPIO_PORT, &GPIO_InitStructure);

   GPIO_PinAFConfig(USB_GPIO_PORT, GPIO_PinSource14, GPIO_AF_OTG_HS_FS);
   GPIO_PinAFConfig(USB_GPIO_PORT, GPIO_PinSource15, GPIO_AF_OTG_HS_FS);



   GPIO_StructInit(&GPIO_InitStructure);

   RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS, ENABLE);
   RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE);

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;

   GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_Init(GPIOA, &GPIO_InitStructure);

   GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_OTG1_FS);
   GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_OTG1_FS);


}
/**
 * @brief  USB_OTG_BSP_EnableInterrupt
 *         Enabele USB Global interrupt
 * @param  None
 * @retval None
 */
void USB_OTG_BSP_EnableInterrupt(USB_OTG_CORE_HANDLE *pdev)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Enable and configure the priority of the USB_OTG IRQ Channel*/
    NVIC_InitStructure.NVIC_IRQChannel = OTG_HS_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 12;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 12;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);



}
/**
 * @brief  USB_OTG_BSP_uDelay
 *         This function provides delay time in micro sec
 * @param  usec : Value of delay required in micro sec
 * @retval None
 */
void USB_OTG_BSP_uDelay(const uint32_t usec)
{
    uint32_t count = 0;
    const uint32_t utime = ((SystemCoreClock / 1000000) * usec / 7);
    do
    {
        if (++count > utime)
        {
            return;
        }
    }
    while (1);
}

/**
 * @brief  USB_OTG_BSP_mDelay
 *          This function provides delay time in milli sec
 * @param  msec : Value of delay required in milli sec
 * @retval None
 */
void USB_OTG_BSP_mDelay(const uint32_t msec)
{
    USB_OTG_BSP_uDelay(msec * 1000);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
