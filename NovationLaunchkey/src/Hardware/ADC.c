

#include "ADC.h"
#include "MultiplexControl.h"
#include "Timer.h"
#include "LED.h"

#include "TimerCallbacks.h"

#include <stdlib.h>

const uint8_t ADC_SCANMAP[] = {ADC_INPUT0_PIN, ADC_INPUT1_PIN, ADC_INPUT2_PIN, ADC_INPUT3_PIN, ADC_INPUT4_PIN};

const uint8_t ADC_LOOKUP[] = 
{
		ADC_PAD_0,
		ADC_PAD_1,
		ADC_PAD_14,
		ADC_PAD_2,
		ADC_PAD_13,
		ADC_PAD_3,
		ADC_PAD_12,
		ADC_PAD_4,

		ADC_PAD_11,
		ADC_PAD_5,
		ADC_PAD_10,
		ADC_PAD_6,
		ADC_PAD_9,
		ADC_PAD_7,
		ADC_PAD_8,
		ADC_PAD_15,

		ADC_KNOB_2,
		ADC_KNOB_1,
		ADC_KNOB_0,
		ADC_KNOB_3,
		ADC_KNOB_4,
		ADC_KNOB_7,
		ADC_KNOB_5,
		ADC_KNOB_6,


		ADC_SLIDER_5,
		ADC_SLIDER_6,
		ADC_SLIDER_7,
		ADC_SLIDER_4,
		ADC_SLIDER_3,
		ADC_SLIDER_0,
		ADC_SLIDER_2,
		ADC_SLIDER_1,

		ADC_VOLUME,
		ADC_PITCHBEND,
		ADC_MODULATION,
		ADC_INVALID0,
		ADC_INVALID1,
		ADC_INVALID2,
		ADC_INVALID3,
		ADC_INVALID4,

};


typedef struct
{
	uint16_t lastValue;
	uint16_t filteredVal;
} ADC_FilteredElement_t;

ADC_FilteredElement_t ADC_Filtered[ADC_INPUT_COUNT];

uint8_t ADCColumn;
uint8_t ADCStatus = 1;

uint8_t ADC_GetCurrentColumn(void)
{
	return ADCColumn;
}

void ADC_SetNextColumn(void)
{
	ADCColumn++;
	if( ADCColumn >= MAX_ADC_COLUMNS )
	{
		ADCColumn = 0;
	}
	//Set the new column
	MUX_ActivateADCColumn(ADCColumn);
}

uint8_t ADC_IsFinishedSample(void)
{
	return ADCStatus;
}

/* ADC interrupt callback function */
void ADC_IntCallback(uint32_t u32UserData)
{
	uint8_t adcColumn = ADC_GetCurrentColumn();

	uint8_t i;
	for(i = 0; i < ADC_INPUTS_PER_COL ; i++)
	{
		int32_t adcSample = DrvADC_GetConversionData(ADC_SCANMAP[i]);
		uint8_t adcLookup = ADC_LOOKUP[(i * MAX_ADC_COLUMNS) + adcColumn];
		ADC_ApplyFilter(adcLookup, adcSample);
	}

	ADC_SetNextColumn();
	SoftTimerStart(SoftTimer1[SC_ADC]);

}


void ADC_Init(void)
{

	DrvGPIO_InitFunction(E_FUNC_ADC0);
	DrvGPIO_InitFunction(E_FUNC_ADC1);
	DrvGPIO_InitFunction(E_FUNC_ADC2);
	DrvGPIO_InitFunction(E_FUNC_ADC3);
	DrvGPIO_InitFunction(E_FUNC_ADC4);


	/* The maximum ADC clock rate is 16MHz */
	DrvADC_Open(ADC_SINGLE_END, ADC_SINGLE_CYCLE_OP, ADC_INPUT_PIN_MASK , EXTERNAL_12MHZ, 10);
	DrvADC_EnableADCInt(ADC_IntCallback, 0);
	MUX_ActivateADCColumn(0);

}

void ADC_StartConversion(void)
{
	DrvADC_StartConvert();
}



//u8ChannelNum is between 0 and ADC_INPUT_COUNT;
uint16_t ADC_GetSample(uint8_t u8ChannelNum)
{

	if(u8ChannelNum < ADC_INPUT_COUNT)
	{
		return (ADC_Filtered[u8ChannelNum].filteredVal);
	}
	return 0;
}

uint16_t ADC_GetRawSample(uint8_t u8ChannelNum)
{

	if(u8ChannelNum < ADC_INPUT_COUNT)
	{
		return (ADC_Filtered[u8ChannelNum].lastValue);
	}
	return 0;
}


void ADC_ApplyFilter(uint8_t index, uint16_t sample)
{
	ADC_FilteredElement_t* ele = &ADC_Filtered[index];

	int32_t signedValue;
	
	signedValue = sample;// + ele->lastValue;
	//signedValue = signedValue / 2;

	if(  (signedValue < (ADC_MAX_VAL-(ADC_NOM_STEP_SIZE/2))) &&
		 (signedValue > (ADC_MIN_VAL+(ADC_NOM_STEP_SIZE/2))) )
	{

		if( abs(signedValue - ele->lastValue) >= (ADC_NOM_STEP_SIZE/2) )
		{
			ele->filteredVal = (((int32_t)(signedValue-(ADC_NOM_STEP_SIZE/2)) * ADC_MULT_FACTOR) ) / ADC_STEP_SIZE;
			ele->filteredVal = ele->filteredVal + 1;
			ele->lastValue = sample;
		}
	}
	else
	{

		if( abs(signedValue - ele->lastValue) >= (ADC_NOM_STEP_SIZE/4) )
		{
			ele->filteredVal = (signedValue) / ADC_NOM_STEP_SIZE;
			ele->lastValue = sample;
		}
	}


}









