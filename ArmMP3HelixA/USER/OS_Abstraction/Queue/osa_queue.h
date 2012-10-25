#ifndef __OSA_QUEUE_H
#define __OSA_QUEUE_H


#include "queue.h"


//OS Abstraction Layer

//FreeRTOS Queue
typedef xQueueHandle Queue_t;
#define QueueCreate(uxQueueLength, uxItemSize)				xQueueCreate( uxQueueLength, uxItemSize )
#define QueueSend( xQueue, pvItemToQueue, xTicksToWait )  	xQueueSendToBack( xQueue, pvItemToQueue, xTicksToWait )
#define QueueReceive( xQueue, pvBuffer, xTicksToWait ) 		xQueueReceive( xQueue, pvBuffer, xTicksToWait )
#define QueueDelete(pxQueue)  								vQueueDelete( pxQueue )
#define QueueMessageCount(xQueue)							uxQueueMessagesWaiting(xQueue)
#define QueuePeek( xQueue, pvBuffer, xTicksToWait )			xQueuePeek( xQueue, pvBuffer, xTicksToWait )

#define QUEUE_BLOCK_WAIT	0
#define QUEUE_SUCCESS		pdPASS


#endif

