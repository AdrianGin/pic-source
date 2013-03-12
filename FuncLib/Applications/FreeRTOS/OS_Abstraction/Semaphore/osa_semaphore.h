#ifndef __SEMAPHORE_H
#define __SEMAPHORE_H

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"


//OS Abstraction Layer

//FreeRTOS Semaphore
typedef xSemaphoreHandle Semaphore_t;
#define SemaphoreCreate(xSemaphore)				vSemaphoreCreateBinary( xSemaphore )
#define SemaphoreCreateCounting(maxlen, initValue) 	xSemaphoreCreateCounting(maxlen, initValue)
#define SemaphoreTake(xSemaphore, xBlockTime)  	xSemaphoreTake( xSemaphore, xBlockTime )
#define SemaphoreGive(xSemaphore) 				xSemaphoreGive( xSemaphore)
#define SemaphoreGiveISR(xSemaphore, xHigherPriorityTaskWoken) 				xSemaphoreGiveFromISR( xSemaphore , xHigherPriorityTaskWoken)
#define SemaphoreDelete(xSemaphore)  			vSemaphoreDelete( xSemaphore )

#define SEMAPHORE_BLOCK_WAIT	portMAX_DELAY

#endif

