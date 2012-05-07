#include "hardwareSpecific.h"
#include "waveplayer.h"
#include "wavedriver.h"
#include <string.h>

uint8_t Buff[WAVE_OUTBUFFER_SIZE]; /* Audio output FIFO */
//FATFS filesys;

volatile uint8_t audioReadptr;
volatile uint8_t audioWriteptr;
volatile uint8_t audioState;
uint8_t fastMode;

uint8_t is16Bit;
uint8_t isStereo;


uint8_t waveIsPlaying(void)
{
    return audioState;
}

/* This assumes the filesystem has been mounted, 
 * waveContinuePlaying needs to be constantly called in the main loop thereafter
 */
uint8_t wavePlayFile(waveHeader_t* wavefile, uint8_t* filename)
{
    uint32_t ret;
    uint8_t retries = WAVE_RETRY_COUNT;

    while (retries--)
    {
        ret = waveParseHeader(wavefile, filename);
        if (ret > WAVE_MINIMUM_SAMPLES)
        {
            
            waveAudioSetup();
            DEBUG(("Success2\n"));
            waveAudioOn();
            DEBUG(("Success!\n"));
            return WAVE_SUCCESS;
        }
        else
        {
            char outputString[10];
            memset(outputString, 0, 10);
            //uint8toa( outputString, ret & 0x000000FF);
            //DEBUG(outputString);
            uartTx(PrimaryUART, ret & 0x000000FF);
            uartTx(PrimaryUART, ret & 0x0000FF00);
            //uartTxString(PrimaryUART, ret & 0x000000FF);
            //uartTxString(PrimaryUART, ret & 0x000000FF);
            DEBUG(("File too small!\n"));
        }
    }

    return ret;
}

void wavePutByte(uint8_t byte)
{
    static uint8_t highByte = 1;

    /* Throw away every other byte, due to the Little Endianness */
    if (is16Bit && highByte)
    {
        highByte = 0;
        return;
    }

    highByte = 1;

    /* Wait for a bit */
    /* Forward data to Audio Fifo */
    while ((((uint8_t) (audioWriteptr + 1)) == audioReadptr) && (audioState))
    {
    }

    /* Do the signed 16bit -128 -> 127 to unsigned 8bit 0 - >255here */
    Buff[audioWriteptr++] = byte + (is16Bit << 7);
    //audioWriteptr &= WAVE_OUTMASK;
}

/* If this returns false, then the song has finished */
uint8_t waveContinuePlaying(waveHeader_t* wavefile)
{
    WORD bytesWritten;
    WORD bytesToPlay;

    if (!waveIsPlaying())
    {
        return 0;
    }

    if (wavefile->dataSize > WAVE_OUTBLOCK_SIZE)
    {
        bytesToPlay = WAVE_OUTBLOCK_SIZE;
    }
    else
    {
        bytesToPlay = wavefile->dataSize;
    }

    pf_read(0, bytesToPlay, &bytesWritten);
    wavefile->dataSize = wavefile->dataSize - bytesToPlay;

    if (bytesWritten != bytesToPlay)
    {
        DEBUG(("IO Error"));
        return 0;
    }

    if (wavefile->dataSize)
    {
        return 1;
    }
    else
    {
        return 0;
    }


}

/* Returns the number of data bytes otherwise returns an error code */
uint32_t waveParseHeader(waveHeader_t* wavefile, uint8_t* filename)
{

    WORD bytesRead = 0;
    uint32_t chunkSize = 0;

    memset(wavefile, 0, sizeof (waveHeader_t));
    memset(Buff, 0, sizeof (Buff));
    is16Bit = 0;
    isStereo = 0;

    if (pf_open(filename) != FR_OK) return WAVE_IO_ERROR + 1;
    if (pf_lseek(0) != FR_OK) return WAVE_IO_ERROR + 2;

    /* Check RIFF-WAVE file header */
    if (pf_read(Buff, 12, &bytesRead)) return WAVE_IO_ERROR + 3;

    /* Make sure it is a WAVE file */
    if (bytesRead != 12 || LD_DWORD(Buff + 8) != FCC('W', 'A', 'V', 'E')) return WAVE_INVALID_FILE;

    for (;;)
    {
        /* Get Chunk ID and size */
        if (pf_read(Buff, 8, &bytesRead)) return WAVE_IO_ERROR + 4;
        if (bytesRead != 8) return WAVE_INVALID_FILE;
        chunkSize = LD_DWORD(Buff + 4); /* Chunk size */

        /* Switch by chunk type */
        switch (LD_DWORD(Buff))
        {
            case FCC('f', 'm', 't', ' '): /* 'fmt ' chunk */
                if (chunkSize & 1) chunkSize++;
                if (chunkSize > 128 || chunkSize < 16) return WAVE_INVALID_FILE; /* Check chunk size */
                if (pf_read(Buff, chunkSize, &bytesRead)) return WAVE_IO_ERROR + 5; /* Get the chunk content */
                if (bytesRead != chunkSize) return WAVE_INVALID_FILE;
                if (Buff[FMT_FORMAT] != 1) return WAVE_INVALID_FILE; /* Check coding type (1: LPCM) */
                if (Buff[FMT_NUM_CHANNELS] < 1 && Buff[FMT_NUM_CHANNELS] > 2) return WAVE_INVALID_FILE; /* Check channels (1/2: Mono/Stereo) */

                wavefile->channelCount = Buff[FMT_NUM_CHANNELS]; /* Save channel flag */
                /* Setup the offsets for stereo */
                if (wavefile->channelCount == 2)
                {
                    isStereo = 1;
                }

                if (Buff[FMT_RESOLUTION] != 8 && Buff[FMT_RESOLUTION] != 16) return WAVE_INVALID_FILE; /* Check resolution (8/16 bit) */
                wavefile->resolution = Buff[FMT_RESOLUTION]; /* Save resolution flag */
                /* Setup the resolution byte offsets */
                if (wavefile->resolution == 16)
                {
                    is16Bit = 1;
                }

                wavefile->sampleRate = LD_DWORD(&Buff[FMT_SAMPLERATE]); /* Check sampling freqency (8k-48k) */
                if (wavefile->sampleRate < WAVE_MINSAMPLE_RATE || wavefile->sampleRate > WAVE_MAXSAMPLE_RATE) return WAVE_INVALID_FILE;

                if (isStereo && is16Bit && wavefile->sampleRate >= 44100)
                {
                    fastMode = 1;
                }
                else
                {
                    fastMode = 0;
                }


                /* Set interval timer (sampling period) */
                /* Use OCR2 */
                waveSetSampleRate(wavefile->sampleRate);
                // = (F_CPU/32/wavefile->sampleRate) - 1;
                break;

            case FCC('d', 'a', 't', 'a'): /* 'data' chunk (start to play) */
                if (chunkSize < WAVE_MINIMUM_SAMPLES) return WAVE_INVALID_FILE; /* Check size */
                wavefile->dataSize = chunkSize;
                return chunkSize;

            case FCC('D', 'I', 'S', 'P'): /* 'DISP' chunk (skip) */
            case FCC('f', 'a', 'c', 't'): /* 'fact' chunk (skip) */
            case FCC('L', 'I', 'S', 'T'): /* 'LIST' chunk (skip) */
                if (chunkSize & 1) chunkSize++;
                if (pf_lseek(filesys.fptr + chunkSize)) return WAVE_IO_ERROR + 6;
                break;

            default: /* Unknown chunk */
                return WAVE_INVALID_FILE;
        }
    }
}
