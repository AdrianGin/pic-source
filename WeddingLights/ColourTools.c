
#include "ColourTools.h"
#include <stdint.h>
#include "rgbUtils/rgbutils.h"


uint32_t CT_NomaliseColour(uint32_t r, uint32_t g, uint32_t b)
{
   uint16_t maxVal;
   maxVal = r;
   if( g > maxVal )
   {
      maxVal = g;
   }
   if( b > maxVal )
   {
      maxVal = b;
   }

   //No need to normalise
   if( maxVal <= MAX_COLOUR_VALUE )
   {
   }
   else
   {
      //normalise rgb colours with MAX_COLOUR_VALUE
      r = (r * MAX_COLOUR_VALUE) / maxVal;
      g = (g * MAX_COLOUR_VALUE) / maxVal;
      b = (b * MAX_COLOUR_VALUE) / maxVal;
   }

   return RGB(r,g,b);
}


uint32_t CT_addColours(uint32_t c1, uint32_t c2)
{
   uint16_t r1 = R_RGB(c1);
   uint16_t g1 = G_RGB(c1);
   uint16_t b1 = B_RGB(c1);

   uint16_t r2 = R_RGB(c2);
   uint16_t g2 = G_RGB(c2);
   uint16_t b2 = B_RGB(c2);

   r1 += r2;
   g1 += g2;
   b1 += b2;

   return CT_NomaliseColour(r1, g1, b1);

}

