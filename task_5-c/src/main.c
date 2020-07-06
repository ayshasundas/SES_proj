#include "ses_common.h"
#include "ses_button.h"
#include "ses_pwm.h"
#include "ses_motorFrequency.h"
#include "ses_timer.h"
#include "ses_led.h"
#include "ses_uart.h"
#include "ses_lcd.h"
#include "ses_scheduler.h"

uint8_t err = 0, Kp = 0, Ki = 0, Kd = 0, u = 0, Perr = 0;
uint16_t F_tar = 9000, F_curr = 0, P = 0, D = 0, it = 0;

void pid_controller_init(uint8_t kp, uint8_t ki, uint8_t kd)
{
    Kp = kp;
    Ki = ki;
    Kd = kd;
}

uint8_t pid_controller()
{   
    F_curr=motorFrequency_getRecent()*60;
    err = F_tar - F_curr;
    P = Kp * err;
    it = (it * 10) + (err * Ki * 1); //unit is 1/10
    D = Kd * (Perr - err) * 10;
    Perr = err;
    u = (P * 10 + it + D * 10) / 10;
    if (u < 0)
    {
        u = 0;
    }
    else if (u > 255)
    {
        u = 255;
    }
    return u;
}

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
    pid_controller_init(1, 0, 0);
    sei();
    while (1)
    {
        fprintf(uartout, "Motor freq rpm recent\n%d\n", (motorFrequency_getRecent()) * 60);
        pwm_setDutyCycle(pid_controller());
        _delay_ms(100);
    }
}