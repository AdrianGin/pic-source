/*
 * rgbutils.h
 *
 *  Created on: 20/02/2013
 *      Author: Adrian
 */

#ifndef RGBUTILS_H_
#define RGBUTILS_H_


//assumes 24bit, 888 RGB
#define MAX_COLOUR_VALUE	(255)
#define R_RGB(colour)	((colour & 0x00FF0000) >> 16)
#define G_RGB(colour)	((colour & 0x0000FF00) >> 8)
#define B_RGB(colour)	((colour & 0x000000FF))

#define NORMALISE_COLOUR(r,g,b)

#define RGB(r,g,b)		((r << 16) | (g << 8) | (b))


#define SCALE(number, factor, divisor)	((number * factor) / divisor )
#define SCALE_COLOUR(colour, factor, divisor)	RGB( SCALE(LPD_RED(colour), factor, divisor), \
													 SCALE(LPD_GREEN(colour), factor, divisor), \
													 SCALE(LPD_BLUE(colour), factor, divisor))



#endif /* RGBUTILS_H_ */
