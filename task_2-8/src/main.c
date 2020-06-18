#include "ses_common.h"
#include "ses_adc.h"
#include <avr/io.h>
#include <util/delay.h>
#include <ses_led.h>
#include "ses_uart.h"






int main(void){
	
uart_init(57600);
while(1){
int16_t light=adc_read(ADC_LIGHT_CH);//reading ADC for light sensor
//_delay_ms(2500);
int16_t temp= adc_getTemperature();//reading ADC for temperature sensor
int16_t tempc=temp/10;
fprintf(uartout, "TEMPERATURE %3u.%1u\n LIGHT %u \n",tempc,temp-(tempc*10),light);
_delay_ms(2500);
}

}
