

#include <stdio.h>
#include "FreeRTOS.h"

#include "task.h"


void vApplicationStackOverflowHook( xTaskHandle xTask, signed portCHAR *pcTaskName )
{
	printf("STACK OVER FLOW! %s\n", pcTaskName);
}
