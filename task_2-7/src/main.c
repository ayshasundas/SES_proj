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


void adc_init(void){

DDR_REGISTER(TEMP_SENSOR_PORT) &= (~(1 << TEMP_SENSOR_PIN));
DDR_REGISTER(LIGHT_SENSOR_PORT) &= (~(1 << LIGHT_SENSOR_PIN));
DDR_REGISTER(JOYSTICK_PORT) &= (~(1 << JOYSTICK_PIN));
TEMP_SENSOR_PORT &= (~(1 << TEMP_SENSOR_PIN));
LIGHT_SENSOR_PORT &= (~(1 << LIGHT_SENSOR_PIN));
JOYSTICK_PORT &= (~(1 << JOYSTICK_PIN));
PRR0 &= (~(1<<PRADC));
ADMUX |= ADC_VREF_SRC<<REFS0;
ADMUX &= (~(1<<ADLAR));
ADCSRA=((ADCSRA | ADC_PRESCALE ) & (~(1<<ADPS2))); 
ADCSRA &= (~(1<<ADATE));
ADCSRA|=(1<<ADEN);

}

void adc_dis(void)
{
	ADCSRA &= (~(1<<ADEN));
}

uint16_t adc_read(uint8_t adc_channel){
adc_init();
uint16_t res=0;

if((adc_channel==ADC_TEMP_CH) | (adc_channel==ADC_LIGHT_CH) | (adc_channel==ADC_JOYSTICK_CH)){	
ADMUX = ((ADMUX & (~15)) | adc_channel); 	
ADCSRA |= (1<<ADSC);	
while(1){

if((ADCSRA & (1 << ADSC))==0){
res=ADCW;
break;
}

else
{
	_delay_ms(1000);
}

}
adc_dis();
return res;
}
else{
	adc_dis();
	return ADC_INVALID_CHANNEL;
}
}	

int main(void){
adc_init();	
uart_init(57600);
uint16_t adcvalue= adc_read(ADC_TEMP_CH);

fprintf(uartout, "adc raw value %x \n ",adcvalue);

return 0;
}