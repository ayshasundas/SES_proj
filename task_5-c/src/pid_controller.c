/* INCLUDES ******************************************************************/
#include <avr/io.h>
#include "ses_motorFrequency.h"
#include "pid_controller.h"

#define Value_for_rpm        60
#define For_One_revoltion    6
#define For_millis           1000

void pid_controller_init(int kp, int ki, int kd, pidsettings *ptr)
{
    ptr->Kp = kp;
    ptr->Ki = ki;
    ptr->Kd = kd;
    ptr->err = 0;
    ptr->Perr = 0;
    ptr->it = 0;
}

uint8_t pid_controller(int F_tar, pidsettings *ptr)
{
    int u = 0; 
    int F_curr = ((motorFrequency_getMedian() * Value_for_rpm) / For_One_revoltion);
    ptr->err = F_tar - F_curr;
    int P = (ptr->Kp * ptr->err) / For_millis; // values of kp & ki are in millis
    ptr->it = ((ptr->Ki * 1 * ptr->err) / For_millis) + (ptr->it);
    int D = ptr->Kd * (ptr->Perr - ptr->err);
    u = ((P) + ptr->it + (D)); //Value of Duty cycle
    ptr->Perr = ptr->err;
    
    // For Anti-Windup
    
    if (u < 0)  
    {
        u = 0;
    }
    else if (u > 255)
    {
        u = 255;
    }
    return (uint8_t)u;
}