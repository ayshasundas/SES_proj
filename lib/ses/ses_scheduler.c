/*INCLUDES ************************************************************/
#include "ses_timer.h"
#include "ses_scheduler.h"
#include "util/atomic.h"
#include "ses_uart.h"

/* PRIVATE VARIABLES **************************************************/
/** list of scheduled tasks */
static taskDescriptor* taskList = NULL;

/*FUNCTION DEFINITION *************************************************/
static void scheduler_update(void) {
	// TODO
	taskDescriptor *n=taskList;	
	while(n!=NULL)
	{	if(n->expire > 0)
		{
		n->expire = (n->expire) - 1;
		}
		if(n->expire==0)
		{
			n->execute=1;
			n->expire=n->period;
		}
		
		n=n->next;
		
	}
}

void scheduler_init() {
	// initializing timer2
timer2_start();
timer2_setCallback(scheduler_update);
//initializing scheduler

}

void scheduler_run() {
	// TODO
	
	while(1)
	{	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) 
		{
		taskDescriptor *n=taskList;	
		while(n!=NULL)
		{
			if(n->execute==1 && n->period>0)
			{
				n->task((void *) n->param);
				n->execute=0;
			}
			else if( n->execute==1 && n->period==0)
			{
				n->task((void *) n->param);
				n->execute=0;
				scheduler_remove(n);
			}
			
			n=n->next;
		}
		}
		

	}
}

bool scheduler_add(taskDescriptor * toAdd) {
	// TODO
int flag=1;
if(toAdd)
{	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
	taskDescriptor *n=taskList;	
	while (n != NULL && flag!=0) 
	{ 

	if(n==toAdd)
	{  
		 flag=0;
	}
	else
	{
        n = n->next; 
		flag=1;
    }

	}
	n=NULL;


	if(flag==1)
	{
		if(taskList==NULL)
		{
			taskList=toAdd;
			taskList->next=NULL;
			
		}
		else
		{
			taskDescriptor *n1=taskList;	
			while(n1->next != NULL)	
			{
				n1=n1->next;
			}
			n1->next=toAdd;
			toAdd->next=NULL;
			n1=NULL;
			
		}
		
	}
}
	return flag;
}
else
{
	return flag;
}

}

void scheduler_remove(taskDescriptor * toRemove) {
	// TODO
ATOMIC_BLOCK(ATOMIC_RESTORESTATE) 
{
	taskDescriptor *n=taskList;
	taskDescriptor *prev;
	while(n!=NULL){	
	fprintf(uartout,"task list before remove %x\n",n);
	n= n->next; 
	}
	n=taskList;
	while (n != NULL) 
	{ 
		if(taskList==toRemove)// if task to be removed lies at the head of the list
		{
		taskList=taskList->next;
		n->next=NULL;
		break;
		}

		else if(n==toRemove)
		{
			prev->next=n->next;
			n->next=NULL;
			break;
		}
		prev=n;
		n = n->next; 
	}
	n=taskList;
	while(n!=NULL){	
	fprintf(uartout,"task list after remove %x\n",n);
	n= n->next; 
	}
	n=NULL;
}	
}
