/* INCLUDES ******************************************************************/
#include "ses_timer.h"

/* DEFINES & MACROS **********************************************************/
#define TIMER1_CYC_FOR_5MILLISEC    //TODO
#define TIMER2_CYC_FOR_1MILLISEC   249	//Top(value stored in OCRA) value to be compared to with counter


/*FUNCTION DEFINITION ********************************************************/
void timer2_setCallback(pTimerCallback cb) {
	// TODO


}

void timer2_start() {
	// TODO
	TCCR2A|=(1<<WGM21);
}


void timer2_stop() {
    // TODO
}

void timer1_setCallback(pTimerCallback cb) {
	// TODO
}


void timer1_start() {
	// TODO
}


void timer1_stop() {
	// TODO
}

ISR(TIMER1_COMPA_vect) {
	// TODO
}

ISR(TIMER2_COMPA_vect) {
	// TODO
}
