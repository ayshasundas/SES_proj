#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "pscheduler.h"
#include "ses_led.h"


#define    tos                    36      //(32*Gen purpose registers)+SREG+(2*PC)+1= top of the stack
#define    for_lower_8bits        0x00FF  // For splitting 16-bit address to least significant 8 bits
#define    for_higher_8bits       8       // For splitting 16-bit address to most significant 8 bits

void taskA (void) 
{//Toggling red led after 2 sec
led_redInit();
while (1) 
{ 
  led_redToggle();
  _delay_ms(2000);  
}
}



void taskB (void) 
{//toggling yellow led after 3 sec
led_yellowInit();  
while (1) 
{ 
   led_yellowToggle();
    _delay_ms(3000);
}   
}

void taskC (void) 
{//toggling green led after 4 sec
led_greenInit();
while (1) 
{
    led_greenToggle();
    _delay_ms(4000);
 }
 
}

void stack_init(void)
{
  //for initialising stacks for task B and C
  //storing the addresses of tasks in their stacks for pre-filling the PC
  //saving the stack pointer(pointing to top of the stack) for respective tasks.
  task_t pc_add_B= (&taskB);
  uint16_t pc_addr_taskB= (uint16_t)pc_add_B;
  t2.stack[TASK_STACK_SIZE-1]=(pc_addr_taskB & for_lower_8bits);//lower 8 bits of taskB's address(16-bit) 
  t2.stack[TASK_STACK_SIZE-2]=(pc_addr_taskB>>for_higher_8bits );//higher 8 bits of taskB's address(16-bit) 
  t2.pstack=&(t2.stack[TASK_STACK_SIZE-tos]);

  task_t pc_add_C= (&taskC);
  uint16_t pc_addr_taskC= (uint16_t)pc_add_C;
  t3.stack[TASK_STACK_SIZE-1]=(pc_addr_taskC & for_lower_8bits);//lower 8 bits of taskC's address(16-bit) 
  t3.stack[TASK_STACK_SIZE-2]=(pc_addr_taskC>>for_higher_8bits );//higher 8 bits of taskC's address(16-bit) 
  t3.pstack=&(t3.stack[TASK_STACK_SIZE-tos]);

}

task_t taskList[] = {taskA, taskB, taskC};//Adding tasks to the scheduler
uint8_t numTasks = 3;







int main(void) {
  //initialising the memory for stacks with zero.
  memset(t1.stack,0,TASK_STACK_SIZE);
  memset(t2.stack,0,TASK_STACK_SIZE);
  memset(t3.stack,0,TASK_STACK_SIZE);
  //initialising the stacks for task B and C
  stack_init();
  
  pscheduler_run(taskList, numTasks);/*Giving SP the address of highest memory location of taskA's stack
                                      *Starting timer 2
                                      *Enabling global interrupts 
                                      *Running task A initially*/

  return 0;
    
}
