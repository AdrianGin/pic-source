/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The TouchPanel application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
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
#include "stm32f10x.h"
#include "TouchPanel/TouchPanel.h"
#include "systick.h"
#include "GLCD/GLCD.h"

#include "USART/usart.h"
#include "Physics/physics.h"

#include "hw_config.h"
#include "FatFS\ff.h"
#include "Graphics\gfxEngine.h"
#include "FSUtils\FSUtil.h"

#include "InertiaTouch/InertiaTouch.h"
#include "FluidTouch/FluidTouch.h"

#include "UserGUI.h"
#include "MIDIPlayback/midiplayback.h"
#include "LightSys\LightSys.h"
#include "LPD8806\LPD8806.h"

#include "MIDILightLogic/MIDILightLogic.h"

#include "usb_lib.h"

#include "Graphics/BMPDraw.h"

#include "printf/printf.h"

#include "app_cfg.h"
#include "intertaskComm.h"

#include <stdlib.h>
#include <string.h>

FATFS fs;
MIDI_HEADER_CHUNK_t MIDIHdr;
volatile uint8_t globalFlag;


void FlashLEDs(uint16_t abit)
{
	LPD8806_Test();
	LPD8806_Update();
	delay_ms(abit);
	LPD8806_Clear();
	LPD8806_Update();
	delay_ms(abit);
}

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

	int16_t counters[10];
	uint8_t ret;

	char* LBItem;
	char* fnPath;
	Coordinate* point;

	DIR dir;

	char path[255];
	uint16_t tickCounter = 0;
	static uint8_t Togstate = 0;

	InitInterTaskComms();

	USART_Configuration();
	GPIO_Configuration();
	NVIC_Configuration();

	Set_USBClock();
	USB_Config();
	USB_Init();


	LPD8806_Init();
	LS_Init();

	MLL_Init();

	TIM_MIDI_Configuration();
	AUX_TIM_Configuration();

	GPIO_SetBits(GPIOC, GPIO_Pin_13);

	delay_init();
	delay_ms(10);

    LCD_Initializtion();
  //LCD_BackLight_Init();
	LCD_Clear(WHITE);
	//delay_init();
	//delay_ms(5000);

	gfxEngine_Init();

	ret = f_mount(0, &fs);
	xprintf("Mount %d\n", ret);

	strcpy(path, "");
	//scan_files(path);

  TP_Init();
  TouchPanel_Calibrate();
  /* Infinite loop */

  //PhysicsInit();
  FluidTouchInit();


  UserGUI_Init();


	LPD8806_SetPixel(0, RGB(0,255,255) );
	LPD8806_SetPixel(LED_COUNT-1, RGB(0,255,0) );
	LPD8806_SetPixel(LED_COUNT / 2, RGB(255,0,255) );

	//LPD8806_Test();
	LPD8806_Update();



	FSUtil_OpenDir(&dir, "/MIDI");
	LCD_PauseUpdateScreen();

	while( 1 )
	{
		fnPath = FSUtil_GetDirObj(&dir);
		if( fnPath != NULL )
		{
			xprintf("Path:%s\n", fnPath);
			GFX_LB_AddItem(&GFX_LB, fnPath);
			//free(fnPath);
		}
		else
		{
			break;
		}
	}




	App_TouchScreenTaskCreate();
	App_GLCDScreenTaskCreate(&MIDIHdr);
	App_MIDIPlaybackTaskCreate(&MIDIHdr);

    /* Start the scheduler. */
	vTaskStartScheduler();

  while (1)	
  {
	  //LCD_Clear(Black);

	  //for(i = 0; i < 240; i++ )
	  {

		  if( counters[2] >= 5 )
		  {
			  FluidTouchMain();
			  counters[2] = 0;

			  //LPD8806_Update();
		  }

		  ProcessUARTBuffer();
		  ProcessUSBMIDIBuffer_LightSys();
		  //

//
		  if( globalFlag & 0x02 )
		  {
			  LCD_VSyncLow();
			  GPIO_ResetBits(GPIOC, GPIO_Pin_13);
			  globalFlag &= ~0x02;
		  }

		  if( globalFlag & 0x04 )
		  {
			  LCD_VSyncHigh();
			  LS_ProcessAutoTurnOff();
			  GPIO_SetBits(GPIOC, GPIO_Pin_13);
			  globalFlag &= ~0x04;
		  }

		  //if( counters[0] >= 200 )
		  if( globalFlag & 0x08 )
		  {
			  globalFlag &= ~0x08;
			  point = FT_GetLastPoint();
			  if( GFX_LB_ProcessTouchInputs(&GFX_LB) == LB_REQUIRES_REDRAW)
			  {
				  //SetTouchPoint(point->x, point->y);

				  LBItem = (char*)GFX_LB_ReturnSelectedItemPtr(&GFX_LB);

				  if( LBItem )
				  {
					  FRESULT tmp;

					  strcpy(path, "/MIDI/");
					  strcat(path, LBItem);
					  MPB_ResetMIDI();

					  LS_ClearLightTimers();
					  LS_ClearLights();

					  tmp =  MPB_PlayMIDIFile(&MIDIHdr, (uint8_t*)path);
					  xprintf("SELECTED:: %s, FR=%d\n", path, tmp);
					  if(tmp == FR_OK)
					  {
						  xprintf("SUCCESS!!\n");
					  }
				  }

				  LCD_Clear(WHITE);

				  SetClip(1);
				  GFX_LB_Draw(&GFX_LB);
				  SetClip(0);
				  setPixel(point->x,point->y);
				  gfxWriteString(point->x, point->y, "Hi");
			  }


			  counters[0] = 0;
		  }

		  if( globalFlag & 0x10 )
		  {
			  //LPD8806_Update();
			  globalFlag &= ~0x10;
		  }

		  if (globalFlag & 0x01)
		  {
			  //if( tickCounter == 4)
			  {
				  MIDIHdr.masterClock++;
				  globalFlag &= ~1;
				  if( MPB_ContinuePlay(&MIDIHdr, MPB_PB_ALL_ON) == MPB_FILE_FINISHED )
				  {
					  myprintf("End of MIDI File:  ", 1);

					  //TimerStop();
				  }
				  LPD8806_Update();
			  }
			  Togstate = Togstate ^ 1;

				if( Togstate )
				{
					//GPIO_SetBits(GPIOC, GPIO_Pin_13);

				}
				else
				{
					//GPIO_ResetBits(GPIOC, GPIO_Pin_13);
				}
			  tickCounter++;
              if( (tickCounter >= ((MIDIHdr.PPQ / 24)) ) )
              //if(tickCounter == 255)
              {
                  tickCounter = 0;
                  //MIDI_Tx(0xF8);
              }

		  }


		  counters[0]++;
		  counters[1]++;
		  counters[2]++;
		  counters[3]++;
	  }




  }
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
