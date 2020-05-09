#include <avr/io.h>
#include <util/delay.h>
#include <ses_common.h>
#include <ses_led.h>

/**Toggles the red LED of the SES-board*/
int main(void) {
	
	DDRG |= 0x02;
	while (1) {
		_delay_ms(400);
		PORTG ^= 0x02;
	}
	return 0;
}
