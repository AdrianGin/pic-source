#ifndef SHOOTING_STAR_CLASS
#define SHOOTING_STAR_CLASS

#include <stdint.h>

#include "Pattern.h"
#include "rgbUtils/rgbutils.h"

class ShootingStar : public Pattern
{


public:

   typedef enum {
      FORWARD,
      REVERSE,
   } eDirections;

   enum {
      MIN_POS = 0,
   };

   ShootingStar(int16_t position, uint16_t length, uint32_t colour, uint16_t prescaler, uint8_t fadeSpeed, eDirections fadeDirection, eDirections direction) noexcept;
   virtual ~ShootingStar();


   uint8_t fadeSpeed;

   eDirections fadeDirection;
   eDirections direction;

   //Assigns a buffer of 'length' colours to colourBUffer
   void init();

   //Updates the colours in the colour buffer based off the parameters
   virtual void update();

   virtual void reset(void);
   virtual bool isFinished();

protected:
   eDirections initFadeDirection;
   eDirections initDirection;


};

#endif
