#include <avr/io.h>
#include <util/delay.h>
#include "pscheduler.h"
#include "ses_led.h"

void stack_init(void)
{
  
uint16_t pc_addr=&t2.stack[255];
t2.stack[TASK_STACK_SIZE-1]=(pc_addr & 0x00FF);//lower address
t2.stack[TASK_STACK_SIZE-2]=(pc_addr>>8);//higher address


}



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
task_t taskList[] = {taskA, taskB};
uint8_t numTasks = 2;







int main(void) {
  memset(t1.stack,0,TASK_STACK_SIZE);
  memset(t2.stack,0,TASK_STACK_SIZE);
  stack_init();
  pscheduler_run(taskList, numTasks);
  return 0;
    
}