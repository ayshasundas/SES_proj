#include "ses_rotary.h"
#include "ses_common.h"
#include "ses_uart.h"
#include "ses_lcd.h"
#include "ses_timer.h"


int main ()
{
    uart_init(57600);
    lcd_init();
    rotary_init(1);
    timer1_start();
    sei();
    while(1)
    {
    }

}