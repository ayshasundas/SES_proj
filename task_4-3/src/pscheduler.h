#ifndef PSCHEDULER_H_
#define PSCHEDULER_H_

#include "ses_common.h"

#define TASK_STACK_SIZE	256

typedef void (*task_t)(void);// Function Pointer



/**The state of a task must be stored in
 * some struct
 */
typedef struct context_t 
{
	uint8_t stack[TASK_STACK_SIZE];
	void *pstack;
	struct context_t *next;
}context;

context t1,t2,t3;//for task A, B and C

/**
 * Starts hardware timer 2 of MCU with a period
 * of 1 ms.
 */
void timer2_start();

/**
* Giving SP the address of highest memory location of taskA's stack 
* Starting timer 2
* Enabling global interrupts 
* Running task A initially
*/
void pscheduler_run(task_t *TL, uint8_t numtasks);


#endif /* PSCHEDULER_H_ */