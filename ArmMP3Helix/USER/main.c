/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The MP3 Player(helix) application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-21
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
#include "hw_config.h"


uint8_t bigReadBuffer[1000];

/* Private variables ---------------------------------------------------------*/
static  OS_STK         App_TaskStartStk[APP_TASK_START_STK_SIZE];
OS_FLAG_GRP            *Sem_F;	   /* 事件标志 */

/* Private function prototypes -----------------------------------------------*/
#if (OS_VIEW_MODULE == DEF_ENABLED)
extern  void  App_OSViewTaskCreate        (void);
#endif
static  void  TargetInit                  (void);
static  void  App_TaskCreate              (void);
static  void  App_TaskStart               (void *p_arg);
extern  void  App_MicrochipGUITaskCreate  (void);
extern  void  App_TouchScreenTaskCreate   (void);
extern  void  App_ReadButtonTaskCreate    (void);
extern  void  App_MP3DecodeTaskCreate     (void);

/*******************************************************************************
* Function Name  : main
* Description    : Main program
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
int main(void)
{
    CPU_INT08U  os_err;
	os_err = os_err; /* prevent warning... */

	/* Note:  由于使用UCOS, 在OS运行之前运行,注意别使能任何中断. */
	CPU_IntDis();                    /* Disable all ints until we are ready to accept them.  */

    OSInit();                        /* Initialize "uC/OS-II, The Real-Time Kernel".         */

	os_err = OSTaskCreateExt((void (*)(void *)) App_TaskStart,  /* Create the start task.                               */
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_START_PRIO,
                             (INT16U          ) APP_TASK_START_PRIO,
                             (OS_STK        * )&App_TaskStartStk[0],
                             (INT32U          ) APP_TASK_START_STK_SIZE,
                             (void          * )0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
	
#if OS_TASK_NAME_EN > 0
    OSTaskNameSet(APP_TASK_START_PRIO, (CPU_INT08U *)"Start Task", &os_err);
#endif

	OSStart();               /* Start multitasking (i.e. give control to uC/OS-II).  */

	return (0);
}

/*******************************************************************************
* Function Name  : App_TaskStart
* Description    : The startup task.The uC/OS-II ticker should only be initialize once multitasking starts.
* Input          : - p_arg: Argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static  void  App_TaskStart (void *p_arg)
{   
    INT8U   err;

	(void)p_arg;
			
    OS_CPU_SysTickInit();                                    /* Initialize the SysTick.                              */

#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                            /* Determine CPU capacity.                              */
#endif
	
	Sem_F =  OSFlagCreate( 0,&err );                         /* 建立一个事件标志组，0初始化                          */

    App_TaskCreate();                                        /* Create application tasks.                            */
	
	OSTimeDlyHMSM(0, 0, 1, 0);	                             /* 1 second                                             */ 

	TargetInit();

	for(;;)
   	{

      	OSTimeDlyHMSM(0, 1, 0, 0);							 /* Delay One minute */
    }	
}

/*******************************************************************************
* Function Name  : App_TaskCreate
* Description    : Create the application tasks.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static  void  App_TaskCreate (void)
{
#if (OS_VIEW_MODULE == DEF_ENABLED)
	App_OSViewTaskCreate();
#endif	
	
	App_ReadButtonTaskCreate();
//
	App_MicrochipGUITaskCreate();
//
	App_TouchScreenTaskCreate();
//
	App_MP3DecodeTaskCreate();
}

/*******************************************************************************
* Function Name  : TargetInit
* Description    : 初始化系统和外设 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void TargetInit(void)
{
  uint8_t HZLib[2];	INT8U   err;
  UINT nRead;



  //SPI_FLASH_Init();
  //SPI_FLASH_Test();
  GPIO_Configuration();
  NVIC_Configuration();
  USART_Configuration();  
  
  printf("-- Basic MP3 Project %s \r\n", SOFTPACK_VERSION);
  printf("-- %s --\r\n", BOARD_NAME); 
  printf("-- Compiled: %s %s \r\n", __DATE__, __TIME__); 

  /* 检测SD卡存在 */
  LCD_Clear(Black);

  if( SD_Detect() == SD_PRESENT )
  {
	 ;
  }
  else
  {
    printf("-- Please connect a SD card \r\n");
    GUI_Text(36,100,"Please insert SD card",White,Black);
    while(SD_Detect()!=SD_PRESENT);
    printf("-- SD card connection detected \r\n");
  }

  GUI_Text(44,120,"SD card detected OK",White,Black);
  //OSTimeDlyHMSM(0, 0, 0, 100);	 /* 1 second  */

  SearchMusic(MUSIC_DIRECTORY);

  //df_read_open(0);
  //df_read(HZLib,2);  
  /* 写入字库 */
  if( !( HZLib[0] == 0xaa && HZLib[1] == 0x55 ) )
  {   
    LCD_Clear(Black); 
    GUI_Text(60,100,"No Detect HZLib",White,Black);
	GUI_Text(68,120,"Writing HZLib",White,Black);
    //df_write_open( HZLIB_ADDR );   
	res = f_mount(0,&fs);   /* 挂载文件系统 */
	printf("resMnt=%d\n", res);
	//res = f_open(&mp3FileObject , "0:/Music/4416s.wav" , FA_OPEN_EXISTING | FA_READ);
	printf("resOpen=%d\n", res);
	//res = f_open(&mp3FileObject , SYSTEM_FILE_HZLIB , FA_OPEN_EXISTING | FA_READ);
//	for(;;)
//	{
//
//		res = f_read(&mp3FileObject,(uint8_t *)&bigReadBuffer[0],800, &nRead);
//		printf("resRead=%d\n", res);
//		res = f_lseek(&mp3FileObject, 500);
//		printf("resSeek=%d\n", res);
//	}

//    res = f_open(&mp3FileObject , SYSTEM_FILE_HZLIB , FA_OPEN_EXISTING | FA_READ);
//	if( res != 0 )
//	{
//	  GUI_Text(76,140,"HZLib Error",White,Black);
//	  while(1);
//	}
// 	SSTF016B_Erase( HZLIB_ADDR/SEC_SIZE , mp3FileObject.fsize/SEC_SIZE + 1 );	
//  	do
// 	{
//       res = f_read(&mp3FileObject,readBuf,READBUF_SIZE,&n_Read);
//       df_write(readBuf,n_Read);  
// 	}	  
//     while( n_Read == READBUF_SIZE && res == 0 );
// 	if( mp3FileObject.fptr != mp3FileObject.fsize )
// 	{
// 	  GUI_Text(76,140,"HZLib Error",White,Black);	 
// 	  while(1);
// 	}
	
	GUI_Text(88,140,"HZLib OK",White,Black);	 
	/* 字库存在标志 */
	HZLib[0] = 0xaa;  HZLib[1] = 0x55;
// 	df_write_open(0);   
//  	df_write(HZLib,2);
    OSTimeDlyHMSM(0, 0, 1, 0);	 /* 1 second  */ 
  }

  OSFlagPost(Sem_F, 2, OS_FLAG_SET, &err);	  /* TargetIni初始化完成 */ 
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

