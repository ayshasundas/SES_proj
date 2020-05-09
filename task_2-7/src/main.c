#include "ses_common.h"
#include "ses_adc.h"
#include <avr/io.h>
#include <util/delay.h>
#include <ses_led.h>


#define TEMP_SENSOR_PORT       	PORTF //PF2
#define TEMP_SENSOR_PIN         	2

#define LIGHT_SENSOR_PORT       	PORTF //PF4
#define LIGHT_SENSOR_PIN         	4

#define JOYSTICK_PORT       	PORTF //PF5
#define JOYSTICK_PIN         	5

#define ADC_VREF_SRC        3

void adc_init(void){
DDR_REGISTER(TEMP_SENSOR_PORT) &= (~(1 << TEMP_SENSOR_PIN));
DDR_REGISTER(LIGHT_SENSOR_PORT) &= (~(1 << LIGHT_SENSOR_PIN));
DDR_REGISTER(JOYSTICK_PORT) &= (~(1 << JOYSTICK_PIN));
TEMP_SENSOR_PORT &= (~(1 << TEMP_SENSOR_PIN));
LIGHT_SENSOR_PORT &= (~(1 << LIGHT_SENSOR_PIN));
JOYSTICK_PORT &= (~(1 << JOYSTICK_PIN));
PRR0 &= (~(1));
ADMUX |= ADC_VREF_SRC<<6;
ADMUX &= (~(1<<5));

}