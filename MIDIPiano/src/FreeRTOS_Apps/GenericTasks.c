

#include "printf/printf.h"
#include "FreeRTOS.h"

#include "task.h"


void vApplicationStackOverflowHook( xTaskHandle xTask, signed portCHAR *pcTaskName )
{
	xprintf("STACK OVER FLOW! %s\n", pcTaskName);
}
