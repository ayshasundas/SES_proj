#include "ses_scheduler.h"
#include "ses_common.h"
#include "ses_timer.h"
#include "ses_led.h"
#include "ses_uart.h"
#include "ses_lcd.h"
#include "ses_button.h"
#include "Fsm.h"



#define Hour_Conversion     3600000
#define Min_Conversion     60000
#define Sec_Conversion     1000

typedef struct time_t {
uint8_t hour;
uint8_t minute;
uint8_t second;
uint16_t milli;
} time_t;

time_t t1;

void Milli_to_Time(systemTime_t cT, time_t t)
{
    t.hour= (cT/Hour_Conversion);
    t.minute= ((cT-(t.hour*Hour_Conversion))/Min_Conversion);
    t.second= ((cT-(t.hour*Hour_Conversion)-(t.minute*Min_Conversion))/Sec_Conversion);
    t.milli= ((cT-(t.hour*Hour_Conversion)-(t.minute*Min_Conversion)-(t.second*Sec_Conversion))/Min_Conversion);

}

systemTime_t Time_to_Milli(time_t t)
{
   return ( (t.hour*Hour_Conversion) + (t.minute*Min_Conversion) );

}



int main()
{

    
}