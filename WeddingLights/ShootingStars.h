#include <stdint.h>

#include "rgbUtils/rgbutils.h"

class ShootingStar
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
   ~ShootingStar();


   uint16_t length;
   uint32_t colour;

   uint16_t prescaler;
   uint8_t fadeSpeed;

   eDirections fadeDirection;
   eDirections direction;

   //Assigns a buffer of 'length' colours to colourBUffer
   void init();

   //Updates the colours in the colour buffer based off the parameters
   void update();

   uint32_t* getBuffer();

   void reverseDir(void);
   bool isFinished();

private:

   int16_t delayCounter;
   int16_t position;
   uint32_t* colourBuffer;





};


