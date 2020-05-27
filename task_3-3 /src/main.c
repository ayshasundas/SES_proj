#include <avr/io.h>
#include <util/delay.h>
#include <ses_common.h>
#include <ses_led.h>
#include "ses_timer.h"

/* DEFINES & MACROS **********************************************************/
#define TIMER1_CYC_FOR_5MILLISEC    77//Top(value stored in OCRA) value to be compared to with counter
#define TIMER2_CYC_FOR_1MILLISEC   249	//Top(value stored in OCRA) value to be compared to with counter
volatile pTimerCallback t2;
volatile pTimerCallback t1;

#define LED_YELLOW_PORT 	PORTF //PF7
#define LED_YELLOW_PIN      	7

/*FUNCTION DEFINITION ********************************************************/
void timer2_setCallback(pTimerCallback cb) {
	// calling function if its pointer is valid
if(cb!=NULL){
	cb();
}

}

void timer2_start() {
	// setting CTC mode of operation, enabling the compare match A interrupt, setting prescalar to 64
	// And setting Top(value stored in OCRA) value to be compared to with counter to 249
	TCCR2A|= (1<<WGM21);
	TCCR2A &= (~(1<<WGM20));
	TCCR2B |= (1<<CS22);
	TCCR2B &= (~((1<<WGM22) | (1<<CS21) | (1<<CS20)));
    TIMSK2 |= (1<<OCIE2A);
	OCR2A=TIMER2_CYC_FOR_1MILLISEC;
}


void timer2_stop() {
    // stoping the timer by selecting no clock source
	TCCR2B &= (~((1<<CS22) | (1<<CS21) | (1<<CS20)));
}

void softwareTimer1(void){
//toggling yellow led every 1 sec	
static uint16_t counter=0;//counter to decrease the frequency of yellow led
counter++;
if(counter >= 200){	
led_yellowToggle();//toggling every 1 sec
counter=0;
}
}

void softwareTimer2(void){
//toggling yellow led every 1 sec	
static uint16_t counter=0;//counter to decrease the frequency of yellow led
counter++;
if(counter >= 1000){	
led_yellowToggle();//toggling every 1 sec
counter=0;
}
}

void timer1_setCallback(pTimerCallback cb) {
	// calling function if its pointer is valid
if(cb!=NULL){
	cb();
}	
}


void timer1_start() {
// setting CTC mode of operation, enabling the compare match A interrupt, setting prescalar to 64
// And setting Top(value stored in OCRA) value to be compared to with counter to 77
	TCCR1B|= ((1<<WGM12) | (1<<CS12) | (1<<CS10));
	TCCR1A &= (~((1<<WGM10) | (1<<WGM11)));
	TCCR1B &= (~((1<<WGM13) | (1<<CS11)));
    TIMSK1 |= (1<<OCIE1A);
	OCR1A=TIMER1_CYC_FOR_5MILLISEC;
}


void timer1_stop() {
// stoping the timer by selecting no clock source
TCCR1B &= (~((1<<CS12) | (1<<CS11) | (1<<CS10)));
}

ISR(TIMER1_COMPA_vect) {
// clearing interrupt flag for compare A
//calling callback function for timer1
TIFR1 |= (1<<OCF1A);
timer1_setCallback(t1);		
}

ISR(TIMER2_COMPA_vect) {
// clearing interrupt flag for compare A
//calling callback function for timer2
TIFR2 |= (1<<OCF2A);
timer2_setCallback(t2);	
}

void led_yellowInit(void) {
// TODO
DDR_REGISTER(LED_YELLOW_PORT)|= (1 << LED_YELLOW_PIN);
LED_YELLOW_PORT |= ( 1<< LED_YELLOW_PIN);
}

void led_yellowToggle(void) {
// 
LED_YELLOW_PORT ^= (1 << LED_YELLOW_PIN);
}

void led_yellowOff(void) {
// 
LED_YELLOW_PORT |= ( 1<< LED_YELLOW_PIN);
}


/**Toggles the red LED of the SES-board*/
int main(void) {
led_yellowInit();
timer1_start();
t2=	softwareTimer2;
t1=	softwareTimer1;
sei();
while(1){
//Do nothing 	
}

}
