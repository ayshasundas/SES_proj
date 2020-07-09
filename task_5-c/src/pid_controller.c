/* INCLUDES ******************************************************************/
#include <avr/io.h>
#include "ses_motorFrequency.h"
#include "pid_controller.h"


void pid_controller_init(int kp, int ki, int kd, pidsettings *ptr)
{
    ptr->Kp = kp;
    ptr->Ki = ki;
    ptr->Kd = kd;
    ptr->err=0;
    ptr->Perr=0;
    ptr->it=0;
}

uint8_t pid_controller(int F_tar,pidsettings * ptr)
{   
    int u=0;
    int F_curr=motorFrequency_getMedian();
    ptr->err = F_tar - F_curr;
    int P = ptr->Kp * ptr->err;
    ptr->it = (ptr->Ki * 1 * ptr->err) + (ptr->it);
    int D = ptr->Kd * (ptr->Perr - ptr->err);
    /*if ((u < 255) && (u >= 0)) 
    {
       ptr->it = ptr->it + ptr->err;
    }*/
     u = ((P ) + ptr->it + (D));
     ptr->Perr = ptr->err;
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