#include "ses_common.h"
#include "ses_button.h"
#include "ses_pwm.h"
#include "ses_motorFrequency.h"
#include "ses_timer.h"
#include "ses_led.h"
#include "ses_uart.h"
#include "ses_lcd.h"
#include "ses_scheduler.h"

/*static volatile int flag_joystick = 0;
static taskDescriptor td1;
uint16_t motorSpeed_rpm=0;

#define first_time_button_pressed             1 // showing that any button is pressed first time
#define second_time_button_pressed            2 // showing that any button is pressed second time

void callback_for_joystick(void)
{
    
    flag_joystick++;
    if(flag_joystick==first_time_button_pressed )
        {
            //OCR0B=0;
             pwm_setDutyCycle(100);
             //led_redOn();
        }
    else if(flag_joystick==second_time_button_pressed)
        {
            pwm_setDutyCycle(255);
            //led_redOff();
            flag_joystick=0;
        } 
}*/


/*void Motor_freq_rpm(void * ptr)
{
    lcd_init();
    lcd_clear();

    if(motorFrequency_getRecent()==0 || motorFrequency_getMedian()==0)
    {
        led_greenOn();
    }
    else
    {
        led_greenOff();
    }
    
    fprintf(lcdout, "Motor_freq_rpm recent\n%d\n",(motorFrequency_getRecent())*60);
    fprintf(lcdout, "Motor_freq_rpm median\n%d\n",(motorFrequency_getMedian())*60);

    fprintf(uartout, "Motor freq rpm recent\n%d\n",(motorFrequency_getRecent())*60);
    fprintf(uartout, "Motor freq in rpm median\n%d\n",(motorFrequency_getMedian())*60);


}*/



int main(void)
{
    
    uart_init(57600);
    lcd_init();
    uint8_t i=0;
    uint8_t j=0;
    int flag=0;
    while(1)
    {
      
       if(i<32 && flag==0)
       {
            lcd_setPixel(i, j, 1);
       lcd_setPixel(i, j+1, 1);
       lcd_setPixel(i+1, j, 1);
       lcd_setPixel(i+1, j+1, 1);
       lcd_setPixel(i+2, j, 1);
       lcd_setPixel(i+2, j+1, 1);
           i=i+1;
           
       }
       else
       {
           flag=1;
            lcd_setPixel(i, j, 1);
       lcd_setPixel(i, j+1, 1);
       lcd_setPixel(i-1, j, 1);
       lcd_setPixel(i-1, j+1, 1);
       lcd_setPixel(i-2, j, 1);
       lcd_setPixel(i-2, j+1, 1);
           i--;
           if(i==0)
           {
               flag=0;
           }
       }
       
       
       //lcd_setPixel(i+1, j, 1); 
       //lcd_setPixel(i+1, j+1, 1);
       
       j=j+1;
       
    }
  
}