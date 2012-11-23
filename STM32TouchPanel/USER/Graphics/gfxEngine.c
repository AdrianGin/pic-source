




#include "GenericTypeDefs.h"
#include "GLCD.h"
#include "fontRender.h"
#include "ComicSans.h"
#include "Tahoma.h"

#include "gfxEngine.h"

#include "LCDAbstraction.h"

#include "ff.h"
#include "BMPDraw.h"

BMPFile_t BMPFile;

void gfxEngine_Init(void)
{

	_fontOrientation = ORIENT_HOR;
	SetFontFlash( (void*)&Verdana_24);
	SetFontFlash( (void*)&Tahoma_12);

    SetClip(CLIP_ENABLE);
    SetClipRgn(-300, -400, 400, 300);

}



void gfxWriteString(int16_t x, int16_t y, uint8_t* string)
{
	OutTextXY(x,y,string);
}

void gfxWrite(uint8_t ch)
{
	OutChar(ch);
}


/*********************************************************************
* Function: WORD Bar(SHORT left, SHORT top, SHORT right, SHORT bottom)
*
* PreCondition: none
*

* Input: left,top - top left corner coordinates,
*        right,bottom - bottom right corner coordinates
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the shape is not yet completely drawn.
*         - Returns 1 when the shape is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: draws rectangle filled with current color
*
* Note: none
*
********************************************************************/
void gfxFillRect(int16_t left, int16_t top, int16_t right, int16_t bottom)
{
	int16_t   x, y;

    for(y = top; y < bottom + 1; y++)
    {
        for(x = left; x < right + 1; x++)
        {
            PutPixel(x, y);
        }
    }
}

void gfxDrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{

}


void gfxDrawBMP(char* bmpImgPath)
{

	uint8_t ret;

	FIL temp;
    BMPFile.pImageFile = &temp;


    return;



    ret = f_open( BMPFile.pImageFile, bmpImgPath, FA_OPEN_EXISTING | FA_READ );
//    printf("retOpen=%d\n", ret);
    BMP_ReadHeader(&BMPFile);
//    printf("BMP Width: %d\n", BMPFile.lWidth);
//    printf("BMP Length:%d\n", BMPFile.lHeight);
//    printf("BMP Planes:%d\n", BMPFile.bNumOfPlanes);
//    printf("BMP Depth:%d\n", BMPFile.bBitsPerPixel);
//    printf("BMP CompType:%d\n", BMPFile.blCompressionType);
    ret = BMP_Print(&BMPFile);

    f_close(BMPFile.pImageFile);
}
