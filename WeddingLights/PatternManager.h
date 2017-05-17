#ifndef PATTERN_MANAGER_CLASS
#define PATTERN_MANAGER_CLASS


#include <stdint.h>

#include "rgbUtils/rgbutils.h"
#include "Pattern.h"

class PatternManager
{


public:

   PatternManager(uint8_t maxPatterns, uint16_t ledCount) noexcept;
   ~PatternManager();

   void init();
   void update();
   uint8_t addPattern(Pattern* pattern, uint16_t startPosition);
   void mixColours(void);

   uint32_t* getBuffer(void) { return &colourBuffer[0]; }

   uint16_t  ledCount;
   uint16_t*      startPositions;

private:


   Pattern** arrayOfPatterns;
   uint8_t maxPatterns;


   uint32_t* colourBuffer;



};



#endif

