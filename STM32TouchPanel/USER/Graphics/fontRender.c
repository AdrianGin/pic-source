
#include <stdlib.h>

#include "fontRender.h"
#include "LCDAbstraction.h"


// Current line type
SHORT   _lineType;

// Current line thickness
BYTE    _lineThickness;

// Font orientation
BYTE    _fontOrientation;

// Current cursor x-coordinates
SHORT   _cursorX;

// Current cursor y-coordinates
SHORT   _cursorY;

// information on currently set font
GFX_FONT_CURRENT currentFont;


#ifdef USE_ANTIALIASED_FONTS
static GFX_COLOR   _fgcolor100;
static GFX_COLOR   _fgcolor25;
static GFX_COLOR   _fgcolor75;
static GFX_COLOR   _bgcolor100;
static GFX_COLOR   _bgcolor25;
static GFX_COLOR   _bgcolor75;

/*********************************************************************
* Function: static void __attribute__((always_inline)) calculateColors(void)
*
* PreCondition: _bgcolor100 and _fgcolor100 must be set
*
* Input: none
*
* Output: none
*
* Side Effects: _bgcolor25, _bgcolor75, _fgcolor25 and _fgcolor75 will be calculated
*
* Overview: calculates mid values of colors
*
* Note: Internal to this file
*
********************************************************************/
__inline static void calculateColors(void)
{
    GFX_COLOR   _fgcolor50;
    GFX_COLOR   _bgcolor50;

    _fgcolor50  = (_fgcolor100 &  0xF7DE) >> 1;
    _fgcolor25  = (_fgcolor50  &  0xF7DE) >> 1;
    _fgcolor75  = _fgcolor50   +  _fgcolor25;

    _bgcolor50  = (_bgcolor100 &  0xF7DE) >> 1;
    _bgcolor25  = (_bgcolor50  &  0xF7DE) >> 1;
    _bgcolor75  = _bgcolor50   +  _bgcolor25;

    _fgcolor25 += _bgcolor75;
    _fgcolor75 += _bgcolor25;
}

#endif //#ifdef USE_ANTIALIASED_FONTS


/*********************************************************************
* Function: WORD OutChar(XCHAR ch)
*
* PreCondition: none
*
* Input: character code
*
* Output: For NON-Blocking configuration:
*         - Returns 0 when device is busy and the character is not yet completely drawn.
*         - Returns 1 when the character is completely drawn.
*         For Blocking configuration:
*         - Always return 1.
*
* Side Effects: none
*
* Overview: outputs a character
*
* Note: none
*
********************************************************************/
WORD OutChar(XCHAR ch)
{
    static OUTCHAR_PARAM OutCharParam;

    // initialize variables

    OutCharParam.pChTable = NULL;
    OutCharParam.pChTableExtended = NULL;
    OutCharParam.xAdjust = 0;
    OutCharParam.yAdjust = 0;
    OutCharParam.xWidthAdjust = 0;
    OutCharParam.heightOvershoot = 0;

    // check for error conditions (a return value of 0xFFFF means error)
    if((UXCHAR)ch < (UXCHAR)currentFont.fontHeader.firstChar)
        return (-1);
    if((UXCHAR)ch > (UXCHAR)currentFont.fontHeader.lastChar)
        return (-1);
#ifndef USE_ANTIALIASED_FONTS
    if(currentFont.fontHeader.bpp > 1)
        return (-1);
#endif

    switch(*((SHORT *)currentFont.pFont))
    {

        case FLASH:
            OutCharGetInfoFlash(ch, &OutCharParam);
            break;

        default:
            return 1;
    }

    return (OutCharRender(ch, &OutCharParam));
}



/*********************************************************************
* Function: void SetFontFlash(void* pFont)
*
* PreCondition: none
*
* Input: pFont - pointer to the font image in FLASH.
*
* Output: none
*
* Side Effects: none
*
* Overview: Sets the current font located in FLASH.
*
* Note: This function has a weak attribute, the driver layer
*       may implement this same function to support driver layer
*       features.
*
********************************************************************/
void SetFontFlash(void *pFont)
{
    FONT_HEADER *pHeader;
    pHeader = (FONT_HEADER *) ((FONT_FLASH *)pFont)->address;
    memcpy(&(currentFont.fontHeader), pHeader, sizeof(FONT_HEADER));
    currentFont.pFont = pFont;
}

/*********************************************************************
* Function:  void OutCharGetInfoFlash (XCHAR ch, OUTCHAR_PARAM *pParam)
*
* PreCondition: none
*
* Input: ch - character code
*        pParam - pointer to character information structure.
*
* Output: none
*
* Side Effects: none
*
* Overview: Gathers the parameters for the specified character of the
*           currently set font from flash memory. This is a step performed
*           before the character is rendered.
*
* Note: Application should not call this function. This function is for
*       versatility of implementing hardware accelerated text rendering
*       only.
*
********************************************************************/
void OutCharGetInfoFlash (XCHAR ch, OUTCHAR_PARAM *pParam)
{
    GLYPH_ENTRY_EXTENDED    *pChTableExtended;
    GLYPH_ENTRY             *pChTable;

    // set color depth of font,
    // based on 2^bpp where bpp is the color depth setting in the FONT_HEADER
    pParam->bpp = 1 << currentFont.fontHeader.bpp;

    if(currentFont.fontHeader.extendedGlyphEntry)
    {
        pChTableExtended = (GLYPH_ENTRY_EXTENDED *) (((FONT_FLASH *)currentFont.pFont)->address + sizeof(FONT_HEADER)) + ((UXCHAR)ch - (UXCHAR)currentFont.fontHeader.firstChar);
        pParam->pChImage = (BYTE *) (((FONT_FLASH *)currentFont.pFont)->address + pChTableExtended->offset);
        pParam->chGlyphWidth = pChTableExtended->glyphWidth;
        pParam->xWidthAdjust = pChTableExtended->glyphWidth - pChTableExtended->cursorAdvance;
        pParam->xAdjust = pChTableExtended->xAdjust;
        pParam->yAdjust = pChTableExtended->yAdjust;

        if(pParam->yAdjust > 0)
        {
            pParam->heightOvershoot = pParam->yAdjust;
        }
    }
    else
    {
        pChTable = (GLYPH_ENTRY *) (((FONT_FLASH *)currentFont.pFont)->address + sizeof(FONT_HEADER)) + ((UXCHAR)ch - (UXCHAR)currentFont.fontHeader.firstChar);
        pParam->pChImage = (BYTE *) (((FONT_FLASH *)currentFont.pFont)->address + ((DWORD)(pChTable->offsetMSB) << 8) + pChTable->offsetLSB);
        pParam->chGlyphWidth = pChTable->width;
    }
}









/*********************************************************************
* Function: WORD OutText(XCHAR* textString)
*
* PreCondition: none
*
* Input: textString - pointer to text string
*
* Output: non-zero if drawing done (used for NON-BLOCKING configuration)
*
* Side Effects: none
*
* Overview: outputs text from current position
*
* Note: none
*
********************************************************************/
WORD OutText(XCHAR *textString)
{
    #ifndef USE_NONBLOCKING_CONFIG

    XCHAR   ch;
    while((UXCHAR)15 < (UXCHAR)(ch = *textString++))
        while(OutChar(ch) == 0);
    return (1);

    #else

    XCHAR       ch;
    static WORD counter = 0;

    while((UXCHAR)(ch = *(textString + counter)) > (UXCHAR)15)
    {
        if(OutChar(ch) == 0)
            return (0);
        counter++;
    }

    counter = 0;
    return (1);
    #endif
}

/*********************************************************************
* Function: WORD OutTextXY(SHORT x, SHORT y, XCHAR* textString)
*
* PreCondition: none
*
* Input: x,y - starting coordinates, textString - pointer to text string
*
* Output: non-zero if drawing done (used for NON-BLOCKING configuration)
*
* Side Effects: none
*
* Overview: outputs text from x,y position
*
* Note: none
*
********************************************************************/
WORD OutTextXY(SHORT x, SHORT y, XCHAR *textString)
{
    #ifndef USE_NONBLOCKING_CONFIG
    MoveTo(x, y);
    OutText(textString);
    return (1);

    #else

    static BYTE start = 1;

    if(start)
    {
        MoveTo(x, y);
        start = 0;
    }

    if(OutText(textString) == 0)
    {
        return (0);
    }
    else
    {
        start = 1;
        return (1);
    }

    #endif
}










/*********************************************************************
* Function:  void OutCharRender(XCHAR ch, OUTCHAR_PARAM *pParam)
*
* PreCondition: none
*
* Input: ch - character code
*        pParam - pointer to character information structure.
*
* Output: none
*
* Side Effects: none
*
* Overview: Performs the actual rendering of the character using PutPixel().
*
* Note: Application should not call this function. This function is for
*       versatility of implementing hardware accelerated text rendering
*       only.
*
********************************************************************/
WORD OutCharRender(XCHAR ch, OUTCHAR_PARAM *pParam)
{
    BYTE        temp = 0;
    BYTE        mask;
    BYTE        restoremask;
    SHORT       xCnt, yCnt, x = 0, y;
#ifdef USE_ANTIALIASED_FONTS
    BYTE        val, shift;
    GFX_COLOR   bgcolor;
#endif

#ifdef USE_ANTIALIASED_FONTS
    if(pParam->bpp == 1)
    {
        restoremask = 0x01;
    }
    else
    {
        if(pParam->bpp == 2)
        {
            restoremask = 0x03;
        }
        else
        {
            return -1; // BPP > 2 are not yet supported
        }

        bgcolor = GetPixel(GetX(), GetY() + (currentFont.fontHeader.height >> 1));

        if((_fgcolor100 != GetColor()) || (_bgcolor100 != bgcolor))
        {
            _fgcolor100 = GetColor();
            _bgcolor100 = bgcolor;
            calculateColors();
        }
    }
#else
    restoremask = 0x01;
#endif

    if(_fontOrientation == ORIENT_HOR)
    {
        y = GetY() + pParam->yAdjust;
        for(yCnt = 0; yCnt < currentFont.fontHeader.height + pParam->heightOvershoot; yCnt++)
        {
            x = GetX() + pParam->xAdjust;
            mask = 0;

#ifdef USE_ANTIALIASED_FONTS
            shift = 0;
#endif

            for(xCnt = 0; xCnt < pParam->chGlyphWidth; xCnt++)
            {
                if(mask == 0)
                {
                    temp = *(pParam->pChImage)++;
                    mask = restoremask;

#ifdef USE_ANTIALIASED_FONTS
                    shift = 0;
#endif
                }

#ifdef USE_ANTIALIASED_FONTS
                if(pParam->bpp == 1)
                {
                    if(temp & mask)
                    {
                        PutPixel(x, y);
                    }
                }
                else
                {
                    val = (temp & mask) >> shift;
                    if(val)
                    {
                        if(GFX_Font_GetAntiAliasType() == ANTIALIAS_TRANSLUCENT)
                        {
                            bgcolor = GetPixel(x, y);
                            if(_bgcolor100 != bgcolor)
                            {
                                _bgcolor100 = bgcolor;
                                calculateColors();
                            }
                        }

                        switch(val)
                        {
                            case 1:     SetColor(_fgcolor25);
                                        break;

                            case 2:     SetColor(_fgcolor75);
                                        break;

                            case 3:     SetColor(_fgcolor100);
                        }

                        PutPixel(x, y);
                    }
                }

                mask  <<=  pParam->bpp;
                shift  +=  pParam->bpp;
#else
                if(temp & mask)
                {
                    PutPixel(x, y);
                }

                mask <<= 1;
#endif
                x++;
            }
            y++;
        }

        // move cursor
        _cursorX = x - pParam->xAdjust - pParam->xWidthAdjust;
    }
    else    // If extended glyph is used, then vertical alignment may not be rendered properly and hence users must position the texts properly
    {
        y = GetX() + pParam->xAdjust;
        for(yCnt = 0; yCnt < currentFont.fontHeader.height + pParam->heightOvershoot; yCnt++)
        {
            x = GetY() + pParam->yAdjust;
            mask = 0;

#ifdef USE_ANTIALIASED_FONTS
            shift = 0;
#endif

            for(xCnt = 0; xCnt < pParam->chGlyphWidth; xCnt++)
            {
                if(mask == 0)
                {
                    temp = *(pParam->pChImage)++;
                    mask = restoremask;

#ifdef USE_ANTIALIASED_FONTS
                    shift = 0;
#endif
                }

#ifdef USE_ANTIALIASED_FONTS
                if(pParam->bpp == 1)
                {
                    if(temp & mask)
                    {
                        PutPixel(y, x);
                    }
                }
                else
                {
                    val = (temp & mask) >> shift;
                    if(val)
                    {
                        if(GFX_Font_GetAntiAliasType() == ANTIALIAS_TRANSLUCENT)
                        {
                            bgcolor = GetPixel(x, y);
                            if(_bgcolor100 != bgcolor)
                            {
                                _bgcolor100 = bgcolor;
                                calculateColors();
                            }
                        }
                        switch(val)
                        {
                            case 1: SetColor(_fgcolor25);
                                    break;

                            case 2: SetColor(_fgcolor75);
                                    break;

                            case 3: SetColor(_fgcolor100);
                        }
                        PutPixel(y, x);
                    }
                }

                mask  <<=  pParam->bpp;
                shift  +=  pParam->bpp;
#else
                if(temp & mask)
                {
                    PutPixel(y, x);
                }
                mask  <<=  1;
#endif
                x--;
            }
            y++;
        }

        // move cursor
        _cursorY = x - pParam->xAdjust;
    }

    // restore color
#ifdef USE_ANTIALIASED_FONTS
    if(pParam->bpp > 1)
    {
        SetColor(_fgcolor100);
    }
#endif
    return (1);

}
