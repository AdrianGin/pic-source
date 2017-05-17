#ifndef PATTERN_CLASS
#define PATTERN_CLASS

#include <stdint.h>

#include "rgbUtils/rgbutils.h"

class Pattern
{


public:

   enum {
      MIN_POS = 0,
   };

   typedef enum {
      RED_INDEX,
      GREEN_INDEX,
      BLUE_INDEX,
   } eColourIndex;

   Pattern(int16_t position, uint16_t length, uint32_t colour, uint16_t prescaler) noexcept;
   virtual ~Pattern();


   uint16_t length;
   uint32_t colour;
   uint16_t prescaler;

   //Assigns a buffer of 'length' colours to colourBUffer
   void init();
   bool doDelay(void);

   //Updates the colours in the colour buffer based off the parameters
   virtual void update() = 0;

   uint32_t* getBuffer();

   virtual void reset(void) = 0;
   virtual bool isFinished() = 0;

   uint32_t modulateColour(eColourIndex index, uint8_t* waveform, uint16_t waveIndex, int16_t scalar);

protected:


   int16_t delayCounter;
   int16_t initPosition;
   int16_t position;
   uint32_t* colourBuffer;

};

#endif
