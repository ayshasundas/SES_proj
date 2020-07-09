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

static volatile int flag_joystick = 0;
static taskDescriptor td1;
pidsettings pid;

#define first_time_button_pressed       1  // showing that any button is pressed first time
#define second_time_button_pressed      2 // showing that any button is pressed second time

void callback_for_joystick(void)
{

    flag_joystick++;
    if (flag_joystick == first_time_button_pressed)
    {
        scheduler_add(&td1); //Adding task_1 to the scheduler
    }
    else if (flag_joystick == second_time_button_pressed)
    {
        pwm_setDutyCycle(255);
        scheduler_remove(&td1);
        flag_joystick = 0;
    }
}

void pid_control(void *ptr)
{

    pwm_setDutyCycle(pid_controller(3000, &pid)); //target frequency should be in rpm
}

int main(void)
{
    //Initializing task_1 parameters
    td1.period = 1000; //1sec
    td1.expire = td1.period;
    td1.param = NULL;
    td1.task = pid_control;

    pwm_init();
    led_greenInit();
    led_yellowInit();
    button_setJoystickButtonCallback(callback_for_joystick);
    button_init(1);
    timer1_start(); // for button check state func
    motorFrequency_init();
    scheduler_init();
    pid_controller_init(10, 5, 0, &pid);
    timer5_start();
    sei();
    scheduler_run();
}