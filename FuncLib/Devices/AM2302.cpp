/*

Copyright (c) 2016 Adrian Gin (adrian.gin[at]gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/


#include <stdint.h>
#include "AM2302.h"
#include "GPIO.h"
#include "Log.h"



namespace Devices
{

AM2302::AM2302(GPIO& sda, void(*delay_us)(uint16_t)) noexcept : sda(sda), delay_us(delay_us)
{
	humidity = 0;
	temperature = 0;
}

void AM2302::Init(void)
{
	sda.Init( Devices::GPIO::OUTPUT );
	sda.SetOutput( Devices::GPIO::HIGH );
}


uint16_t AM2302::GetRawTemperature()
{
	return temperature;
}

uint16_t AM2302::GetRawHumidity()
{
	return humidity;
}


int8_t AM2302::GetTemperature()
{
	if( temperature & TEMPSIGN_BIT )
	{
		return -(temperature / 10);
	}

	return (temperature / 10);
}

int8_t AM2302::GetHumidity()
{
	return humidity / 10;
}

//Sends a start signal of 1ms low, followed by a bus release,
//Waits for 100us and waits for pin to go low. (Should be after 180us)
uint8_t AM2302::RequestData(void)
{
	uint8_t connectionError = 0;

	Init();

	sda.SetOutput( Devices::GPIO::LOW );
	delay_us(1000);
	sda.SetOutput( Devices::GPIO::HIGH );

	//Release bus
	sda.Init( Devices::GPIO::INPUT );

	connectionError += WaitState(Devices::GPIO::HIGH);
	connectionError += WaitState(Devices::GPIO::LOW);

	connectionError += WaitState(Devices::GPIO::HIGH);
	connectionError += WaitState(Devices::GPIO::LOW);

	uint16_t humidity = 0;
	uint16_t temperature = 0;
	uint8_t  parity = 0;

	for( uint8_t i = 0; i < 16; ++i)
	{
		humidity = humidity << 1;
		humidity |= GetBit();

	}

	for( uint8_t i = 0; i < 16; ++i)
	{
		temperature = temperature << 1;
		temperature |= GetBit();

	}

	for( uint8_t i = 0; i < 8; ++i)
	{
		parity = parity << 1;
		parity |= GetBit();
	}

	uint8_t calculatedParity = ((humidity & 0xFF00) >> 8) + (humidity & 0xFF) + \
							   ((temperature & 0xFF00) >> 8) + (temperature & 0xFF);



	if( connectionError )
	{
		return ERR_CONNECTION;
	}

	if( parity == calculatedParity )
	{

		this->humidity = humidity;
		this->temperature = temperature;
		return 0;
	}
	else
	{

		LOG_PRINT_DEC( API::Log::INFO, (char*)"Humidity : ", humidity);
		LOG_PRINT_DEC( API::Log::INFO, (char*)"Temperature: ", temperature);

		LOG_PRINT_DEC( API::Log::ERR,  (char*)"Parity Error Rx: ", parity);
		LOG_PRINT_DEC( API::Log::ERR,  (char*)"Expected: ", calculatedParity);

		return ERR_PARITY;
	}
}




uint8_t AM2302::WaitState(Devices::GPIO::LogicLevel level)
{
	for( uint8_t retry = 255; retry != 0; --retry )
	{
		if( sda.ReadInput() == level )
		{
			return 0;
		}
		delay_us(10);
	}

	return 1;
}





uint8_t AM2302::GetBit()
{
	WaitState( Devices::GPIO::LOW );
	WaitState( Devices::GPIO::HIGH );
	delay_us(30);
	return sda.ReadInput();
}


}




