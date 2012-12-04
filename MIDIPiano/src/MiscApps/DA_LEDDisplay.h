


#include <stdint.h>
#include "Graphics/BMPDraw.h"

extern volatile uint8_t DALED_NextInput;
extern uint8_t DALED_BlurMode;

void DALED_WaitForInput(void);
void DALED_BMP_Print(BMPFile_t* pBmpDec);



