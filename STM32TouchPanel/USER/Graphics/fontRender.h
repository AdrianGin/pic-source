
#ifndef FONT_RENDER_H
#define FONT_RENDER_H

#include "GenericTypeDefs.h"

/*********************************************************************
* Overview: Primitive lines are drawn using line type and line thickness.
*			There are 3 line styles and 2 types of line thickness.
*
*********************************************************************/

#define USE_ANTIALIASED_FONTS
#define USE_FONT_FLASH
#define COLOR_DEPTH 16
#define USE_BLOCKING_CONFIG
/*********************************************************************
* Macros:  SetColor(color)
*
* Overview: Sets current drawing color.
*
* PreCondition: none
*
* Input: color - Color coding is based on GFX_COLOR definition. GFX_COLOR
*                definition is based on the color depth (COLOR_DEPTH) used.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
#define SetColor(color) _color = (color)

/*********************************************************************
* Macros:  GetColor()
*
* Overview: Returns current drawing color.
*
* PreCondition: none
*
* Input: none
*
* Output: Color where coding is based on GFX_COLOR definition. GFX_COLOR
*         definition is based on the color depth (COLOR_DEPTH) used.
*
* Side Effects: none
*
********************************************************************/
#define GetColor() _color


// Solid Line Style
    #define SOLID_LINE  0

// Dotted Line Style
    #define DOTTED_LINE 1

// Dashed Line Style
    #define DASHED_LINE 4

// Normal Line (thickness is 1 pixel)
    #define NORMAL_LINE 0

// Thick Line (thickness is 3 pixels)
    #define THICK_LINE  1

/*********************************************************************
* Overview: Drawing bitmaps will have two modes, normal rendering
*			and stretched rendering. Stretched rendering effectively
*			doubles the image size in the horizontal and vertical
*			direction.
*
*********************************************************************/

// Normal image stretch code
    #define IMAGE_NORMAL    1

// Stretched image stretch code
    #define IMAGE_X2    2

// Current line style
extern SHORT    _lineType;

// Current line thickness
extern BYTE     _lineThickness;

// constants used for circle/arc computation
    #define SIN45   	46341   // sin(45) * 2^16)
    #define ONEP25  	81920   // 1.25 * 2^16
// constants used to get sine(v) and cosine(v)
	#define GETSINE		0x00
	#define GETCOSINE	0x01

// Current cursor coordinates
extern SHORT    _cursorX;
extern SHORT    _cursorY;

// Font orientation
extern BYTE     _fontOrientation;

#define ORIENT_HOR  0
#define ORIENT_VER  1

extern BYTE _antialiastype;
/*********************************************************************
* Overview: Fonts that enables anti-aliasing can be set to use
*           opaque or translucent type of anti-aliasing.
*
*********************************************************************/

// Mid colors are calculated only once while rendering each character. This is ideal for rendering text over a constant background.
    #define ANTIALIAS_OPAQUE        0

// Mid values are calculated for every necessary pixel. This feature is useful when rendering text over an image
// or when the background is not one flat color.
    #define ANTIALIAS_TRANSLUCENT   1

        #define XCHAR   char
        #define UXCHAR  unsigned char

typedef enum
{
	FLASH         = 0x0000,                 // internal flash
	EXTERNAL      = 0x0001,                 // external memory
	FLASH_JPEG    = 0x0002,                 // internal flash
	EXTERNAL_JPEG = 0x0003,                 // external memory
	RAM           = 0x0004,                 // RAM
	EDS_EPMP      = 0x0005,                 // memory in EPMP, base addresses are
                                            // are set in the hardware profile

	IMAGE_MBITMAP = 0x0000,                 // data resource is type Microchip bitmap
	IMAGE_JPEG    = 0x0100,                 // data resource is type JPEG

    COMP_NONE     = 0x0000,                 // no compression
	COMP_RLE      = 0x1000,                 // compressed with RLE
	COMP_IPU      = 0x2000,                 // compressed with DEFLATE (for IPU)
} GFX_RESOURCE;


/* &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */
//                         STRUCTURES FOR FONTS
/* &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& */

/*********************************************************************
* Overview: Structure describing the font header.
*
*********************************************************************/
typedef struct
{
    BYTE        fontID;                     // User assigned value
    BYTE        extendedGlyphEntry : 1;     // Extended Glyph entry flag. When set font has extended glyph feature enabled.
    BYTE        res1               : 1;     // Reserved for future use  (must be set to 0)
    BYTE        bpp                : 2;     // Actual BPP = 2^bpp
    BYTE        orientation        : 2;     // Orientation of the character glyphs (0,90,180,270 degrees)
                                            //   - 00 - Normal
                                            //   - 01 - Characters rotated 270 degrees clockwise
                                            //   - 10 - Characters rotated 180 degrees
                                            //   - 11 - Characters rotated 90 degrees clockwise
                                            // Note: Rendering DO NOT rotate the characters. The table contains rotated characters
                                            //       and will be rendered as is.
    BYTE        res2               : 2;     // Reserved for future use (must be set to 0).
    WORD        firstChar;                  // Character code of first character (e.g. 32).
    WORD        lastChar;                   // Character code of last character in font (e.g. 3006).
    WORD        height;                     // Font characters height in pixels.
} FONT_HEADER;

/*********************************************************************
* Overview: Structures describing the glyph entry.
*
*********************************************************************/
typedef struct
{
    BYTE        width;                      // width of the glyph
    BYTE        offsetLSB;                  // Least Significant Byte of the glyph location offset
    WORD        offsetMSB;                  // Most Significant (2) Bytes of the glyph location offset
} GLYPH_ENTRY;

typedef struct
{
    DWORD       offset;                     // Offset Order: LSW_LSB LSW_MSB MSW_MSB MSW_MSB
    WORD        cursorAdvance;              // x-value by which cursor has to advance after rendering the glyph
    WORD        glyphWidth;                 // width of the glyph
    INT16       xAdjust;                    // x-position is adjusted as per this signed number
    INT16       yAdjust;                    // y-position is adjusted as per this signed number
} GLYPH_ENTRY_EXTENDED;

/*********************************************************************
* Overview: Internal structure for currently set font.
*
*********************************************************************/
typedef struct
{
   void         *pFont;                     // pointer to the currently set font
   FONT_HEADER  fontHeader;                 // copy of the currently set font header
#ifdef USE_ANTIALIASED_FONTS
   BYTE  antiAliasType;                     // anti-alias type set
#endif
} GFX_FONT_CURRENT;

/*********************************************************************
* Overview: Structure for character information when rendering the character.
*
*********************************************************************/
typedef struct
{
#ifdef USE_FONT_FLASH
    GLYPH_ENTRY             *pChTable;
    GLYPH_ENTRY_EXTENDED    *pChTableExtended;
#endif
    BYTE                    bpp;
	SHORT                   chGlyphWidth;
    BYTE                    *pChImage;
    SHORT                   xAdjust;
    SHORT                   yAdjust;
    SHORT                   xWidthAdjust;
    SHORT                   heightOvershoot;
} OUTCHAR_PARAM;

/*********************************************************************
* Overview: Structure for font stored in FLASH memory.
*
*********************************************************************/
typedef struct
{
    GFX_RESOURCE  type;                     // must be FLASH
    const char    *address;                 // font image address in FLASH
} FONT_FLASH;


void SetFontFlash(void *pFont);
WORD OutCharRender(XCHAR ch, OUTCHAR_PARAM *pParam);
void OutCharGetInfoFlash (XCHAR ch, OUTCHAR_PARAM *pParam);
WORD OutChar(XCHAR ch);
WORD OutText(XCHAR *textString);
WORD OutTextXY(SHORT x, SHORT y, XCHAR *textString);

/*********************************************************************
* Macros:  GetX()
*
* Overview: This macro returns the current graphic cursor x-coordinate.
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define GetX()  _cursorX

/*********************************************************************
* Macros:  GetX()
*
* Overview: This macro returns the current graphic cursor y-coordinate.
*
* PreCondition: none
*
* Input: none
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
    #define GetY()  _cursorY

#define MoveTo(x, y) \
_cursorX = x;        \
_cursorY = y;

/*********************************************************************
* Macros:  MoveRel(dX,dY)
*
* Overview: This macro moves the graphic cursor relative to the
*			current location. The given dX and dY displacement can
*			be positive or negative numbers.
*
* PreCondition: none
*
* Input: dX - Specifies the displacement of the graphic cursor for
*			  the horizontal direction.
*		 dY - Specifies the displacement of the graphic cursor for
*			  the vertical direction.
*
* Output: none
*
* Side Effects: none
*
********************************************************************/
#define MoveRel(dX, dY) \
_cursorX += dX;         \
_cursorY += dY;



/*********************************************************************
* Macro: GFX_Font_GetAntiAliasType()
*
* Overview: Returns the font anti-alias type.
*
* PreCondition: Compiler switch USE_ANTIALIASED_FONTS must be enabled
*
* Input: none
*
* Output: Return the current font anti-alias type.
*        - ANTIALIAS_TRANSLUCENT - (or 1) when font anti-alias is type translucent
*        - ANTIALIAS_OPAQUE - (or 0) when font anti-alias is type opaque
*
********************************************************************/
    #ifdef USE_ANTIALIASED_FONTS
        #define GFX_Font_GetAntiAliasType()  ANTIALIAS_TRANSLUCENT
    #else
        #define GFX_Font_GetAntiAliasType()
    #endif




#endif



