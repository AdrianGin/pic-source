

#include "Strobe.h"
#include <stdlib.h>
#include <string.h>

#include "ColourTools.h"

Strobe::Strobe(int16_t position, uint16_t length, uint32_t colour, uint16_t prescaler, uint8_t fadeTime, uint8_t fadeInSpeed, uint8_t fadeOutSpeed, uint8_t fadePhase)
noexcept :
Pattern(position, length, colour, prescaler), fadeTime(fadeTime), fadeInSpeed(fadeInSpeed), fadeOutSpeed(fadeOutSpeed)

{
   fadeMode = FADE_IN;
   fadeProgress = fadePhase;

}

Strobe::~Strobe()
{

}

void Strobe::init(void)
{

}




void Strobe::update(void)
{

   if( !Pattern::doDelay() )
   {
      return;
   }

   if( (fadeMode == FADE_IN) && (fadeProgress != MAX_FADE_PROGRESS) )
   {
      //Fade in all colours
      for( uint16_t i = 0; i < length; ++i )
      {
         colourBuffer[i] = CT_addColours(colourBuffer[i], SCALE_COLOUR(colour, fadeInSpeed, MAX_COLOUR_VALUE));
      }

      fadeProgress++;
   }


   if( (fadeMode == FADE_OUT) && (fadeProgress != MAX_FADE_PROGRESS) )
   {
      //Fade out all colours
      for( uint16_t i = 0; i < length; ++i )
      {
         colourBuffer[i] = SCALE_COLOUR(colourBuffer[i], fadeOutSpeed, MAX_COLOUR_VALUE);
      }
      fadeProgress++;
   }

}

bool Strobe::isFinished(void)
{
   if( fadeProgress >= fadeTime )
   {
      return true;
   }

   return false;
}


void Strobe::reset(void)
{
   fadeProgress = 0;
   if( fadeMode == FADE_IN)
   {
      fadeMode = FADE_OUT;
   }
   else
   {
      fadeMode = FADE_IN;
   }
}




