#include "FreeRTOS.h"
#include "task.h"

#include "intertaskComm.h"


#include "MIDIParser/midiparser.h"
#include <stdint.h>

//Semaphore_t Sem_TouchPanelRdy;
//Semaphore_t Sem_SystemRdy;
//Semaphore_t Sem_GUIRdy;
//
////MP3 Decoding Semaphores
//Semaphore_t Sem_DMAComplete;// = NULL;
//Semaphore_t Sem_StopMP3Decode;

Semaphore_t Sem_MIDITick;
Semaphore_t Sem_LightSysUpdate;


Semaphore_t Sem_ProcessMIDIInput;
Queue_t Queue_WaitForMIDIInput;

Queue_t Queue_MIDIEventInput;



uint8_t WaitForMIDIInputValue;
uint8_t RedrawRequired;

//Queue_t Queue_GUI_MP3_Message;

//xTaskHandle mp3DecodeHandle;

xTaskHandle MIDIPlayBackHandle;
xTaskHandle LightSystemHandle;
uint32_t SeekValue;

uint8_t InitInterTaskComms(void)
{

//	SemaphoreCreate(Sem_SystemRdy);
//	SemaphoreTake(Sem_SystemRdy, 0);
//
//	SemaphoreCreate(Sem_GUIRdy);
//	SemaphoreTake(Sem_GUIRdy, 0);
//
//	SemaphoreCreate(Sem_TouchPanelRdy);
//	SemaphoreTake(Sem_TouchPanelRdy, 0);
//
//	SemaphoreCreate(Sem_DMAComplete);
	Sem_ProcessMIDIInput = SemaphoreCreateCounting(88, 0);
//
//	SemaphoreCreate(Sem_StopMP3Decode);
//	SemaphoreTake(Sem_StopMP3Decode, 0);

	SemaphoreCreate(Sem_MIDITick);
	SemaphoreTake(Sem_MIDITick, 0);

	SemaphoreCreate(Sem_LightSysUpdate);
	SemaphoreTake(Sem_LightSysUpdate, 0);

	Queue_WaitForMIDIInput = QueueCreate(1, sizeof(uint8_t));
	Queue_MIDIEventInput = QueueCreate(100, sizeof(MIDI_CHAN_EVENT_t));

//	Queue_GUI_MP3_Message = QueueCreate(1, sizeof(uint8_t*));
//	SeekValue = 0;

	return 0;

}
