#include "Pattern.h"
#include <stdlib.h>
#include <string.h>

Pattern::Pattern(int16_t position, uint16_t length, uint32_t colour, uint16_t prescaler)
noexcept : position(position), length(length), colour(colour), prescaler(prescaler)

{

   colourBuffer = (uint32_t*)malloc(length * 4);
   initPosition = position;
   delayCounter = 0;
   memset(colourBuffer, 0x00, length * 4);
}

Pattern::~Pattern()
{

}

void Pattern::init(void)
{

}

bool Pattern::doDelay(void)
{
   if( ++delayCounter == prescaler )
   {
      delayCounter = 0;
      return true;
   }
   else
   {
      return false;
   }
}


uint32_t Pattern::modulateColour(eColourIndex index, uint8_t* waveform, uint16_t waveIndex, int16_t scalar)
{

   int16_t modcolour;
   int16_t modAmount;

   uint8_t r,g,b;

   r = R_RGB(colour);
   g = G_RGB(colour);
   b = B_RGB(colour);

   modAmount = ((waveform[waveIndex] * scalar) / 255);


   switch( index )
   {
   case RED_INDEX:
      modcolour = R_RGB(colour) + modAmount;
      break;

   case GREEN_INDEX:
      modcolour = G_RGB(colour) + modAmount;
      break;

   case BLUE_INDEX:
      modcolour = B_RGB(colour) + modAmount;
      break;
   }


   if( modcolour <= 0 )
   {
      modcolour = 0;
   }
   if( modcolour >= MAX_COLOUR_VALUE)
   {
      modcolour = MAX_COLOUR_VALUE;
   }

   switch( index )
   {
   case RED_INDEX:
      r = modcolour;
      break;

   case GREEN_INDEX:
      g = modcolour;
      break;

   case BLUE_INDEX:
      b = modcolour;
      break;
   }

   return RGB(r,g,b);

}


uint32_t* Pattern::getBuffer(void)
{
   return &colourBuffer[0];
}





