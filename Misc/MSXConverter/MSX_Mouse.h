#ifndef MSX_MOUSE_H
#define MSX_MOUSE_H

#define MSX_STROBE              (1<<0)
#define MSX_LEFT_BUTTON_PIN     (1<<1)
#define MSX_RIGHT_BUTTON_PIN    (1<<2)

#define MSX_TRIGGER_DIR  (DDRB)
#define MSX_TRIGGER_PORT (PORTB)
#define MSX_STROBE_PIN   (PINB)

#define MSX_POS_DIR     (DDRD)
#define MSX_POS_PORT    (PORTD)
#define MSX_POS_PINS    (0xF0)

#define MSX_MAX_MOUSE_VAL (80)

#define MSX_MAX_X_DATA  (MSX_MAX_MOUSE_VAL)
#define MSX_MIN_X_DATA  (-MSX_MAX_MOUSE_VAL)

#define MSX_MAX_Y_DATA  (MSX_MAX_MOUSE_VAL)
#define MSX_MIN_Y_DATA  (-MSX_MAX_MOUSE_VAL)

#define MSX_WAIT_FOR_LOW() while( (MSX_STROBE_PIN & MSX_STROBE) ){;}
#define MSX_WAIT_FOR_HIGH() while( !(MSX_STROBE_PIN & MSX_STROBE) ){;}

/* The button Map format */
#define MSX_LEFT_TRIGGER (1<<0)
#define MSX_RIGHT_TRIGGER (1<<1)

uint8_t MSX_SendPosition(uint8_t xPos, uint8_t yPos);
uint8_t MSX_SendButtons(uint8_t buttonMap);
void MSX_ValidateInputs(int8_t* xPos, int8_t* yPos);


#endif
