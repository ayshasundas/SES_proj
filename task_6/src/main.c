#include "ses_scheduler.h"
#include "ses_common.h"
#include "ses_timer.h"
#include "ses_led.h"
#include "ses_uart.h"
#include "ses_lcd.h"
#include "ses_button.h"
#include "Fsm.h"
#include <inttypes.h>
#include <stdint.h>
// Definitions
#define Hour_Conversion     3600000
#define Min_Conversion      60000
#define Sec_Conversion      1000
#define For_Overflow_min    60
#define For_Overflow_hr     24
#define For_5sec            20

#define TRANSITION(newState) (fsm->state = newState, RET_TRANSITION)

int setting_alarm = 0;
static taskDescriptor td1, td2, td3, td4, td5, td6, td7;
time_t Alarm_time, Sys_time;
Fsm clock;

fsmReturnStatus set_hours(Fsm *fsm, const Event *event);
fsmReturnStatus normal_mode(Fsm *fsm, const Event *event);

/********************************************* Milli_to_Time Func *******************************************/
void Milli_to_Time(systemTime_t cT, time_t *t)
{
    //Takes time value in milli seconds and stores the converted value in a structure time_t
    uint32_t temp = ((uint32_t)cT / (uint32_t)Hour_Conversion);

    t->hour = (uint8_t)temp;

    temp = ((cT - (t->hour * Hour_Conversion)) / Min_Conversion);
    t->minute = (uint8_t)temp;

    temp = ((cT - (t->hour * Hour_Conversion) - (t->minute * Min_Conversion)) / Sec_Conversion);
    t->second = (uint8_t)temp;
    temp = ((cT - (t->hour * Hour_Conversion) - (t->minute * Min_Conversion) - (t->second * Sec_Conversion)) / Min_Conversion);
    t->milli = (uint8_t)temp;
}

/********************************************* Time_to_Milli Func *******************************************/
systemTime_t Time_to_Milli(time_t t)
{   
    //takes time value in standard format and returns it in milliseconds
    return ((t.hour * Hour_Conversion) + (t.minute * Min_Conversion));
}

/********************************************* Alarm_beep State *******************************************/
fsmReturnStatus Alarm_beep(Fsm *fsm, const Event *event)
{
    switch (event->signal)
    {
    case ENTRY:
        scheduler_add(&td6);//Red_Led_Toggle task
        return RET_HANDLED;
    case JOYSTICK_PRESSED:
    case ROTARY_PRESSED:
    case TIME_5SEC_EXPIRED:
        return TRANSITION(normal_mode);
    case EXIT:
        scheduler_remove(&td6);//Red_Led_Toggle task
        led_redOff();
        fsm->isAlarmEnabled = 0;
        led_yellowOff();
        return RET_HANDLED;
    default:
        return RET_IGNORED;
    }
}

/********************************************* normal_mode State *******************************************/
fsmReturnStatus normal_mode(Fsm *fsm, const Event *event)
{
    switch (event->signal)
    {

    case ENTRY:
        scheduler_add(&td4);//curr_time_display task
        return RET_HANDLED;
    case ROTARY_PRESSED:
        fsm->isAlarmEnabled = !(fsm->isAlarmEnabled);
        if (fsm->isAlarmEnabled)
        {
            led_yellowOn();
            scheduler_add(&td5);// Alarm_ON task
        }
        else
        {
            led_yellowOff();
            scheduler_remove(&td5);// Alarm_ON task
        }

        return RET_HANDLED;
    case JOYSTICK_PRESSED:
        scheduler_remove(&td4);//curr_time_display task
        if (fsm->isAlarmEnabled)
        {
            scheduler_remove(&td5);// Alarm_ON task
        }
        return TRANSITION(set_hours);
    case ALARM_TIME_MATCHED:
        scheduler_remove(&td5);// Alarm_ON task
        return TRANSITION(Alarm_beep);
    case EXIT:
        if (!setting_alarm)
        {
            setting_alarm = 1;
        }
        else
        {
            setting_alarm = 0;
        }

        return RET_HANDLED;
    default:
        return RET_IGNORED;
    }
}

/********************************************* set_minutes State *******************************************/
fsmReturnStatus set_minutes(Fsm *fsm, const Event *event)
{
    static uint8_t mm = 0;
    switch (event->signal)
    {
    case ENTRY:
        lcd_init();
        lcd_clear();
        if (!setting_alarm)
        {
            fprintf(lcdout, "Setting Clock Min:\n");
        }
        else
        {
            fprintf(lcdout, "Setting Alarm Min:\n");
        }
        return RET_HANDLED;
    case ROTARY_PRESSED:
        mm = (mm + 1) % For_Overflow_min;
        lcd_init();
        lcd_clear();
        fprintf(lcdout, "%02d:%02d\n", Sys_time.hour, mm);
        return RET_HANDLED;
    case JOYSTICK_PRESSED:
        return TRANSITION(normal_mode);
    case EXIT:

        if (!setting_alarm)
        {
            Sys_time.minute = mm;
            scheduler_setTime(Time_to_Milli(Sys_time)); //Updating System time
            scheduler_add(&td7);//green_led_toggle task
        }
        else
        {

            Alarm_time.minute = mm;
            if (fsm->isAlarmEnabled)
            {
                scheduler_add(&td5);//Alarm_ON task
            }
        }

        mm = 0;
        return RET_HANDLED;
    default:
        return RET_IGNORED;
    }
}

/********************************************* set_hours State *******************************************/
fsmReturnStatus set_hours(Fsm *fsm, const Event *event)
{
    static uint8_t hh = 0;
    switch (event->signal)
    {
    case ENTRY:
        lcd_init();
        lcd_clear();

        if (!setting_alarm)
        {
            fprintf(lcdout, "Setting clock Hrs:\n");
        }
        else
        {
            fprintf(lcdout, "Setting Alarm Hrs:\n");
        }

        return RET_HANDLED;
    case ROTARY_PRESSED:
        hh = (hh + 1) % For_Overflow_hr;
        lcd_init();
        lcd_clear();
        fprintf(lcdout, "%02d:00\n", hh);
        return RET_HANDLED;
    case JOYSTICK_PRESSED:
        return TRANSITION(set_minutes);
    case EXIT:

        if (!setting_alarm)
        {
            Sys_time.hour = hh;
        }
        else
        {
            Alarm_time.hour = hh;
        }

        hh = 0;
        return RET_HANDLED;
    default:
        return RET_IGNORED;
    }
}

/********************************************* Clock_init State *******************************************/
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

/********************************************* Joystick Pressed Dispatch Task *******************************************/
static void joystickPressedDispatch(void *param)
{
    Fsm *fsm = (Fsm *)param;
    Event e = {.signal = JOYSTICK_PRESSED};
    fsm_dispatch(fsm, &e);
}

/********************************************* Rotary Pressed Dispatch Task *******************************************/
static void rotaryPressedDispatch(void *param)
{
    Fsm *fsm = (Fsm *)param;
    Event e = {.signal = ROTARY_PRESSED};
    fsm_dispatch(fsm, &e);
}

/********************************************* Button Check State Task *******************************************/
void button_debouncing(void *ptr)
{ //Task for debouncing the buttons and calling their callbacks when a particular button is pressed, respectively

    button_checkState();
}

void callback_for_joystick()
{
    scheduler_add(&td3); // Add joystickPressedDispatch task
}

void callback_for_rotary()
{
    scheduler_add(&td2); //Add rotaryPressedDispatch task
}

/********************************************* Current time display Task *******************************************/
void curr_time_display(void *param)
{
    lcd_clear();
    lcd_setCursor(0, 0);
    Fsm *fsm = (Fsm *)param;
    Milli_to_Time(scheduler_getTime(), &fsm->timeSet); //get the current time in millisec and convert it to user friendly format

    fprintf(lcdout, "%02d:%02d:%02d\n", fsm->timeSet.hour, fsm->timeSet.minute, fsm->timeSet.second);
}

/********************************************* Alarm On Task *******************************************/
void Alarm_ON(void *param)
{ //It compares currenttime with pre-set alarm time every second and calls dispatch func on a match
    Fsm *fsm = (Fsm *)param;
    Milli_to_Time(scheduler_getTime(), &fsm->timeSet);
    if (Alarm_time.hour == fsm->timeSet.hour && Alarm_time.minute == fsm->timeSet.minute && fsm->timeSet.second == 0)
    {
        Event e = {.signal = ALARM_TIME_MATCHED};
        fsm_dispatch(fsm, &e);
    }
}

/********************************************* Red_Led_Toggle Task *******************************************/
void Red_Led_Toggle(void *ptr)
{
    //Toggles red led for 5sec if user has not pressed any button within that time.
    Fsm *fsm = (Fsm *)ptr;
    static int time_5sec = 0;
    led_redToggle();
    time_5sec++;

    if (time_5sec == For_5sec)
    {
        time_5sec = 0;
        Event e = {.signal = TIME_5SEC_EXPIRED};
        fsm_dispatch(fsm, &e);
    }
}

/********************************************* Green_Led_Toggle Task *******************************************/
void Green_Led_Toggle(void *ptr)
{
    led_greenToggle();//toggles synchronously with the counter of the seconds
}

/********************************************* Main *******************************************/
int main()
{
    uart_init(57600);
    //by default the alarm value is 00:00
    Alarm_time.hour = 0;
    Alarm_time.minute = 0;
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
    td2.param = &clock;
    td2.task = rotaryPressedDispatch;

    td3.period = 0;
    td3.expire = 0;
    td3.execute = 1;
    td3.param = &clock;
    td3.task = joystickPressedDispatch;

    td4.period = 1000;
    td4.expire = 0;
    td4.execute = 1;
    td4.param = &clock;
    td4.task = curr_time_display;

    td5.period = 1000;
    td5.expire = td5.period;
    td5.param = &clock;
    td5.task = Alarm_ON;

    td6.period = 250;
    td6.expire = 0;
    td6.execute = 1;
    td6.param = &clock;
    td6.task = Red_Led_Toggle;

    td7.period = 1000;
    td7.expire = td7.period;
    td7.param = NULL;
    td7.task = Green_Led_Toggle;

    scheduler_add(&td1);

    scheduler_init();

    sei();
    fsm_init(&clock, clock_init);
    scheduler_run();
}