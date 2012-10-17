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


/* Private variables ---------------------------------------------------------*/
extern  GOL_MSG msg;               /* GOL message structure to interact with GOL */
extern  OS_FLAG_GRP    *Sem_F;	   /* 事件标志 */
static  OS_STK    App_TaskMP3DecodeStk[APP_TASK_MP3DECODE_STK_SIZE];
OS_EVENT *mp3Mbox;				   /* 播放MP3邮箱 */

/* OS计数信号量 */	
OS_EVENT *DMAComplete; 
OS_EVENT *StopMP3Decode;

/* Private function prototypes -----------------------------------------------*/
static void uctsk_MP3Decode (void);


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
    CPU_INT08U  os_err;

	os_err = os_err; /* prevent warning... */

	os_err = OSTaskCreate((void (*)(void *)) uctsk_MP3Decode,				
                          (void          * ) 0,							
                          (OS_STK        * )&App_TaskMP3DecodeStk[APP_TASK_MP3DECODE_STK_SIZE - 1],		
                          (INT8U           ) APP_TASK_MP3DECODE_PRIO  );							

	#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_MP3DECODE_PRIO, "Task uctsk_MP3Decode", &os_err);
	#endif

    DMAComplete = OSSemCreate(0);

	StopMP3Decode = OSSemCreate(0);

    mp3Mbox = OSMboxCreate( (void *) 0);	/* 创建消息邮箱 */
}


static void uctsk_MP3Decode (void) {                 
 
  INT8U   err;
  char    *RxedMessage;

  WM8731_Init();
    
  OSFlagPend( Sem_F,(OS_FLAGS) 1,OS_FLAG_WAIT_SET_ALL,0,&err );  /* 等待触摸屏校准完成 */

   	for(;;)
   	{  
	   RxedMessage = (char*)OSMboxPend(mp3Mbox,0,&err);
       if( err == OS_ERR_NONE )
       {
	     printf("-- start decode music %s \r\n",RxedMessage);
         PlayAudioFile( &mp3FileObject , RxedMessage );	     
	   } 
    }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
