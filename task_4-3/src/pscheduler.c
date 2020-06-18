
#include "pscheduler.h"
#include "ses_uart.h"

#define TIMER2_CYC_FOR_1MILLISEC   	249 //Top(value stored in OCRA) value to be compared with counter
#define First_task_halted			1
#define Second_task_halted			2
#define Third_task_halted			3

volatile int flag=0;// For running tasks sequentially (TaskA,TaskB,TaskC)

void timer2_start() 
{
	// setting CTC mode of operation, enabling the compare match A interrupt, setting prescalar to 64
	// And setting Top(value stored in OCRA) value to 249, which would be compared with counter value 
	TCCR2A|= (1<<WGM21);
	TCCR2A &= (~(1<<WGM20));
	TCCR2B |= (1<<CS22);
	TCCR2B &= (~((1<<WGM22) | (1<<CS21) | (1<<CS20)));
    TIMSK2 |= (1<<OCIE2A);
	OCR2A=TIMER2_CYC_FOR_1MILLISEC;
}

#define portSAVE_CONTEXT()									\
	asm volatile (	"push	r0						\n\t"	\
					"in		r0, __SREG__			\n\t"	\
					"cli							\n\t"	\
					"push	r0						\n\t"	\
					"push	r1						\n\t"	\
					"clr	r1						\n\t"	\
					"push	r2						\n\t"	\
					"push	r3						\n\t"	\
					"push	r4						\n\t"	\
					"push	r5						\n\t"	\
					"push	r6						\n\t"	\
					"push	r7						\n\t"	\
					"push	r8						\n\t"	\
					"push	r9						\n\t"	\
					"push	r10						\n\t"	\
					"push	r11						\n\t"	\
					"push	r12						\n\t"	\
					"push	r13						\n\t"	\
					"push	r14						\n\t"	\
					"push	r15						\n\t"	\
					"push	r16						\n\t"	\
					"push	r17						\n\t"	\
					"push	r18						\n\t"	\
					"push	r19						\n\t"	\
					"push	r20						\n\t"	\
					"push	r21						\n\t"	\
					"push	r22						\n\t"	\
					"push	r23						\n\t"	\
					"push	r24						\n\t"	\
					"push	r25						\n\t"	\
					"push	r26						\n\t"	\
					"push	r27						\n\t"	\
					"push	r28						\n\t"	\
					"push	r29						\n\t"	\
					"push	r30						\n\t"	\
					"push	r31						\n\t"	\
					);




#define portRESTORE_CONTEXT()								\
	asm volatile (	"pop	r31						\n\t"	\
					"pop	r30						\n\t"	\
					"pop	r29						\n\t"	\
					"pop	r28						\n\t"	\
					"pop	r27						\n\t"	\
					"pop	r26						\n\t"	\
					"pop	r25						\n\t"	\
					"pop	r24						\n\t"	\
					"pop	r23						\n\t"	\
					"pop	r22						\n\t"	\
					"pop	r21						\n\t"	\
					"pop	r20						\n\t"	\
					"pop	r19						\n\t"	\
					"pop	r18						\n\t"	\
					"pop	r17						\n\t"	\
					"pop	r16						\n\t"	\
					"pop	r15						\n\t"	\
					"pop	r14						\n\t"	\
					"pop	r13						\n\t"	\
					"pop	r12						\n\t"	\
					"pop	r11						\n\t"	\
					"pop	r10						\n\t"	\
					"pop	r9						\n\t"	\
					"pop	r8						\n\t"	\
					"pop	r7						\n\t"	\
					"pop	r6						\n\t"	\
					"pop	r5						\n\t"	\
					"pop	r4						\n\t"	\
					"pop	r3						\n\t"	\
					"pop	r2						\n\t"	\
					"pop	r1						\n\t"	\
					"pop	r0						\n\t"	\
					"out	__SREG__, r0			\n\t"	\
					"pop	r0						\n\t"	\
				);




void context_switch(void)
{ 
//Switch to next task in sequence	
if(flag==First_task_halted)
{	 
	SP=(uint16_t)t2.pstack;
}
else if(flag==Second_task_halted)
{
	SP=(uint16_t)t3.pstack;
}
else if(flag==Third_task_halted)
{
	SP=(uint16_t)t1.pstack;
	flag=0;//Flag being reset
}
}


/**This is a so-called "naked" interrupt and thus the compiler
 * creates no additional assembler code when entering/ leaving
 * the function. Note that a "reti" (return from interrupt)
 * becomes necessary.
 *
 */
ISR(TIMER2_COMPA_vect, ISR_NAKED) 
{ 	 
	portSAVE_CONTEXT();//Saving the context of the task being halted
	flag++;
	//Storing the SP for task being halted, which will be used for restoring the context
	if(flag==First_task_halted)
	{
		t1.pstack=(void *)SP;
		
	}
	else if(flag==Second_task_halted)
	{
		t2.pstack=(void *)SP;
		
	}
	else if(flag==Third_task_halted)
	{
		t3.pstack=(void *)SP;
		
	}

	/* Switch to the context of next task */
	context_switch();
	/* Restore the context of
	the task being resumed */
	portRESTORE_CONTEXT();
	/* Return from the interrupt. 
	If a context switch has occurred this will return to a different task. */  
	asm volatile ("reti \n\t");

}



void pscheduler_run(task_t * TL, uint8_t numtasks)
{   
	SP=(uint16_t)(&t1.stack[255]);//Giving SP the address of highest memory location of taskA's stack
	timer2_start();//starting timer2 with 1ms period
	sei();//Enabling global interrupts
	TL[0]();//Executing task A

}