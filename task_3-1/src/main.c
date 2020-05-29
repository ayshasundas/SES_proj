#include <avr/io.h>
#include <util/delay.h>
#include "ses_button.h"
#include "ses_led.h"
#include "ses_common.h"
#include "ses_uart.h"
//main function
int main(void) {
uart_init(57600);
button_setJoystickButtonCallback(led_redToggle);
button_setRotaryButtonCallback(led_greenToggle);
led_redInit();
led_greenInit();
button_init(0);

sei();// enabling global interrupts


while(1)
{
//doing nothing just wasting CPU cycles :D
}

}
