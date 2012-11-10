

#include "GenericTypeDefs.h"

/*********************************************************************
* Overview: Clipping region control codes to be used with SetClip(...)
*           function.
*********************************************************************/
#define CLIP_DISABLE                0   // Disables clipping.
#define CLIP_ENABLE                 1   // Enables clipping.


extern GFX_COLOR            _color;

/*********************************************************************
* Overview: Clipping region control and border settings.
*
*********************************************************************/
// Clipping region enable control
extern SHORT _clipRgn;
// Left clipping region border
extern SHORT _clipLeft;
// Top clipping region border
extern SHORT _clipTop;
// Right clipping region border
extern SHORT _clipRight;
// Bottom clipping region border
extern SHORT _clipBottom;



void PutPixel(SHORT x, SHORT y);
WORD    GetPixel(SHORT x, SHORT y);


void SetClipRgn(SHORT left, SHORT top, SHORT right, SHORT bottom);
void SetClip(BYTE control);
