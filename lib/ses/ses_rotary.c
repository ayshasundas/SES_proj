#include "ses_common.h"
#include "ses_rotary.h"
#include "ses_timer.h"
#include "ses_led.h"
#include "ses_uart.h"
#include "ses_lcd.h"

/************************ Definitions *************/

#define A_ROTARY_PORT PORTB //PB5
#define A_ROTARY_PIN 5

#define B_ROTARY_PORT PORTG //PG2
#define B_ROTARY_PIN 2

#define ROTARY_NUM_DETENT_CHECKS 5
#define Clockwise              0x01
#define Anti_Clockwise         0x02

static volatile int flag = 0;

static volatile pTypeRotaryCallback a; // global volatile function pointer for Clockwise Rotation callback
static volatile pTypeRotaryCallback b; // global volatile function pointer for AntiClockwise Rotation callback

void rotary_setClockwiseCallback(pTypeRotaryCallback callback)
{
    if (callback)
    {
        a = callback;
    }
}

void rotary_setCounterClockwiseCallback(pTypeRotaryCallback callback)
{
    if (callback)
    {
        b = callback;
    }
}

bool button_isAPressed(void)
{ 
    if (!(PIN_REGISTER(A_ROTARY_PORT) & (1 << A_ROTARY_PIN)))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

bool button_isBPressed(void)
{ 
    if (!(PIN_REGISTER(B_ROTARY_PORT) & (1 << B_ROTARY_PIN)))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
//*************************************************************************************************
void check_rotary()
{
    uart_init(57600);
    static uint8_t state[ROTARY_NUM_DETENT_CHECKS] = {};
    static uint8_t index = 0;
    static uint8_t CurrentState = 0;
    uint8_t lastState = CurrentState;
    state[index] = 0;
    if (button_isAPressed())
    {
        state[index] |= Clockwise;
    }
    if (button_isBPressed())
    {
        state[index] |= Anti_Clockwise;
        
    }

    index++;

    if (index == ROTARY_NUM_DETENT_CHECKS)
    {
        index = 0;
    }
    

    uint8_t j = 0xFF;
    for (uint8_t i = 0; i < ROTARY_NUM_DETENT_CHECKS; i++)
    {
        j = j & state[i];
        
    }
    
    CurrentState = j;

    
    /*
    **Since both signals arives while using rotary encoder, following code 
    **checks the change in states and decides on the last state because we are  
    **only interested in whatever input is on first in order to decide whether
    **Rotary is rotated Clockwise or AntiClockwise
    */
    if ((lastState != CurrentState) && (lastState == Clockwise) && (flag == 0))
    {
        flag++;
        a();
    }

    else if ((lastState != CurrentState) && (lastState == Anti_Clockwise) && (flag == 0))
    {
        flag++;
        b();
    }
    else if ((lastState != CurrentState) && (lastState == Clockwise || lastState == Anti_Clockwise))
    {
        flag = 0;
    }
}
void rotary_init()
{
    DDR_REGISTER(A_ROTARY_PORT) &= (~(1 << A_ROTARY_PIN));
    DDR_REGISTER(B_ROTARY_PORT) &= (~(1 << B_ROTARY_PIN));
    A_ROTARY_PORT |= (1 << A_ROTARY_PIN);
    B_ROTARY_PORT |= (1 << B_ROTARY_PIN);
}
