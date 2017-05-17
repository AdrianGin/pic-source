#ifndef STROBE_CLASS
#define STROBE_CLASS

#include <stdint.h>

#include "Pattern.h"
#include "rgbUtils/rgbutils.h"

class Strobe : public Pattern
{


public:

   typedef enum {
      FADE_IN,
      FADE_OUT,
   } eStrobeState;

   enum {
      MAX_FADE_PROGRESS = 255,
   };

   Strobe(int16_t position, uint16_t length, uint32_t colour, uint16_t prescaler, uint8_t fadeTime, uint8_t fadeInSpeed, uint8_t fadeOutSpeed, uint8_t fadePhase) noexcept;
   ~Strobe();

   uint8_t fadeTime;
   uint8_t fadeInSpeed;
   uint8_t fadeOutSpeed;

   //Assigns a buffer of 'length' colours to colourBUffer
   void init();

   //Updates the colours in the colour buffer based off the parameters
   virtual void update();

   virtual void reset(void);
   virtual bool isFinished();

protected:

   uint16_t fadeProgress;
   eStrobeState fadeMode;

};

#endif
