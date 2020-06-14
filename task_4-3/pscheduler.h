#ifndef PSCHEDULER_H_
#define PSCHEDULER_H_
#include "ses_common.h"


typedef void (*task_t)(void);

void pscheduler_run(task_t TL, uint8_t numtasks);

#endif /* PSCHEDULER_H_ */