#ifndef _PGM
#define _PGM

#include <stdint.h>

#ifndef __ATTR_PROGMEM__
#define __ATTR_PROGMEM__ __attribute__((space(psv)))
#endif


/**
   \ingroup avr_pgmspace
   \def PROGMEM

   Attribute to use in order to declare an object being located in
   flash ROM.
 */
#define PROGMEM __ATTR_PROGMEM__


typedef void prog_void;
typedef char prog_char PROGMEM;
typedef unsigned char prog_uchar PROGMEM;

typedef int8_t    prog_int8_t   PROGMEM;
typedef uint8_t   prog_uint8_t  PROGMEM;
typedef int16_t   prog_int16_t  PROGMEM;
typedef uint16_t  prog_uint16_t PROGMEM;
typedef int32_t   prog_int32_t  PROGMEM;
typedef uint32_t  prog_uint32_t PROGMEM;

#define PGM_P prog_char*


#endif

