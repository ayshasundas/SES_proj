#include "ses_common.h"
#include "ses_adc.h"
#include <avr/io.h>
#include <util/delay.h>
#include <ses_led.h>
#include "ses_uart.h"


#define ADC_TEMP_MAX 		40
#define ADC_TEMP_MIN 		20
#define ADC_TEMP_RAW_MAX	257
#define ADC_TEMP_RAW_MIN	483	
#define ADC_TEMP_FACTOR		50	




int16_t adc_getTemperature(void)
{

int16_t adc = adc_read(ADC_TEMP_CH);
if(adc==ADC_INVALID_CHANNEL)
{
	return ADC_INVALID_CHANNEL;
}
else
{
int16_t slope = ((ADC_TEMP_MAX - ADC_TEMP_MIN)*ADC_TEMP_FACTOR	*10) / (ADC_TEMP_RAW_MAX - ADC_TEMP_RAW_MIN);//unit is 1/10
int16_t offset = (ADC_TEMP_MAX*ADC_TEMP_FACTOR)	 - ((ADC_TEMP_RAW_MAX * slope)/10);
int16_t temp = ((((adc * slope)/10) + offset)*10)/ADC_TEMP_FACTOR ;// unit is 1/10
return temp;
}
}


int main(void){
	
uart_init(57600);
while(1){
int16_t light=adc_read(ADC_LIGHT_CH);
//_delay_ms(2500);
int16_t temp= adc_getTemperature();
int16_t tempc=temp/10;
fprintf(uartout, "TEMPERATURE %3u.%1u\n LIGHT %u \n",tempc,temp-(tempc*10),light);
_delay_ms(2500);
}

}
