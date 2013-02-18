
#include <stdint.h>
#include "BMPDraw.h"
#include "LCDAbstraction.h"

#include "fontRender.h"

#include "GLCD/GLCD.h"


uint16_t BMP_CursorX;
uint16_t BMP_CursorY;

uint16_t BMP_Scale;
int16_t BMP_Direction[2];
uint16_t BMP_Rotation;

uint8_t readBuf[512];

void* _BMP_readbuf(FIL* ptr, void* buf, uint16_t size)
{
	UINT br;
    f_read(ptr, buf, size, (UINT*)&br);
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



void BMP_SetCursor(uint16_t x, uint16_t y)
{
	BMP_CursorX = x;
	BMP_CursorY = y;
}

void BMP_SetRotation(int16_t dirX, int16_t dirY, uint16_t rotation)
{
	BMP_Direction[0] = dirX;
	BMP_Direction[1] = dirY;
	BMP_Rotation = rotation;
}


void BMP_SetScaling(uint16_t scale)
{
	BMP_Scale = scale;
}


void BMP_PrintToLCD(BMPFile_t* pBmpDec)
{

	uint16_t j,k;
	uint16_t pixelsRead;
	uint16_t cursorX,cursorY;


	uint16_t rowPos = 0;

    cursorX = BMP_CursorX;
    cursorY = BMP_CursorY;

    LCD_SetCursor(cursorX + pBmpDec->lWidth, cursorY + pBmpDec->lHeight);

    for( j = 0; j < pBmpDec->lHeight; )
    {
    	//Keep getting data until no more row data is left.
    	pixelsRead = BMP_GetRowData_24BPP(pBmpDec, &readBuf[0], PIXEL_BUFFER_COUNT*BMP_24BPP, rowPos, j);

    	LCD_WriteIndex(0x0022);
    	if( pixelsRead )
    	{
    		for(k = 0; k < pixelsRead; k++)
    		{
    			uint8_t r,g,b;
    			uint32_t pixel;

                b = readBuf[(3*k)+0];
                g = readBuf[(3*k)+1];
                r = readBuf[(3*k)+2];

                pixel = b >> 3;
                pixel = pixel | ((g>>2)<<5);
                pixel = pixel | ((r>>3)<<11);

                SetColor(pixel);
                if( BMP_Rotation == 90 )
                {
                	PutPixel(cursorY, cursorX);
                }
                else
                {
                	PutPixel(cursorX , cursorY);
                }
                //LCD_WriteData(pixel);


                if( (BMP_Direction[0] == 1) && (BMP_Direction[1] == 1))
                {
                	cursorY = cursorY + BMP_Direction[1];
                }
                else
                {
                	cursorX = cursorX + BMP_Direction[0];
                }
    		}
    		rowPos = rowPos + pixelsRead;
    	}
    	else
    	{
    		//New row
    		rowPos = 0;
            if( (BMP_Direction[0] == 1) && (BMP_Direction[1] == 1))
            {
            	cursorX = cursorX + BMP_Direction[0];
            	cursorY = BMP_CursorY;
            }
            else
            {
            	cursorY = cursorY + BMP_Direction[1];
            	cursorX = BMP_CursorX;
            }

    		//cursorY--;
    		LCD_SetCursor(cursorX + pBmpDec->lWidth, cursorY + pBmpDec->lHeight);

    		j++;
    	}
    }
}


uint16_t BMP_GetRowData_24BPP(BMPFile_t* pBmpDec, uint8_t* readBuf, uint16_t bufSize, uint16_t rowPos, uint16_t height)
{

	uint16_t pixelsToRead;
	if( rowPos >=  pBmpDec->lWidth)
	{
		return 0;
	}

	if( BMP_SetHeightPointer_24BPP(pBmpDec, height) )
	{
		//Goto requested row start position.
		IMG_FSEEK(pBmpDec->pImageFile, IMG_FTELL(pBmpDec->pImageFile)+(rowPos*BMP_24BPP), 0);
		//Read in a maximum of 'bufSize' bytes or until the end of row, whichever is greater.
		pixelsToRead = pBmpDec->lWidth - rowPos;
		if( pixelsToRead >= bufSize/BMP_24BPP)
		{
			pixelsToRead = bufSize/BMP_24BPP;
		}
		IMG_FREAD(readBuf, pixelsToRead*BMP_24BPP, 1, pBmpDec->pImageFile);
		return pixelsToRead;
	}

	return 0;
}


uint8_t BMP_SetHeightPointer_24BPP(BMPFile_t* pBmpDec, uint16_t height)
{
	uint32_t imgOffset;

	imgOffset = pBmpDec->lImageOffset;
	if( height )
	{
		imgOffset = imgOffset + (height * (pBmpDec->lWidth*BMP_24BPP + (pBmpDec->lWidth & 0x03)));
	}


	if( height < pBmpDec->lHeight)
	{
		IMG_FSEEK(pBmpDec->pImageFile, imgOffset, 0);
		return 1;
	}

	//Invalid height. Height overrun!
	return 0;
}


