#ifndef MAIN_H
#define MAIN_H



#define DEFAULT_PROCESS (PLAY_MODE)

extern uint8_t ActiveProcess;
extern uint16_t BenchMarkCount;

extern const char VersionId[];
enum {
   PLAY_MODE = 0,
   RECEIVE_SYSEX,
   CONTROLLER_MODE
} processes;
void Play(void);
void Benchmark(void);
void MidiControllerMode(void);

/* Interrupt Macros */
#define UI_INT_PORT  P1IN
#define UI_INT_PIN   (1<<3)


#endif
