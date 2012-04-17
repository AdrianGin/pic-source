


#include <stdint.h>
#include <stdlib.h>
#include "stack/stack.h"
/*

Copyright (c) 2011 Adrian Gin (adrian.gin[at]gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

/* Ring buffer implementation */

#include "stack.h"


/* Critical means disable interrupts on entry and restore interrupt
 * state on exit */

void* FIFO_PopData(STACK_t* stack)
{
    //buffer not empty?
    if (stack->readPtr != stack->writePtr)
    {
        //wrap around read position
        stack->readPtr &= (stack->size - 1);
        //get byte from buffer, update read position and return
        return stack->memPtrArray[stack->readPtr++];
    } else
    {
        return STACK_OVERFLOW; /* This is really trying to get a nonexistant byte */
    }
}


// LIFO is like reading from the writePtr
void* LIFO_PopData(STACK_t* stack)
{
    //buffer not empty?
    if (stack->writePtr != stack->readPtr)
    {
        //wrap around read position
        stack->writePtr--;
        stack->writePtr &= (stack->size - 1);
        //get byte from buffer, update read position and return
        return stack->memPtrArray[stack->writePtr];
    } else
    {
        return STACK_OVERFLOW; /* This is really trying to get a nonexistant byte */
    }
}

/* returns the size of the stack */
uint8_t STACK_PushData(STACK_t* stack, void* data)
{
    //is there space in the buffer?
    int bufferMask = (stack->size - 1);

    if ( ((stack->writePtr + 1) & bufferMask) != (stack->readPtr & bufferMask))
    {
        //wrap around write position
        stack->writePtr &= bufferMask;

        //write the character
        stack->memPtrArray[stack->writePtr++] = data;
        //update size info

        return STACK_Len(stack);
    } 

    return STACK_OVERFLOW;
    
}

uint8_t STACK_Len(STACK_t* stack)
{
    if( stack->readPtr > stack->writePtr )
    {
        return stack->writePtr + stack->size - stack->readPtr;
    }
    return stack->writePtr - stack->readPtr;
}

uint8_t STACK_isEmpty(STACK_t* stack)
{
   return stack->writePtr == stack->readPtr;
}


void STACK_Clear(STACK_t* stack)
{
    stack->readPtr = 0;
    stack->writePtr = 0;
}

