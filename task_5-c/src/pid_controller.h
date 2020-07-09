#ifndef PID_CONTROLLER_H_
#define PID_CONTROLLER_H_

/*INCLUDES *******************************************************************/
#include "ses_common.h"

//Structure for defining parameters of PID controller

typedef struct ses_PIDcontroller 
{
   int err; //Error
   int Kp; //Proportional-gain
   int Ki;  //Integral-gain
   int Kd;  //Derivative-gain
   int Perr; //Previous gain
   int it;  //Integral Total

}pidsettings;

/**
 * Sets the values of PID gains initially 
 *
 * @param kp Value to be set for proportional gain   
 * 
 * @param ki Value to be set for integral gain
 * 
 * @param kd Value to be set for derivative gain
 * 
 * @param ptr Pointer to PIDController Data Structure
**/
void pid_controller_init(int kp, int ki, int kd,pidsettings *ptr);



/**
 * It maintains the target frequency of the motor by using PID values 
 * 
 * @param F_tar Traget freuency in rpm to be reached   
 * 
 * @param  ptr Pointer to PIDController Data Structure 
 * 
 * @return  returns 8-bit value to be set as duty cycle 
**/

uint8_t pid_controller(int F_tar,pidsettings * ptr);

#endif /* PIDCONTROLLER_H_ */