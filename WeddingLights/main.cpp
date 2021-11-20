
#include <PatternManager.h>
#include "hw_config.h"
#include "LPD8806/LPD8806.h"
#include "SysTick/systick.h"

#include "ShootingStars.h"
#include "FadingStar.h"
#include "Strobe.h"
#include "XmasTwinkle.h"

#include "Modulator.h"
#include "RandomModulator.h"

#include <math.h>

#define STROBE_FADE_OUT (230)
#define STROBE_FADE_LENGTH (10)

const int8_t sinewave[] =
{
      0,
      3,
      6,
      9,
      12,
      16,
      19,
      22,
      25,
      28,
      31,
      34,
      37,
      40,
      43,
      46,
      49,
      52,
      54,
      57,
      60,
      63,
      66,
      68,
      71,
      73,
      76,
      78,
      81,
      83,
      86,
      88,
      90,
      92,
      94,
      96,
      98,
      100,
      102,
      104,
      106,
      108,
      109,
      111,
      112,
      114,
      115,
      116,
      118,
      119,
      120,
      121,
      122,
      123,
      123,
      124,
      125,
      125,
      126,
      126,
      126,
      127,
      127,
      127,
      127,
      127,
      127,
      127,
      126,
      126,
      125,
      125,
      124,
      124,
      123,
      122,
      121,
      120,
      119,
      118,
      117,
      116,
      114,
      113,
      112,
      110,
      108,
      107,
      105,
      103,
      101,
      99,
      97,
      95,
      93,
      91,
      89,
      87,
      84,
      82,
      80,
      77,
      75,
      72,
      69,
      67,
      64,
      61,
      59,
      56,
      53,
      50,
      47,
      44,
      41,
      39,
      36,
      32,
      29,
      26,
      23,
      20,
      17,
      14,
      11,
      8,
      5,
      2,
      -2,
      -5,
      -8,
      -11,
      -14,
      -17,
      -20,
      -23,
      -26,
      -29,
      -32,
      -36,
      -39,
      -41,
      -44,
      -47,
      -50,
      -53,
      -56,
      -59,
      -61,
      -64,
      -67,
      -69,
      -72,
      -75,
      -77,
      -80,
      -82,
      -84,
      -87,
      -89,
      -91,
      -93,
      -95,
      -97,
      -99,
      -101,
      -103,
      -105,
      -107,
      -108,
      -110,
      -112,
      -113,
      -114,
      -116,
      -117,
      -118,
      -119,
      -120,
      -121,
      -122,
      -123,
      -124,
      -124,
      -125,
      -125,
      -126,
      -126,
      -127,
      -127,
      -127,
      -127,
      -127,
      -127,
      -127,
      -126,
      -126,
      -126,
      -125,
      -125,
      -124,
      -123,
      -123,
      -122,
      -121,
      -120,
      -119,
      -118,
      -116,
      -115,
      -114,
      -112,
      -111,
      -109,
      -108,
      -106,
      -104,
      -102,
      -100,
      -98,
      -96,
      -94,
      -92,
      -90,
      -88,
      -86,
      -83,
      -81,
      -78,
      -76,
      -73,
      -71,
      -68,
      -66,
      -63,
      -60,
      -57,
      -54,
      -52,
      -49,
      -46,
      -43,
      -40,
      -37,
      -34,
      -31,
      -28,
      -25,
      -22,
      -19,
      -16,
      -12,
      -9,
      -6,
      -3,
      0,

};


const int8_t coswave[] =
{
      127,
      127,
      127,
      127,
      126,
      126,
      126,
      125,
      125,
      124,
      123,
      122,
      121,
      121,
      120,
      118,
      117,
      116,
      115,
      113,
      112,
      110,
      109,
      107,
      105,
      104,
      102,
      100,
      98,
      96,
      94,
      92,
      90,
      87,
      85,
      83,
      80,
      78,
      75,
      73,
      70,
      68,
      65,
      62,
      59,
      57,
      54,
      51,
      48,
      45,
      42,
      39,
      36,
      33,
      30,
      27,
      24,
      21,
      18,
      15,
      12,
      9,
      5,
      2,
      -1,
      -4,
      -7,
      -10,
      -13,
      -16,
      -19,
      -23,
      -26,
      -29,
      -32,
      -35,
      -38,
      -41,
      -44,
      -47,
      -50,
      -52,
      -55,
      -58,
      -61,
      -64,
      -66,
      -69,
      -71,
      -74,
      -77,
      -79,
      -81,
      -84,
      -86,
      -88,
      -91,
      -93,
      -95,
      -97,
      -99,
      -101,
      -103,
      -105,
      -106,
      -108,
      -110,
      -111,
      -113,
      -114,
      -115,
      -117,
      -118,
      -119,
      -120,
      -121,
      -122,
      -123,
      -124,
      -124,
      -125,
      -125,
      -126,
      -126,
      -127,
      -127,
      -127,
      -127,
      -127,
      -127,
      -127,
      -127,
      -126,
      -126,
      -125,
      -125,
      -124,
      -124,
      -123,
      -122,
      -121,
      -120,
      -119,
      -118,
      -117,
      -115,
      -114,
      -113,
      -111,
      -110,
      -108,
      -106,
      -105,
      -103,
      -101,
      -99,
      -97,
      -95,
      -93,
      -91,
      -88,
      -86,
      -84,
      -81,
      -79,
      -77,
      -74,
      -71,
      -69,
      -66,
      -64,
      -61,
      -58,
      -55,
      -52,
      -50,
      -47,
      -44,
      -41,
      -38,
      -35,
      -32,
      -29,
      -26,
      -23,
      -19,
      -16,
      -13,
      -10,
      -7,
      -4,
      -1,
      2,
      5,
      9,
      12,
      15,
      18,
      21,
      24,
      27,
      30,
      33,
      36,
      39,
      42,
      45,
      48,
      51,
      54,
      57,
      59,
      62,
      65,
      68,
      70,
      73,
      75,
      78,
      80,
      83,
      85,
      87,
      90,
      92,
      94,
      96,
      98,
      100,
      102,
      104,
      105,
      107,
      109,
      110,
      112,
      113,
      115,
      116,
      117,
      118,
      120,
      121,
      121,
      122,
      123,
      124,
      125,
      125,
      126,
      126,
      126,
      127,
      127,
      127,
      127,
};

int8_t randomWave[256];

const int kXmasCount = 10*3;

RandomModulator** xmasMods;
XmasTwinkle** xmasLights;

void InitRandomWave()
{
	for( int i = 0; i < 256; i++)
	{
		int val = rand() % 216;
		randomWave[i] = val;
	}
}

int main(void)
{

	LPD8806_Init();

	InitRandomWave();

   FadingStar* red = new FadingStar(0, 40, RGB(255,0,0), 9, 250, ShootingStar::FORWARD, ShootingStar::FORWARD);
   FadingStar* red2 = new FadingStar(39, 40, RGB(64,255,64), 15, 245, ShootingStar::FORWARD, ShootingStar::REVERSE);


   ShootingStar* blue = new ShootingStar(LED_COUNT/2, LED_COUNT/2, RGB(0,0,255), 10, 64, ShootingStar::FORWARD, ShootingStar::FORWARD);
   ShootingStar* green = new ShootingStar(0, LED_COUNT, RGB(0,255,0), 10, 127, ShootingStar::FORWARD, ShootingStar::FORWARD);

   Strobe* strobe1 = new Strobe(0, 20, RGB(210,0,255), 20, STROBE_FADE_LENGTH, 50, STROBE_FADE_OUT, 0);
   Strobe* strobe2 = new Strobe(0, 20, RGB(255,240,0), 20, STROBE_FADE_LENGTH, 50, STROBE_FADE_OUT, (Strobe::MAX_FADE_PROGRESS / 100));
   Strobe* strobe3 = new Strobe(0, 20, RGB(0,240,255), 20, STROBE_FADE_LENGTH, 50, STROBE_FADE_OUT, 2*(Strobe::MAX_FADE_PROGRESS / 100));
   Strobe* strobe4 = new Strobe(0, 20, RGB(255,0,220), 20, STROBE_FADE_LENGTH, 50, STROBE_FADE_OUT, 3*(Strobe::MAX_FADE_PROGRESS / 100));
   Strobe* strobe5 = new Strobe(0, 20, RGB(255,150,0), 20, STROBE_FADE_LENGTH, 50, STROBE_FADE_OUT, 4*(Strobe::MAX_FADE_PROGRESS / 100));

   xmasLights = new XmasTwinkle*[kXmasCount];


   XmasTwinkle* xmasWhite = new XmasTwinkle(0, 3, RGB(255,255,255), 2, 512);
   XmasTwinkle* xmasGreen = new XmasTwinkle(0, 3, RGB(0,255,0), 2, 512, 128);
   XmasTwinkle* xmasRed = new XmasTwinkle(0, 3, RGB(255,0,0), 2, 512, 256);


   PatternManager* sm = new PatternManager(50, LED_COUNT);

   xmasMods = new RandomModulator*[kXmasCount];

   uint32_t xmasLeds[3] = {RGB(255,255,255), RGB(0,255,0), RGB(255,0,0)};

   for( int i = 0; i< kXmasCount; ++i )
   {
   		xmasLights[i] = new XmasTwinkle(0, 3, xmasLeds[i%3], 2, 512, i * (512 / kXmasCount));
   		xmasMods[i] = new RandomModulator((int16_t*)&sm->startPositions[i], 0, LED_COUNT, 1024, i * (1024 / kXmasCount));
   }

   Modulator* mod1 = new Modulator( (int16_t*)&sm->startPositions[0], 0, 216, (int8_t*)coswave, 256, 0, 217, 255, 127, 30 );

   Modulator* modRand = new Modulator( (int16_t*)&sm->startPositions[0], 0, LED_COUNT, (int8_t*)randomWave, 256, 0, LED_COUNT, 255, 127, 1024 );

  // Modulator* mod2 = new Modulator( (int16_t*)&sm->startPositions[1], 0, 216, (int8_t*)coswave, 256, 32, 217, 255, 127, 30 );
  // Modulator* mod3 = new Modulator( (int16_t*)&sm->startPositions[2], 0, 216, (int8_t*)coswave, 256, 64, 217, 255, 127, 30 );
  /// Modulator* mod4 = new Modulator( (int16_t*)&sm->startPositions[3], 0, 216, (int8_t*)coswave, 256, 127, 217, 255, 127, 30 );
  // Modulator* mod5 = new Modulator( (int16_t*)&sm->startPositions[4], 0, 216, (int8_t*)coswave, 256, 192, 217, 255, 127, 30 );



 //  sm->addPattern(strobe1, 0);
 //  sm->addPattern(strobe2, 20);
 //  sm->addPattern(strobe3, 40);
 //  sm->addPattern(strobe4, 60);
 //  sm->addPattern(strobe5, 80);

   //sm->addPattern(red, LED_COUNT/2 - 10);
   for( int i = 0; i< kXmasCount; ++i )
   {
      sm->addPattern(xmasLights[i], i*5);
   }


   //sm->addPattern(red, 0);
  // sm->addPattern(red2, LED_COUNT/2 + 10);
  // sm->addPattern(green, 0);
  // sm->addPattern(blue, 0);


   sm->init();

   delay_init();

   LPD8806_Init();
   LPD8806_Test();

   LPD8806_Update();

   while(1)
   {
      sm->update();

      for( uint16_t j = 0; j < sm->ledCount; ++j)
      {
         uint32_t* memBuf = sm->getBuffer();
         LPD8806_SetPixel(j, memBuf[j]);
      }

      LPD8806_Update();
      delay_us(500);

      //modRand->update();
      for( int i = 0; i< kXmasCount; ++i )
      {
      		xmasMods[i]->update();
      }

    //  mod1->update();
     // mod2->update();
     // mod3->update();
     // mod4->update();
     // mod5->update();

   }
   return 0;
}