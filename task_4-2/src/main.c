#include <avr/io.h>
#include <util/delay.h>
#include "ses_common.h"
#include "ses_led.h"
#include "ses_button.h"
#include "ses_timer.h"
#include "ses_uart.h"
#include "ses_scheduler.h"

enum LED_COLORS{
LED_RED=1,
LED_GREEN=2,
LED_YELLOW=3
};
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


/**Toggles the red LED of the SES-board*/
int main(void) {
uart_init(57600);
led_greenInit();
led_redInit();
led_yellowInit();    
int led=LED_GREEN;
taskDescriptor t1;
t1.period=2000;  
t1.expire=t1.period;
t1.param=&led;  
t1.task=task_1;
fprintf(uartout, "led %d\n param value %d\n",led,*(int *)t1.param);
taskDescriptor* node1= &t1;
scheduler_add(node1);
scheduler_init();
sei();
scheduler_run();

}

