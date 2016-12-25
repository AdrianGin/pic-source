/**
 ******************************************************************************
 * @file    Ringbuffer.h
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

#ifndef _RINGBUFFER_H
#define _RINGBUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

enum {
    RINGBUFFER_SUCCESS = 0,
    //For a failed write
    RINGBUFFER_FULL,

    //For a failed read
    RINGBUFFER_NO_DATA,
};


typedef struct  {
    uint8_t* buf;
    uint16_t size; /**< The size of the buffer in bytes. Note that the actual usable size is 1 less than the buffer size */

    volatile uint16_t writePos;
    volatile uint16_t readPos;

} Ringbuffer_t;


/**
  * @brief  Resets and initialises the ring buffer.
  * @param  rb Pointer to the ring buffer instance
  * @retval RINGBUFFER_SUCCESS
  */
uint8_t RingBuffer_Clear(Ringbuffer_t* rb);

/**
  * @brief  Returns the number of bytes used in buffer
  * @param  rb Pointer to the ring buffer instance
  * @retval The number of bytes used, typically between 0 and size-1
  */
uint16_t RingBuffer_GetSpaceUsed(Ringbuffer_t* rb);

/**
  * @brief  Returns the number of bytes free in buffer
  * @param  rb Pointer to the ring buffer instance
  * @retval The number of bytes remaining in buffer, typically between 0 and size-1
  */
uint16_t RingBuffer_GetSpaceAvailable(Ringbuffer_t* rb);

/**
  * @brief  Writes one byte to the buffer
  * @param  rb Pointer to the ring buffer instance
  * @param  byte the byte to be written
  * @retval The number of bytes written, either 0 to 1
  */
uint16_t RingBuffer_Write(Ringbuffer_t* rb, uint8_t byte);


/**
  * @brief  Writes n bytes to the ring buffer
  * @param  rb Pointer to the ring buffer instance
  * @param  bytes pointer to series of bytes to be written
  * @param  n the number of bytes to write
  * @retval The number of bytes successfully written
  *
  * If the buffer is full during the write, the function will continuously attempt to write
  * bytes until the buffer is emptied, typically in another thread/interrupt.
  *
  * RingBuffer_GetSpaceAvailable should be checked to ensure adequate space is available first.
  *
  */
uint16_t RingBuffer_WriteBuffer(Ringbuffer_t* rb, uint8_t* bytes, uint16_t n);


/**
  * @brief  Reads one byte from the buffer
  * @param  rb Pointer to the ring buffer instance
  * @param  result pointer to where the read byte will be stored.
  * @retval The number of bytes read, either 0 to 1
  */
uint8_t RingBuffer_Read(Ringbuffer_t* rb, uint8_t* result);

/**
  * @brief  Reads n bytes from the ring buffer
  * @param  rb Pointer to the ring buffer instance
  * @param  result pointer to series of bytes to be read
  * @param  n the number of bytes to read
  * @retval The number of bytes successfully read
  *
  * If the buffer is empty during the read, the function will continue to read from the buffer
  * The return value can be used to determine how many bytes were actually read.
  *
  * RingBuffer_GetSpaceUsed should be called prior to this determine the number of bytes in the
  * buffer. User should not attempt to read more bytes than are in the buffer.
  *
  */
uint16_t RingBuffer_ReadBuffer(Ringbuffer_t* rb, uint8_t* result, uint16_t n);

#ifdef __cplusplus
}
#endif

#endif



/**
 * @}
 */

/**
  * @}
  */

