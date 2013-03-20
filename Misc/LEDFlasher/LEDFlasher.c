#include <stdint.h>

#include <avr/io.h>
#include <util/delay.h>

#include <avr/interrupt.h>

int main(void)
{
   UCSR0A |= (1 << U2X0);
   UCSR0B |= ((1 <<TXEN0) | (1 << RXEN0));
   UDR0 = 10;

   DDRC |= ((1 << 0) | (1 << 1));
   UDR0 = 15;
   PORTC |= (1<<0);

   UDR0 = 25;   

   while(1)
   {
      PORTC ^= (1 << 0);
      for( uint8_t i = 0; i < 10 ; i++ )
      {
         cli();
      }
      PORTC ^= (1 << 1);
      for( uint8_t i = 0; i < 10 ; i++ )
      {
         cli();
      }
   }

   return 0;
}
