
#include "hardwareSpecific.h"

/* Using Timer2 */
void waveAudioOn(void)
{
    OC1CONbits.OCM = 0x06;
    OC2CONbits.OCM = 0x06;
    OC1RS = 255;
    OC2RS = 255;

    //Period of Timer should be asap!
    PR2 = 255;
    WAVE_LEFT_REG = 128;
    WAVE_RIGHT_REG = 128;
    T2CONbits.TON = 1;
    audioState = WAVE_OUTPUT_ON;

    IPC0bits.T1IP = 0x01;
    IFS0bits.T1IF = 0x00;
    IEC0bits.T1IE = 0x01;
    T1CONbits.TON = 0x01;

}

void waveAudioOff(void)
{
    /* Simply turn off the timer */
    T2CONbits.TON = 0;
    WAVE_LEFT_REG = 128;
    //WAVE_RIGHT_REG = 128;
    audioState = WAVE_OUTPUT_OFF;
}

/* AudioSetup must be called at the start of a new wave file */
void waveAudioSetup(void)
{
    //TCCR1A |= (1 << COM1A0) | (1 << COM1A1) | (1 << WGM10);
    //TCCR1B |= (1 << CS10) | (1 << WGM12);

#if WAVE_STEREO_ENABLED
    //DDRB |= (1 << 2);
    //TCCR1A |= (1 << COM1B0) | (1 << COM1B1);
#endif

    /* Set levels back to centre */
    WAVE_LEFT_REG = 128;
    WAVE_RIGHT_REG = 128;
    audioReadptr = 0;
    audioWriteptr = 0;
}

void waveProcessBuffer(waveHeader_t* wavefile)
{
    /* Left is first */
    WAVE_LEFT_REG = Buff[(audioReadptr)];
    /* Right is second */
    /* This will not do anything if WAVE_STEREO_ENABLED is not set to 1 */
    WAVE_RIGHT_REG = Buff[(audioReadptr + isStereo)];
    audioReadptr = (audioReadptr + 1 + isStereo) & WAVE_OUTMASK;
    //audioReadptr = (audioReadptr + 1 + isStereo);
}

void waveSetSampleRate(uint16_t sampleRate)
{
    PR1 = (F_CPU/sampleRate) - 1;
}


