#include <avr/io.h>
#include <util/delay.h>
#include "ses_common.h"
#include "ses_led.h"
#include "ses_button.h"
#include "ses_timer.h"
#include "ses_lcd.h"
#include "ses_uart.h"
#include "ses_scheduler.h"

#define LED_YELLOW_PORT 	PORTF //PF7
#define LED_YELLOW_PIN      	7
#define Counter_value_joystick_for_5sec        1000
#define Counter_value_rotary_for_1dsec        20 //for (1/10) of a second which is 1 deci second
#define first_time_rotary_pressed     1 // showing that rotary button is pressed first time
#define second_time_rotary_pressed     2 // showing that rotary button is pressed second time

static volatile int flag = 0;
static taskDescriptor td1,td2;

enum LED_COLORS{
LED_RED=1,
LED_GREEN=2,
LED_YELLOW=3
};

void flagging_func(void)
{   // For indicating either rotary button is pressed for first time or second time, in order to start or stop the stopwatch
     flag++; 
         
}

void stopwatch(void)
{   //Function for stopwatch
    // Starts stopwatch when rotary button is pressed first time
    //Stops when rotary button is pressed second time
    static int counter_r=0;
    static int deci_sec=0;
    if(flag==first_time_rotary_pressed)
    {   

       counter_r++; 
       if(counter_r==Counter_value_rotary_for_1dsec )
       {    lcd_clear();
            lcd_init();
           deci_sec++;
           fprintf(lcdout,"Current time: %d.%d s",deci_sec/10,deci_sec%10);
           counter_r=0;
       }
    }
    else if(flag==second_time_rotary_pressed )
    {
        lcd_clear();
        lcd_init();
        fprintf(lcdout,"Stop watch stopped");
        flag=0;
        deci_sec=0;
    }
}

void task_1(void * ptr)
{
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

void task_2(void * ptr)
{
    int * ptr1= (int *)ptr;
    static int counter_j=0;
   

     button_setJoystickButtonCallback(led_yellowToggle);
     button_setRotaryButtonCallback(flagging_func);
     button_checkState();
    /*********************************************************************/
    // For Joystick button
     if( !(LED_YELLOW_PORT & (1 << LED_YELLOW_PIN) ) ) //indicating yellow led is on
        {
            
            if(counter_j==Counter_value_joystick_for_5sec )
                {
                    led_yellowOff();
                    counter_j=0;
                }
            counter_j++;
            fprintf(uartout, "counter %d\n ",counter_j);
        }
    else
        {
            counter_j=0;
        }
    /*********************************************************************/
    // For Rotary button
    stopwatch();
    
    

       

}


/**Toggles the red LED of the SES-board*/
int main(void) {
uart_init(57600);
lcd_init();
led_greenInit();
led_redInit();
led_yellowInit(); 
button_init(1);   
int led1=LED_GREEN;
int led2_j=LED_YELLOW;
//int led2_r=LED_YELLOW;

td1.period=2000;  
td1.expire=td1.period;
td1.param=(void *)&led1;  
td1.task=task_1;
td2.period=5;  
td2.expire=td2.period;
td2.param=&led2_j;  
td2.task=task_2;
taskDescriptor* node1= &td1;
scheduler_add(node1);
taskDescriptor* node2= &td2;
scheduler_add(node2);
scheduler_init();
sei();
scheduler_run();

}

