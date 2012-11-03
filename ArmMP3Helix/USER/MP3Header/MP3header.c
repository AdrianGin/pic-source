/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               MP3header.c
** Descriptions:            None
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-12-6
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
struct tag_info mp3_info;
const int Frame_sampfreqs[4] = { 44100, 48000, 32000, 0 };	   /* MPEG1 LAYER3 */
const int Frame_bitrates[15] = { 0, 32000, 40000, 48000, 56000, 64000, 80000, 96000, 
				  112000, 128000, 160000, 192000, 224000, 256000, 320000 };	/* MPEG1 LAYER3 */

/* 定义回读字节记录 */
uint8_t  recount = 0; 
/* 定义前跳字节记录 */ 
uint32_t jump = 0;     
/* 记录总的音乐文件的数目 */ 
uint8_t MusicFileCount = 0;
/* MP3链表指针 */
dlink PlayFile;		   

//Start of Frame Mask
#define MP3_SOF	(0xFFFA)
#define MP3_SOF_SIZE (0x24)
//Location of the 'Xing' header
#define MP3_VBR_FRAMES_OFFSET (8)
//Fixed for MPEG1 Layer 3
#define MP3_SAMPLES_PER_FRAME	(1152)
enum {CBR, VBR};

/*******************************************************************************
* Function Name  : GetMP3MaxTime
* Description    : calculate MP3 time
* Input          : - MP3buffer: file system buffer
*				   - buffersize: buffer size
*				   - mp3filelen: file total size
* Output         : None
* Return         : MP3 time
* Attention		 : None
*******************************************************************************/
uint32_t GetMP3MaxTime(uint8_t *MP3buffer, uint16_t buffersize,uint32_t mp3filelen)
{
   uint32_t count = 0 ;	    /* 记录读过的字节 */
   uint32_t nFrames;
   uint16_t val ;	        /* 改值用来记录开始帧 */
   uint16_t i ;
   uint8_t  XBR ;           /* XBR=1(VBR) XBR=0(CBR) */
   uint8_t  bitrate_index;  /* 比特率索引 */
   uint8_t  sample_index;   /* 采样频率索引 */
   uint8_t  padding_bit;	/* padding位	*/
   uint32_t framesize;	    /* 帧长度 */
   uint32_t mp3maxtime;	    /* 播放时间以毫秒为单位 */
   uint32_t maxframe;	    /* 帧数 */
   uint32_t tag2end;		/* tag2结束位置 */
   uint8_t  start = 1;	    /* 记录是否读过ID3V2.3.0格式的TAG2 */

   recount = 0 ;       /* 回读字节清零 */
   jump = 0;		   /* 前跳字节清零 */
   if(start)
    {
	  /* 30 26 b2 75 8e 66 cf 11 a6 d9 00 aa 00 62 ce 6c 代表WMA */
	  if(MP3buffer[0]==0x30&&MP3buffer[1]==0x26&&MP3buffer[2]==0xb2&&MP3buffer[3]==0x75)
	     return  1;
	  else if(MP3buffer[0]!='I'||MP3buffer[1]!='D'||MP3buffer[2]!='3') 
	     goto decode;                /* ID3V2.3.0 */
	  if(MP3buffer[3]!=0x03)         /* ID3V2.3.0 */
		 return 1;
      tag2end = 10+(MP3buffer[6]&0x7F)*0x200000+(MP3buffer[7]&0x7F)*0x400+(MP3buffer[8]&0x7F)*0x80+(MP3buffer[9]&0x7F);	/* 计算得到tag2结束位置 */
      count = tag2end;         /* 有TAG2，从TAG2开始读 */  
      if(count>buffersize - 1) /* 溢出 */
        {
		  jump = count - buffersize ; /* 计算需前跳的字节 */
		  start = 0;                  /* 已读过TAG2 */
		  return 0;
		} 
	 }

decode:
	//Find the first MP3_SOF
   val = MP3buffer[count] ;
   while (((val & MP3_SOF) != MP3_SOF) && count < buffersize-1)   /* Locate the first frame */
   {
	val <<= 8;
	val |= MP3buffer[++count];
   }

   if(count>=buffersize-1)
    {
	   if(MP3buffer[buffersize-1]==0xff)
	    {
		   recount = 1;
		}
	   
	   return 0;
	}

   if(count + 47 >= buffersize-1)
    {
	  recount = 48;
	  return 0;
	}
   
    /* The BitRate, SampleRate and Padding Bit reside in the next 8 bits */
    count++;
   	bitrate_index = (MP3buffer[count])>>4;
    sample_index = ((MP3buffer[count])>>2)&0x03;
    padding_bit  = ((MP3buffer[count])>>1)&0x01;

	count = count + 2;
    
	/* Determine whether the file is CBR or VBR */
	XBR=CBR;
	for(i=count; i < buffersize;i++)
	 {
	   if(MP3buffer[i]=='X'&&MP3buffer[i+1]=='i'&&MP3buffer[i+2]=='n'&&MP3buffer[i+3]=='g')
	    {
		   count = i;
		   XBR = VBR;
		   break;
		}
	 }

//Determine Frame Size:
//Frame Size = ( (Samples Per Frame / 8 * Bitrate) / Sampling Rate) + Padding Size
//Duration = File Size / Bitrate * 8 (CBR)
	if(XBR==CBR)   /* CBR */
	 {
	    framesize = (u32)(((144 * Frame_bitrates[bitrate_index]) / Frame_sampfreqs[sample_index])) + padding_bit ;
		maxframe = (mp3filelen/ (Frame_bitrates[bitrate_index]/8));
	 }
//Duration = Number of Frames * Samples Per Frame / Sampling Rate (VBR)
     else  /* VBR */
	 {
	     count = count + MP3_VBR_FRAMES_OFFSET;
	     //Number of Frames is in Big Endian, need to convert.
	     nFrames = MP3buffer[count+3] << 0 |
	    		   MP3buffer[count+2] << 8 |
	    		   MP3buffer[count+1] << 16 |
	    		   MP3buffer[count]   << 24;

	     maxframe = nFrames * MP3_SAMPLES_PER_FRAME / Frame_sampfreqs[sample_index];

      }
 	  mp3maxtime = maxframe*1000;  /* convert seconds to milliseconds */
	  start = 1;
	
	  return mp3maxtime;         /* 返回播放时间 */        
}

/*******************************************************************************
* Function Name  : TimeFormat
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TimeFormat(uint32_t srctime,TIME *destime)
{
   uint32_t  hour;
   uint32_t  minute;
   uint32_t  second;
   uint32_t  msecond;
   
   msecond = srctime % 1000;  /* 得到微秒数 */
   second  = srctime / 1000;  /* 临时记录所有秒数 */
   minute  = second / 60;     /* 临时记录所有分钟数 */
   second  = second % 60;	  /* 得到秒数 */
   hour    = minute / 60;	  /* 得到小时数 */
   minute  = minute % 60;	  /* 得到分钟数 */

   destime->hour = (uint8_t)hour ;
   destime->minute = (uint8_t)minute;
   destime->second = (uint8_t)second;
   destime->msecond = (uint16_t)msecond;  
}

/*******************************************************************************
* Function Name  : GetMP3Time
* Description    : 计算MP3播放时间
* Input          : - path: MP3存放路径
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
int GetMP3Time(char* path)
{  
   int TotalTime=0;  /* 音乐播放的总时间 */
   uint16_t mp3recordstart=0;

   res = f_open(&mp3FileObject ,path, FA_OPEN_EXISTING | FA_READ);	
   printf("FOPEN=%d\n", res);

   /* 获取MP3文件的播放时间 */
   if(mp3FileObject.fsize != 0)  /* 歌曲长度不为零 */
   {
      /* 计算歌曲播放总时间 */
      while(mp3recordstart < mp3FileObject.fsize)              
	  {
		 /* 调整读取位置 */
	     mp3recordstart = mp3recordstart - recount + jump;	  
	     /* 从SD卡中读取数据到缓存 */
		 res = f_read(&mp3FileObject, readBuf, READBUF_SIZE, &n_Read);
		 if( res != FR_OK )
		 {
			 printf("Read=%d\n", res);
			 while(1);
		 }
		 if(TotalTime == 0)
		 {
		   /* 对MP3文件头分析得到播放时间 */
		   TotalTime = GetMP3MaxTime(readBuf, READBUF_SIZE, mp3FileObject.fsize);	
		 } 
		 else if(TotalTime == 1)
		 {
			break;
		 }
		 else
		 {
		    break;
		 }
		 /* 调整读取位置 */
		 mp3recordstart += READBUF_SIZE;	 			
	  }
   }

   f_close(&mp3FileObject);

   return TotalTime;
}

/*******************************************************************************
* Function Name  : Read_ID3V1
* Description    : Read the ID3 Tag from the MP3, and add it to the flying text array
* Input          : - FileObject: file system
*                  - info: struct tag_info 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Read_ID3V1(FIL *FileObject, struct tag_info *info)
{
    res = f_lseek(FileObject,FileObject->fsize - 128 );
	res = f_read(FileObject, &readBuf, 128 , &n_Read);
       
    if (strncmp("TAG", (char *) readBuf, 3) == 0)	/* ID3V1 */
    {
      strncpy(info->title, (char *) readBuf + 3, MIN(MP3_MAX_TITLE_LEN, sizeof(info->title) - 1));
 	  strncpy(info->artist,(char *) readBuf + 3 + MP3_MAX_TITLE_LEN, MIN(MP3_MAX_TITLE_LEN, sizeof(info->artist) - 1));
    }
}

/*******************************************************************************
* Function Name  : Read_ID3V2
* Description    : Read the ID3 Tag from the MP3, and add it to the flying text array
* Input          : - FileObject: file system
*                  - info: struct tag_info 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void Read_ID3V2(FIL *FileObject, struct tag_info *info)
{
	uint32_t p = 0;
	
  	res = f_lseek(FileObject,0); 
	res = f_read(FileObject, &readBuf, READBUF_SIZE , &n_Read);

	if (strncmp("ID3", (char *) readBuf, 3) == 0)
	{
		uint32_t tag_size, frame_size, i;
		uint8_t version_major;
		int frame_header_size;

		tag_size = ((uint32_t) readBuf[6] << 21) | ((uint32_t) readBuf[7] << 14) | ((uint16_t) readBuf[8] << 7) | readBuf[9];

		info->data_start = tag_size;
		version_major = readBuf[3];
		if (version_major >= 3)
		{
			frame_header_size = 10;
		}
		else
		{
			frame_header_size = 6;
		}
		i = p = 10;

		/* iterate through frames */
		while (p < tag_size)
		{
			if (version_major >= 3)
			{
				frame_size = ((uint32_t) readBuf[i + 4] << 24) | ((uint32_t) readBuf[i + 5] << 16) | ((uint16_t) readBuf[i + 6] << 8) | readBuf[i + 7];
			}
			else
			{
				frame_size = ((uint32_t) readBuf[i + 3] << 14) | ((uint16_t) readBuf[i + 4] << 7) | readBuf[i + 5];
			}
			if (i + frame_size + frame_header_size + frame_header_size >= sizeof(readBuf))
			{
				if (frame_size + frame_header_size > sizeof(readBuf))
				{
       	            res = f_lseek(FileObject,FileObject->fptr + p + frame_size + frame_header_size); 
	                res = f_read(FileObject, &readBuf, READBUF_SIZE , &n_Read);
					p += frame_size + frame_header_size;
					i = 0;
					continue;
				}
				else
				{
					int r = sizeof(readBuf) - i;
					memmove(readBuf, readBuf + i, r);
					res = f_read(FileObject, (char *) readBuf + r, i , &n_Read);
					i = 0;
				}
			}

			if (strncmp("TT2", (char *) readBuf + i, 3) == 0 || strncmp("TIT2", (char *) readBuf + i, 4) == 0)
			{
				strncpy(info->title, (char *) readBuf + i + frame_header_size + 1, MIN(frame_size - 1, sizeof(info->title) - 1));
				if( ( info->title[0] == 0xFE && info->title[1] == 0xFF ) || ( info->title[0] == 0xFF && info->title[1] == 0xFE ) )
				{
				    /* unicode 格式*/
					memset(info->title,0,sizeof(info->title));
					printf( "-- MP3 title no support unicode \r\n");			
				}       
			}
			else if (strncmp("TP1", (char *) readBuf + i, 3) == 0 || strncmp("TPE1", (char *) readBuf + i, 4) == 0)
			{
				strncpy(info->artist, (char *) readBuf + i + frame_header_size + 1, MIN(frame_size - 1, sizeof(info->artist) - 1));
				
				if( ( info->artist[0] == 0xFE && info->artist[1] == 0xFF ) || ( info->artist[0] == 0xFF && info->artist[1] == 0xFE ) )
				{
				    /* unicode 格式*/
					memset(info->artist,0,sizeof(info->artist));
					printf( "-- MP3 artist no support unicode \r\n");
				}
			}

			p += frame_size + frame_header_size;
			i += frame_size + frame_header_size;
		}
	}
}							  


/*******************************************************************************
* Function Name  : SearchMusic
* Description    : 在指定目录搜索MP3文件 并存入链表
* Input          : - path: 目录
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void SearchMusic(char* path)
{	
    DIR dirs;
    dlink before_node = NULL,new_node = NULL;
	FILINFO MusicFinfo;	
	TIME destime;								       	
	char *ShortFileName;
	char *longFileName;
    char CurFileType;	 /* 当前所读取的文件类型 */
	char Filepath[80];	 /* 记录完整的文件信息 */
	char FileName[_MAX_LFN];


	f_mount(0, &fs);	 /* 挂载文件系统 */ 
	res = f_opendir(&dirs, path);



	MusicFinfo.lfname = FileName;
	MusicFinfo.lfsize = _MAX_LFN;

    if(res == FR_OK) 
	{	   
       while ( ( (res = f_readdir(&dirs, &MusicFinfo)) == FR_OK ) && MusicFinfo.fname[0] )
	   {
    	   //printf("RES=%d\n", res);
    	    longFileName = MusicFinfo.lfname;
            ShortFileName = MusicFinfo.fname;
			/* 获取文件类型 */
			if( (strstr(ShortFileName,"MP3") !=NULL) || (strstr(ShortFileName,"mp3") !=NULL) )
			{
			   CurFileType = MP3_FILE;
			}
			else if( (strstr(ShortFileName,"WAV") !=NULL) || (strstr(ShortFileName,"wav") !=NULL) )
			{
			   CurFileType = WAVE_FILE;
			}
			else
			{
			   CurFileType = UNKNOWN_FILE;
			}



			if( CurFileType == MP3_FILE )
   			{
			  MusicFileCount++;
			  if( MusicFileCount == 1 )
			  {
			 	PlayFile = (dlink) MP3HeaderMalloc(sizeof( struct PlayFileList ) );
				if( PlayFile == NULL)
				   return ;
				memset(PlayFile->filename,' ',sizeof(PlayFile->filename) );	  /* 空格 */
				strcpy(PlayFile->filename ,ShortFileName);
				//strcpy(PlayFile->shortFileName ,ShortFileName);

				printf("-- search %s ",ShortFileName);				
				PlayFile->next = NULL;
				PlayFile->back = NULL;
				before_node = PlayFile;
				strcpy(Filepath,path);
				strcat(Filepath,"/");	
				strcat(Filepath,ShortFileName);	
				printf("-- file path %s ",Filepath); 
				TimeFormat(GetMP3Time(Filepath), &destime);	  /* 保存播放时间 */
				printf("-- play time %d:%d \r\n", destime.minute , destime.second );	
								
				PlayFile->filename[ strlen(PlayFile->filename) ]  = ' ';	  /* 寻找字符串结束符*/
								
				PlayFile->filename[ sizeof(PlayFile->filename) -6 ]  = destime.minute/10 + '0';	  /* 分钟 */
				PlayFile->filename[ sizeof(PlayFile->filename) -5 ]  = destime.minute%10 + '0';	  /* 分钟 */
				PlayFile->filename[ sizeof(PlayFile->filename) -4 ]  = ':';	  
				PlayFile->filename[ sizeof(PlayFile->filename) -3 ]  = destime.second/10 + '0';	  /* 秒 */
				PlayFile->filename[ sizeof(PlayFile->filename) -2 ]  = destime.second%10 + '0';	  /* 秒 */
				PlayFile->filename[ sizeof(PlayFile->filename) -1 ]  = 0;	  /* 字符串结束符 */

				PlayFile->fileType = CurFileType;
				printf("-- listbox display %s \r\n", PlayFile->filename );				
              }
			  else
			  {
			    new_node = (dlink) MP3HeaderMalloc(sizeof( struct PlayFileList ) );
			    if( new_node == NULL)
			       return ;
				memset(new_node->filename,' ',sizeof(new_node->filename) );	  /* 空格 */
                strcpy(new_node->filename,ShortFileName);
			    printf("-- search %s ",ShortFileName);
                new_node->next = NULL;
			    new_node->back = before_node;
			    before_node->next = new_node;
			    before_node = new_node;
				strcpy(Filepath,path);
				strcat(Filepath,"/");	
				strcat(Filepath,ShortFileName);	
				printf("-- file path %s ",Filepath); 
				TimeFormat(GetMP3Time(Filepath), &destime);	  /* 保存播放时间 */
				printf("-- play time %d:%d \r\n", destime.minute , destime.second );

				new_node->filename[ strlen(new_node->filename) ]  = ' ';	  /* 寻找字符串结束符*/
								
				new_node->filename[ sizeof(new_node->filename) -6 ]  = destime.minute/10 + '0';	  /* 分钟 */
				new_node->filename[ sizeof(new_node->filename) -5 ]  = destime.minute%10 + '0';	  /* 分钟 */
				new_node->filename[ sizeof(new_node->filename) -4 ]  = ':';	  
				new_node->filename[ sizeof(new_node->filename) -3 ]  = destime.second/10 + '0';	  /* 秒 */
				new_node->filename[ sizeof(new_node->filename) -2 ]  = destime.second%10 + '0';	  /* 秒 */
				new_node->filename[ sizeof(new_node->filename) -1 ]  = 0;	  /* 字符串结束符 */

				PlayFile->fileType = CurFileType;
				printf("-- listbox display %s \r\n", new_node->filename );	
			  }
		    }


			if( CurFileType == WAVE_FILE )
			{
				  MusicFileCount++;
				  if( MusicFileCount == 1 )
				  {
				 	PlayFile = (dlink) MP3HeaderMalloc(sizeof( struct PlayFileList ) );
					if( PlayFile == NULL)
					   return ;
					memset(PlayFile->filename,' ',sizeof(PlayFile->filename) );	  /* 空格 */
					strcpy(PlayFile->filename,ShortFileName);
					PlayFile->fileType = CurFileType;

					printf("-- search %s ",ShortFileName);
					PlayFile->next = NULL;
					PlayFile->back = NULL;
					before_node = PlayFile;
					strcpy(Filepath,path);
					strcat(Filepath,"/");
					strcat(Filepath,ShortFileName);
					printf("-- file path %s ",Filepath);
	              }
				  else
				  {
				    new_node = (dlink) MP3HeaderMalloc(sizeof( struct PlayFileList ) );
				    if( new_node == NULL)
				       return ;
					memset(new_node->filename,' ',sizeof(new_node->filename) );	  /* 空格 */
	                strcpy(new_node->filename,ShortFileName);
	                PlayFile->fileType = CurFileType;


				    printf("-- search %s ",ShortFileName);
	                new_node->next = NULL;
				    new_node->back = before_node;
				    before_node->next = new_node;
				    before_node = new_node;
					strcpy(Filepath,path);
					strcat(Filepath,"/");
					strcat(Filepath,ShortFileName);
					printf("-- file path %s ",Filepath);
					printf("-- play time %d:%d \r\n", destime.minute , destime.second );
					printf("-- listbox display %s \r\n", new_node->filename );
				  }

			}






		}
       printf("RES=%d\n", res);
	}
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

