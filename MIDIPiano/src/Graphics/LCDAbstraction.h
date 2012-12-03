#ifndef LCD_ABSTRACTION_H
#define LCD_ABSTRACTION_H


#ifdef __cplusplus    /* Insert start of extern C construct */
extern "C" {
#endif

#include "GenericTypeDefs.h"
#include "GLCD.h"
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




//void PutPixel(SHORT x, SHORT y);
WORD    GetPixel(SHORT x, SHORT y);


void SetClipRgn(SHORT left, SHORT top, SHORT right, SHORT bottom);
void SetClip(BYTE control);

void PutPixel(SHORT x, SHORT y);
//{
//	if(_clipRgn)
//	{
//		if(x < _clipLeft)
//			return;
//		if(x > _clipRight)
//			return;
//		if(y < _clipTop)
//			return;
//		if(y > _clipBottom)
//			return;
//	}
//	LCD_SetPoint(x, y, _color);
//}

#ifdef __cplusplus  /* Insert end of extern C construct. */
}                   /* The C header file can now be */
#endif              /* included in either C or C++ code. */



#endif
