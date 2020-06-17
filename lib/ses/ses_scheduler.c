/*INCLUDES ************************************************************/
#include "ses_timer.h"
#include "ses_scheduler.h"
#include "util/atomic.h"
#include "ses_uart.h"

/* PRIVATE VARIABLES **************************************************/
/** list of scheduled tasks */
static taskDescriptor* taskList = NULL;
taskDescriptor *task_run;

/*FUNCTION DEFINITION *************************************************/
static void scheduler_update(void) 
{
	taskDescriptor *n=taskList;	//temporary pointer for traversing the task list
	while(n!=NULL)
	{	if(n->expire > 0)
		{
		n->expire = (n->expire) - 1;
		}
		if(!(n->expire))
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

void scheduler_run() 
{
	while(1)
	{	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) 
		{
			taskDescriptor *n=taskList;//temporary pointer for traversing the task list
			while(n!=NULL)
			{
				if(n->execute)
				{
					task_run=n;
					break;
				}
				
				n=n->next;
			}
		}
		if( (task_run->execute) && (task_run->period>0) )
		{
			task_run->task((void *) task_run->param);
			task_run->execute=0;
		}
		else if( (task_run->execute) && (!(task_run->period)))
		{
			task_run->task((void *) task_run->param);
			task_run->execute=0;
			scheduler_remove(task_run);
		}
		
	}
}

bool scheduler_add(taskDescriptor * toAdd) 
{	
	int flag=1;
	if(toAdd)
	{	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		taskDescriptor *n=taskList;//temporary pointer for traversing the task list	
		while (n!=NULL) 
		{ 

			if(n==toAdd)
			{  
				flag=0;
				break;
			}
			else
			{
				n = n->next; 
				flag=1;
			}

		}
		n=NULL;


		if(flag)
		{
			if(!taskList)
			{
				taskList=toAdd;
				taskList->next=NULL;
				
			}
			else
			{
				taskDescriptor *n1=taskList;//temporary pointer for traversing the task list	
				while(n1->next!=NULL)	
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

void scheduler_remove(taskDescriptor * toRemove) 
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) 
	{
		taskDescriptor *n=taskList;//temporary pointer for traversing the task list
		taskDescriptor *prev;//for holding the address of previous node 
		while (n!=NULL) 
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
		n=NULL;
	}	
}
