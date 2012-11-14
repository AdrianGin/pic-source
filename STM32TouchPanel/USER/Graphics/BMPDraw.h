#ifndef _BMP_DRAW_H
#define _BMP_DRAW_H

#include <stdint.h>
//Requires some sort of FAT file System
#include "ff.h"

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

#define BMP_BADFILE_FORMAT  (100)
#define IMG_FREAD(ptr, size, n, fileptr) _BMP_readbuf(fileptr, ptr, size)
#define IMG_FSEEK(ptr, offset, whence) f_lseek(ptr, offset)


uint8_t BMP_ReadHeader(BMPFile_t* pBmpDec);
uint8_t BMP_Print(BMPFile_t* pBmpDec);
void BMP_SetCursor(uint16_t x, uint16_t y);
void BMP_SetRotation(int16_t dirX, int16_t dirY);
void BMP_SetScaling(uint16_t scale);

#endif



