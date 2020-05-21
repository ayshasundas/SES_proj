#include <avr/io.h>
#include <util/delay.h>
#include "ses_button.h"
#include "ses_common.h"
#include "ses_uart.h"

#define BUTTON_JOYSTICK_PORT       	PORTB //PB7
#define BUTTON_JOYSTICK_PIN         	7

#define BUTTON_ROTARY_PORT 	   PORTB //PB6
#define BUTTON_ROTARY_PIN      	6

#define LED_RED_PORT       	PORTG //PG1
#define LED_RED_PIN         	1

#define LED_GREEN_PORT 		PORTF //PF6
#define LED_GREEN_PIN       	6
 
volatile  pButtonCallback joystick;
 volatile pButtonCallback rotary;
 
void led_greenInit(void) {
// TODO
DDR_REGISTER(LED_GREEN_PORT)|= (1 << LED_GREEN_PIN);
}

void led_greenToggle(void) {
// TODO
LED_GREEN_PORT ^= (1 << LED_GREEN_PIN);
}
void led_greenOff(void) {
// TODO
LED_GREEN_PORT |= (1<< LED_GREEN_PIN );
}

void led_redInit(void) {
// TODO
DDR_REGISTER(LED_RED_PORT)|= (1<< LED_RED_PIN );

}

void led_redToggle(void) {
// TODO
LED_RED_PORT ^= (1<< LED_RED_PIN );
}
void led_redOff(void) {
// TODO
LED_RED_PORT |= (1 << LED_RED_PIN);
}


void button_init(void){
DDR_REGISTER(BUTTON_JOYSTICK_PORT) &= (~(1 << BUTTON_JOYSTICK_PIN));
DDR_REGISTER(BUTTON_ROTARY_PORT) &= (~(1 << BUTTON_ROTARY_PIN));
BUTTON_JOYSTICK_PORT |= (1 << BUTTON_JOYSTICK_PIN);
BUTTON_ROTARY_PORT |= (1 << BUTTON_ROTARY_PIN);
PCICR |= (1 << PCIE0);
PCMSK0 |= ((1<<PCINT6));
PCMSK0 |= ((1<<PCINT7));
}

bool button_isJoystickPressed(void){
if((PIN_REGISTER(BUTTON_JOYSTICK_PORT) & (1 << BUTTON_JOYSTICK_PIN)) == 0){
    return 1;
}
else{
    return 0;
}
}

bool button_isRotaryPressed(void){
if((PIN_REGISTER(BUTTON_ROTARY_PORT) & (1 << BUTTON_ROTARY_PIN)) == 0){
    return 1;
}
else{
    return 0;
}
}


ISR(PCINT0_vect){
// execute callbacks here
if (button_isJoystickPressed()==1 && ((PCMSK0 & (1<<PCINT7))!=0)){

   joystick= led_redToggle;

}
else if(button_isRotaryPressed()==1 && ((PCMSK0 & (1<<PCINT6))!=0)){

     rotary= led_greenToggle;
   
}
}

void button_setJoystickButtonCallback(pButtonCallback callback)
{
    if(callback)
{
callback();
}

}



void button_setRotaryButtonCallback(pButtonCallback callback)
{
    if(callback)
{
callback();
}

}







/**Toggles the red LED of the SES-board*/
int main(void) {

uart_init(57600); 
joystick= led_redOff;
rotary= led_greenOff;
led_redInit();
led_greenInit();
sei();
button_init();

while(1)
{
button_setJoystickButtonCallback(joystick);
_delay_ms(1000);
button_setRotaryButtonCallback(rotary);
}

}