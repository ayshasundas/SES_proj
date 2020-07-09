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

#define first_time_button_pressed  1 // showing that any button is pressed first time
#define second_time_button_pressed 2 // showing that any button is pressed second time
#define Value_for_rpm              60
#define For_One_revoltion          6

/************************************* Callback for joystick Button *****************************/

void callback_for_joystick(void)
{

    flag_joystick++;
    if (flag_joystick == first_time_button_pressed)
    {
        pwm_setDutyCycle(90);
    }
    else if (flag_joystick == second_time_button_pressed)
    {
        pwm_setDutyCycle(255);
        flag_joystick = 0;
    }
}

/************************ Task for displaying motor freq in rpm ********************************/
void Motor_freq_rpm(void *ptr)
{
    //Displays the motor speed on lcd in rpm 

    lcd_init();//initializes lcd
    lcd_clear();

    if (motorFrequency_getRecent() == 0 || motorFrequency_getMedian() == 0)// if motor is stopped
    {
        led_greenOn();
    }
    else
    {
        led_greenOff();
    }

    fprintf(lcdout, "Speed_rpm_recent\n%d\n", ((motorFrequency_getRecent() * Value_for_rpm)/For_One_revoltion ));
    fprintf(lcdout, "Speed_rpm_median\n%d\n", ((motorFrequency_getMedian() * Value_for_rpm)/For_One_revoltion ));

    //For debugging purpose, due to low lcd screen visibility
    fprintf(uartout, "Motor freq rpm recent\n%d\n", ((motorFrequency_getRecent()* Value_for_rpm)/For_One_revoltion ));
    fprintf(uartout, "Motor freq in rpm median\n%d\n", ((motorFrequency_getMedian()* Value_for_rpm)/For_One_revoltion));
}

/****************************** Main function **************************************************/

int main(void)
{

    uart_init(57600);//initializing uart

    //Initializing task_1 parameters 
    td1.period = 1000;
    td1.expire = td1.period;
    td1.param = NULL;
    td1.task = Motor_freq_rpm;

    scheduler_add(&td1);//Adding task_1 to the scheduler

    pwm_init();
    led_greenInit();
    led_yellowInit();
    button_setJoystickButtonCallback(callback_for_joystick);
    button_init(1);
    timer1_start();// for button check state func
    motorFrequency_init();
    scheduler_init();
    timer5_start();
    sei();
    scheduler_run();
}