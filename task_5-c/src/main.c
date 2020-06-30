#include "ses_common.h"
#include "ses_button.h"
#include "ses_pwm.h"
#include "ses_motorFrequency.h"
#include "ses_timer.h"
#include "ses_led.h"
#include "ses_uart.h"
#include "ses_lcd.h"
#include "ses_scheduler.h"

static volatile int flag_joystick = 0;
static taskDescriptor td1;
uint16_t motorSpeed_rpm=0;

#define first_time_button_pressed             1 // showing that any button is pressed first time
#define second_time_button_pressed            2 // showing that any button is pressed second time

void callback_for_joystick(void)
{
    
    flag_joystick++;
    if(flag_joystick==first_time_button_pressed )
        {
            //OCR0B=0;
             pwm_setDutyCycle(100);
             //led_redOn();
        }
    else if(flag_joystick==second_time_button_pressed)
        {
            pwm_setDutyCycle(255);
            //led_redOff();
            flag_joystick=0;
        } 
}


void Motor_freq_rpm(void * ptr)
{
    lcd_init();
    lcd_clear();

    if(motorFrequency_getRecent()==0 || motorFrequency_getMedian()==0)
    {
        led_greenOn();
    }
    else
    {
        led_greenOff();
    }
    
    fprintf(lcdout, "Motor_freq_rpm recent\n%d\n",(motorFrequency_getRecent())*60);
    fprintf(lcdout, "Motor_freq_rpm median\n%d\n",(motorFrequency_getMedian())*60);

    fprintf(uartout, "Motor freq rpm recent\n%d\n",(motorFrequency_getRecent())*60);
    fprintf(uartout, "Motor freq in rpm median\n%d\n",(motorFrequency_getMedian())*60);


}



int main(void)
{
    
    uart_init(57600);

    td1.period=1000;  
    td1.expire=td1.period;
    td1.param=NULL;  
    td1.task=Motor_freq_rpm;

    scheduler_add(&td1);

    
    pwm_init();
    led_greenInit();
    led_yellowInit();
    button_setJoystickButtonCallback(callback_for_joystick);
    button_init(1); 
    timer1_start();
    motorFrequency_init();

    scheduler_init();
    timer5_start();
    sei();
    scheduler_run();
    

}