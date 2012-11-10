




#include "GenericTypeDefs.h"
#include "GLCD.h"
#include "fontRender.h"
#include "ComicSans.h"

#include "gfxEngine.h"

#include "LCDAbstraction.h"

#include "ff.h"
#include "BMPDraw.h"

BMPFile_t BMPFile;

void gfxEngine_Init(void)
{

	_fontOrientation = ORIENT_HOR;
	SetFontFlash( (void*)&Verdana_24);
    SetClip(CLIP_ENABLE);
    SetClipRgn(-300, -400, 400, 300);

}


void gfxWriteString(int16_t x, int16_t y, uint8_t* string)
{
	SetColor(Red);
	OutTextXY(x,y,string);
}

void gfxWrite(uint8_t ch)
{
	SetColor(Red);
	OutChar(ch);
}



void gfxDrawBMP(void)
{
	uint8_t ret;

	FIL temp;
    BMPFile.pImageFile = &temp;

    ret = f_open( BMPFile.pImageFile, "bride.bmp", FA_OPEN_EXISTING | FA_READ );
    printf("retOpen=%d\n", ret);
    BMP_ReadHeader(&BMPFile);
    printf("BMP Width: %d\n", BMPFile.lWidth);
    printf("BMP Length:%d\n", BMPFile.lHeight);
    printf("BMP Planes:%d\n", BMPFile.bNumOfPlanes);
    printf("BMP Depth:%d\n", BMPFile.bBitsPerPixel);
    printf("BMP CompType:%d\n", BMPFile.blCompressionType);
    ret = BMP_Print(&BMPFile);
}
