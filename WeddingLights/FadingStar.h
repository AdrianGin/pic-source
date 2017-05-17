#ifndef FADING_STAR_CLASS
#define FADING_STAR_CLASS

#include <stdint.h>
#include "ShootingStars.h"
#include "rgbUtils/rgbutils.h"

class FadingStar: public ShootingStar
{


public:


   FadingStar(int16_t position, uint16_t length, uint32_t colour, uint16_t prescaler, uint8_t fadeSpeed, eDirections fadeDirection, eDirections direction) noexcept;
   virtual ~FadingStar();

   //Updates the colours in the colour buffer based off the parameters
   virtual void update();
   virtual void reset(void);
   virtual bool isFinished(void);

protected:
   uint8_t restartDelay;

private:





};

#endif
