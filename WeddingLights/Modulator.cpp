#include "Modulator.h"
#include <stdlib.h>
#include <string.h>


Modulator::Modulator(int16_t* variable, int16_t minVal, int16_t maxVal, int8_t* waveform, uint16_t waveLength, uint16_t initPhase, int16_t scalar, int16_t divisor, int16_t offset, uint16_t prescaler) noexcept :
variable(variable), minVal(minVal), maxVal(maxVal), waveform(waveform), waveLength(waveLength), currentIndex(initPhase), scalar(scalar), divisor(divisor), offset(offset), prescaler(prescaler)
{
   delayCounter = 0;
}

Modulator::~Modulator(void)
{

}



bool Modulator::doDelay(void)
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



void Modulator::update(void)
{
   if( !doDelay() )
   {
      return;
   }

   *variable = (int16_t)(( (waveform[currentIndex] + offset) * scalar) / divisor);
   if( *variable <= minVal )
   {
      *variable = minVal;
   }

   if( *variable >= maxVal)
   {
      *variable = maxVal;
   }


   if( ++currentIndex >= waveLength )
   {
      currentIndex = 0;
   }


}
