#include "ses_common.h"
#include "ses_button.h"
#include "ses_pwm.h"
#include "ses_timer.h"
#include "ses_led.h"

static volatile int flag_joystick = 0;

#define first_time_button_pressed             1 // showing that any button is pressed first time
#define second_time_button_pressed            2 // showing that any button is pressed second time

void callback_for_joystick(void)
{
    
    flag_joystick++;
    if(flag_joystick==first_time_button_pressed )
        {
            //OCR0B=0;
             pwm_setDutyCycle(170);
             //led_redOn();
        }
    else if(flag_joystick==second_time_button_pressed)
        {
            pwm_setDutyCycle(255);
            //led_redOff();
            flag_joystick=0;
        } 
}



int main(void)
{
    //led_redInit();
    pwm_init();
    button_setJoystickButtonCallback(callback_for_joystick);
    button_init(1); 
    timer1_start();
    sei();
    while(1)
    { 

    }
    

}