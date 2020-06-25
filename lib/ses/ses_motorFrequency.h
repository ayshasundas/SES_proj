#ifndef MOTORFREQUENCY_H_
#define MOTORFREQUENCY_H_

/*INCLUDES *******************************************************************/
#include "ses_common.h"


void motorFrequency_init(void);
uint16_t motorFrequency_getRecent(void);
uint16_t motorFrequency_getMedian(void);
void timer5_start();
















#endif /* MOTORFREQUENCY_H_ */