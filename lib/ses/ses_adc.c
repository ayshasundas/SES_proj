#include "ses_common.h"
#include "ses_adc.h"
#include <avr/io.h>
#include <util/delay.h>
#include <ses_led.h>
#include "ses_uart.h"


#define TEMP_SENSOR_PORT       			PORTF //PF2
#define TEMP_SENSOR_PIN         		2

#define LIGHT_SENSOR_PORT       		PORTF //PF4
#define LIGHT_SENSOR_PIN         		4

#define JOYSTICK_PORT       			PORTF //PF5
#define JOYSTICK_PIN         			5

#define ADC_VREF_SRC       				3
#define For_clearing_first_four_bits	15	

#define ADC_TEMP_MAX 					40//in degrees Celcius
#define ADC_TEMP_MIN 					20//in degrees Celcius
#define ADC_TEMP_RAW_MAX				257
#define ADC_TEMP_RAW_MIN				483	
#define ADC_TEMP_FACTOR					50	

#define unit_10							10


void adc_init(void)
{
//Initializes the ADC	
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
{//Disables the ADC
	ADCSRA &= (~(1<<ADEN));
}

uint16_t adc_read(uint8_t adc_channel)
{
//Read the raw ADC value of the given channel
//returns the raw ADC value
adc_init();
uint16_t res=0;

if((adc_channel==ADC_TEMP_CH) || (adc_channel==ADC_LIGHT_CH) || (adc_channel==ADC_JOYSTICK_CH))
{	
	ADMUX = ((ADMUX & (~For_clearing_first_four_bits)) | adc_channel); 	
	ADCSRA |= (1<<ADSC);	
	while(1)
	{

		if(!(ADCSRA & (1 << ADSC)))
		{
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
else
{
	adc_dis();
	return ADC_INVALID_CHANNEL;
}
}

int16_t adc_getTemperature(void)
{ 
	int16_t adc = adc_read(ADC_TEMP_CH);
	if(adc==ADC_INVALID_CHANNEL)
	{
		return ADC_INVALID_CHANNEL;
	}
	else
	{
		int16_t slope = ((ADC_TEMP_MAX - ADC_TEMP_MIN)*ADC_TEMP_FACTOR	*unit_10) / (ADC_TEMP_RAW_MAX - ADC_TEMP_RAW_MIN);//unit is 1/10
		int16_t offset = (ADC_TEMP_MAX*ADC_TEMP_FACTOR)	 - ((ADC_TEMP_RAW_MAX * slope)/unit_10);
		int16_t temp = ((((adc * slope)/unit_10) + offset)*unit_10)/ADC_TEMP_FACTOR ;// unit is 1/10
		return temp;
	}
}
