/*INCLUDES ************************************************************/
#include "ses_timer.h"
#include "ses_scheduler.h"
#include "util/atomic.h"


/* PRIVATE VARIABLES **************************************************/
/** list of scheduled tasks */
static taskDescriptor* taskList = NULL;

/*FUNCTION DEFINITION *************************************************/
static void scheduler_update(void) {
	// TODO
}

void scheduler_init() {
	// TODO
}

void scheduler_run() {
	// TODO
}

bool scheduler_add(taskDescriptor * toAdd) {
	// TODO
}

void scheduler_remove(taskDescriptor * toRemove) {
	// TODO
}
