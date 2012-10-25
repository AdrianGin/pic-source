/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               uctsk_MP3Decode.c
** Descriptions:            The MP3Decode application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-26
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

/* Includes ------------------------------------------------------------------*/
#include <includes.h>            

#include "app_cfg.h"
#include "intertaskComm.h"

/* Private variables ---------------------------------------------------------*/
//extern  GOL_MSG msg;               /* GOL message structure to interact with GOL */



/* Private function prototypes -----------------------------------------------*/
void uctsk_MP3Decode(void * pvArg);

/*******************************************************************************
* Function Name  : App_MP3DecodeTaskCreate
* Description    : 创建播放mp3文件任务
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void  App_MP3DecodeTaskCreate (void)
{
	xTaskCreate( uctsk_MP3Decode , ( signed char * ) "MP3Dec" , APP_TASK_MP3DECODE_STK_SIZE , NULL , APP_TASK_MP3DECODE_PRIO , &mp3DecodeHandle );
}


void uctsk_MP3Decode(void * pvArg)
{

  uint8_t   err;
  char**    RxedMessage;


  //Init Audio Channels here
  //WM8731_Init();

  //OSFlagPend( Sem_F,(OS_FLAGS) 1,OS_FLAG_WAIT_SET_ALL,0,&err );  /* 等待触摸屏校准完成 */
//
   	for(;;)
   	{
   	   printf("-- Waiting for Selection");
   	   err = QueueReceive(Queue_GUI_MP3_Message, &RxedMessage, QUEUE_BLOCK_WAIT);
   	   printf("-- Selection Made");
   	   vTaskDelay(1000/portTICK_RATE_MS);
   	   if( err == QUEUE_SUCCESS )
       {
	     printf("-- start decode music %s \r\n", RxedMessage);
         PlayAudioFile( &mp3FileObject , (char*)RxedMessage );
         QueueSend(Queue_GUI_MP3_Message, &RxedMessage, 0);
	   }
    }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
