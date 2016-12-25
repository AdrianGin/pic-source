#ifndef _USB_MIDI_H
#define _USB_MIDI_H




#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "Ringbuffer.h"


#define TO_HST_ENDPOINTS (1)
#define TO_DEV_ENDPOINTS (1)

#define MIDI_TICK       (0xF8)
#define MIDI_START      (0xFA)
#define MIDI_CONTINUE   (0xFB)
#define MIDI_STOP       (0xFC)



enum {
	USB_MIDI_REF = 0,
	DIN_MIDI_REF,
	NUM_MIDI_REF,
};


typedef struct
{
   uint8_t header;
   uint8_t data[3];

} USBMIDI_Packet_t;


typedef struct USBMIDI USBMIDI_t;

struct USBMIDI
{
   uint8_t refcableIndex;

   USBMIDI_Port_t* USBMIDI_TxPort;
   USBMIDI_Port_t* USBMIDI_RxPort;

   Ringbuffer_t* TxFIFO;
   Ringbuffer_t* RxFIFO;

   void (*MsgReceivedCallback)(USBMIDI_t*, Ringbuffer_t*);
};

extern Ringbuffer_t FIFO_USB_ToHost[];
extern Ringbuffer_t FIFO_USB_ToDevice[];

extern USBMIDI_t USBMIDIPort;

/**
 * @brief Called within the main loop, this function checks to see if there are any pending messages
 *        in the TxBuffer to be sent. If there are it initiates a transfer.
 *
 *        If there are received bytes from the Host, it copy these into the RxBuffer.
 *
 * @param Context
 */
void USBMIDI_Poll(USBMIDI_t* Context);

/**
 * @brief Empties the Intenral USB Buffer into a user defined buffer and invokes the Rx callback
 * @param Context The instance of the USBMIDI Port
 * @return the number of bytes processed from the Internal Buffer to the User Defined buffer
 */
uint32_t USBMIDI_RxPoll(USBMIDI_t* Context);

/**
 * @brief Function used to populate the TxBuffer, user should check that there is space in the buffer before
 *        invoking this.
 * @param Context The instance of the USBMIDI port
 * @param buf     Buffer stream to send
 * @param size    Number of bytes in the buffer stream
 * @returns number of bytes sucessfully written;
 */
uint8_t USBMIDI_TxBuffer(USBMIDI_t* Context, uint8_t* buf, uint16_t size);
/**
 * @brief This is used to send any bytes in the TxBuffer to the Host
 *        The TxBuffer should be populated using the USBMIDI_TxBuffer
 * @param Context The instance of the USBMIDI port
 * @return number of bytes in the buffer still to send.
 */
uint16_t USBMIDI_TxPoll(USBMIDI_t* Context);


/**
 * @brief Used as a simple example of how to receive a USBMIDI byte stream
 * @param Context The USBMIDI cable which has data pending
 * @param rb The RingBuffer containing the data
 */
void USBMIDI_DefaultRxCallback(USBMIDI_t* Context, Ringbuffer_t* rb);


/**
 * @brief Used as a simple example of how to send a single stream of bytes to MIDI. This does not support sending SysEx
 * @param byte The MIDI byte stream.
 */
void USBMIDI_DefaultTxByte(uint8_t byte);


uint8_t USBMIDI_HealthCheck(void);

#ifdef __cplusplus
}
#endif

#endif
