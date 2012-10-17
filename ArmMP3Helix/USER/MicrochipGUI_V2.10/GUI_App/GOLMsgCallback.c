/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               GOLMsgCallback.c
** Descriptions:            None
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-30
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

/* Private variables ---------------------------------------------------------*/
/* 播放MP3邮箱 */
extern OS_EVENT *mp3Mbox;		
/* OS计数信号量 */	
extern OS_EVENT *DMAComplete;
extern OS_EVENT *StopMP3Decode;
/* MP3播放状态 */
extern AUDIO_Playback_status_enum AUDIO_Playback_status ;


/* Private function prototypes -----------------------------------------------*/
WORD MsgMP3(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg);


WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
{
   switch(ScreenState)
   {
     case DISPLAY_MP3:        
          MsgMP3(objMsg, pObj, pMsg);		/* 按键反馈 */
          return 1; 
     default:
          return 1;    
   }
}


WORD MsgMP3(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
{ 
  SLIDER   *pSld;
  LISTBOX  *pLb;
  BUTTON   *Btn;
  LISTITEM *pItem;
  PICTURE  *pPicture;
  char     asci;
  char     TxedMessage[40];
  pLb =(LISTBOX*)GOLFindObject(ID_LISTBOX1);

   switch(GetObjID(pObj))
   {
     case ID_LISTBOX1:   /* 列表框 */	  
          if( (pMsg->uiEvent == EVENT_PRESS && objMsg == LB_MSG_TOUCHSCREEN) ||
        	  (pMsg->uiEvent == EVENT_KEYSCAN && pMsg->param2 == SCAN_CR_PRESSED) )
		  {
		    static LISTITEM *LastpItem = NULL;
            LbMsgDefault(objMsg, (LISTBOX*)pObj, pMsg);

            memset(pMsg, 0, sizeof(GOL_MSG));

	        pItem = LbGetSel(pLb,NULL); 

		    if(pItem != NULL)
  		    {  	
	           if( ( LastpItem != NULL ) && (LastpItem != pItem) )
	           {
			      LbSetBitmap(LastpItem, (void*)&musicoff);
               }
			   			 			  						   
			   pSld = (SLIDER*)GOLFindObject(ID_SLIDER3); 
               SldSetPos(pSld,LbGetCount(pLb)-LbGetFocusedItem(pLb)-1);
               SetState(pSld, SLD_DRAW_THUMB);
		       SldDraw(pSld);

			   Btn = (BUTTON*)GOLFindObject(ID_BUTTON2);
			   BtnSetBitmap(Btn,(void*)&play_off);
			   BtnDraw((BUTTON*)Btn);	

		       LastpItem = pItem;
		       LbSetBitmap(pItem, (void*)&musicon);
			   SetState(pLb, LB_DRAW_ITEMS);
		       LbDraw(pLb);

			   memset(TxedMessage,0,sizeof(TxedMessage));
			   strcpy(TxedMessage,MUSIC_DIRECTORY);
			   strcat(TxedMessage,"/");
			   strcat(TxedMessage,pItem->pText);





			   printf("-- select music %s \r\n",TxedMessage);
		        if( (strstr(TxedMessage,"MP3") !=NULL) || (strstr(TxedMessage,"mp3") !=NULL) ||
		            (strstr(TxedMessage,"WAV") !=NULL) || (strstr(TxedMessage,"wav") !=NULL) )
		        {
		           if( (strstr(TxedMessage,"MP3") !=NULL) )
		           {
				       TxedMessage[ strstr(TxedMessage,"MP3") - TxedMessage + 3 ] = 0;
				   }
		           else	if( (strstr(TxedMessage,"mp3") !=NULL) )
		           {
				       TxedMessage[ strstr(TxedMessage,"MP3") - TxedMessage + 3 ] = 0;
				   }
		           else	if( (strstr(TxedMessage,"WAV") !=NULL) )
		           {
				       TxedMessage[ strstr(TxedMessage,"WAV") - TxedMessage + 3 ] = 0;
				   }
		           else	if( (strstr(TxedMessage,"wav") !=NULL) )
		           {
				       TxedMessage[ strstr(TxedMessage,"wav") - TxedMessage + 3 ] = 0;
				   }

			     if(AUDIO_Playback_status == IS_PLAYING)
			     {		      
			        OSSemPost(StopMP3Decode);	           /* 停止解码     */	 
				    AUDIO_Playback_Stop();
			     }
			     OSTimeDlyHMSM(0, 0, 0, 100);	               /* 1 second     */
			     OSMboxPost(mp3Mbox,(void*)TxedMessage );  /* 开始解码任务 */	 
			   }	 
		     }
		   }
		  return 1;
	  
      case ID_SLIDER2:   /* 播放进度条     */	
	       if(objMsg == SLD_MSG_INC || objMsg == SLD_MSG_DEC )
	       {  
	         pSld = (SLIDER*)GOLFindObject(ID_SLIDER2);
		     SldMsgDefault(objMsg,(SLIDER*)pObj, pMsg);	
			 /* 跳转READBUF_SIZE个整数倍*/
			 MP3_Data_Index = ( (uint32_t)( ( (float)SldGetPos(pSld)/200 ) * ( mp3FileObject.fsize - mp3_info.data_start) ) - \
			                  ( (uint32_t)( ( (float)SldGetPos(pSld)/200 ) * ( mp3FileObject.fsize - mp3_info.data_start) ) % READBUF_SIZE ) );

			 //OSTimeDlyHMSM(0, 0, 0, 200);
			 res = f_lseek(&mp3FileObject, mp3_info.data_start + MP3_Data_Index );
			 printf("lseek=%d", res);
		     //OSTimeDlyHMSM(0, 0, 0, 200);
		     SetState(pSld,SLD_DRAW_THUMB); 
		   } 
		   return 1;
		  	      	  
	  case ID_SLIDER3:   /* 列表框滑动条 */
	       if(objMsg == SLD_MSG_INC || objMsg == SLD_MSG_DEC )
		   {
	         pSld = (SLIDER*)GOLFindObject(ID_SLIDER3); 
	         /*  Process message by default */
             SldMsgDefault(objMsg, (SLIDER*)pObj, pMsg);
             /* Set new list box position	*/
             if(LbGetFocusedItem(pLb) != LbGetCount(pLb) - SldGetPos(pSld) -1 )
		     {
               LbSetFocusedItem(pLb,LbGetCount(pLb) - SldGetPos(pSld) - 1 );
               SetState(pLb, LB_DRAW_ITEMS);
             }
		   }
           return 1;		

      case ID_BUTTON1: 	                 /* Previous         */
	       if(objMsg == BTN_MSG_PRESSED)
		   {		      
			  /* 控制列表框 */
			  pItem = LbGetSel(pLb,NULL);
			  if(pItem != NULL)
		      {				
			  	 Btn = (BUTTON*)GOLFindObject(ID_BUTTON2); 
		         BtnSetBitmap(Btn,(void*)&play_off);
				 BtnDraw((BUTTON*)Btn);	
				 				 	         
				 pSld =(SLIDER*)GOLFindObject(ID_SLIDER3);

		         if( (*pItem).pPrevItem == NULL )	   /* 如果已经是第一个单元 */
		         {
				    pItem = LbGetSel(pLb,NULL);	
				 }
				 else
				 {	
				    LbSetBitmap(pItem, (void*)&musicoff);
				    pItem = (*pItem).pPrevItem;					
				 }				 
				 		  
	             LbSetFocusedItem(pLb,LbGetFocusedItem(pLb)-1);	 
				 LbSetBitmap(pLb->pFocusItem, (void*)&musicon);
				 LbChangeSel(pLb,pLb->pFocusItem);
    	         SetState(pLb, LB_DRAW_ITEMS);

			     if(LbGetFocusedItem(pLb) != LbGetCount(pLb)-SldGetPos(pSld)-1)
				 {
					SldSetPos(pSld,LbGetCount(pLb)-LbGetFocusedItem(pLb)-1);
            		SetState(pSld, SLD_DRAW_THUMB);
				 }
				 pItem = LbGetSel(pLb,NULL);			
			     memset(TxedMessage,0,sizeof(TxedMessage));
			     strcpy(TxedMessage,MUSIC_DIRECTORY);
			     strcat(TxedMessage,"/");
			     strcat(TxedMessage,pItem->pText);
			     printf("-- select music %s \r\n",TxedMessage);
			     if( (strstr(TxedMessage,"MP3") !=NULL) || (strstr(TxedMessage,"mp3") !=NULL) )	  
			     {
			       if( (strstr(TxedMessage,"MP3") !=NULL) )
			       {
				      TxedMessage[ strstr(TxedMessage,"MP3") - TxedMessage + 3 ] = 0;
				   }
			       else	if( (strstr(TxedMessage,"mp3") !=NULL) )
			       {
				      TxedMessage[ strstr(TxedMessage,"MP3") - TxedMessage + 3 ] = 0; 
				   }

			       if (outOfData != 1)
				   {
				     OSSemPost(StopMP3Decode);	             /* 停止解码     */	 
				     AUDIO_Playback_Stop();
				   }

				   BtnSetBitmap(pObj,(void*)&previous_off);
			       BtnDraw((BUTTON*)pObj); 

			       OSTimeDlyHMSM(0, 0, 1, 0);	             /* 1 second   */

                   BtnSetBitmap(pObj,(void*)&previous_on);
			       
				   OSMboxPost(mp3Mbox,(void*)TxedMessage );  /* 开始解码任务 */
			     }
			  }
		   }
		   return 1;

      case ID_BUTTON2: 	                 /* Play       */
	       if(objMsg == BTN_MSG_PRESSED)
		   {
		      if( ((BUTTON *)pObj)->pBitmap == (void*)&play_on )	 /* 如果已经暂停播放 */
			  {	 
			     if( ( AUDIO_Playback_status == NO_SOUND ) && !( outOfData == 1 ) )
				 {
			        BtnSetBitmap(pObj,(void*)&play_off);
				    BtnDraw((BUTTON*)pObj); 
					OSTimeDlyHMSM(0, 0, 0, 200);  /* delay 200MS */
			
			        DMA_Cmd(DMA2_Channel4, ENABLE);
                    //SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
                    AUDIO_Playback_status = IS_PLAYING;       
                    codec_send( ACTIVE_CONTROL | ACTIVE );	   /* WM8731 Enable */
					OSSemPost(DMAComplete);	     /* 发送信号量 */			        
				 }
				 else if( ( AUDIO_Playback_status == NO_SOUND ) && ( outOfData == 1 ) )	 /* 播放已经结束 */
				 {
			        BtnSetBitmap(pObj,(void*)&play_off);
				    BtnDraw((BUTTON*)pObj); 

					pItem = LbGetSel(pLb,NULL); 
				    memset(TxedMessage,0,sizeof(TxedMessage));
			        strcpy(TxedMessage,MUSIC_DIRECTORY);
			        strcat(TxedMessage,"/");
			        strcat(TxedMessage,pItem->pText);
			        printf("-- select music %s \r\n",TxedMessage);
			        if( (strstr(TxedMessage,"MP3") !=NULL) || (strstr(TxedMessage,"mp3") !=NULL) ||
			            (strstr(TxedMessage,"WAV") !=NULL) || (strstr(TxedMessage,"wav") !=NULL) )
			        {
			           if( (strstr(TxedMessage,"MP3") !=NULL) )
			           {   
					       TxedMessage[ strstr(TxedMessage,"MP3") - TxedMessage + 3 ] = 0;
					   }
			           else	if( (strstr(TxedMessage,"mp3") !=NULL) )
			           {
					       TxedMessage[ strstr(TxedMessage,"MP3") - TxedMessage + 3 ] = 0; 
					   }
			           else	if( (strstr(TxedMessage,"WAV") !=NULL) )
			           {
					       TxedMessage[ strstr(TxedMessage,"WAV") - TxedMessage + 3 ] = 0;
					   }
			           else	if( (strstr(TxedMessage,"wav") !=NULL) )
			           {
					       TxedMessage[ strstr(TxedMessage,"wav") - TxedMessage + 3 ] = 0;
					   }
			           OSTimeDlyHMSM(0, 0, 1, 0);	               /* 1 second     */
			           OSMboxPost(mp3Mbox,(void*)TxedMessage );  /* 开始解码任务 */	 
			        }
				 }
			  }
		      else											/* 正在播放中 */
		      {	
			     BtnSetBitmap(pObj,(void*)&play_on);
				 BtnDraw((BUTTON*)pObj); 
				 OSTimeDlyHMSM(0, 0, 0, 200);  /* delay 200MS */
				 		 
				 //SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, DISABLE);
	             DMA_Cmd(DMA2_Channel4, DISABLE);
                 AUDIO_Playback_status = NO_SOUND; 
				 //codec_send( ACTIVE_CONTROL | INACTIVE );    /* WM8731 Disable */
			    
				 /* 显示歌曲码率、歌曲名 */
				 pPicture =(PICTURE*)GOLFindObject(ID_PICTURE7);
			     PictDraw(pPicture);

			   	 SetColor( BLACK );

				 asci = ( (mp3_info.bit_rate/1000)/100 + '0' );
				 OutTextXY(178 , 60 , &asci );	

				 asci = ( ( (mp3_info.bit_rate/1000)%100 ) /10 + '0' );
				 OutTextXY(186 , 60 , &asci );

				 asci = ( ( (mp3_info.bit_rate/1000)%100 ) %10 + '0' );
				 OutTextXY(194 , 60 , &asci);

		         OutTextXY(202 , 60 , "Kbps");	

			     pPicture =(PICTURE*)GOLFindObject(ID_PICTURE4);
			     PictDraw(pPicture);
	
	             SetColor( BLACK );
			     OutTextXY(150 - ( (float)(strlen(mp3_info.title)/2) * 8 ),30,mp3_info.title); 		  							 
		      }
		   }
		   return 1;

      case ID_BUTTON3: 	                 /* Stop	   */
	       if(objMsg == BTN_MSG_PRESSED)
		   {
		      pItem = LbGetSel(pLb,NULL); 
			  if(pItem != NULL)
			  {
			    Btn = (BUTTON*)GOLFindObject(ID_BUTTON2);
			    BtnSetBitmap(Btn,(void*)&play_on);
			    BtnDraw((BUTTON*)Btn);	

				BtnSetBitmap(pObj,(void*)&stop_off);
			    BtnDraw((BUTTON*)pObj); 

			    OSTimeDlyHMSM(0, 0, 0, 200);  /* delay 200MS */

                BtnSetBitmap(pObj,(void*)&stop_on);

				outOfData = 1;
			    AUDIO_Playback_Stop();
			  }
		   }	       
		   return 1;

      case ID_BUTTON4: 	                 /* Next         */
	       if(objMsg == BTN_MSG_PRESSED)
		   {		      
			  /* 控制列表框 */
			  pItem = LbGetSel(pLb,NULL);
			  if(pItem != NULL)
		      {
		         Btn = (BUTTON*)GOLFindObject(ID_BUTTON2); 
		         BtnSetBitmap(Btn,(void*)&play_off);
				 BtnDraw((BUTTON*)Btn);	

				 pSld =(SLIDER*)GOLFindObject(ID_SLIDER3);			

		         if( (*pItem).pNextItem == NULL )	   /* 如果已经是最后一个单元 */
		         {
				    pItem = LbGetSel(pLb,NULL);	
				 }
				 else
				 {	
				    LbSetBitmap(pItem, (void*)&musicoff);
				    pItem = (*pItem).pNextItem;					
				 }			 
				  
                 LbSetFocusedItem(pLb,LbGetFocusedItem(pLb)+1);	
				 LbSetBitmap(pLb->pFocusItem, (void*)&musicon);
				 LbChangeSel(pLb,pLb->pFocusItem);		 
                 SetState(pLb, LB_DRAW_ITEMS);

			     if(LbGetFocusedItem(pLb) != LbGetCount(pLb)-SldGetPos(pSld)-1)
				 {
					SldSetPos(pSld,LbGetCount(pLb)-LbGetFocusedItem(pLb)-1);
            		SetState(pSld, SLD_DRAW_THUMB);
				 }

				 pItem = LbGetSel(pLb,NULL);	
			     memset(TxedMessage,0,sizeof(TxedMessage));
			     strcpy(TxedMessage,MUSIC_DIRECTORY);
			     strcat(TxedMessage,"/");
			     strcat(TxedMessage,pItem->pText);
			     printf("-- select music %s \r\n",TxedMessage);
			     if( (strstr(TxedMessage,"MP3") !=NULL) || (strstr(TxedMessage,"mp3") !=NULL) )	  
			     {
			       if( (strstr(TxedMessage,"MP3") !=NULL) )
			       {
				       TxedMessage[ strstr(TxedMessage,"MP3") - TxedMessage + 3 ] = 0;
				   }
			       else	if( (strstr(TxedMessage,"mp3") !=NULL) )
			       {
				       TxedMessage[ strstr(TxedMessage,"MP3") - TxedMessage + 3 ] = 0;
				   } 

			       if (outOfData != 1)
				   {
				     OSSemPost(StopMP3Decode);	             /* 停止解码     */	 
				     AUDIO_Playback_Stop();
				   }

				   BtnSetBitmap(pObj,(void*)&next_off);
			       BtnDraw((BUTTON*)pObj); 

			       OSTimeDlyHMSM(0, 0, 1, 0);	             /* 1 second   */

                   BtnSetBitmap(pObj,(void*)&next_on);

			       OSMboxPost(mp3Mbox,(void*)TxedMessage );  /* 开始解码任务 */	 
			     }
			  }
		   }	       
		   return 1;

      case ID_BUTTON5: 	                 /* Mute */
		   if(objMsg == BTN_MSG_PRESSED)
		   {			 
		     if( ((BUTTON *)pObj)->pBitmap == (void*)&UNMUTE_HOVER)
		   	 {	
		       BtnSetBitmap(pObj,(void*)&MUTE_HOVER);
		       codec_send( ACTIVE_CONTROL | INACTIVE );    /* WM8731 Disable */
			 }
			 else
			 {
			   BtnSetBitmap(pObj,(void*)&UNMUTE_HOVER);
			   codec_send( ACTIVE_CONTROL | ACTIVE );	   /* WM8731 Enable */
			 }
			 OSTimeDlyHMSM(0, 0, 0, 50);  /* delay 50MS */
		   }
		   return 1;
      case ID_SLIDER1:	  /* Drag Volume Slider */
	       if(objMsg == SLD_MSG_INC || objMsg == SLD_MSG_DEC )
		   {
	  	     pSld = (SLIDER*)GOLFindObject(ID_SLIDER1); 
		     /*  Process message by default */
		     SldMsgDefault(objMsg, (SLIDER*)pObj, pMsg);
		     WM8731_Vol( SldGetPos(pSld) + 47 );	 /* 调节音量 */	
		   }	
	  	   return 1;

      case ID_PICTURE2:	  /* Volume Up */
		   if(objMsg == PICT_MSG_SELECTED)
		   {
		     pSld = (SLIDER*)GOLFindObject(ID_SLIDER1); 
			 SldSetPos(pSld, SldGetPos(pSld) + 1 );
			 SetState(pSld, SLD_DRAW_THUMB);
		     WM8731_Vol( SldGetPos(pSld) + 47 );	 /* 调节音量 */			
		   }
	  	   return 1;

      case ID_PICTURE3:	  /* Voume Down */
		   if(objMsg == PICT_MSG_SELECTED)
		   {
		     pSld = (SLIDER*)GOLFindObject(ID_SLIDER1); 
			 SldSetPos(pSld, SldGetPos(pSld) - 1 );
			 SetState(pSld, SLD_DRAW_THUMB);
		     WM8731_Vol( SldGetPos(pSld) + 47 );	 /* 调节音量 */			
		   }
	  	   return 1;	
				
	  case ID_BUTTON6:				     /* LB scroll down */
	       if(objMsg == BTN_MSG_PRESSED)	
		   {
			 pSld =(SLIDER*)GOLFindObject(ID_SLIDER3); 

		     BtnSetBitmap(pObj,(void*)&press_down);
		     BtnDraw((BUTTON*)pObj);

		     OSTimeDlyHMSM(0, 0, 0, 200);  /* delay 200MS */

	         LbSetFocusedItem(pLb,LbGetFocusedItem(pLb)-1);
    	     SetState(pLb, LB_DRAW_ITEMS);

             SldSetPos(pSld,SldGetPos(pSld)+1);
             SetState(pSld, SLD_DRAW_THUMB);

		     BtnSetBitmap(pObj,(void*)&press_up);
		   }
		   return 1;

	  case  ID_BUTTON7:                  /* LB scroll up */
		    if(objMsg == BTN_MSG_PRESSED)
			{
			  pSld =(SLIDER*)GOLFindObject(ID_SLIDER3);	
		  	
		      BtnSetBitmap(pObj,(void*)&rpress_dowm);
		      BtnDraw((BUTTON*)pObj);		 

		      OSTimeDlyHMSM(0, 0, 0, 200);  /* delay 200MS */

              LbSetFocusedItem(pLb,LbGetFocusedItem(pLb)+1);
              SetState(pLb, LB_DRAW_ITEMS);

              SldSetPos(pSld,SldGetPos(pSld)-1);
              SetState(pSld, SLD_DRAW_THUMB);

		      BtnSetBitmap(pObj,(void*)&rpress_up);
		    }
		    return 1;
				         			   	    	
	  default: 
           return 1; 	
   }	
} 


WORD ExternalMemoryCallback(EXTDATA* memory, LONG offset, WORD nCount, void* buffer)
{		  
   return 1;
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
