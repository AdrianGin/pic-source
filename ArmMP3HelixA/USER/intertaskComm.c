#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


#include "intertaskComm.h"

#include <stdint.h>

Semaphore_t Sem_TouchPanelRdy;
Semaphore_t Sem_SystemRdy;
Semaphore_t Sem_GUIRdy;

//MP3 Decoding Semaphores
Semaphore_t Sem_DMAComplete;
Semaphore_t Sem_StopMP3Decode;


Queue_t Queue_GUI_MP3_Message;

uint8_t InitInterTaskComms(void)
{

	SemaphoreCreate(Sem_SystemRdy);
	SemaphoreTake(Sem_SystemRdy, 0);

	SemaphoreCreate(Sem_GUIRdy);
	SemaphoreTake(Sem_GUIRdy, 0);

	SemaphoreCreate(Sem_TouchPanelRdy);
	SemaphoreTake(Sem_TouchPanelRdy, 0);

	SemaphoreCreate(Sem_DMAComplete);
	SemaphoreTake(Sem_DMAComplete, 0);

	SemaphoreCreate(Sem_StopMP3Decode);
	SemaphoreTake(Sem_StopMP3Decode, 0);

	Queue_GUI_MP3_Message = QueueCreate(1, sizeof(uint8_t*));

	return 0;

}
