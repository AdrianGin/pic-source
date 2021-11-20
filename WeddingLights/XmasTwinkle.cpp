/*
 * XmasTwinkle.cpp
 *
 *  Created on: 20/11/2021
 *      Author: Adrian
 */

#include <XmasTwinkle.h>

XmasTwinkle::XmasTwinkle(int16_t position, uint16_t length, uint32_t colour, uint16_t prescaler, uint16_t endPhase, uint16_t startPhase)
: Pattern(position, length, colour, prescaler), IsFadeIn(false), m_endPhase(endPhase), phase(startPhase)
{

}

XmasTwinkle::~XmasTwinkle()
{

}


void XmasTwinkle::update()
{
   if( !Pattern::doDelay() )
   {
      return;
   }

   phase++;

	if( phase >= m_endPhase)
	{
		phase = 0;
		IsFadeIn = !IsFadeIn;
	}

	//uint16_t scaleColour = phase >= (m_endPhase / 2) ? 255 - (phase - (m_endPhase / 2)) : phase;
	uint16_t scaleColour = phase >= (m_endPhase / 2) ? 255 - (phase - (m_endPhase / 2)) : phase;

   //if( IsFadeIn )
   {

   		colourBuffer[0] = SCALE_COLOUR(colour, scaleColour / 2, MAX_COLOUR_VALUE);
   		colourBuffer[1] = SCALE_COLOUR(colour, scaleColour, MAX_COLOUR_VALUE);
   		colourBuffer[2] = SCALE_COLOUR(colour, scaleColour / 2, MAX_COLOUR_VALUE);
   }

}

void XmasTwinkle::reset(void)
{

}

bool XmasTwinkle::isFinished(void)
{
	return false;
}














