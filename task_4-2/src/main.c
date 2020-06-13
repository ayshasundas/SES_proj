#include <avr/io.h>
#include <util/delay.h>
#include "ses_common.h"
#include "ses_led.h"
#include "ses_button.h"
#include "ses_timer.h"
#include "ses_lcd.h"
#include "ses_scheduler.h"
#include "util/atomic.h"


#define Counter_value_rotary_for_1dsec        20 //for (1/10) of a second which is 1 deci second
#define first_time_button_pressed             1 // showing that any button is pressed first time
#define second_time_button_pressed            2 // showing that any button is pressed second time


static volatile int flag_rotary = 0;//shared variable in "callback_for_rotary" and "Stopwatch" task 
                                    //indicating number of times rotary button is pressed

static volatile int flag_joystick = 0;//shared variable in "callback_for_joystick" and "Yellow_led_Off" task
                                      //indicating number of times joystick button is pressed

static taskDescriptor td1,td2,td3,td4;

enum LED_COLORS{
LED_RED=1,
LED_GREEN=2,
LED_YELLOW=3
};

/*********************************** Non-Periodic Task for turning off yellow led after 5sec ********************************************/

void Yellow_led_Off(void * ptr)
{   //Task for turning off yellow led, if user has not pressed the button again within 5sec
    led_yellowOff();
    flag_joystick=0;
    
}

/*********************************** Callback for Joystick Button ********************************************/

void callback_for_joystick(void)
{   //callback for joystick button
    //if button is pressed first time, it turns on the yellow led and add "Yellow_led_Off" task to the schedular
    //if button is pressed again within 5sec, it turns off the yellow led and remove the above mentioned task from the scheduler
    led_yellowToggle(); 
    flag_joystick++;
    if(flag_joystick==first_time_button_pressed )
        {
            td4.period=0;  
            td4.expire=5000;
            td4.param=NULL;  
            td4.task=Yellow_led_Off;
            scheduler_add(&td4);

        }
    else if(flag_joystick==second_time_button_pressed  && td4.expire>0)
        {
            scheduler_remove(&td4);
            flag_joystick=0;
        }    
   
    
}

/*********************************** Callback for Rotary Button  ********************************************/
void callback_for_rotary(void)
{   // A Flagging Funbction For indicating either rotary button is pressed for first time or second time,
    // in order to start or stop the stopwatch
     flag_rotary++; 
         
}

/*********************************** Stopwatch Task ********************************************/

void stopwatch(void * ptr)
{   //Function for stopwatch
    // Starts stopwatch when rotary button is pressed first time
    //Stops when rotary button is pressed second time
    static int counter_stopwatch=0;
    static int deci_sec=0;
    if(flag_rotary==first_time_button_pressed)
        {   
            counter_stopwatch++; 
            if(counter_stopwatch==Counter_value_rotary_for_1dsec )
                {   
                    lcd_clear();
                    lcd_init();
                    deci_sec++;
                    fprintf(lcdout,"Current time: %d.%d s",deci_sec/10,deci_sec%10);
                    counter_stopwatch=0;
                 }
        }
    else if(flag_rotary==second_time_button_pressed )
        {
            lcd_clear();
            lcd_init();
            fprintf(lcdout,"Stop watch stopped\n");
            fprintf(lcdout,"Time: %d.%d s",deci_sec/10,deci_sec%10);
            flag_rotary=0;
            deci_sec=0;
        }
}

/*********************************** For LED Toggle Task ********************************************/

void Led_Toggle(void * ptr)
{   //Toggles Led of any chosen color after every 2sec
   int * ptr1= (int *)ptr;
   if(*ptr1 == LED_GREEN) 
    {
        led_greenToggle();
    }
    else if(*ptr1 == LED_RED) 
    {
        led_redToggle();
    }
    else if(*ptr1 == LED_YELLOW) 
    {
        led_yellowToggle();
    }

}

/*********************************** For button debounce Task ********************************************/


void button_debouncing(void * ptr)
{   //Task for debouncing the buttons and calling their callbacks when a particular button is pressed, respectively
   
     button_checkState();
}




/*********************************** Main Function ********************************************/


int main(void) 
{
    button_setJoystickButtonCallback(callback_for_joystick);
    button_setRotaryButtonCallback(callback_for_rotary);
    lcd_init();
    led_greenInit();
    led_redInit();
    led_yellowInit(); 
    button_init(1);   
    int led1=LED_GREEN;

    td1.period=2000;  
    td1.expire=td1.period;
    td1.param=(void *)&led1;  
    td1.task=Led_Toggle;

    td2.period=5;  
    td2.expire=td2.period;
    td2.param=NULL;  
    td2.task=button_debouncing;

    td3.period=5;  
    td3.expire=td3.period;
    td3.param=NULL;  
    td3.task=stopwatch;

    scheduler_add(&td1);
    scheduler_add(&td2);
    scheduler_add(&td3);

    scheduler_init();
    sei();
    scheduler_run();

}


