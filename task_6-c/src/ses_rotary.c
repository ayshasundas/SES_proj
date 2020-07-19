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

static volatile int counter = 0

    //#define BUTTON_NUM_DEBOUNCE_CHECKS      5 // Number of times a button bounces, after which we consider it as a single button press
    //#define BUTTON_DEBOUNCE_POS_JOYSTICK    0x01
    //#define BUTTON_DEBOUNCE_POS_ROTARY      0x02

    static volatile pTypeRotaryCallback a; // global volatile function pointer for joystick button callback
static volatile pTypeRotaryCallback b;     // global volatile function pointer for rotary button callback

ISR(INT0_vect)
{
    EIFR |= (1 << INTF0);
    if (counter)
    {
        //b();
        fprintf(uartout, "AC and b is pressed\n");
        counter = 0;
    }
    else
    {
        counter = 1;
    }
}

ISR(INT1_vect)
{
    EIFR |= (1 << INTF1);
    if (counter)
    {
        //a();

        counter = 0;
    }
    else
    {
        counter = 1;
    }
}

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

void check_rotary()
{
    uart_init(57600);
    static uint8_t p = 0;
    static bool sampling = false;
    bool button_a = PIN_REGISTER(A_ROTARY_PORT) & (1 << A_ROTARY_PIN);
    bool button_b = PIN_REGISTER(B_ROTARY_PORT) & (1 << B_ROTARY_PIN);
    if (button_a != button_b)
    {
        sampling = true;
        if (button_a == 0)
        {
            fprintf(uartout, "CC and button_a is pressed\n");
        }
        else if (button_b == 0)
        {
            fprintf(uartout, "AC and button_b is pressed\n");
        }
    }
    if (sampling && p < 122)
    {
        lcd_setPixel((button_a) ? 0 : 1, p, true);
        lcd_setPixel((button_b) ? 4 : 5, p, true);
        p++;
    }

    //For debouncing the buttons
    static uint8_t state[BUTTON_NUM_DEBOUNCE_CHECKS] = {};
    static uint8_t index = 0;
    static uint8_t debouncedState = 0;
    uint8_t lastDebouncedState = debouncedState;
    // each bit in every state byte represents one button
    state[index] = 0;
    if (button_isJoystickPressed())
    {
        state[index] |= BUTTON_DEBOUNCE_POS_JOYSTICK;
    }
    if (button_isRotaryPressed())
    {
        state[index] |= BUTTON_DEBOUNCE_POS_ROTARY;
    }

    index++;

    if (index == BUTTON_NUM_DEBOUNCE_CHECKS)
    {
        index = 0;
    }
    // init compare value and compare with ALL reads, only if
    // we read BUTTON_NUM_DEBOUNCE_CHECKS consistent "1's" in the state
    // array, the button at this position is considered pressed
    uint8_t j = 0xFF;
    for (uint8_t i = 0; i < BUTTON_NUM_DEBOUNCE_CHECKS; i++)
    {
        j = j & state[i];
    }

    debouncedState = j;

    //for calling the corresponding button callback as soon as the debouncedState indicates a (debounced) button press (not release)

    if ((lastDebouncedState != debouncedState) & ((debouncedState & BUTTON_DEBOUNCE_POS_JOYSTICK) == BUTTON_DEBOUNCE_POS_JOYSTICK))
    {
        joystick();
    }

    else if ((lastDebouncedState != debouncedState) & ((debouncedState & BUTTON_DEBOUNCE_POS_ROTARY) == BUTTON_DEBOUNCE_POS_ROTARY))
    {
        rotary();
    }
}

void rotary_init()
{
    DDR_REGISTER(A_ROTARY_PORT) &= (~(1 << A_ROTARY_PIN));
    DDR_REGISTER(B_ROTARY_PORT) &= (~(1 << B_ROTARY_PIN));
    A_ROTARY_PORT |= (1 << A_ROTARY_PIN);
    B_ROTARY_PORT |= (1 << B_ROTARY_PIN);

    EIMSK &= (~(1 << INT0));
    EIMSK &= (~(1 << INT1));
    EICRA |= ((1 << ISC01) | (1 << ISC11));
    EICRA &= ~((1 << ISC00) | (1 << ISC10));
    EIMSK |= (1 << INT0);
    EIMSK |= (1 << INT1);

    //initialization for both techniques (e.g. setting up the DDR register)
    //initializing buttons for both joystick and rotary and also enabling the interrupts for these buttons using pin change interrupt register

    timer1_setCallback(check_rotary);
}
