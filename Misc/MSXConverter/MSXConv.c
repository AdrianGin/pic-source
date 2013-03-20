#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "hardUart/hardUart.h"
#include "MSX_Mouse.h"
#include "PS2_Mouse.h"


volatile AVR_USART_t PrimaryUART = {
   &UCSR0A,
   &UCSR0B,
   &UCSR0C,
   &UBRR0H,
   &UBRR0L,
   &UDR0,

   &ReceiveBuffer,
   &TransmitBuffer
};


PS2_Position_t event;
volatile PS2_Position_t readyData;


volatile uint8_t dataReady;
volatile uint8_t dataSent = 1;



int main(void)
{

    uint8_t dataIn;
    uint8_t outputString[10];

    uartInit(&PrimaryUART, 0);
    uartSetBaud(&PrimaryUART, 0, BAUD(9600));

    PS2_Init();

    sei();

    MSX_POS_DIR |= MSX_POS_PINS;
    MSX_TRIGGER_DIR |= (MSX_LEFT_BUTTON_PIN | MSX_RIGHT_BUTTON_PIN);
    MSX_TRIGGER_PORT |= (MSX_LEFT_BUTTON_PIN | MSX_RIGHT_BUTTON_PIN);

    uint8_t MSX_Ready = 1;
        
    while(1)
    {
        dataReady = PS2_GetPositionPacket(&event);
        if( dataReady )
        {
            if( MSX_Ready == 1)
            {
                readyData.xPos = event.xPos;
                readyData.yPos = event.yPos;
                readyData.buttonMap = event.buttonMap;
                MSX_Ready = 0;
         
            }  

            uint8_t xPos = event.xPos;
            uint8_t yPos = event.yPos;
            uint8_t buttonMap = event.buttonMap;

			uartTx(&PrimaryUART, buttonMap);

/*
            uartTxString(&PrimaryUART, "Buttons: ");
            utoa(buttonMap, outputString, 16);
            uartTxString(&PrimaryUART, outputString);
            uartNewLine(&PrimaryUART);

            uartTxString(&PrimaryUART, "X: ");
            utoa(xPos, outputString, 10);
            uartTxString(&PrimaryUART, outputString);
            uartNewLine(&PrimaryUART);

            uartTxString(&PrimaryUART, "Y: ");
            utoa(yPos, outputString, 10);
            uartTxString(&PrimaryUART, outputString);
            uartNewLine(&PrimaryUART);*/
            //MSX_SendPosition(event.xPos, event.yPos);         
        }


        if( MSX_Ready == 0 )
        {

            if( MSX_SendPosition(readyData.xPos, readyData.yPos) )
            {   
                    MSX_Ready = 1;   
            }
        
            if( (readyData.buttonMap & MSX_LEFT_TRIGGER) || (readyData.buttonMap & MSX_RIGHT_TRIGGER ))
            {
                MSX_SendButtons(readyData.buttonMap);
            }

            if( !(readyData.buttonMap & MSX_LEFT_TRIGGER) )
            {
                MSX_TRIGGER_PORT |= (MSX_LEFT_BUTTON_PIN);
            }

            if( !(readyData.buttonMap & MSX_RIGHT_TRIGGER) )
            {
                MSX_TRIGGER_PORT |= (MSX_RIGHT_BUTTON_PIN);
            }


        }


        

    }
    

    return 0;
}





ISR(USART_TX_vect, ISR_NOBLOCK)
{
   // Tx the next byte if there are still bytes in the buffer
   if( !ringbuffer_isEmpty((RINGBUFFER_T*)PrimaryUART.TransmitBuffer) )
   {
      UDR0 = ringbuffer_get((RINGBUFFER_T*)PrimaryUART.TransmitBuffer);
   }
}


ISR(USART_RX_vect)
{
   uint8_t buffer = UDR0;

   PS2_SendByte(buffer);
}



