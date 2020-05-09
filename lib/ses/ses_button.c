#include "ses_common.h"
#include "ses_button.h"

#define BUTTON_JOYSTICK_PORT       	PORTB //PB7
#define BUTTON_JOYSTICK_PIN         	7

#define BUTTON_ROTARY_PORT 	   PORTB //PB6
#define BUTTON_ROTARY_PIN      	6



void button_init(void){
DDR_REGISTER(BUTTON_JOYSTICK_PORT) &= (~(1 << BUTTON_JOYSTICK_PIN));
DDR_REGISTER(BUTTON_ROTARY_PORT) &= (~(1 << BUTTON_ROTARY_PIN));
BUTTON_JOYSTICK_PORT |= (1 << BUTTON_JOYSTICK_PIN);
BUTTON_ROTARY_PORT |= (1 << BUTTON_ROTARY_PIN);
}

bool button_isJoystickPressed(void){
if(PIN_REGISTER(BUTTON_JOYSTICK_PORT) & (1 << BUTTON_JOYSTICK_PIN)==0){
    return 1;
}
else{
    return 0;
}
}

bool button_isRotaryPressed(void){
if(PIN_REGISTER(BUTTON_ROTARY_PORT) & (1 << BUTTON_ROTARY_PIN)==0){
    return 1;
}
else{
    return 0;
}
}
