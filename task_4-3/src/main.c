#include "pscheduler.h"
#include "ses_led.h"




void taskA (void) 
{
led_redInit();
while (1) 
{ 
  led_redToggle();
  _delay_ms(2000);  
}
}



void taskB (void) 
{
led_yellowInit();  
while (1) 
{ 
   led_yellowToggle();
    _delay_ms(3000);
}   
}

void taskC (void) 
{
led_greenInit();
while (1) 
{
    led_greenToggle();
    _delay_ms(4000);
 }
 
}

task_t taskList[] = {taskA, taskB, taskC};
uint8_t numTasks = 3;






int main(void) {
    
    pscheduler_run(taskList, numTasks);
    return 0;
    
}