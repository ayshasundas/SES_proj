
#include "pscheduler.h"
#include "ses_uart.h"

#define TIMER2_CYC_FOR_1MILLISEC   	249 //Top(value stored in OCRA) value to be compared with counter
void * CurrentTCB;
void *first;


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
					"lds	r26, CurrentTCB		   	\n\t"	\
					"lds	r27, CurrentTCB	 + 1	\n\t"	\
					"in		r0,  __SP_L__ 			\n\t"	\
					"st		x+, r0					\n\t"	\
					"in		r0,  __SP_H__ 			\n\t"	\
					"st		x+, r0					\n\t"	\
					);



#define portRESTORE_CONTEXT()								\
	asm volatile (	"lds	r26, CurrentTCB		    \n\t"	\
					"lds	r27, CurrentTCB + 1	    \n\t"	\
					"ld		r28, x+					\n\t"	\
					"out	__SP_L__, r28			\n\t"	\
					"ld		r29, x+					\n\t"	\
					"out	__SP_H__, r29			\n\t"	\
					"pop	r31						\n\t"	\
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

}

void vPortYieldFromTick( void )
{
	
 /* This is a naked function so the context
 is saved. */
 portSAVE_CONTEXT();

 /* See if a context switch is required.
 Switch to the context of a task made ready
 to run by vTaskIncrementTick() if it has a
 priority higher than the interrupted task. */
context_switch();
 /* Restore the context. If a context switch
 has occurred this will restore the context of
 the task being resumed. */
 portRESTORE_CONTEXT();
 /* Return from this naked function. */
 asm volatile ( "ret" );


}

/**This is a so-called "naked" interrupt and thus the compiler
 * creates no additional assembler code when entering/ leaving
 * the function. Note that a "reti" (return from interrupt)
 * becomes necessary.
 *
 */
ISR(TIMER2_COMPA_vect, ISR_NAKED) 
{ 	 
	vPortYieldFromTick();
	/* Return from the interrupt. 
	If a context switch has occurred this will return to a different task. */  
	asm volatile ("reti \n\t");

}



void pscheduler_run(task_t * TL, uint8_t numtasks)
{   
	SP=(&t1.stack[255]);
	CurrentTCB=&t1.pstack;
	timer2_start();
	TL[0]();


   

}