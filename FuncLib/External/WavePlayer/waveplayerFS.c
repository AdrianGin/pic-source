#include "hardwareSpecific.h"
#include "waveplayerFS.h"
#include "wavedriver.h"
#include <string.h>



//FIL waveFile;

uint8_t waveIsPlaying(waveFile_t* wavefile)
{
    return wavefile->audioStatus;
}

uint16_t waveBufferedBytes(waveFile_t* waveFile)
{
    if( waveFile->audioReadptr > waveFile->audioWriteptr )
    {
        return waveFile->audioWriteptr + WAVE_OUTBUFFER_SIZE - waveFile->audioReadptr;
    }
    return waveFile->audioWriteptr - waveFile->audioReadptr;
}


/* This assumes the filesystem has been mounted, 
 * waveContinuePlaying needs to be constantly called in the main loop thereafter
 */
uint8_t wavePlayFile(waveFile_t* wavefile, uint8_t* filename)
{
    uint32_t ret;
    uint8_t retries = WAVE_RETRY_COUNT;

    while (retries--)
    {
        ret = waveParseHeader(wavefile, filename);
        if (ret > WAVE_MINIMUM_SAMPLES)
        {
            
            waveAudioSetup(wavefile);
            DEBUG(("Success2\n"));
            waveAudioOn(wavefile);
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

uint8_t wavePutByte(waveFile_t* waveFile, uint8_t byte)
{
    /* Wait for a bit */
    /* Forward data to Audio Fifo */
    while ((((waveFile->audioWriteptr + 1) & WAVE_OUTMASK) == waveFile->audioReadptr) && (waveFile->audioStatus & WAVE_AUDIO_STATUS_PLAYING))
    {
        return WAVE_BUFFER_INCOMPLETE;
    }

    /* Throw away every other byte, due to the Little Endianness */
    if ( (waveFile->waveHeader.resolution == 16) && !(waveFile->audioStatus & WAVE_AUDIO_STATUS_HIGHBYTE))
    {
        waveFile->audioStatus |= WAVE_AUDIO_STATUS_HIGHBYTE;
        return WAVE_BYTE_COMPLETE;
    }
    waveFile->audioStatus &= ~WAVE_AUDIO_STATUS_HIGHBYTE;


    /* Do the signed 16bit -128 -> 127 to unsigned 8bit 0 - >255here */
    waveFile->Buffer[waveFile->audioWriteptr++] = byte + ((waveFile->waveHeader.resolution == 16) << 7);
    waveFile->audioWriteptr &= WAVE_OUTMASK;
    return WAVE_BYTE_COMPLETE;
}

/* If this returns false, then the song has finished */
uint8_t waveContinuePlaying(waveFile_t* waveFile)
{
    WORD bytesWritten;
    WORD bytesToPlay;

    waveHeader_t* wavefile = &waveFile->waveHeader;


    if (!waveIsPlaying(waveFile))
    {
        return 0;
    }

    if( wavefile->dataSize == 0)
    {
        return 0;
    }

    if(waveBufferedBytes(waveFile) > (WAVE_TRANSFER_BYTE_SIZE))
    {
        LATA ^= 0x02;
        return 1;
    }

    if (wavefile->dataSize > WAVE_TRANSFER_BYTE_SIZE)
    {
        //bytesToPlay = WAVE_OUTBLOCK_SIZE;
        bytesToPlay = WAVE_TRANSFER_BYTE_SIZE;
    }
    else
    {
        bytesToPlay = wavefile->dataSize;
    }

    uint8_t tempBuf[WAVE_TRANSFER_BYTE_SIZE];

    f_read( (FIL*)&waveFile->fileptr, (char*)&tempBuf[0], bytesToPlay, &bytesWritten);

    if (bytesWritten != bytesToPlay)
    {
        DEBUG(("IO Error"));
        return 0;
    }


    if (wavefile->dataSize)
    {
        uint16_t i;
        uint8_t ret;
        for(i = 0; i < bytesWritten; i++)
        {
            ret = wavePutByte(waveFile, tempBuf[i]);
            if( ret == WAVE_BUFFER_INCOMPLETE)
            {
                break;
            }
        }

        if( ret == WAVE_BUFFER_INCOMPLETE )
        {
            LATA ^= 1;
            f_lseek((FIL*)&waveFile->fileptr, f_tell(&waveFile->fileptr) - (bytesWritten - i));
        }
        wavefile->dataSize = wavefile->dataSize - i;

        return 1;
    }
    else
    {
        return 0;
    }


}

/* Returns the number of data bytes otherwise returns an error code */
uint32_t waveParseHeader(waveFile_t* wavefile, uint8_t* filename)
{

    WORD bytesRead = 0;
    uint32_t chunkSize = 0;

    FIL* waveFile = (FIL*)&wavefile->fileptr;
    uint8_t* Buff = (uint8_t*)&wavefile->Buffer[0];
    memset( wavefile, 0 , sizeof(waveFile_t));

    if (f_open(waveFile, filename, FA_READ) != FR_OK) return WAVE_IO_ERROR + 1;
    if (f_lseek(waveFile, 0) != FR_OK) return WAVE_IO_ERROR + 2;

    /* Check RIFF-WAVE file header */
    if (f_read(waveFile, Buff, 12, &bytesRead)) return WAVE_IO_ERROR + 3;

    /* Make sure it is a WAVE file */
    if (bytesRead != 12 || LD_DWORD(Buff + 8) != FCC('W', 'A', 'V', 'E')) return WAVE_INVALID_FILE;

    for (;;)
    {
        /* Get Chunk ID and size */
        if (f_read(waveFile, Buff, 8, &bytesRead)) return WAVE_IO_ERROR + 4;
        if (bytesRead != 8) return WAVE_INVALID_FILE;
        chunkSize = LD_DWORD(Buff + 4); /* Chunk size */

        /* Switch by chunk type */
        switch (LD_DWORD(Buff))
        {
            case FCC('f', 'm', 't', ' '): /* 'fmt ' chunk */
                if (chunkSize & 1) chunkSize++;
                if (chunkSize > 128 || chunkSize < 16) return WAVE_INVALID_FILE; /* Check chunk size */
                if (f_read(waveFile, Buff, chunkSize, &bytesRead)) return WAVE_IO_ERROR + 5; /* Get the chunk content */
                if (bytesRead != chunkSize) return WAVE_INVALID_FILE;
                if (Buff[FMT_FORMAT] != 1) return WAVE_INVALID_FILE; /* Check coding type (1: LPCM) */
                if (Buff[FMT_NUM_CHANNELS] < 1 && Buff[FMT_NUM_CHANNELS] > 2) return WAVE_INVALID_FILE; /* Check channels (1/2: Mono/Stereo) */

                wavefile->waveHeader.channelCount = Buff[FMT_NUM_CHANNELS]; /* Save channel flag */
                /* Setup the offsets for stereo */
                if (wavefile->waveHeader.channelCount == 2)
                {
                    //isStereo = 1;
                }

                if (Buff[FMT_RESOLUTION] != 8 && Buff[FMT_RESOLUTION] != 16) return WAVE_INVALID_FILE; /* Check resolution (8/16 bit) */
                wavefile->waveHeader.resolution = Buff[FMT_RESOLUTION]; /* Save resolution flag */
                /* Setup the resolution byte offsets */
                if (wavefile->waveHeader.resolution == 16)
                {
                    //is16Bit = 1;
                }

                wavefile->waveHeader.sampleRate = LD_DWORD(&Buff[FMT_SAMPLERATE]); /* Check sampling freqency (8k-48k) */
                if (wavefile->waveHeader.sampleRate < WAVE_MINSAMPLE_RATE || wavefile->waveHeader.sampleRate > WAVE_MAXSAMPLE_RATE) return WAVE_INVALID_FILE;

                /* Set interval timer (sampling period) */
                /* Use OCR2 */
                waveSetSampleRate(wavefile->waveHeader.sampleRate);
                //PR1 = (F_CPU/wavefile->waveHeader.sampleRate) - 1;
                break;

            case FCC('d', 'a', 't', 'a'): /* 'data' chunk (start to play) */
                if (chunkSize < WAVE_MINIMUM_SAMPLES) return WAVE_INVALID_FILE; /* Check size */
                wavefile->waveHeader.dataSize = chunkSize;
                return chunkSize;

            case FCC('D', 'I', 'S', 'P'): /* 'DISP' chunk (skip) */
            case FCC('f', 'a', 'c', 't'): /* 'fact' chunk (skip) */
            case FCC('L', 'I', 'S', 'T'): /* 'LIST' chunk (skip) */
                if (chunkSize & 1) chunkSize++;
                if (f_lseek(waveFile, waveFile->fptr + chunkSize)) return WAVE_IO_ERROR + 6;
                break;

            default: /* Unknown chunk */
                return WAVE_INVALID_FILE;
        }
    }
}
