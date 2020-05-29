/* INCLUDES ******************************************************************/
#include "ses_timer.h"

/* DEFINES & MACROS **********************************************************/
#define TIMER1_CYC_FOR_5MILLISEC    77 //Top(value stored in OCRA) value to be compared to with counter
#define TIMER2_CYC_FOR_1MILLISEC   249	//Top(value stored in OCRA) value to be compared to with counter
static volatile pTimerCallback t2;
static volatile pTimerCallback t1;

/*FUNCTION DEFINITION ********************************************************/
void timer2_setCallback(pTimerCallback cb) {
	// setting callback if its pointer is valid
if(cb!=NULL){
	t2=cb;
}


}

void timer2_start() {
	// setting CTC mode of operation, enabling the compare match A interrupt, setting prescalar to 64
	// And setting Top(value stored in OCRA) value to be compared to with counter to 249
	TCCR2A|= (1<<WGM21);
	TCCR2A &= (~(1<<WGM20));
	TCCR2B |= (1<<CS22);
	TCCR2B &= (~((1<<WGM22) | (1<<CS21) | (1<<CS20)));
    TIMSK2 |= (1<<OCIE2A);
	OCR2A=TIMER2_CYC_FOR_1MILLISEC;
}


void timer2_stop() {
    // stoping the timer by selecting no clock source
	TCCR2B &= (~((1<<CS22) | (1<<CS21) | (1<<CS20)));
}

void timer1_setCallback(pTimerCallback cb) {
		// setting callback if its pointer is valid
if(cb!=NULL){
	t1=cb;
}
}


void timer1_start() {
	// setting CTC mode of operation, enabling the compare match A interrupt, setting prescalar to 64
// And setting Top(value stored in OCRA) value to be compared to with counter to 77
	TCCR1B|= ((1<<WGM12) | (1<<CS12) | (1<<CS10));
	TCCR1A &= (~((1<<WGM10) | (1<<WGM11)));
	TCCR1B &= (~((1<<WGM13) | (1<<CS11)));
    TIMSK1 |= (1<<OCIE1A);
	OCR1A=TIMER1_CYC_FOR_5MILLISEC;
}


void timer1_stop() {
// stoping the timer by selecting no clock source
TCCR1B &= (~((1<<CS12) | (1<<CS11) | (1<<CS10)));
}

ISR(TIMER1_COMPA_vect) {
// clearing interrupt flag for compare A
//calling callback function for timer1
TIFR1 |= (1<<OCF1A);
t1();	
}

ISR(TIMER2_COMPA_vect) {
// clearing interrupt flag for compare A
//calling callback function for timer2
TIFR2 |= (1<<OCF2A);
t2();
}
