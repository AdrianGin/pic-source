#include "FadingStar.h"
#include <stdlib.h>
#include <string.h>

FadingStar::FadingStar(int16_t position, uint16_t length, uint32_t colour, uint16_t prescaler, uint8_t fadeSpeed, eDirections fadeDirection, eDirections direction) noexcept :
ShootingStar(position, length, colour, prescaler, fadeSpeed, fadeDirection, direction)
{
   restartDelay = 0;
}

FadingStar::~FadingStar()
{

}

#define MIN_BRIGHTNESS (2)

uint32_t ScaleColour(uint32_t colour, uint16_t factor, uint16_t divisor)
{

   uint8_t r = R_RGB(colour);
   uint8_t g = G_RGB(colour);
   uint8_t b = B_RGB(colour);

   if( r )
   {
      r = (r * factor) / divisor;
      if( r <= MIN_BRIGHTNESS )
      {
         r = MIN_BRIGHTNESS;
      }
   }

   if( g )
   {
      g = (g * factor) / divisor;
      if( g <= MIN_BRIGHTNESS )
      {
         g = MIN_BRIGHTNESS;
      }
   }

   if( b )
   {
      b = (b * factor) / divisor;
      if( b <= MIN_BRIGHTNESS )
      {
         b = MIN_BRIGHTNESS;
      }
   }


   if( (r + g + b) <= (3*MIN_BRIGHTNESS))
   {
      r = 0;
      g = 0;
      b = 0;
   }

   return RGB(r,g, b);
}


uint32_t FadeColour(uint32_t colour, uint8_t amount)
{
   uint8_t r = R_RGB(colour);
   if( r >= amount )
   {
      r -= amount;
   }
   else
   {
      r = 0;
   }

   uint8_t g = G_RGB(colour);
   if( g >= amount )
   {
      g -= amount;
   }
   else
   {
      g = 0;
   }

   uint8_t b = B_RGB(colour);
   if( b >= amount )
   {
      b -= amount;
   }
   else
   {
      b = 0;
   }

   return RGB(r,g,b);
}


void FadingStar::update(void)
{

   if( !Pattern::doDelay() )
   {
      return;
   }

   int8_t inc = (direction == FORWARD) ? 1 : -1;
   int16_t newPos = position + inc;
   int16_t lastPos = position - inc;

   if( fadeDirection == FORWARD )
   {
      if( (position >= MIN_POS) && (position < length) )
      {
         if( direction == FORWARD )
         {
            colourBuffer[position] = SCALE_COLOUR(colour, ((((length-1)-position)*255)/(length-1)), MAX_COLOUR_VALUE);
         }
         else
         {
            colourBuffer[position] = SCALE_COLOUR(colour, 255-(((length-position)*255)/length), MAX_COLOUR_VALUE);
         }
      }
   }
   else
   {

      //Fade out all colours
      for( uint16_t i = 0; i < length; ++i )
      {
         colourBuffer[i] = ScaleColour(colourBuffer[i], fadeSpeed, MAX_COLOUR_VALUE);
      }
   }


   position = newPos;

   if( ((position >= (length-1)) && (direction == FORWARD) && (fadeDirection == FORWARD)))
   {
      fadeDirection = REVERSE;
      direction = REVERSE;
   }

   if( ((position <= (0)) && (direction == REVERSE) && (fadeDirection == FORWARD)))
   {
      fadeDirection = REVERSE;
      direction = REVERSE;
   }


}

void FadingStar::reset(void)
{
   position = initPosition;
   direction = initDirection;
   fadeDirection = initFadeDirection;

}

bool FadingStar::isFinished(void)
{
   if( ((position <= ((length-1)*-1)) && (direction == REVERSE))  )
   {
      return true;
   }

   return false;
}

