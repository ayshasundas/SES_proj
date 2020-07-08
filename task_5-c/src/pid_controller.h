#ifndef PID_CONTROLLER_H_
#define PID_CONTROLLER_H_

/*INCLUDES *******************************************************************/
#include "ses_common.h"

typedef struct ses_PIDcontroller
{
   int err;
   int Kp; 
   int Ki;
   int Kd;
   int Perr;
   int it;

}pidsettings;



void pid_controller_init(int kp, int ki, int kd,pidsettings *ptr);


uint8_t pid_controller(int F_tar,pidsettings * ptr);

#endif /* PIDCONTROLLER_H_ */