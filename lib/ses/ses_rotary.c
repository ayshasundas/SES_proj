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

#define BUTTON_NUM_DEBOUNCE_CHECKS 5

static volatile int counter = 0;
static volatile int flag = 0;

//#define BUTTON_NUM_DEBOUNCE_CHECKS      5 // Number of times a button bounces, after which we consider it as a single button press
//#define BUTTON_DEBOUNCE_POS_JOYSTICK    0x01
//#define BUTTON_DEBOUNCE_POS_ROTARY      0x02

static volatile pTypeRotaryCallback a; // global volatile function pointer for joystick button callback
static volatile pTypeRotaryCallback b; // global volatile function pointer for rotary button callback

void rotary_setClockwiseCallback(pTypeRotaryCallback callback)
{
    //callback for Right button
    if (callback)
    {
        a = callback;
    }
}

void rotary_setCounterClockwiseCallback(pTypeRotaryCallback callback)
{
    //callback for rotary button
    if (callback)
    {
        b = callback;
    }
}

bool button_isAPressed(void)
{ // For getting the state of the joystick button.
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
{ // For getting the state of the rotary button.
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
    static uint8_t state[BUTTON_NUM_DEBOUNCE_CHECKS] = {};
    static uint8_t index = 0;
    static uint8_t debouncedState = 0;
    uint8_t lastDebouncedState = debouncedState;
    state[index] = 0;
    if (button_isAPressed())
    {
        state[index] |= 0x01;
    }
    if (button_isBPressed())
    {
        state[index] |= 0x02;
        
    }

    index++;

    if (index == BUTTON_NUM_DEBOUNCE_CHECKS)
    {
        index = 0;
    }
    
    uint8_t j = 0xFF;
    for (uint8_t i = 0; i < BUTTON_NUM_DEBOUNCE_CHECKS; i++)
    {
        j = j & state[i];
        
    }
    
    debouncedState = j;

    

    if ((lastDebouncedState != debouncedState) & (lastDebouncedState == 0x01) & (flag == 0))
    {

        flag++;
        fprintf(uartout, "Clockwise and right is pressed\n");
    }

    else if ((lastDebouncedState != debouncedState) & (lastDebouncedState == 0x02) & (flag == 0))
    {
        flag++;
        fprintf(uartout, "AntiClockwise and left is pressed\n");
    }
    else if ((lastDebouncedState != debouncedState) & !(lastDebouncedState == 0x03))
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
