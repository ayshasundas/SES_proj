#include <avr/io.h>
#include <util/delay.h>
#include "ses_common.h"
#include "ses_led.h"
#include "ses_timer.h"


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


/**Toggles the red LED of the SES-board*/
int main(void) {
led_yellowInit();
timer2_start();
timer2_setCallback(softwareTimer2);
sei();
while(1){
//Do nothing 	
}

}
