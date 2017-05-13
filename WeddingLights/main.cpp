
#include "hw_config.h"
#include "LPD8806/LPD8806.h"
#include "SysTick/systick.h"

#include "ShootingStars.h"

int main(void)
{

   ShootingStar* red = new ShootingStar(0, LED_COUNT/2, RGB(255,0,0), 20, 127, ShootingStar::FORWARD, ShootingStar::FORWARD);


   ShootingStar* blue = new ShootingStar(0, LED_COUNT/2, RGB(0,0,255), 10, 64, ShootingStar::FORWARD, ShootingStar::FORWARD);

   ShootingStar* green = new ShootingStar(0, LED_COUNT, RGB(127,0,127), 5, 200, ShootingStar::FORWARD, ShootingStar::FORWARD);


   red->init();
   blue->init();
   green->init();



   delay_init();

   LPD8806_Init();
   LPD8806_Test();

   LPD8806_Update();

   while(1)
   {
      red->update();
      blue->update();
      green->update();

      for( uint16_t j = 0; j < red->length; ++j)
      {
         uint32_t* memBuf = red->getBuffer();
         LPD8806_SetPixel(j, memBuf[j]);
      }

      for( uint16_t j = 0; j < blue->length; ++j)
      {
         uint32_t* memBuf = blue->getBuffer();
         LPD8806_SetPixel(j + LED_COUNT/2, memBuf[j]);
      }

      for( uint16_t j = 0; j < green->length; ++j)
      {
         uint32_t* memBuf = green->getBuffer();
         LPD8806_SetPixel(j, memBuf[j]);
      }

      if( red->isFinished() )
      {
         red->reverseDir();
      }

      if( blue->isFinished() )
      {
         blue->reverseDir();
      }

      if( green->isFinished() )
      {
         green->reverseDir();
      }


      LPD8806_Update();

   }
   return 0;
}
