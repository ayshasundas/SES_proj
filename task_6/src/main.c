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


taskDescriptor td1, td2, td3;
Fsm clock;

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

fsmReturnStatus normal_mode(Fsm *fsm, const Event *event)
{
    switch (event->signal)
    {
    case ENTRY:
        Milli_to_Time(scheduler_getTime(), fsm->timeSet);
        lcd_clear();
        fprintf(lcdout, "%d:%d:%d\n", fsm->timeSet.hour, fsm->timeSet.minute, fsm->timeSet.second);
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
        fprintf(lcdout, "%d:%d\n", fsm->timeSet.hour, mm);
        return RET_HANDLED;
    case JOYSTICK_PRESSED:
        return TRANSITION(normal_mode);
    case EXIT:
        fsm->timeSet.minute = mm;
        scheduler_setTime(Time_to_Milli(fsm->timeSet)); //Updating System time
        //starts the clock???????
        return RET_HANDLED;
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
        fprintf(lcdout, "%d:00\n", hh);
        return RET_HANDLED;
    case JOYSTICK_PRESSED:
        return TRANSITION(set_minutes);
    case EXIT:
        fsm->timeSet.hour = hh;
        return RET_HANDLED;
    default:
        return RET_IGNORED;
    }
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




void joystickPressedDispatch(void *param)
{
    Event e = {.signal = JOYSTICK_PRESSED};
    fsm_dispatch(&clock, &e);
}

void rotaryPressedDispatch(void *param)
{
    Event e = {.signal = ROTARY_PRESSED};
    fsm_dispatch(&clock, &e);
}

void button_debouncing(void *ptr)
{ //Task for debouncing the buttons and calling their callbacks when a particular button is pressed, respectively

    button_checkState();
}

void callback_for_joystick()
{
    scheduler_add(&td3);
}

void callback_for_rotary()
{
    scheduler_add(&td2);
}

int main(void)
{

    lcd_init();
    button_setJoystickButtonCallback(callback_for_joystick);
    button_setRotaryButtonCallback(callback_for_rotary);
    led_greenInit();
    led_redInit();
    led_yellowInit();
    button_init(1);

    td1.period = 5;
    td1.expire = td1.period;
    td1.param = NULL;
    td1.task = button_debouncing;

    td2.period = 0;
    td2.expire = 0;
    td2.execute = 1;
    td2.param = NULL;
    td2.task = rotaryPressedDispatch;

    td3.period = 0;
    td3.expire = 0;
    td3.execute = 1;
    td3.param = NULL;
    td3.task = joystickPressedDispatch;

    scheduler_add(&td1);

    scheduler_init();
    sei();
    fsm_init(&clock, clock_init);
    scheduler_run();
}