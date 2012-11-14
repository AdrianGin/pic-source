


#include <stdint.h>

void gfxEngine_Init(void);
void gfxWrite(uint8_t ch);
void gfxWriteString(int16_t x, int16_t y, uint8_t* string);
void gfxDrawBMP(char* bmpImgPath);

void gfxDrawRect(int16_t left, int16_t top, int16_t right, int16_t bottom);

