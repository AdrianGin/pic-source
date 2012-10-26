/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               uctsk_ReadButton.c
** Descriptions:            The uctsk_ReadButton application function
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
#include <menu.h>

#include "intertaskComm.h"

/* Private variables ---------------------------------------------------------*/
/* 播放MP3邮箱 */
/* MP3播放状态 */
extern AUDIO_Playback_status_enum AUDIO_Playback_status ;
extern  GOL_MSG msg;        /* GOL message structure to interact with GOL */

extern uint8_t Audio_Type;

/* Private function prototypes -----------------------------------------------*/
static void uctsk_ReadButton   (void);
static void GPIO_Configuration (void);
static void ADC_Configuration  (void);
static void GUI_ReadButton     (void);


/*******************************************************************************
* Function Name  : App_ReadButtonTaskCreate
* Description    : 创建读开发板按键任务
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void  App_ReadButtonTaskCreate (void)
{
	//xTaskCreate( uctsk_ReadButton , ( signed char * ) "ReadButton" , APP_TASK_READBUTTON_STK_SIZE , NULL , APP_TASK_READBUTTON_PRIO , NULL );

}

static void uctsk_ReadButton (void) {  
  uint8_t   err;
  float AD_value;  
  GPIO_Configuration();
  ADC_Configuration();
  LCD_BackLight_Init();

   for(;;)
   {   
//	  if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==SET)
//	  {
//         AD_value=ADC_GetConversionValue(ADC1);
//         AD_value = (AD_value/4096)*110;
//		 LCD_BackLight(AD_value);
//	  }

	  GUI_ReadButton();

	  vTaskDelay(20/portTICK_RATE_MS);
   }
}

/*******************************************************************************
* Function Name  : GUI_ReadButton
* Description    : synchronization GUI
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void GUI_ReadButton (void)
{  
   LISTBOX  *pLb;
   SLIDER   *pSld;

   pLb =(LISTBOX*)GOLFindObject(ID_LISTBOX1);

   if( !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2) )	   /* JOY_LEFT is press */
   {
	   vTaskDelay(50/portTICK_RATE_MS);                      /* 按键防抖动        */
	  if( !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2) )  /* 列表框箭头下      */
	  {
	      pSld =(SLIDER*)GOLFindObject(ID_SLIDER3);	

		  LbSetFocusedItem(pLb,LbGetFocusedItem(pLb)+1);
          SetState(pLb, LB_DRAW_ITEMS);

          SldSetPos(pSld,SldGetPos(pSld)-1);
          SetState(pSld, SLD_DRAW_THUMB);
	  }
   }

   if( !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) )	   /* JOY_RIGHT is press */
   {
	   vTaskDelay(50/portTICK_RATE_MS);                      /* 按键防抖动         */
	  if( !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) )  /* 列表框箭头上      */
	  {
		     pSld =(SLIDER*)GOLFindObject(ID_SLIDER3); 

	         LbSetFocusedItem(pLb,LbGetFocusedItem(pLb)-1);
    	     SetState(pLb, LB_DRAW_ITEMS);

             SldSetPos(pSld,SldGetPos(pSld)+1);
             SetState(pSld, SLD_DRAW_THUMB);
	  }
   }

   if( !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1) )	   /* JOY_UP is press    */
   {
	   vTaskDelay(50/portTICK_RATE_MS);                      /* 按键防抖动         */
	  if( !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1) )  /* 快退               */
	  {
	      if (LbGetSel(pLb,NULL) != NULL && !( outOfData == 1 ))	 /* 列表框没有选项 */
		  {
	         pSld = (SLIDER*)GOLFindObject(ID_SLIDER2);

		     SldSetPos(pSld,SldGetPos(pSld)-2);

		     if( Audio_Type == MP3_FILE )
		     {
	         /* 跳转READBUF_SIZE个整数倍*/
		    	 MP3_Data_Index = ( (uint32_t)( ( (float)SldGetPos(pSld)/200 ) * ( mp3FileObject.fsize - mp3_info.data_start) ) - \
		    			 	 	  ( (uint32_t)( ( (float)SldGetPos(pSld)/200 ) * ( mp3FileObject.fsize - mp3_info.data_start) ) % READBUF_SIZE ) );
		     }
		     else
		     {
		    	 MP3_Data_Index = ( (uint32_t)( ( (float)SldGetPos(pSld)/200 ) * ( mp3FileObject.fsize)));
		     }

		     SeekValue = mp3_info.data_start + MP3_Data_Index;
		     //res = f_lseek(&mp3FileObject, mp3_info.data_start + MP3_Data_Index );
		     //printf("lseek=%d", mp3_info.data_start + MP3_Data_Index);
		     SetState(pSld,SLD_DRAW_THUMB); 
		  }
	  }
   }

   if( !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3) )	   /* JOY_DOWN is press  */
   {
	   vTaskDelay(50/portTICK_RATE_MS);                      /* 按键防抖动         */
	  if( !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3) )  /* 快进               */
	  {
	      if (LbGetSel(pLb,NULL) != NULL && !( outOfData == 1 ))	 /* 列表框没有选项 */
	      {
	         pSld = (SLIDER*)GOLFindObject(ID_SLIDER2);

		     SldSetPos(pSld,SldGetPos(pSld)+2);


		     if( Audio_Type == MP3_FILE )
		     {
	         /* 跳转READBUF_SIZE个整数倍*/
		    	 MP3_Data_Index = ( (uint32_t)( ( (float)SldGetPos(pSld)/200 ) * ( mp3FileObject.fsize - mp3_info.data_start) ) - \
		    			 	 	  ( (uint32_t)( ( (float)SldGetPos(pSld)/200 ) * ( mp3FileObject.fsize - mp3_info.data_start) ) % READBUF_SIZE ) );
		     }
		     else
		     {
		    	 MP3_Data_Index = ( (uint32_t)( ( (float)SldGetPos(pSld)/200 ) * ( mp3FileObject.fsize)));
		     }

		     SeekValue = mp3_info.data_start + MP3_Data_Index;
		     //res = f_lseek(&mp3FileObject, mp3_info.data_start + MP3_Data_Index );
		     //printf("lseek=%d", mp3_info.data_start + MP3_Data_Index);
		     SetState(pSld,SLD_DRAW_THUMB); 
		  }
	  }
   }

   if( !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) )	   /* JOY_SEL is press   */
   {
	   vTaskDelay(50/portTICK_RATE_MS);                     /* 按键防抖动         */
	  if( !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) )  /* 列表框focus确定    */
	  {
		  //msg->type =
		  msg.uiEvent = EVENT_KEYSCAN;
		  msg.type    = TYPE_KEYBOARD;
		  msg.param1 = pLb->hdr.ID;
		  msg.param2 = SCAN_CR_PRESSED;
		  //pSld =(SLIDER*)GOLFindObject(ID_SLIDER3);

		  //LbSetFocusedItem(pLb,0);
		  //SetState(pLb, LB_DRAW_ITEMS);

		  //SldSetPos(pSld,SldGetPos(pSld)-1);
		  //SetState(pSld, SLD_DRAW_THUMB);

	  }
   }
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA , ENABLE);
  /**
  *  JOY_UP -> PC1 , JOY_DOWN -> PC3 , JOY_LEFT -> PC2 , JOY_RIGHT -> PA0 , JOY_SEL -> PA1
  *  KeyA -> PC13 , KeyB -> PB2
  */				
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure PB.02 (ADC Channel8) as analog input -------------------------*/
//   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//   GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : ADC_Configuration
* Description    : Configure the ADC.
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void ADC_Configuration(void)
{
  ADC_InitTypeDef ADC_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_AFIO, ENABLE);
    
  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	   /* 独立模式 */
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;			   /* 连续多通道模式 */
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	   /* 连续转换 */
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  /* 转换不受外界决定 */
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;		       /* 右对齐 */
  ADC_InitStructure.ADC_NbrOfChannel = 1;					   /* 扫描通道数 */
  ADC_Init(ADC1, &ADC_InitStructure);

 /* ADC1 regular channel8 configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);  /* 通道X,采用时间为55.5周期,1代表规则通道第1个 */	
  ADC_Cmd(ADC1, ENABLE);                 /* Enable ADC1 */                      
  ADC_SoftwareStartConvCmd(ADC1,ENABLE);    /* 使能转换开始 */
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
