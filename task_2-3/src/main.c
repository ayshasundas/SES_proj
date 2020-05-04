#include <avr/io.h>
#include <util/delay.h>
#include "ses_lcd.h"
#include "ses_uart.h"

/**Toggles the red LED of the SES-board*/
int main(void) {
uart_init(57600);
lcd_init();
	while (1) {
fprintf(uartout, "START\n");
_delay_ms(3000);
fprintf(lcdout, "START");
_delay_ms(3000);
	}
	return 0;
}
