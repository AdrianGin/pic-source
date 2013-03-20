#ifndef PS2_MOUSE_H
#define PS2_MOUSE_H


#define PS2_MOUSE_X_OVF  (1<<6)
#define PS2_MOUSE_Y_OVF  (1<<7)
#define PS2_MOUSE_X_SIGN (1<<4)
#define PS2_MOUSE_Y_SIGN (1<<5)

#define PS2_MOUSE_LEFT  (1<<0)
#define PS2_MOUSE_RIGHT (1<<1)

#define PS2_DATA_ERROR  (2)
#define PS2_DATA_READY  (1)
#define PS2_NO_DATA     (0)

#define PS2_DIR     (DDRB)
#define PS2_OUT     (PORTB)
#define PS2_PIN     (PINB)
#define PS2_CLK     (1<<6)
#define PS2_DATA    (1<<7)

/* Data is ready when the clk is low */
#define PS2_DATARDY ((PINB & PS2_CLK) == 0 ? 1 : 0)
#define PS2_DATABIT ((PINB & PS2_DATA) == 0 ? 0 : 1)

#define PS2_STARTBIT (PS2_DATARDY && !PS2_DATABIT)
#define PS2_STOPBIT  (PS2_DATARDY &&  PS2_DATABIT)
#define PS2_ACKBIT   (!PS2_DATARDY && PS2_DATABIT)

#define PS2_CLK_LOW  (PS2_DATARDY)
#define PS2_CLK_HIGH (!PS2_DATARDY)

#define PS2_WAIT_CLK_LOW()    while(PS2_CLK_HIGH){;}
#define PS2_WAIT_CLK_HIGH()   while(PS2_CLK_LOW){;}

#define PS2_WAIT_DATA_LOW()     while(!PS2_DATABIT){;}
#define PS2_WAIT_DATA_HIGH()    while(PS2_DATABIT){;}

#define PS2_SET_DATA()  PS2_OUT |= PS2_DATA
#define PS2_CLR_DATA()  PS2_OUT &= ~PS2_DATA


typedef struct
{
    uint8_t xPos;
    uint8_t yPos;
    uint8_t buttonMap;

} PS2_Position_t;




uint8_t PS2_GetByte(uint8_t* inByte);
void PS2_SendByte(uint8_t outByte);
uint8_t PS2_GetPositionPacket(PS2_Position_t* event);
void PS2_Init(void);
uint8_t PS2_ValidateData(PS2_Position_t* input);






#endif

