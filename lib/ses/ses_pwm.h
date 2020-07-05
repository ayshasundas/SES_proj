#ifndef PWM_H_
#define PWM_H_

/*INCLUDES *******************************************************************/
#include "ses_common.h"


/**
 * Initializes timer0 to fast pwm mode by setting WGM02-00 to 3
 * using processor clock
 */
void pwm_init();


/**
 * Sets the duty cycle for pulse received at pin OC0B
 * @param dutyCycle     8-bit unsigned value to be fed to OCR0B Register for setting the duty cycle
 */
void pwm_setDutyCycle(uint8_t dutyCycle);






















#endif /* PWM_H_ */