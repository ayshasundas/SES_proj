/* INCLUDES ******************************************************************/
#include "ses_motorFrequency.h"
#include "ses_led.h"
#include <avr/io.h>
#include "util/atomic.h"

#define INT0_PORT       	       PORTD //PD0
#define INT0_PIN                   0

#define TIMER5_CYC_FOR_10MILLISEC    	 2499 //Top(value stored in OCRA) value to be compared with counter
#define Counter_Value_For_10msec           2
#define N                                  90
static volatile uint16_t counter_spikes=0;
volatile uint16_t num_spikes_in_10msec=0;
uint16_t num_spikes_ini=0;
uint16_t num_spikes_inst=0;
volatile uint16_t array[N]={0};
uint16_t b1=0,b2=0;

void sort(int n, volatile uint16_t* ptr) 
{ 
    uint16_t t; 
  
    // Sort the numbers using pointers 
    for (int i = 0; i < n; i++) { 
  
        for (int j = i + 1; j < n; j++) { 
  
            if (*(ptr + j) < *(ptr + i)) { 
  
                t = *(ptr + i); 
                *(ptr + i) = *(ptr + j); 
                *(ptr + j) = t; 
            } 
        } 
    }

}    

void motorFrequency_init(void)
{
    DDR_REGISTER(INT0_PORT) &= (~(1 << INT0_PIN));
    EIMSK &= (~(1<< INT0));
    EICRA |= ((1<< ISC01) | (1<< ISC00));
    EIMSK |= (1<< INT0);
}

uint16_t motorFrequency_getMedian(void)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
    sort(N, array);
    int a=N/2;
    b1=array[a-1];
    b2=array[a];
    }
    uint16_t median_spike_10msec=(b1+b2)/2; 
    return (uint16_t)((median_spike_10msec*100)/6);
}

uint16_t motorFrequency_getRecent(void)
{
    return (uint16_t)((num_spikes_in_10msec*100)/6);//in Hertz
}

void timer5_start() 
{
	// setting CTC mode of operation, enabling the compare match A interrupt, setting prescalar to 64
	// And setting Top(value stored in OCRA) value to be compared to with counter to 249
	TCCR5B|= ((1<<WGM52) | (1<<CS51) | (1<<CS50));
	TCCR5A &= (~((1<<WGM50) | (1<<WGM51)));
	TCCR5B &= (~((1<<WGM53) | (1<<CS52)));
    TIMSK5 |= (1<<OCIE5A);
	OCR5A=TIMER5_CYC_FOR_10MILLISEC ;
}

void softwareTimer5(void){

static uint16_t counter=0;
counter++;

if(counter==1)
{
    num_spikes_ini=counter_spikes;
    
}

else if(counter == Counter_Value_For_10msec)
{	static int i=0;
    num_spikes_in_10msec=counter_spikes-num_spikes_ini;
    array[i]=num_spikes_in_10msec;
    
    if(i==N-1)
    {
        i=0;
    }
    else
    {
       i++; 
    }
    
    counter=0;
    counter_spikes=0;

}



}

ISR(INT0_vect)
{
    EIFR |= (1 << INTF0);
    led_yellowToggle();
    counter_spikes++;
} 

ISR(TIMER5_COMPA_vect) 
{
	TIFR5 |= (1<<OCF5A);// for clearing interrupt flag for compare A
	softwareTimer5();
}