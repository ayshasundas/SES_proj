#include "ses_common.h"
#include "ses_button.h"
#include "ses_pwm.h"
#include "ses_motorFrequency.h"
#include "pid_controller.h"
#include "ses_timer.h"
#include "ses_led.h"
#include "ses_uart.h"
#include "ses_lcd.h"
#include "ses_scheduler.h"






/*static volatile int flag_joystick = 0;
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
}*/

/*void Motor_freq_rpm(void * ptr)
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


}*/

int main(void)
{
    uart_init(57600);
    pwm_init();
    led_greenInit();
    led_yellowInit();
    lcd_init();
    motorFrequency_init();
    timer5_start();
    pidsettings pid;
    pid_controller_init(50, 100, 1, &pid);
    sei();
    while (1)
    {
        fprintf(uartout, "Motor freq rpm\n%d\n", ((motorFrequency_getMedian() * 60)/6));
        pwm_setDutyCycle(pid_controller(3000,&pid));//target frequency should be in rpm
        _delay_ms(10);
    }
}