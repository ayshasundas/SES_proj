/* INCLUDES ******************************************************************/
#include "ses_motorFrequency.h"
#include <avr/io.h>

#define INT0_PORT       	       PORTD //PG5
#define INT0_PIN                   0



void motorFrequency_init()
{
    DDR_REGISTER(INT0_PORT) |= (1 << INT0_PIN);
    EIMSK |= (0<< INT0);
    EICRA |= (1<< ISC01 | 1<< ISC00);
    EIMSK |= (1<< INT0);
}


ISR(INT0)
{
    
    EIFR |= (1 << INTF0);
} 