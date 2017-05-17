
#include "PatternManager.h"
#include "ColourTools.h"

#include <stdlib.h>
#include <string.h>



PatternManager::PatternManager(uint8_t maxPatterns, uint16_t ledCount)
noexcept : maxPatterns(maxPatterns), ledCount(ledCount)
{
   arrayOfPatterns = (Pattern**)malloc(maxPatterns * sizeof(Pattern*));
   memset(arrayOfPatterns, 0x00, maxPatterns * sizeof(Pattern*));

   startPositions = (uint16_t*)malloc(maxPatterns * sizeof(uint16_t));
   memset(startPositions, 0x00, maxPatterns * sizeof(uint16_t));

   colourBuffer = (uint32_t*)malloc(ledCount * sizeof(uint32_t));
   memset(colourBuffer, 0x00, ledCount * sizeof(uint32_t));


}

void PatternManager::init()
{
   for( uint8_t i = 0; i < maxPatterns; ++i )
   {
      if( arrayOfPatterns[i] != (Pattern*)(0x00) )
      {
         arrayOfPatterns[i]->init();
      }
   }
}

void PatternManager::mixColours(void)
{
   uint32_t r;
   uint32_t g;
   uint32_t b;


   for( uint16_t j = 0; j < ledCount; ++j)
   {

      r = 0;
      g = 0;
      b = 0;

      for( uint8_t i = 0; i < maxPatterns; ++i )
      {
         if( (arrayOfPatterns[i] != (Pattern*)(0x00)) &&
             ((startPositions[i] <= j) && (startPositions[i] + arrayOfPatterns[i]->length) > j))
         {
            uint32_t* memBuf = arrayOfPatterns[i]->getBuffer();
            r += R_RGB(memBuf[j - startPositions[i]]);
            g += G_RGB(memBuf[j - startPositions[i]]);
            b += B_RGB(memBuf[j - startPositions[i]]);
         }
      }

      colourBuffer[j] = CT_NomaliseColour(r,g,b);

   }
}



void PatternManager::update()
{
   for( uint8_t i = 0; i < maxPatterns; ++i )
   {
      if( arrayOfPatterns[i] != (Pattern*)(0x00) )
      {
         arrayOfPatterns[i]->update();

         if( arrayOfPatterns[i]->isFinished() )
         {
            arrayOfPatterns[i]->reset();
         }

      }
   }

   //Update the frame buffer here;
   mixColours();


}




uint8_t PatternManager::addPattern(Pattern* pattern, uint16_t startPosition)
{
   for( uint8_t i = 0; i < maxPatterns; ++i )
   {
      if( arrayOfPatterns[i] == (Pattern*)(0x00) )
      {
         arrayOfPatterns[i] = pattern;
         startPositions[i] = startPosition;
         return 0;
      }
   }

   return 1;
}


