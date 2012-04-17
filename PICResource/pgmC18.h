#ifndef _PGM
#define _PGM

#include <stdint.h>

#ifndef __ATTR_PROGMEM__
#define __ATTR_PROGMEM__ rom
#endif


/**
   \ingroup avr_pgmspace
   \def PROGMEM

   Attribute to use in order to declare an object being located in
   flash ROM.
 */
#define PROGMEM __ATTR_PROGMEM__


typedef PROGMEM void prog_void;
typedef PROGMEM char prog_char ;
typedef PROGMEM unsigned char prog_uchar;

typedef PROGMEM int8_t    prog_int8_t  ;
typedef PROGMEM uint8_t   prog_uint8_t ;
typedef PROGMEM int16_t   prog_int16_t ;
typedef PROGMEM uint16_t  prog_uint16_t;
typedef PROGMEM int32_t   prog_int32_t;
typedef PROGMEM uint32_t  prog_uint32_t;

#define PGM_P const prog_char *


#endif
