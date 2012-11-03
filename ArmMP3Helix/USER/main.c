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

#include "app_cfg.h"
#include "intertaskComm.h"



/* Private variables ---------------------------------------------------------*/
//static  OS_STK         App_TaskStartStk[APP_TASK_START_STK_SIZE];
//OS_FLAG_GRP            *Sem_F;	   /* 事件标志 */

/* Private function prototypes -----------------------------------------------*/
static  void  TargetInit                  (void);
static  void  App_TaskCreate              (void);
static  void  App_TaskStart               (void *p_arg);
extern  void  App_MicrochipGUITaskCreate  (void);
extern  void  App_TouchScreenTaskCreate   (void);
extern  void  App_ReadButtonTaskCreate    (void);
extern  void  App_MP3DecodeTaskCreate     (void);


/* Private define ------------------------------------------------------------*/


xQueueHandle xQueue;

/* Private function prototypes -----------------------------------------------*/
static void prvSetupHardware( void );
void vLEDTask(void * pvArg);
void vLEDTask2(void * pvArg);

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

	prvSetupHardware();

	InitInterTaskComms();

//	GPIO_Configuration();
//	NVIC_Configuration();
//	USART_Configuration();
	//TargetInit();



	xQueue = xQueueCreate(1, 1);
	//App_TouchScreenTaskCreate();

	xTaskCreate( App_TaskStart , ( signed char * ) "AppCreator" , APP_TASK_START_STK_SIZE , NULL , APP_TASK_START_PRIO , NULL );
    /* Start the scheduler. */
  vTaskStartScheduler();
	return (0);
}


/*******************************************************************************
* Function Name  : vLEDTask
* Description    : LED Task
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void vLEDTask(void * pvArg)
{
	uint8_t pxRxedMessage;
	pxRxedMessage = 1;
  while(1)
  {
    /*====LED-ON=======*/
				//GPIO_SetBits(GPIOB , GPIO_Pin_12);
		//xQueueReceive( xQueue, &pxRxedMessage,  0);

				vTaskDelay(1000);
			/*====LED-OFF=======*/
			//GPIO_ResetBits(GPIOB , GPIO_Pin_12);
		//xQueueSendToBack( xQueue, &pxRxedMessage,  0);
				vTaskDelay(1000);

	}
}


void vLEDTask2(void * pvArg)
{
	uint8_t buffer[APP_TASK_COUNT*40];
size_t heapSize;

  while(1)
  {
//		 if( uxQueueMessagesWaiting( xQueue ) == 0 )
//        {
//    /*====LED-ON=======*/
//    //GPIO_SetBits(GPIOB , GPIO_Pin_14);
//    vTaskDelay(100);
//	/*====LED-OFF=======*/
//	//GPIO_ResetBits(GPIOB , GPIO_Pin_14);
//    vTaskDelay(100);
//				}
//		 else
		 {

			 vTaskList(buffer);
			 printf("%s", buffer);
			 vTaskDelay(20000/portTICK_RATE_MS);
//			 MP3_Data_Index = 1000;
//			 SeekValue = MP3_Data_Index;
			 heapSize = xPortGetFreeHeapSize();
			 printf("HeapFree=%d\n", heapSize);
		 }


  }
}


/*******************************************************************************
* Function Name  : App_TaskStart
* Description    : The startup task.The uC/OS-II ticker should only be initialize once multitasking starts.
* Input          : - p_arg: Argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void App_TaskStart(void * pvArg)
{   
//    INT8U   err;
//
//	(void)p_arg;
//
//    OS_CPU_SysTickInit();                                    /* Initialize the SysTick.                              */
//
//#if (OS_TASK_STAT_EN > 0)
//    OSStatInit();                                            /* Determine CPU capacity.                              */
//#endif
//
//	Sem_F =  OSFlagCreate( 0,&err );                         /* 建立一个事件标志组，0初始化                          */
//
    App_TaskCreate();                                        /* Create application tasks.                            */
//
//	OSTimeDlyHMSM(0, 0, 1, 0);	                             /* 1 second                                             */
//
	TargetInit();
//
    //
	for(;;)
   	{

		vTaskDelay(10000);							 /* Delay One minute */
		vTaskDelete( NULL );
    }
}


void uctsk_MP3Decode(void * pvArg);

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
	

	uint8_t err;
//	xTaskCreate( vLEDTask , ( signed char * ) "LEDTask1" , APP_TASK_LED_STK_SIZE , NULL , APP_TASK_LED_PRIO , NULL );
	xTaskCreate( vLEDTask2 , ( signed char * ) "LEDTask2" , APP_TASK_LED2_STK_SIZE , NULL , APP_TASK_LED2_PRIO , NULL );

	App_ReadButtonTaskCreate();
//
	App_MicrochipGUITaskCreate();
//
	App_TouchScreenTaskCreate();
//
	App_MP3DecodeTaskCreate();
}


/*******************************************************************************
* Function Name  : prvSetupHardware
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void prvSetupHardware( void )
{

  GPIO_Configuration();
  NVIC_Configuration();
  USART_Configuration();
  LCD_BackLight_Init();
  //SystemInit();
  /* Configure HCLK clock as SysTick clock source. */
  SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );
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
  uint8_t HZLib[2];	uint8_t   err;

  //SPI_FLASH_Init();
  //SPI_FLASH_Test();
  printf("-- Basic MP3 Project %s \r\n", SOFTPACK_VERSION);
  printf("-- %s --\r\n", BOARD_NAME); 
  printf("-- Compiled: %s %s \r\n", __DATE__, __TIME__); 

  //Wait until all other Init tasks have completed.
	while( SemaphoreTake(Sem_GUIRdy, 10) == pdFAIL)
	{
		printf("Waiting");
		vTaskDelay(1000/portTICK_RATE_MS);
	}

	printf("TargetInit\n");
  /* Clear screen */
  LCD_Clear(Black);

  err = SD_Detect();
  if(err  == SD_PRESENT )
  {
  }
  else
  {
    printf("-- Please connect a SD card \r\n");
    GUI_Text(36,100,"Please insert SD card",White,Black);
    while(SD_Detect()!=SD_PRESENT)
    {
    	vTaskDelay(2000/portTICK_RATE_MS);
    	//GUI_Text(36,100,"Please insert SD card",White,Black);

    }
    printf("-- SD card connection detected \r\n");
  }

  GUI_Text(44,120,"SD card detected OK",White,Black);
  vTaskDelay(2000/portTICK_RATE_MS);
  //  OSTimeDlyHMSM(0, 0, 1, 0);	 /* 1 second  */
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
	f_mount(0,&fs);   /* 挂载文件系统 */
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
//    OSTimeDlyHMSM(0, 0, 1, 0);	 /* 1 second  */
	vTaskDelay(1000/portTICK_RATE_MS);
  }


  SemaphoreGive(Sem_TouchPanelRdy);
//  OSFlagPost(Sem_F, 2, OS_FLAG_SET, &err);	  /* TargetIni初始化完成 */
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

	printf("AssertFailed %s, %d\n", file, line);
  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

