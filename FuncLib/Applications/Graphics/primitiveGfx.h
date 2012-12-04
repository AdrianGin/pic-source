#ifdef __cplusplus    /* Insert start of extern C construct */
extern "C" {
#endif

#include <stdint.h>




void setPixel(int x, int y);
void drawLine(int x, int y, int ex, int ey);
void fillCircle(int x0, int y0, int radius);
void rasterCircle(int x0, int y0, int radius);
void circle(int cx, int cy, int radius);
void plot8points(int cx, int cy, int x, int y);
void plot4points(int cx, int cy, int x, int y);
void SetGlobalColour(uint16_t colour);

#ifdef __cplusplus  /* Insert end of extern C construct. */
}                   /* The C header file can now be */
#endif              /* included in either C or C++ code. */
