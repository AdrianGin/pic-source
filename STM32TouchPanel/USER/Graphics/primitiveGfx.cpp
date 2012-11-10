

#include <stdint.h>
#include <stdlib.h>

#include "primitiveGfx.h"
#include "GLCD.h"

uint16_t GlobalColour = Red;

void SetGlobalColour(uint16_t colour)
{
	GlobalColour = colour;
}

void setPixel(int x, int y)
{
	LCD_SetPoint(x,y, GlobalColour);
	//LCD_DrawLine(x, y, x, y, Red);
}


void drawLine(int x, int y, int ex, int ey)
{
	//Dont even draw it if it's out of bounds.
	if( x < 0 && ex < 0)
		return;

	if( y < 0 && ey < 0)
		return;

	if( x < 0)
	{
		x = 0;
	}

	if( ex < 0)
	{
		ex = 0;
	}

	if( y < 0)
	{
		y = 0;
	}

	if( ey < 0)
	{
		ey = 0;
	}


	LCD_DrawLine(x, y, ex, ey, GlobalColour);
}


void fillCircle(int x0, int y0, int radius)
{
	  int f = 1 - radius;
	  int ddF_x = 1;
	  int ddF_y = -2 * radius;
	  int x = 0;
	  int y = radius;


	  drawLine(x0, y0 - radius, x0, y0 + radius);
	  drawLine(x0 - radius, y0, x0  + radius, y0);


//	  setPixel(x0, y0 + radius);
//	  setPixel(x0, y0 - radius);
//	  setPixel(x0 + radius, y0);
//	  setPixel(x0 - radius, y0);

	  while(x < y)
	  {
	    // ddF_x == 2 * x + 1;
	    // ddF_y == -2 * y;
	    // f == x*x + y*y - radius*radius + 2*x - y + 1;
	    if(f >= 0)
	    {
	      y--;
	      ddF_y += 2;
	      f += ddF_y;
	    }
	    x++;
	    ddF_x += 2;
	    f += ddF_x;

	    drawLine(x0 - x, y0 + y, x0 + x, y0 + y);
	    drawLine(x0 - x, y0 - y, x0 + x, y0 - y);

	    drawLine(x0 - y, y0 + x, x0 + y, y0 + x);
	    drawLine(x0 - y, y0 - x, x0 + y, y0 - x);

//	    setPixel(x0 + x, y0 + y);
//	    setPixel(x0 - x, y0 + y);

//	    setPixel(x0 + x, y0 - y);
//	    setPixel(x0 - x, y0 - y);
//
//	    setPixel(x0 + y, y0 + x);
//	    setPixel(x0 - y, y0 + x);
//
//	    setPixel(x0 + y, y0 - x);
//	    setPixel(x0 - y, y0 - x);
	  }
}

void rasterCircle(int x0, int y0, int radius)
{
  int f = 1 - radius;
  int ddF_x = 1;
  int ddF_y = -2 * radius;
  int x = 0;
  int y = radius;



  setPixel(x0, y0 + radius);
  setPixel(x0, y0 - radius);
  setPixel(x0 + radius, y0);
  setPixel(x0 - radius, y0);

  while(x < y)
  {
    // ddF_x == 2 * x + 1;
    // ddF_y == -2 * y;
    // f == x*x + y*y - radius*radius + 2*x - y + 1;
    if(f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    setPixel(x0 + x, y0 + y);
    setPixel(x0 - x, y0 + y);
    setPixel(x0 + x, y0 - y);
    setPixel(x0 - x, y0 - y);
    setPixel(x0 + y, y0 + x);
    setPixel(x0 - y, y0 + x);
    setPixel(x0 + y, y0 - x);
    setPixel(x0 - y, y0 - x);
  }
}

// 'cx' and 'cy' denote the offset of the circle center from the origin.
void circle(int cx, int cy, int radius)
{
  int error = -radius;
  int x = radius;
  int y = 0;

  // The following while loop may be altered to 'while (x > y)' for a
  // performance benefit, as long as a call to 'plot4points' follows
  // the body of the loop. This allows for the elimination of the
  // '(x != y)' test in 'plot8points', providing a further benefit.
  //
  // For the sake of clarity, this is not shown here.
  while (x >= y)
  {
    plot8points(cx, cy, x, y);

    error += y;
    ++y;
    error += y;

    // The following test may be implemented in assembly language in
    // most machines by testing the carry flag after adding 'y' to
    // the value of 'error' in the previous step, since 'error'
    // nominally has a negative value.
    if (error >= 0)
    {
      error -= x;
      --x;
      error -= x;
    }
  }
}

void plot8points(int cx, int cy, int x, int y)
{
  plot4points(cx, cy, x, y);
  if (x != y) plot4points(cx, cy, y, x);
}

// The '(x != 0 && y != 0)' test in the last line of this function
// may be omitted for a performance benefit if the radius of the
// circle is known to be non-zero.
void plot4points(int cx, int cy, int x, int y)
{
  setPixel(cx + x, cy + y);
  if (x != 0) setPixel(cx - x, cy + y);
  if (y != 0) setPixel(cx + x, cy - y);
  if (x != 0 && y != 0) setPixel(cx - x, cy - y);
}












