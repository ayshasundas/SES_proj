#ifndef FSM_H_
#define FSM_H_

/*INCLUDES *******************************************************************/
#include "ses_common.h"

/* TYPES ********************************************************************/
typedef struct Event Event;
typedef struct Fsm Fsm;

/* a state is represented by a function pointer, called for
* each transition emanating in this state */
typedef void (*State)(Fsm *, const Event *);
/* base type for state machine */

struct Fsm {
State state; /* current state */
};

/* base type for events*/
struct Event {
int signal;
};

/* dispatches events to state machine, called in application*/
inline static void fsm_dispatch(Fsm * fsm, const Event * event) {
fsm->state(fsm, event);
}

/* sets and calls initial state of state machine */
inline static void fsm_init(Fsm * fsm, State init) {
fsm->state = init;
fsm_dispatch(fsm, NULL);
}














#endif /* FSM_H_ */