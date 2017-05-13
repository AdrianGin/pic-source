#include "ShootingStars.h"
#include <stdlib.h>
#include <string.h>

ShootingStar::ShootingStar(int16_t position, uint16_t length, uint32_t colour, uint16_t prescaler, uint8_t fadeSpeed, eDirections fadeDirection, eDirections direction)
noexcept : position(position), length(length), colour(colour), prescaler(prescaler), fadeSpeed(fadeSpeed), fadeDirection(fadeDirection), direction(direction)

{

   colourBuffer = (uint32_t*)malloc(length * 4);
}

ShootingStar::~ShootingStar()
{

}

void ShootingStar::init(void)
{
   delayCounter = 0;
   position = 0;
   memset(colourBuffer, 0x00, length * 4);
}

void ShootingStar::update(void)
{

   if( ++delayCounter == prescaler )
   {
      delayCounter = 0;
   }
   else
   {
      return;
   }

   int8_t inc = (direction == FORWARD) ? 1 : -1;
   int16_t newPos = position + inc;
   int16_t lastPos = position - inc;

   //Fade out all colours
   for( uint16_t i = 0; i < length; ++i )
   {
      colourBuffer[i] = SCALE_COLOUR(colourBuffer[i], 255-fadeSpeed, MAX_COLOUR_VALUE);
   }

   if( (newPos >= MIN_POS) && (newPos < length) )
   {
      colourBuffer[position] = colour;
      position = newPos;
   }
}

bool ShootingStar::isFinished(void)
{
   if( (position == (length-1) && (direction == FORWARD)) || ((position == 0) && (direction == REVERSE))  )
   {
      return true;
   }

   return false;
}


void ShootingStar::reverseDir(void)
{
   if(direction == FORWARD)
   {
      direction = REVERSE;
   }
   else
   {
      direction = FORWARD;
   }
}

uint32_t* ShootingStar::getBuffer(void)
{
   return &colourBuffer[0];
}


