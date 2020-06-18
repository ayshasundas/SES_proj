#ifndef PSCHEDULER_H_
#define PSCHEDULER_H_

#include "ses_common.h"

#define TASK_STACK_SIZE	256
typedef void (*task_t)(void);



/**The state of a task must be stored in
 * some struct
 */
typedef struct context_t {
	uint8_t stack[TASK_STACK_SIZE];
	void *pstack;
	struct context_t *next;
}context;

/**
 * Starts hardware timer 2 of MCU with a period
 * of 1 ms.
 */
void timer2_start();


void pscheduler_run(task_t *TL, uint8_t numtasks);

context t1,t2,t3;
#endif /* PSCHEDULER_H_ */