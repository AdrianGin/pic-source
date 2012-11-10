/****************************************Copyright (c)**************************************************                         
**
**                                 http://www.powermcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			WM8731.h
** Descriptions:		WM8731Í·ÎÄ¼þ
**
**------------------------------------------------------------------------------------------------------
** Created by:			AVRman
** Created date:		2010-11-17
** Version:				1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:	
** Version:
** Descriptions:		
********************************************************************************************************/
#ifndef __WM8731_H
#define __WM8731_H

/* Includes ------------------------------------------------------------------*/
//#include "stm32f10x.h"
#include <stdint.h>

#define ADDR_WM8731S    0x34

/* Private function prototypes -----------------------------------------------*/
void WM8731_Init(void);
void I2C_Configuration(void);
void WM8731_Vol(uint8_t vol);
void codec_send(uint16_t s_data);

/* Private define ------------------------------------------------------------*/

/* Register Definitions */
#define LEFT_LINE_IN                    ((uint16_t)0)<<9
#define RIGHT_LINE_IN                   ((uint16_t)1)<<9
#define LEFT_HEADPHONE_OUT              ((uint16_t)2)<<9
#define RIGHT_HEADPHONE_OUT             ((uint16_t)3)<<9
#define ANALOGUE_AUDIO_PATH_CONTROL     ((uint16_t)4)<<9
#define DIGITAL_AUDIO_PATH_CONTRL       ((uint16_t)5)<<9
#define POWER_DOWN_COTROL               ((uint16_t)6)<<9
#define DIGITAL_AUDIO_INTERFACE_FORMAT  ((uint16_t)7)<<9
#define SAMPLING_CONTROL                ((uint16_t)8)<<9
#define ACTIVE_CONTROL                  ((uint16_t)9)<<9
#define RESET_REGISTER                  ((uint16_t)15)<<9


/* R02 LEFT_HEADPHONE_OUT */
#define RHPVOL_MASK                     (127)   // [6:0]
#define LZCEN                           (1<<7)
#define LRHPBOTH                        (1<<8)

/* R03 RIGHT_HEADPHONE_OUT */
#define LHPVOL_MASK                     (127)   // [6:0]
#define LZCEN                           (1<<7)
#define LRHPBOTH                        (1<<8)

/* R04 ANALOGUE_AUDIO_PATH_CONTROL */
#define MICBOOST                        (1)
#define MUTEMIC                         (1<<1)
#define INSEL_LINE_IN                   (0<<1)
#define INSEL_MIC                       (1<<2)
#define BYPASS                          (1<<3)
#define DAC_SEL                         (1<<4)
#define SIDETONE                        (1<<5)
#define SIDEATT_N15DB                   (3<<6)
#define SIDEATT_N12DB                   (2<<6)
#define SIDEATT_N9DB                    (1<<6)
#define SIDEATT_N6DB                    (0<<6)

/* R07 DIGITAL_AUDIO_INTERFACE_FORMAT */
#define FMT_RIGHT_JUSTIFIED             (0)
#define FMT_LEFT_JUSTIFIED              (1)
#define FMT_I2S                         (3)
#define FMT_PCM                         (4)
#define IWL_16BITS                      (0<<2)
#define IWL_20BITS                      (1<<2)
#define IWL_24BITS                      (2<<2)
#define IWL_32BITS                      (3<<2)
#define LRP                             (1<<4)
#define LRSWAP                          (1<<5)
#define MASTER_MODE                     (1<<6)
#define SLAVE_MODE                      (0<<6)
#define BCLK_INVERTED                   (1<<7)

/* R08 SAMPLING_CONTROL	*/
#define NORMAL_MODE                     (0<<0)
#define USB_MODE                        (1<<0)
#define BOSR_USB_250FS                  (0<<1)
#define BOSR_USB_272FS                  (1<<1)
#define BOSR_NORMAL_256FS               (0<<1)
#define BOSR_NORMAL_384FS               (1<<1)
#define CLKIDIV2                        (1<<6)
#define CLKODIV2                        (1<<7)

/* R08 ACTIVE_CONTROL */
#define ACTIVE                          (1)
#define INACTIVE                        (0)

#endif 

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
