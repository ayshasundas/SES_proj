#ifndef FSM_H_
#define FSM_H_

/*INCLUDES *******************************************************************/
#include "ses_common.h"


/* TYPES ********************************************************************/
typedef struct fsm_s Fsm;        //< typedef for alarm clock state machine
typedef struct event_s Event;    //< event type for alarm clock fsm
typedef uint8_t fsmReturnStatus; //< typedef to be used with above enum
/* a state is represented by a function pointer, called for
* each transition emanating in this state */
typedef fsmReturnStatus (*State)(Fsm *, const Event *);
/* base type for state machine */

/** return values */
enum
{
    RET_HANDLED,   //< event was handled
    RET_IGNORED,   //< event was ignored; not used in this implementation
    RET_TRANSITION //< event was handled and a state transition occurred
};

/** Signals */
enum
{
    ENTRY,
    EXIT,
    JOYSTICK_PRESSED,
    ROTARY_PRESSED,
    ALARM_TIME_MATCHED,
    TIME_5SEC_EXPIRED
};

typedef struct time_t
{
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint16_t milli;
} time_t;

struct fsm_s
{
    State state;         //< current state, pointer to event handler
    bool isAlarmEnabled; //< flag for the alarm status
    time_t timeSet;      //< multi-purpose var for system time and alarm time
};

/* base type for events*/
struct event_s
{
    uint8_t signal; //< identifies the type of event
};

/* dispatches events to state machine, called in application*/

inline static void fsm_dispatch(Fsm *fsm, const Event *event)
{

    static Event entryEvent = {.signal = ENTRY};
    static Event exitEvent = {.signal = EXIT};
    State s = fsm->state;
    fsmReturnStatus r = fsm->state(fsm, event);
    if (r == RET_TRANSITION)
    {
        s(fsm, &exitEvent);           //< call exit action of last state
        r=fsm->state(fsm, &entryEvent); //< call entry action of new state
    }
    
}

    /* sets and calls initial state of state machine */
    inline static void fsm_init(Fsm * fsm, State init)
    {
        //... other initialization
        Event entryEvent = {.signal = ENTRY};
        fsm->state = init;
        fsm->state(fsm, &entryEvent);
    }

#endif /* FSM_H_ */