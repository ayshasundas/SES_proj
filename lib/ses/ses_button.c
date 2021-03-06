#include "ses_common.h"
#include "ses_button.h"
#include "ses_timer.h"
#include "ses_led.h"
#include "ses_uart.h"

/************************ Definitions *************/

#define BUTTON_JOYSTICK_PORT       	PORTB //PB7
#define BUTTON_JOYSTICK_PIN         	7

#define BUTTON_ROTARY_PORT 	   PORTB //PB6
#define BUTTON_ROTARY_PIN      	6


#define BUTTON_NUM_DEBOUNCE_CHECKS      5 // Number of times a button bounces, after which we consider it as a single button press
#define BUTTON_DEBOUNCE_POS_JOYSTICK    0x01
#define BUTTON_DEBOUNCE_POS_ROTARY      0x02

static volatile  pButtonCallback joystick; // global volatile function pointer for joystick button callback
static volatile pButtonCallback rotary; // global volatile function pointer for rotary button callback




ISR(PCINT0_vect){
// callback for ISR name= pcint0_vect
//It toggles the red led if interrupt for joystick button is triggered
//It toggles the green led if interrupt for rotary button is triggered

if ( button_isJoystickPressed() && ((PCMSK0 & (1<<PCINT7)) != 0) )
{
    joystick();
    _delay_ms(500);
}
else if( button_isRotaryPressed() && ((PCMSK0 & (1<<PCINT6))!=0))
{

    rotary(); 
    _delay_ms(500);
 
}
}

void button_setJoystickButtonCallback(pButtonCallback callback)
{
//callback for joystick button
if(callback)
{
    joystick=callback;
}
}



void button_setRotaryButtonCallback(pButtonCallback callback)
{
//callback for rotary button
if(callback)
{
    rotary=callback;
}

}


bool button_isJoystickPressed(void)
{// For getting the state of the joystick button.
if( !(PIN_REGISTER(BUTTON_JOYSTICK_PORT) & (1 << BUTTON_JOYSTICK_PIN)) )
{
    return 1;
}
else
{
    return 0;
}
}

bool button_isRotaryPressed(void)
{// For getting the state of the rotary button.
if( !(PIN_REGISTER(BUTTON_ROTARY_PORT) & (1 << BUTTON_ROTARY_PIN)) )
{
    return 1;
}
else
{
    return 0;
}
}

void button_checkState() 
{
//For debouncing the buttons    
static uint8_t state[BUTTON_NUM_DEBOUNCE_CHECKS] = {};
static uint8_t index = 0;
static uint8_t debouncedState = 0;
uint8_t lastDebouncedState = debouncedState;
// each bit in every state byte represents one button
state[index] = 0;
if(button_isJoystickPressed())
{
    state[index] |= BUTTON_DEBOUNCE_POS_JOYSTICK;
}
if(button_isRotaryPressed())
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
for(uint8_t i = 0; i < BUTTON_NUM_DEBOUNCE_CHECKS; i++)
{
    j = j & state[i];   
}

debouncedState = j;

//for calling the corresponding button callback as soon as the debouncedState indicates a (debounced) button press (not release)

if ((lastDebouncedState!=debouncedState) & ((debouncedState & BUTTON_DEBOUNCE_POS_JOYSTICK) == BUTTON_DEBOUNCE_POS_JOYSTICK))
{
    joystick();
}

else if ((lastDebouncedState!=debouncedState) & ((debouncedState & BUTTON_DEBOUNCE_POS_ROTARY) == BUTTON_DEBOUNCE_POS_ROTARY))
{
    rotary(); 
}


}

void button_init(bool debouncing)
{
// TODO initialization for both techniques (e.g. setting up the DDR register)   
//initializing buttons for both joystick and rotary and also enabling the interrupts for these buttons using pin change interrupt register
DDR_REGISTER(BUTTON_JOYSTICK_PORT) &= (~(1 << BUTTON_JOYSTICK_PIN));
DDR_REGISTER(BUTTON_ROTARY_PORT) &= (~(1 << BUTTON_ROTARY_PIN));
BUTTON_JOYSTICK_PORT |= (1 << BUTTON_JOYSTICK_PIN);
BUTTON_ROTARY_PORT |= (1 << BUTTON_ROTARY_PIN);
if(debouncing) 
{
    //TODO initialization for debouncing
    timer1_setCallback(button_checkState);
}
else
{   // TODO initialization for direct interrupts (e.g. setting up the PCICR register)
    PCICR |= (1 << PCIE0);
    PCMSK0 |= ((1<<PCINT6));
    PCMSK0 |= (1<<PCINT7);
}
}

