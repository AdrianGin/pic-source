

#include "BMPDraw.h"
#include "hardwareSpecific.h"
#include "FatFS/ff.h"

#include "SSD1289/SSD1289.h"


void* _BMP_readbuf(FIL* ptr, void* buf, uint16_t size)
{
    uint16_t br;
    
    f_read(ptr, buf, size, &br);
    
    return buf;
}

uint8_t BMP_ReadHeader(BMPFile_t* pBmpDec)
{

    uint8_t bByte1, bByte2;
    uint16_t wWord;
    uint32_t lLong;

    IMG_FSEEK(pBmpDec->pImageFile, 0, 0);

    IMG_FREAD(&bByte1, sizeof(bByte1), 1, pBmpDec->pImageFile);  /* Marker */
    IMG_FREAD(&bByte2, sizeof(bByte2), 1, pBmpDec->pImageFile);  /* Marker */

    if(bByte1 == 'B' && bByte2 == 'M')
    {
              pBmpDec->blBmMarkerFlag = 1;
    }
    else
    {
              return(BMP_BADFILE_FORMAT);
    }

    IMG_FREAD(&lLong, sizeof(lLong), 1, pBmpDec->pImageFile);  /* File length */
    IMG_FREAD(&wWord, sizeof(wWord), 1, pBmpDec->pImageFile);  /* Reserved */
    IMG_FREAD(&wWord, sizeof(wWord), 1, pBmpDec->pImageFile);  /* Reserved */

    IMG_FREAD(&lLong, sizeof(lLong), 1, pBmpDec->pImageFile);  /* Image offset */
    pBmpDec->lImageOffset = lLong;

    IMG_FREAD(&lLong, sizeof(lLong), 1, pBmpDec->pImageFile);  /* Header length */
    pBmpDec->bHeaderType = (BYTE)lLong;

    if(pBmpDec->bHeaderType >= 40)
    {
              IMG_FREAD(&lLong, sizeof(lLong), 1, pBmpDec->pImageFile);  /* Image Width */
              pBmpDec->lWidth = lLong;

              IMG_FREAD(&lLong, sizeof(lLong), 1, pBmpDec->pImageFile);  /* Image Height */
              pBmpDec->lHeight = lLong;

              IMG_FREAD(&wWord, sizeof(wWord), 1, pBmpDec->pImageFile);  /* Number of Planes */
              pBmpDec->bNumOfPlanes = (BYTE)wWord;

              IMG_FREAD(&wWord, sizeof(wWord), 1, pBmpDec->pImageFile);  /* Bits per Pixel */
              pBmpDec->bBitsPerPixel = (BYTE)wWord;

              IMG_FREAD(&lLong, sizeof(lLong), 1, pBmpDec->pImageFile);  /* Compression info */
              pBmpDec->blCompressionType = (BYTE)lLong;

              IMG_FREAD(&lLong, sizeof(lLong), 1, pBmpDec->pImageFile);  /* Image length */
              IMG_FREAD(&lLong, sizeof(lLong), 1, pBmpDec->pImageFile);  /* xPixels per metre */
              IMG_FREAD(&lLong, sizeof(lLong), 1, pBmpDec->pImageFile);  /* yPixels per metre */

              IMG_FREAD(&lLong, sizeof(lLong), 1, pBmpDec->pImageFile);  /* Palette entries */
              pBmpDec->wPaletteEntries = (WORD)lLong;

              if(pBmpDec->wPaletteEntries == 0)
              {
                      WORD wTemp = (WORD)(pBmpDec->lImageOffset - 14 - 40)/4;
                      if(wTemp > 0)
                      {
                               pBmpDec->wPaletteEntries = wTemp; /* This is because of a bug in MSPAINT */
                      }
              }

              IMG_FREAD(&lLong, sizeof(lLong), 1, pBmpDec->pImageFile);  /* Important colors */
              if(pBmpDec->bBitsPerPixel == 16 && pBmpDec->bHeaderType > 40)
              {
                      IMG_FREAD(&lLong, sizeof(lLong), 1, pBmpDec->pImageFile);  /* Red mask */
                      if((WORD)lLong == 0xF800)
                      {
                                 pBmpDec->b16bit565flag = 1;
                      }
              }
    }
    return(0);


}




uint8_t BMP_Print(BMPFile_t* pBmpDec)
{
    
    if(pBmpDec->blBmMarkerFlag == 0 || pBmpDec->bHeaderType < 40 || (pBmpDec->blCompressionType != 0))
    {
        return BMP_BADFILE_FORMAT;
    }

    uint16_t i,j,k;
    
    uint8_t byte;

    uint8_t r,g,b;
    uint16_t pixel;

#define PIXEL_BUFFER_COUNT (128)
    uint8_t* readBuf = &DMASPI1.DMABuffer[512];
    uint8_t bufferIndex = PIXEL_BUFFER_COUNT;
    uint32_t position = 0;

    IMG_FSEEK(pBmpDec->pImageFile, pBmpDec->lImageOffset, 0);


    if( pBmpDec->lHeight < pBmpDec->lWidth)
    {
        SSD1289_SendCommand(0x0011,0x6040 | (1<<3));    DELAY_US(1);
    }
    else
    {
        SSD1289_SendCommand(0x0011,0x6040);    DELAY_US(1);
    }

    SSD1289_PrepareNewScreen();


    for( j = 0; j < pBmpDec->lHeight; j++)
    {
        for( k = 0; k < pBmpDec->lWidth; k++)
        {
            if( bufferIndex >= PIXEL_BUFFER_COUNT)
            {
                IMG_FREAD(readBuf, PIXEL_BUFFER_COUNT*3, 1, pBmpDec->pImageFile);
                bufferIndex = 0;
            }

            b = readBuf[(3*bufferIndex)+0];
            g = readBuf[(3*bufferIndex)+1];
            r = readBuf[(3*bufferIndex)+2];

            pixel = b >> 3;
            pixel = pixel | ((g>>2)<<5);
            pixel = pixel | ((r>>3)<<11);
            SSD1289_PutPixel(pixel);
            bufferIndex++;
        }
    }

    return 0;
}

