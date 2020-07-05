#ifndef MOTORFREQUENCY_H_
#define MOTORFREQUENCY_H_

/*INCLUDES *******************************************************************/
#include "ses_common.h"

/**
 * Initializes external interrupt INT0
 */
void motorFrequency_init();


/**
 * @return  the most recent frequency measurement in Hertz
 */
uint16_t motorFrequency_getRecent();


/**
 * @return   the median of the last N interval measurements
 */
uint16_t motorFrequency_getMedian();

/**
 * Starts hardware timer 5 of MCU with a period
 * of 10 ms.
 */
void timer5_start();
















#endif /* MOTORFREQUENCY_H_ */