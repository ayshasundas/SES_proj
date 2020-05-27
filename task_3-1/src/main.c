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
 
volatile  pButtonCallback joystick; // global volatile function pointer for joystick button callback
volatile pButtonCallback rotary; // global volatile function pointer for rotary button callback
 
void led_greenInit(void) {
// Initializing green led 
DDR_REGISTER(LED_GREEN_PORT)|= (1 << LED_GREEN_PIN);
LED_GREEN_PORT |= (1<< LED_GREEN_PIN );
}

void led_greenToggle(void) {
// Toggling green led
LED_GREEN_PORT ^= (1 << LED_GREEN_PIN);
}
void led_greenOff(void) {
// Turning green led off
LED_GREEN_PORT |= (1<< LED_GREEN_PIN );
}

void led_redInit(void) {
// Initializing red led
DDR_REGISTER(LED_RED_PORT)|= (1<< LED_RED_PIN );
LED_RED_PORT |= (1 << LED_RED_PIN);
}

void led_redToggle(void) {
// toggling red led
LED_RED_PORT ^= (1<< LED_RED_PIN );
}
void led_redOff(void) {
// Turning red led off
LED_RED_PORT |= (1 << LED_RED_PIN);
}


void button_init(void){
//initializing buttons for both joystick and rotary and also enabling the interrupts for these buttons using pin change interrupt register
DDR_REGISTER(BUTTON_JOYSTICK_PORT) &= (~(1 << BUTTON_JOYSTICK_PIN));
DDR_REGISTER(BUTTON_ROTARY_PORT) &= (~(1 << BUTTON_ROTARY_PIN));
BUTTON_JOYSTICK_PORT |= (1 << BUTTON_JOYSTICK_PIN);
BUTTON_ROTARY_PORT |= (1 << BUTTON_ROTARY_PIN);
PCICR |= (1 << PCIE0);
PCMSK0 |= ((1<<PCINT6));
PCMSK0 |= ((1<<PCINT7));
}

bool button_isJoystickPressed(void){
//checking if joystick button is pressed
if((PIN_REGISTER(BUTTON_JOYSTICK_PORT) & (1 << BUTTON_JOYSTICK_PIN)) == 0){
    return 1;
}
else{
    return 0;
}
}

bool button_isRotaryPressed(void){
//checking if rotary button is pressed
if((PIN_REGISTER(BUTTON_ROTARY_PORT) & (1 << BUTTON_ROTARY_PIN)) == 0){
    return 1;
}
else{
    return 0;
}
}


ISR(PCINT0_vect){
// callback for ISR name= pcint0_vect
//It toggles the red led if interrupt for joystick button is triggered
//It toggles the green led if interrupt for rotary button is triggered
if (button_isJoystickPressed()==1 && ((PCMSK0 & (1<<PCINT7))!=0)){

  button_setJoystickButtonCallback(joystick);

}
else if(button_isRotaryPressed()==1 && ((PCMSK0 & (1<<PCINT6))!=0)){

    button_setRotaryButtonCallback(rotary); 
   
}
}

void button_setJoystickButtonCallback(pButtonCallback callback)
{//callback for joystick button
    if(callback!=NULL)
{
callback();
}

}



void button_setRotaryButtonCallback(pButtonCallback callback)
{//callback for rotary button
    if(callback!=NULL)
{
callback();
}

}







//main function
int main(void) {

joystick= led_redToggle; // passing address to the function pointer for joystick_button callback
rotary= led_greenToggle; // passing address to the function pointer for joystick_button callback
led_redInit();
led_greenInit();
sei();// enabling global interrupts
button_init();

while(1)
{
//doing nothing just wasting CPU cycles :D
}

}