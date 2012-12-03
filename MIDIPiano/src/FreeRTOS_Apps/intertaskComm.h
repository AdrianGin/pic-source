#ifndef _INTERTASK_COMM
#define _INTERTASK_COMM

#include "FreeRTOS.h"
#include "task.h"
#include "Semaphore\\osa_semaphore.h"
#include "Queue\\osa_queue.h"

#include <stdint.h>


extern Semaphore_t Sem_TouchPanelRdy;
extern Semaphore_t Sem_SystemRdy;
extern Semaphore_t Sem_GUIRdy;

extern xSemaphoreHandle Sem_DMAComplete;
extern Semaphore_t Sem_StopMP3Decode;

extern Queue_t Queue_GUI_MP3_Message;
extern xTaskHandle mp3DecodeHandle;

extern uint32_t SeekValue;
extern uint32_t MP3_Data_Index;

uint8_t InitInterTaskComms(void);

#endif




