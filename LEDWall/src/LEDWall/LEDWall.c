


#include "LPD8806\LPD8806.h"


#define DELAY_VALUE		(5000)

void LoopToEnd(uint16_t first, uint16_t last, uint32_t colour)
{
	uint16_t i;

	for( i = first; i < last; i++)
	{
		LPD8806_SetPixel(i, colour);
		LPD8806_Update();
		delay_us(DELAY_VALUE);
		LPD8806_SetPixel(i, RGB(0,0,0));
	}

	for( i = last; i != first; i--)
	{
		LPD8806_SetPixel(i-1, colour);
		LPD8806_Update();
		delay_us(DELAY_VALUE);
		LPD8806_SetPixel(i-1, RGB(0,0, 0));
	}
}

void GotoEnd(uint16_t first, uint16_t last, uint32_t colour)
{
	uint16_t i;

	for( i = first; i < last; i++)
	{
		LPD8806_SetPixel(i, colour);
		LPD8806_Update();
		delay_us(DELAY_VALUE);
	}
}

void TurnOffFromEnd(uint16_t first, uint16_t last)
{
	uint16_t i;

	for( i = last; i != first; i--)
	{
		LPD8806_SetPixel(i-1, RGB(0,0, 0));
		LPD8806_Update();
		delay_us(DELAY_VALUE);
	}
}


void Show1(void)
{
	uint16_t i;

	LPD8806_Clear();
	LPD8806_Update();

	for( i = 0 ; i < LED_COUNT; i++)
	{
		//ConvergeOnCentre(0, LED_COUNT, RGB(255,0, 0), RGB(0,255, 0));
		Interlace(0, LED_COUNT, RGB(255,0, 0), RGB(0,255, 0));
		LPD8806_Update();
		delay_us(DELAY_VALUE*100);
		Interlace(0, LED_COUNT, RGB(0,255, 0), RGB(255,0, 0));
		LPD8806_Update();
		delay_us(DELAY_VALUE*100);
		//ConvergeOnCentre(0, LED_COUNT, RGB(0,255, 0), RGB(255,0, 0));
	}

	LPD8806_Clear();
	LPD8806_Update();
}

void Interlace(uint16_t first, uint16_t last, uint32_t colour, uint32_t colour2)
{
	uint16_t i;
	for( i = first; i < last; i=i+2)
	{
		LPD8806_SetPixel(i, colour);
		LPD8806_SetPixel(last-1-i, colour2);
		//LPD8806_Update();
		//delay_us(DELAY_VALUE);
	}

}

void ConvergeOnCentre(uint16_t first, uint16_t last, uint32_t colour, uint32_t colour2)
{
	uint16_t i;

	for( i = first; i < last; i++)
	{
		LPD8806_SetPixel(i, colour);
		LPD8806_SetPixel(last-1-i, colour2);
		LPD8806_Update();
		delay_us(DELAY_VALUE);
	}
}

void Show2(void)
{
	uint16_t i;

	LPD8806_Clear();
	LPD8806_Update();


	for( i = 0; i < LED_COUNT; i=i+24 )
	{
		GotoEnd(i, LED_COUNT, RGB(255,0, 0));
		TurnOffFromEnd(i+24, LED_COUNT);

		//LoopToEnd(i, LED_COUNT);
		xprintf("i=%d\n", i);
		//LPD8806_SetPixel(i, RGB(255,0, 0));
		//LPD8806_Update();
	}

	LPD8806_Clear();
	LPD8806_Update();
}



