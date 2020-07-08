/* INCLUDES ******************************************************************/
#include "ses_pwm.h"
#include <avr/io.h>

/************************ Definitions *************/

#define OC0B_PORT       	       PORTG //PG5
#define OC0B_PIN                   5



void pwm_init(void)
{   
    //Enabling timer0
    //Fast Pwm mode, setting WGM02-00 to 3 (top value defined as 0xFF)
    //inverted mode
    //disabled pre-scalar
    //Receiving pulse at pin OC0B

    DDR_REGISTER(OC0B_PORT) |= (1 << OC0B_PIN);
    PRR0 &= (~(1<<PRTIM0));
    TCCR0A |= ((1<<WGM00) | (1<<WGM01) | (1<<COM0B1) | (1<<COM0B0));//Setting inverting mode of Fast PWM mode
    //TCCR0B = (( (TCCR0B) & (~((1<<CS02) | (1<<CS01))) ) | ((1<<WGM02) | (1<<CS00)) );
    TCCR0B &= (~( (1<<CS02) | (1<<CS01) | (1<<WGM02) ));
    TCCR0B |= (1<<CS00);
    OCR0B=255;


}

void pwm_setDutyCycle(uint8_t dutyCycle)
{
    //Setting the duty cycle for pulse received at pin OC0B
    
    OCR0B=dutyCycle;
}