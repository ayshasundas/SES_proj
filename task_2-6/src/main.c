/* INCLUDES ******************************************************************/
#include <avr/io.h>
#include <util/delay.h>
#include "ses_common.h"
#include "ses_led.h"
#include "ses_lcd.h"

/* DEFINES & MACROS **********************************************************/

// LED wiring on SES board
#define LED_RED_PORT       	PORTG //PG1
#define LED_RED_PIN         	1

#define LED_YELLOW_PORT 	PORTF //PF7
#define LED_YELLOW_PIN      	7

#define LED_GREEN_PORT 		PORTF //PF6
#define LED_GREEN_PIN       	6

// BUTTON wiring on SES board
#define BUTTON_JOYSTICK_PORT       	PORTB //PB7
#define BUTTON_JOYSTICK_PIN         	7

#define BUTTON_ROTARY_PORT 	   PORTB //PB6
#define BUTTON_ROTARY_PIN      	6

/* FUNCTION DEFINITION *******************************************************/

void led_redInit(void) {
// TODO
DDR_REGISTER(LED_RED_PORT)|= (1<< LED_RED_PIN );

}

void led_redToggle(void) {
// TODO
LED_RED_PORT ^= (1<< LED_RED_PIN );
}

void led_redOn(void) {
// TODO
LED_RED_PORT &= (~(1 << LED_RED_PIN));
}

void led_redOff(void) {
// TODO
LED_RED_PORT |= (1 << LED_RED_PIN);
}

void led_yellowInit(void) {
// TODO
DDR_REGISTER(LED_YELLOW_PORT)|= (1 << LED_YELLOW_PIN);

}

void led_yellowToggle(void) {
// TODO
LED_YELLOW_PORT ^= (1 << LED_YELLOW_PIN);
}

void led_yellowOn(void) {
// TODO
LED_YELLOW_PORT &= (~(1 << LED_YELLOW_PIN));
}

void led_yellowOff(void) {
// TODO
LED_YELLOW_PORT |= ( 1<< LED_YELLOW_PIN);
}

void led_greenInit(void) {
// TODO
DDR_REGISTER(LED_GREEN_PORT)|= (1 << LED_GREEN_PIN);
}

void led_greenToggle(void) {
// TODO
LED_GREEN_PORT ^= (1 << LED_GREEN_PIN);
}

void led_greenOn(void) {
// TODO
LED_GREEN_PORT &= (~(1 << LED_GREEN_PIN) );
}

void led_greenOff(void) {
// TODO
LED_GREEN_PORT |= (1<< LED_GREEN_PIN );
}
//*******************BUTTON FUNCTIONS ***************************************
void button_init(void){
DDR_REGISTER(BUTTON_JOYSTICK_PORT) &= (~(1 << BUTTON_JOYSTICK_PIN));
DDR_REGISTER(BUTTON_ROTARY_PORT) &= (~(1 << BUTTON_ROTARY_PIN));
BUTTON_JOYSTICK_PORT |= (1 << BUTTON_JOYSTICK_PIN);
BUTTON_ROTARY_PORT |= (1 << BUTTON_ROTARY_PIN);
}

bool button_isJoystickPressed(void){
if((PIN_REGISTER(BUTTON_JOYSTICK_PORT) & (1 << BUTTON_JOYSTICK_PIN))==0){
    return 1;
}
else{
    return 0;
}
}

bool button_isRotaryPressed(void){
if((PIN_REGISTER(BUTTON_ROTARY_PORT) & (1 << BUTTON_ROTARY_PIN))==0){
    return 1;
}
else{
    return 0;
}
}

int main(void)
{
lcd_init();
led_redInit();
led_greenInit();
button_init();
int i=0;
while(1){

fprintf(lcdout, "Time since reset \n %d sec",i);

if(button_isJoystickPressed()==1 && button_isRotaryPressed()==1)	{
	led_redOn();
	led_greenOn();
}

else if(button_isJoystickPressed()==1){
led_greenOn();
}

else if(button_isRotaryPressed()==1){
led_redOn();
}

else
{
led_greenOff();
led_redOff();
}

i++;
_delay_ms(1000);
lcd_clear();
lcd_init();
}

}

