#include "ses_common.h"
#include "ses_adc.h"
#include <avr/io.h>
#include <util/delay.h>
#include <ses_led.h>
#include "ses_uart.h"


#define TEMP_SENSOR_PORT       	PORTF //PF2
#define TEMP_SENSOR_PIN         	2

#define LIGHT_SENSOR_PORT       	PORTF //PF4
#define LIGHT_SENSOR_PIN         	4

#define JOYSTICK_PORT       	PORTF //PF5
#define JOYSTICK_PIN         	5

#define ADC_VREF_SRC        3

#define ADC_TEMP_MAX 		40
#define ADC_TEMP_MIN 		20
#define ADC_TEMP_RAW_MAX	257
#define ADC_TEMP_RAW_MIN	483	
//#define ADC_TEMP_FACTOR		30	


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
ADCSRA=((ADCSRA | ADC_PRESCALE ) & 251); 
ADCSRA &= 223;
ADCSRA|=128;

}

uint16_t adc_read(uint8_t adc_channel){
adc_init();
uint16_t res=0;

if((adc_channel==2) | (adc_channel==4) | (adc_channel==5)){	
ADMUX = ((ADMUX & (~15)) | adc_channel); 	
ADCSRA |= (1<<6);	
while(1){

if((ADCSRA & (1 << 6))==0){
res=ADCW;
break;
}

else
{
	_delay_ms(1000);
}

}
return res;
}
else{
	return ADC_INVALID_CHANNEL;
}


}	

int16_t adc_getTemperature(void){
	
int16_t adc = adc_read(ADC_TEMP_CH);
int16_t tempvalue= (((ADC_TEMP_MAX*10 + (ADC_TEMP_RAW_MAX *10 * ((ADC_TEMP_MAX - ADC_TEMP_MIN)) / (ADC_TEMP_RAW_MIN-ADC_TEMP_RAW_MAX))))-(adc*10 * ((ADC_TEMP_MAX - ADC_TEMP_MIN)) / (ADC_TEMP_RAW_MIN-ADC_TEMP_RAW_MAX)));
return tempvalue;
}


int main(void){
	
uart_init(57600);
while(1){
int16_t light=adc_read(ADC_LIGHT_CH);
//_delay_ms(2500);
int16_t temp= adc_getTemperature();
int16_t tempc=temp/10;
fprintf(uartout, "TEMPERATURE %3u.%1u\n LIGHT %u",tempc,temp-(tempc*10),temp,light);
_delay_ms(2500);
}
return 0;
}