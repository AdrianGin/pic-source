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

#include "Graphics/gfxFileBrowser.h"

#include "Alphasort/alphasort.h"

#include "ProjectConfig/ProjectConfig.h"

#include "ADC/ADC.h"

#include <stdlib.h>
#include <string.h>


FATFS fs[_VOLUMES];
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
	uint8_t ret;

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
	LPD8806_Clear();
	LPD8806_Update();

	ADC_Configuration();

	TIM_MIDI_Configuration();
	AUX_TIM_Configuration();

	GPIO_SetBits(GPIOC, GPIO_Pin_13 );

	delay_init();
	delay_ms(10);

	LCD_Initializtion();
	//LCD_BackLight_Init();
	LCD_Clear(WHITE);
	//delay_init();
	//delay_ms(5000);

	gfxEngine_Init();

	ret = f_mount(MAIN_DRIVE, &fs[MAIN_DRIVE]);
	xprintf("MountMain:%d\n", ret);

	ret = f_mount(RESOURCE_DRIVE, &fs[RESOURCE_DRIVE]);
	xprintf("MountRes:%d\n", ret);

	ret = f_chdrive(MAIN_DRIVE);

	//scan_files(path);
	LPD8806_SetPixel(0, RGB(0,255,255));
	LPD8806_SetPixel(LED_COUNT - 1, RGB(0,255,0));
	LPD8806_SetPixel(LED_COUNT / 2, RGB(255,0,255));

	//LPD8806_Test();
	LPD8806_Update();


	TP_Init();
	TouchPanel_Calibrate();
	/* Infinite loop */

	SetClip(0);
	BMP_SetCursor(0,239);
	BMP_SetRotation(1,-1, 0);

	LCD_SetRotation(LCD_ROTATION_90);

	gfxDrawBMP("1:/title010.bmp");

	//PhysicsInit();
	FluidTouchInit();

	UserGUI_Init(&GFX_FB.GFXLB);

	LCD_PauseUpdateScreen();

	GFX_FB_Init(&GFX_FB);
	GFX_FB_OpenDirRel(&GFX_FB, "0:/");
	GFX_FB_RepopulateList(&GFX_FB.GFXLB, INC_ALL_DIRS, NULL);
	alphasort_linkedList(&GFX_FB.GFXLB.list, SORT_ASCENDING);

	App_LightSystemTaskCreate();
	App_MIDIPlaybackTaskCreate();
	App_GLCDScreenTaskCreate();
	App_ProcessInputsTaskCreate();
	App_SystemMonitorTaskCreate();

	/* Start the scheduler. */
	vTaskStartScheduler();


	return 0;
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
