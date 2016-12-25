// ____________________________________________________________________________
//
// USB MIDI for Raw USB Access
// ____________________________________________________________________________
//

//C headers
#ifdef __cplusplus
extern "C"
{
#endif


#include <string.h>


#include "USB.h"
#include "usbd_desc.h"
#include "usbd_audio_core.h"
#include "Ringbuffer.h"


#include "USBMIDI.h"


enum
{
   MIDI_TX_FIFO_SIZE = 256,
   MIDI_RX_FIFO_SIZE = 256,
};

uint8_t USBMIDI_TxFIFO[MIDI_TX_FIFO_SIZE];
uint8_t USBMIDI_RxFIFO[MIDI_RX_FIFO_SIZE];

uint8_t USBMIDI_TxFIFO_HS[MIDI_TX_FIFO_SIZE];
uint8_t USBMIDI_RxFIFO_HS[MIDI_RX_FIFO_SIZE];


Ringbuffer_t FIFO_USB_ToHost[TO_HST_ENDPOINTS] =
{
      {&USBMIDI_TxFIFO[0], MIDI_TX_FIFO_SIZE, 0,0 },
      {&USBMIDI_TxFIFO_HS[0], MIDI_TX_FIFO_SIZE, 0,0 },
};

Ringbuffer_t FIFO_USB_ToDevice[TO_DEV_ENDPOINTS] =
{
      {&USBMIDI_RxFIFO[0], MIDI_RX_FIFO_SIZE, 0,0 },
      {&USBMIDI_RxFIFO_HS[0], MIDI_RX_FIFO_SIZE, 0,0 },
};


USBMIDI_t USBMIDIPort = {
      .refcableIndex = 0,
      .USBMIDI_TxPort =  &USBMIDI_Tx,
      .USBMIDI_RxPort =  &USBMIDI_Rx,

      .TxFIFO = &FIFO_USB_ToHost[0],
      .RxFIFO = &FIFO_USB_ToDevice[0],

      .MsgReceivedCallback = USBMIDI_DefaultRxCallback,
};


USBMIDI_t USBMIDIPort_HS = {
      .refcableIndex = 0,
      .USBMIDI_TxPort =  &USBMIDI_Tx_HS,
      .USBMIDI_RxPort =  &USBMIDI_Rx_HS,

      .TxFIFO = &FIFO_USB_ToHost[1],
      .RxFIFO = &FIFO_USB_ToDevice[1],

      .MsgReceivedCallback = USBMIDI_DefaultRxCallback,
};



void USBMIDI_DefaultRxCallback(USBMIDI_t* Context, Ringbuffer_t* rb)
{
   // USB MIDI data always enters in four-byte groups
   uint8_t type, s, d1, d2;

   while ( RingBuffer_GetSpaceUsed(rb) >= sizeof(USBMIDI_Packet_t))
   {
      RingBuffer_Read(rb, &type);
      RingBuffer_Read(rb, &s);
      RingBuffer_Read(rb, &d1);
      RingBuffer_Read(rb, &d2);

      USBMIDI_Packet_t msg;
      msg.header = type;
      msg.data[0] = s;
      msg.data[1] = d1;
      msg.data[2] = d2;

      //Do a simple echo here
      USBMIDI_TxBuffer(Context, (uint8_t*)&msg, sizeof(USBMIDI_Packet_t) );

   }
}


void USBMIDI_DefaultTxByte(uint8_t byte)
{
   if( (USBMIDI_HealthCheck() == USBD_CONFIGURED) && (RingBuffer_GetSpaceAvailable(USBMIDIPort.TxFIFO) >= sizeof(USBMIDI_Packet_t)))
   {

      if( RingBuffer_GetSpaceUsed(USBMIDIPort.TxFIFO) % sizeof(USBMIDI_Packet_t) == 0)
      {
         RingBuffer_Write(USBMIDIPort.TxFIFO, byte >> 4);
      }
      RingBuffer_Write(USBMIDIPort.TxFIFO, byte);
   }
}


/**
 * @brief Called within the main loop, this function checks to see if there are any pending messages
 *        in the TxBuffer to be sent. If there are it initiates a transfer.
 *
 *        If there are received bytes from the Host, it copy these into the RxBuffer.
 *
 * @param Context
 */
void USBMIDI_Poll(USBMIDI_t* Context)     // send any messages
{

   USBMIDI_RxPoll(Context);

   USBMIDI_TxPoll(Context);

   //Continue to process more Messages, until the Internal USB Buffer has been processed.
   if( USBD_audio_MIDI_Port_byteCount(Context->USBMIDI_RxPort) != 0)
   {
      USBD_audio_SetState(Context->USBMIDI_RxPort, USBMIDI_READY);
   }

   //Allow the host to know we are ready to receive more data
   if( (USBD_audio_GetState(Context->USBMIDI_RxPort) != USBMIDI_READY) &&
         (USBD_audio_MIDI_Port_byteCount(Context->USBMIDI_RxPort) == 0) )
   {
      USBD_audio_SetState(Context->USBMIDI_RxPort, USBMIDI_READY);
      USBD_audio_RxEPReady(Context->USBMIDI_RxPort);
   }

}



uint32_t USBMIDI_RxPoll(USBMIDI_t* Context)
{
   // USB MIDI data always enters in four-byte groups
   USBMIDI_Port_t* usbmidirx = Context->USBMIDI_RxPort;
   uint32_t bytesToProcess = USBD_audio_MIDI_Port_byteCount(usbmidirx);
   uint32_t i = 0;

   if( (bytesToProcess) && ( USBD_audio_GetState(usbmidirx) != USBMIDI_PROCESSING) )
   {
      uint8_t* buf = usbmidirx->buf;
      uint8_t readOffset = USBD_audio_MIDI_Port_GetReadPtr(usbmidirx);

      for (i = 0; (i < bytesToProcess) && (RingBuffer_GetSpaceAvailable(Context->RxFIFO) >= sizeof(USBMIDI_Packet_t)) ; i = i + sizeof(USBMIDI_Packet_t))
      {
         //uint8_t msgType = buf[i + readOffset + 0] & 0x0F;

         USBD_audio_MIDI_Port_SetReadPtr(usbmidirx, USBD_audio_MIDI_Port_GetReadPtr(usbmidirx) + sizeof(USBMIDI_Packet_t) );

         RingBuffer_WriteBuffer(Context->RxFIFO, &buf[i + readOffset], sizeof(USBMIDI_Packet_t));
         if( Context->MsgReceivedCallback )
         {
            Context->MsgReceivedCallback(Context, Context->RxFIFO);
         }
      }

      USBD_audio_SetState(usbmidirx, USBMIDI_PROCESSING);
   }

   return bytesToProcess;
}


/**
 * @brief Function used to populate the TxBuffer, user should check that there is space in the buffer before
 *        invoking this.
 * @param Context The instance of the USBMIDI port
 * @param buf     Buffer stream to send
 * @param size    Number of bytes in the buffer stream
 * @returns number of bytes sucessfully written;
 */
uint8_t USBMIDI_TxBuffer(USBMIDI_t* Context, uint8_t* buf, uint16_t size)
{

   Ringbuffer_t* OutputFifo = Context->TxFIFO;
   uint16_t bytesWritten = 0;
   if ((USBMIDI_HealthCheck() == USBD_CONFIGURED) && (RingBuffer_GetSpaceAvailable(OutputFifo) >= size))
   {
      bytesWritten = RingBuffer_WriteBuffer(OutputFifo, buf, size);
   }
   return bytesWritten;
}

/**
 * @brief This is used to send any bytes in the TxBuffer to the Host
 *        The TxBuffer should be populated using the USBMIDI_TxBuffer
 * @param Context The instance of the USBMIDI port
 * @return number of bytes in the buffer still to send.
 */
uint16_t USBMIDI_TxPoll(USBMIDI_t* Context)
{
   USBMIDI_Port_t* usbmiditx = Context->USBMIDI_TxPort;

   uint16_t dataBytes = RingBuffer_GetSpaceUsed(Context->TxFIFO);
   uint16_t bytesToSend = dataBytes;
   if(dataBytes && (USBD_audio_GetState(usbmiditx) == USBMIDI_READY) )
   {
      uint16_t packetCount = 0;
      uint8_t* buf = usbmiditx->buf;
      while( (bytesToSend >= sizeof(USBMIDI_Packet_t)) && ( (packetCount * sizeof(USBMIDI_Packet_t)) < (usbmiditx->bufSize)) )
      {
         uint16_t index = packetCount * sizeof(USBMIDI_Packet_t);
         RingBuffer_ReadBuffer(Context->TxFIFO, &buf[index], sizeof(USBMIDI_Packet_t) );
         packetCount++;
         bytesToSend = bytesToSend - sizeof(USBMIDI_Packet_t);
      }

      USBD_audio_SetState(usbmiditx, USBMIDI_TRANSMITING);
      USBD_audio_MIDI_Port_SetWritePtr(usbmiditx, packetCount * sizeof(USBMIDI_Packet_t));
      USBD_audio_TxEP(usbmiditx, usbmiditx->epnum, &buf[0], packetCount * sizeof(USBMIDI_Packet_t));
   }

   return dataBytes;
}

uint8_t USBMIDI_HealthCheck(void)
{
   return USB_Device_GetState(&USB_User_Dev);
}


//Resets the USB related FIFOs and Parsers
//Used when a USB Connect/Resume is detected.
//Used when USB is suspended too.
void USBMIDI_ClearBuffers(USBMIDI_t* Context)
{
   RingBuffer_Clear(Context->TxFIFO);
   RingBuffer_Clear(Context->RxFIFO);

   USDB_audio_MIDI_Port_FlushBuffers(Context->USBMIDI_RxPort);
   USDB_audio_MIDI_Port_FlushBuffers(Context->USBMIDI_TxPort);
}


// ____________________________________________________________________________
//
#ifdef __cplusplus
}
#endif

