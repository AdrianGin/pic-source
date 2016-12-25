/**
 ******************************************************************************
 * @file    Ringbuffer.c
 * @author  Adrian Gin, Focusrite Audio Engineering Ltd.
 * @version V1.0.0
 * @date    2-August-2016
 * @brief   Ringbuffer routines.
 *
 ******************************************************************************
 */
/** @addtogroup Firmware
 * @{
 */
/** @defgroup Ringbuffer
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "Ringbuffer.h"


uint8_t RingBuffer_Clear(Ringbuffer_t* rb)
{
   rb->writePos = 0;
   rb->readPos = 0;
   return RINGBUFFER_SUCCESS;
}

//Returns the number of bytes in buffer
uint16_t RingBuffer_GetSpaceUsed(Ringbuffer_t* rb)
{
   return (rb->size - 1) - RingBuffer_GetSpaceAvailable(rb);
}


//Returns the number of bytes remaning in buffer
uint16_t RingBuffer_GetSpaceAvailable(Ringbuffer_t* rb)
{
   if (rb->readPos > rb->writePos)
   {
      return (rb->readPos - rb->writePos - 1);
   }
   return (rb->size-1) - (rb->writePos - rb->readPos);
}

//Returns number of bytes written
uint16_t RingBuffer_Write(Ringbuffer_t* rb, uint8_t byte)
{
   uint16_t bytesWritten = 0;

   if (RingBuffer_GetSpaceAvailable(rb))
   {
      rb->buf[rb->writePos] = byte;
      //Wrap around
      if (rb->writePos+1 >= rb->size)
      {
         rb->writePos = 0;
      }
      else
      {
         rb->writePos++;
      }
      bytesWritten = 1;
   }

   return bytesWritten;
}

//Writes n bytes to the ring buffer
//Returns number of bytes written
uint16_t RingBuffer_WriteBuffer(Ringbuffer_t* rb, uint8_t* bytes, uint16_t n)
{
   uint16_t i;
   uint16_t bytesWritten = 0;

   for (i = 0; i < n; i++)
   {
      bytesWritten += RingBuffer_Write(rb, bytes[i]);
   }

   return bytesWritten;
}

//Reads a single byte from the buffer
uint8_t RingBuffer_Read(Ringbuffer_t* rb, uint8_t* result)
{
   if (RingBuffer_GetSpaceUsed(rb))
   {
      *result = rb->buf[rb->readPos];

      //Wrap around
      if ((rb->readPos+1) >= rb->size)
      {
         rb->readPos = 0;
      }
      else
      {
         rb->readPos++;
      }

      return 1;
   }
   return 0;
}


uint16_t RingBuffer_ReadBuffer(Ringbuffer_t* rb, uint8_t* result, uint16_t n)
{
   uint16_t i;
   uint16_t bytesRead = 0;

   for (i = 0; i < n; i++)
   {
      bytesRead += RingBuffer_Read(rb, &result[i]);
   }
   return bytesRead;
}


#ifdef __cplusplus
}
#endif






/**
 * @}
 */

/**
  * @}
  */
