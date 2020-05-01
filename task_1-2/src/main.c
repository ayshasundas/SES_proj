#include <avr/io.h>
#include <util/delay.h>

void wait(uint16_t millis)
{
    uint16_t i;
    while(millis>0){
    for (i = 3500; i > 0 ; i--) 
    {
        //prevent code optimization by using inline assembler
        asm volatile ( "nop" ); // one cycle with no operation

    }
    millis--;
    }

}


/**Toggles the red LED of the SES-board**/
int main(void) {
	DDRG |= 0x02;
	while (1) {
		wait(3000);
		PORTG ^= 0x02;
	}
	return 0;
}
