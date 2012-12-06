

#include "Graphics/BMPDraw.h"
#include <stdint.h>

#include "LPD8806/LPD8806.h"

volatile uint8_t DALED_NextInput;

uint8_t DALED_BlurMode = 0;

uint8_t DALED_WaitForInput(void)
{
	while(DALED_NextInput==0)
	{

	}
	DALED_NextInput = 0;
}



void DALED_BMP_Print(BMPFile_t* pBmpDec)
{

	uint16_t j,k;
	uint16_t pixelsRead;
	uint16_t cursorY;
	uint8_t readBuf[512];

	uint16_t rowPos = 0;
	uint16_t Ystart = LED_COUNT-1;


    cursorY = Ystart;

    for( j = 0; j < pBmpDec->lHeight; )
    {



    	//Keep getting data until no more row data is left.
    	pixelsRead = BMP_GetRowData_24BPP(pBmpDec, &readBuf[0], PIXEL_BUFFER_COUNT*BMP_24BPP, rowPos, j);
    	if( pixelsRead )
    	{
    		for(k = 0; k < pixelsRead; k++)
    		{
    			uint8_t r,g,b;
    			uint32_t pixel;

                b = readBuf[(3*k)+0];
                g = readBuf[(3*k)+1];
                r = readBuf[(3*k)+2];

                LPD8806_SetPixel(cursorY--, RGB(r, g, b));
    		}
    		rowPos = rowPos + pixelsRead;
    	}
    	else
    	{
    		//New row
    		rowPos = 0;
    		cursorY = Ystart;
    		j++;

    		if( DALED_BlurMode )
    		{
            	//Have new pictures
            	LPD8806_Update();
            	delay_ms(145);
    		}
    		else
    		{
            	//Have new pictures
            	LPD8806_Update();
            	delay_ms(97);

        		LPD8806_Clear();
        		LPD8806_Update();
        		delay_ms(48);
    		}



    	}
    }
}


//uint8_t DALED_BMP_Print(BMPFile_t* pBmpDec)
//{
//    uint16_t i,j,k;
//    uint8_t byte;
//    uint8_t r,g,b;
//    uint16_t pixel;
//    uint8_t readBuf[512];
//    uint8_t bufferIndex = PIXEL_BUFFER_COUNT;
//    uint32_t position = 0;
//    uint16_t cursorX,cursorY;
//
//    DALED_NextInput = 0;
//
//    if(pBmpDec->blBmMarkerFlag == 0 || pBmpDec->bHeaderType < 40 || (pBmpDec->blCompressionType != 0))
//    {
//        return BMP_BADFILE_FORMAT;
//    }
//
//    IMG_FSEEK(pBmpDec->pImageFile, pBmpDec->lImageOffset, 0);
//
//    cursorX = BMP_CursorX;
//    cursorY = BMP_CursorY;
//
//    for( j = 0; j < pBmpDec->lHeight; j++)
//    {
//
//
//
//    	for( k = 0; k < pBmpDec->lWidth; k++)
//        {
//            if( bufferIndex >= PIXEL_BUFFER_COUNT)
//            {
//                IMG_FREAD(readBuf, PIXEL_BUFFER_COUNT*3, 1, pBmpDec->pImageFile);
//                bufferIndex = 0;
//            }
//
//            b = readBuf[(3*bufferIndex)+0];
//            g = readBuf[(3*bufferIndex)+1];
//            r = readBuf[(3*bufferIndex)+2];
//
////            pixel = b >> 3;
////            pixel = pixel | ((g>>2)<<5);
////            pixel = pixel | ((r>>3)<<11);
//
//
//            LPD8806_SetPixel(cursorY, RGB(r, g, b));
////            SetColor(pixel);
////            PutPixel(cursorX , cursorY);
//
//            if( (BMP_Direction[0] == 1) && (BMP_Direction[1] == 1))
//            {
//            	cursorY = cursorY + BMP_Direction[1];
//            }
//            else
//            {
//            	cursorX = cursorX + BMP_Direction[0];
//            	//PutPixel(cursorY , cursorX);
//            }
//            //LCD_WriteData(pixel);
//            bufferIndex = bufferIndex + 1;
//        }
//
//        if( (BMP_Direction[0] == 1) && (BMP_Direction[1] == 1))
//        {
//        	cursorX = cursorX + BMP_Direction[0];
//        	cursorY = BMP_CursorY;
//
//        }
//        else
//        {
//        	cursorY = cursorY + BMP_Direction[1];
//        	cursorX = BMP_CursorX;
//        }
//
//    }
//    return 0;
//}
