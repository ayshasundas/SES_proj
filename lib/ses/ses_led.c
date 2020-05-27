/* INCLUDES ******************************************************************/

#include "ses_common.h"
#include "ses_led.h"

/* DEFINES & MACROS **********************************************************/

// LED wiring on SES board
#define LED_RED_PORT       	PORTG //PG1
#define LED_RED_PIN         	1

#define LED_YELLOW_PORT 	PORTF //PF7
#define LED_YELLOW_PIN      	7

#define LED_GREEN_PORT 		PORTF //PF6
#define LED_GREEN_PIN       	6

/* FUNCTION DEFINITION *******************************************************/

void led_redInit(void) {
// making it as output port 
DDR_REGISTER(LED_RED_PORT)|= (1<< LED_RED_PIN );
LED_RED_PORT |= (1 << LED_RED_PIN);

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
// making it as output port
DDR_REGISTER(LED_YELLOW_PORT)|= (1 << LED_YELLOW_PIN);
LED_YELLOW_PORT |= ( 1<< LED_YELLOW_PIN);
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
// making it as output port
DDR_REGISTER(LED_GREEN_PORT)|= (1 << LED_GREEN_PIN);
LED_GREEN_PORT |= (1<< LED_GREEN_PIN );
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