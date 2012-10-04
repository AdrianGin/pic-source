/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               includes.h
** Descriptions:            includes.h for ucos
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-9
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/


#ifndef  __INCLUDES_H__
#define  __INCLUDES_H__

/* Includes ------------------------------------------------------------------*/
#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <stdarg.h>
#include  <math.h>


#include  <stm32f10x.h>
#include  "Graphics.h"

#include "ff.h"	 
#include "GLCD.h"
#include "Audio.h"
#include "WM8731.h"
#include "sdio_sd.h" 
#include "MP3Header.h"
#include "TouchPanel.h"
#include "FlashDriver.h"
#include "SST25VF016B.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "Semaphore\\osa_semaphore.h"
#include "Queue\\osa_queue.h"

/* Private define ------------------------------------------------------------*/
#define   SOFTPACK_VERSION      "1.0"                                                 /* Version */
#define   BOARD_NAME            "HY-Smart Multi Media Development Board V1.0"	      /* Name of the board */

#define   MUSIC_DIRECTORY       "0:/Music"	               /* 音乐存放文件夹 */
#define   SYSTEM_FILE_HZLIB     "0:/SYSTEM/test.BIN"	   /* 系统文件夹 */
#define   HZLIB_ADDR            5						   /* 汉字库在SPI Flash存放位置 */


#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/






