#include <avr/io.h>
#include <util/delay.h>
#include "ses_common.h"
#include "ses_led.h"
#include "ses_button.h"
#include "ses_timer.h"
#include "ses_uart.h"

void softwareTimer1(void){
//toggling yellow led every 1 sec   
static uint16_t counter=0;//counter to decrease the frequency of yellow led
counter++;
if(counter >= 200){ 
led_yellowToggle();//toggling every 1 sec
counter=0;
}
}


/**Toggles the red LED of the SES-board*/
int main(void) {
uart_init(57600);
button_setRotaryButtonCallback(led_greenToggle); // setting callback for joystick_button callback
button_setJoystickButtonCallback(led_redToggle); // setting callback for joystick_button callback
led_redInit();
led_greenInit();
sei();  
button_init(1);
timer1_start();
while(1){
//Do nothing    
}

}

