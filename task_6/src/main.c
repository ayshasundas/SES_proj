#include "ses_scheduler.h"
#include "ses_common.h"
#include "ses_timer.h"
#include "ses_led.h"
#include "ses_uart.h"
#include "ses_lcd.h"
#include "ses_button.h"
#include "Fsm.h"

#define Hour_Conversion 3600000
#define Min_Conversion 60000
#define Sec_Conversion 1000

#define TRANSITION(newState) (fsm->state = newState, RET_TRANSITION)

time_t t1;

void Milli_to_Time(systemTime_t cT, time_t t)
{
    t.hour = (cT / Hour_Conversion);
    t.minute = ((cT - (t.hour * Hour_Conversion)) / Min_Conversion);
    t.second = ((cT - (t.hour * Hour_Conversion) - (t.minute * Min_Conversion)) / Sec_Conversion);
    t.milli = ((cT - (t.hour * Hour_Conversion) - (t.minute * Min_Conversion) - (t.second * Sec_Conversion)) / Min_Conversion);
}

systemTime_t Time_to_Milli(time_t t)
{
    return ((t.hour * Hour_Conversion) + (t.minute * Min_Conversion));
}

fsmReturnStatus clock_init(Fsm *fsm, const Event *event)
{
    switch (event->signal)
    {
    case ENTRY:
        fprintf(lcdout, "00:00\nSet Time:\n");
        return RET_HANDLED;
    case ROTARY_PRESSED:
        return TRANSITION(set_hours);
    default:
        return RET_IGNORED;
    }
}

fsmReturnStatus set_hours(Fsm *fsm, const Event *event)
{
    static uint8_t hh = 0;
    switch (event->signal)
    {
    case ENTRY:
        lcd_clear();
        fprintf(lcdout, "Setting Hrs:\n");
        return RET_HANDLED;
    case ROTARY_PRESSED:
        hh++;
        lcd_clear();
        fprintf(lcdout, "%d:00\n",hh);
        return RET_HANDLED;
    case JOYSTICK_PRESSED:
        return TRANSITION(set_minutes);
    case EXIT:
        fsm->timeSet.hour=hh;
        return RET_HANDLED;
    default:
        return RET_IGNORED;
    }
}

fsmReturnStatus set_minutes(Fsm *fsm, const Event *event)
{
    static uint8_t mm = 0;
    switch (event->signal)
    {
    case ENTRY:
        lcd_clear();
        fprintf(lcdout, "Setting Min:\n");
        return RET_HANDLED;
    case ROTARY_PRESSED:
        mm++;
        lcd_clear();
        fprintf(lcdout, "%d:%d\n",fsm->timeSet.hour,mm);
        return RET_HANDLED;
    case JOYSTICK_PRESSED:
        return TRANSITION(normal_mode);
    case EXIT:
        fsm->timeSet.minute=mm;
        scheduler_setTime(Time_to_Milli(fsm->timeSet));//Updating System time
        //starts the clock???????
        return RET_HANDLED;
    default:
        return RET_IGNORED;
    }
}

fsmReturnStatus normal_mode(Fsm *fsm, const Event *event)
{

}

int main()
{
    lcd_init();
}