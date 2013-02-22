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

extern Semaphore_t Sem_MIDITick;
extern Semaphore_t Sem_LightSysUpdate;
extern Queue_t Queue_WaitForMIDIInput;

extern xTaskHandle MIDIPlayBackHandle;
extern xTaskHandle LightSystemHandle;

extern uint32_t SeekValue;
extern uint32_t MP3_Data_Index;
extern uint8_t WaitForMIDIInputValue;

uint8_t InitInterTaskComms(void);

#define WAIT_FOR_MIDI_TICK()		while( SemaphoreTake(Sem_MIDITick, SEMAPHORE_BLOCK_WAIT) == pdFAIL){}
#define WAIT_FOR_LIGHTSYS_UPDATE()	while( SemaphoreTake(Sem_LightSysUpdate, SEMAPHORE_BLOCK_WAIT) == pdFAIL){}

#endif




