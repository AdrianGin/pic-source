#ifndef MODULATOR_CLASS
#define MODULATOR_CLASS

#include <stdint.h>

#include "rgbUtils/rgbutils.h"

class Modulator
{


public:


   Modulator(int16_t* variable, int16_t minVal, int16_t maxVal, int8_t* waveform, uint16_t waveLength, uint16_t initPhase, int16_t scalar, int16_t divisor, int16_t offset, uint16_t prescaler) noexcept;
   virtual ~Modulator();

   bool doDelay(void);
   void update(void);


protected:

   int16_t* variable;

   int16_t delayCounter;
   uint16_t prescaler;

   int16_t minVal;
   int16_t maxVal;

   uint16_t waveLength;
   uint16_t currentIndex;

   int16_t scalar;
   int16_t divisor;

   int16_t offset;

   int8_t* waveform;






};

#endif
