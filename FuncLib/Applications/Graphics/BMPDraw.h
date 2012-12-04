#ifndef _BMP_DRAW_H
#define _BMP_DRAW_H

#include <stdint.h>
//Requires some sort of FAT file System
#include "FatFS/ff.h"


/*
 * 				  BMP_SetRotation(1, -1);
				  BMP_SetCursor(0, 239);
				  gfxDrawBMP("blueto~1.bmp");
 */

typedef struct _BMPfile
{
        FIL* pImageFile;      /* Image file pointer */
        uint32_t lWidth;
        uint32_t lHeight;
        uint32_t lImageOffset;
        uint16_t wPaletteEntries;
        uint8_t bBitsPerPixel;
        uint8_t bHeaderType;
        uint8_t blBmMarkerFlag : 1;
        uint8_t blCompressionType : 3;
        uint8_t bNumOfPlanes : 3;
        uint8_t b16bit565flag : 1;
} BMPFile_t;

extern uint16_t BMP_CursorX;
extern uint16_t BMP_CursorY;

extern uint16_t BMP_Scale;
extern int16_t BMP_Direction[];

#define BMP_24BPP	(3)

#define PIXEL_BUFFER_COUNT (128)
#define BMP_BADFILE_FORMAT  (100)
#define IMG_FREAD(ptr, size, n, fileptr) _BMP_readbuf(fileptr, ptr, size)
#define IMG_FSEEK(ptr, offset, whence) f_lseek(ptr, offset)
#define IMG_FTELL(ptr)					f_tell(ptr)

uint8_t BMP_ReadHeader(BMPFile_t* pBmpDec);
uint8_t BMP_Print(BMPFile_t* pBmpDec);
void BMP_SetCursor(uint16_t x, uint16_t y);
void BMP_SetRotation(int16_t dirX, int16_t dirY, uint16_t rotation);
void BMP_SetScaling(uint16_t scale);


void BMP_PrintToLCD(BMPFile_t* pBmpDec);
uint16_t BMP_GetRowData_24BPP(BMPFile_t* pBmpDec, uint8_t* readBuf, uint16_t bufSize, uint16_t rowPos, uint16_t height);
uint8_t BMP_SetHeightPointer_24BPP(BMPFile_t* pBmpDec, uint16_t height);

#endif



