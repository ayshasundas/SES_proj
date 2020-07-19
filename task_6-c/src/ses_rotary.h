#ifndef SES_ROTARY_H_
#define SES_ROTARY_H_

/* INCLUDES ******************************************************************/

#include "ses_common.h"
/************Type Definitions****************************/

typedef void (*pTypeRotaryCallback)();//defining function pointer type

/* FUNCTION PROTOTYPES *******************************************************/

/**
 * Initializes rotary encoder and joystick button
 * @param debouncing    flag that specifies if debouncing or direct interrrupt technique should be used 
 */
void rotary_init();

/** 
 * Get the state of the joystick button.
 */
bool button_isAPressed(void);

/** 
 * Get the state of the rotary button.
 */
bool button_isBPressed(void);
/**
 * For debouncing the buttons
 */ 
void check_rotary();
/**
 *  Sets a function to be called when rotary button is pressed
 *
 * @param callback  pointer to the callback function; if NULL, no callback
 *                  will be executed.
 */ 
void rotary_setClockwiseCallback(pTypeRotaryCallback callback);
/**
 *  Sets a function to be called when joystick button is pressed
 *
 * @param callback  pointer to the callback function; if NULL, no callback
 *                  will be executed.
 */ 
void rotary_setCounterClockwiseCallback(pTypeRotaryCallback callback);

#endif /* SES_ROTARY_H_ */
