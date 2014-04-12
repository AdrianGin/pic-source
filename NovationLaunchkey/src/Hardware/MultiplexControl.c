

#include "MultiplexControl.h"


void MUX_GPIO_Init(void)
{

	DrvGPIO_InitFunction(E_FUNC_GPIO);

	DrvGPIO_Open(MUX_LINE_PORT, MUX_LINE0_PIN, E_IO_OUTPUT);
	DrvGPIO_Open(MUX_LINE_PORT, MUX_LINE1_PIN, E_IO_OUTPUT);
	DrvGPIO_Open(MUX_LINE_PORT, MUX_LINE2_PIN, E_IO_OUTPUT);

	DrvGPIO_Open(MUX_ADC_PORT, MUX_ADC0_PIN, E_IO_OUTPUT);
	DrvGPIO_Open(MUX_ADC_PORT, MUX_ADC1_PIN, E_IO_OUTPUT);
	DrvGPIO_Open(MUX_ADC_PORT, MUX_ADC2_PIN, E_IO_OUTPUT);

}



void MUX_ActivateLineColumn(uint8_t column)
{

	int32_t newData = 0;
	if(column >= MAX_LINE_COLUMNS )
	{
		return;
	}

	DrvGPIO_SetPortMask(MUX_LINE_PORT, ~((1<<MUX_LINE0_PIN) | (1<<MUX_LINE1_PIN) | (1<<MUX_LINE2_PIN)) );

	newData |= ((column & 0x01) << MUX_LINE0_PIN);
	newData |= ((column & 0x02) << MUX_LINE1_PIN);
	newData |= ((column & 0x04) << MUX_LINE2_PIN);

	DrvGPIO_SetPortBits(MUX_LINE_PORT, newData);

}



void MUX_ActivateADCColumn(uint8_t column)
{

	int32_t newData = 0;
	if(column >= MAX_ADC_COLUMNS )
	{
		return;
	}

	DrvGPIO_SetPortMask(MUX_ADC_PORT, (1<<MUX_ADC0_PIN) | (1<<MUX_ADC1_PIN) | (1<<MUX_ADC2_PIN) );

	newData |= ((column & 0x01) << MUX_ADC0_PIN);
	newData |= ((column & 0x02) << MUX_ADC1_PIN);
	newData |= ((column & 0x04) << MUX_ADC2_PIN);

	DrvGPIO_SetPortBits(MUX_ADC_PORT, newData);



}




