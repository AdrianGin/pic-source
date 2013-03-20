 /* pause: Set 8Bit Timer0 to clock at 256th the internal clock speed*/
#include <stdint.h>
#include <avr/io.h>
#include <avr/iom8.h>

#define ___RESET 	0x00
#define ___clk64	0x03
#define ___clk256 	0x04
#define ___clk1024  0x05

#define CLK 1   /*Speed of micro in MHz*/


void pause (int PDELAY)
{
      int i;
      TCCR0     =      ___clk256;
      for (i = 0; i < PDELAY; i++)
      {
       TCNT0    =      ___RESET;
       while (TCNT0 < 4) {;}
      }	  
}



#define Time1ms 91
void pausems(int Delay)
{
    int i;
    uint8 j;
    
    for(i = 0; i < Delay; i++)
    {
        for(j=0; j< Time1ms ;j++)   /*Time it takes to make 1ms*/
        {;}
    }
}

