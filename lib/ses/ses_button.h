#ifndef SES_BUTTON_H_
#define SES_BUTTON_H_

/* INCLUDES ******************************************************************/

#include "ses_common.h"
/************Type Definitions****************************/

typedef void (*pButtonCallback)();//defining function pointer type

/* FUNCTION PROTOTYPES *******************************************************/

/**
 * Initializes rotary encoder and joystick button
 * @param debouncing    flag that specifies if debouncing or direct interrrupt technique should be used 
 */
void button_init(bool debouncing);

/** 
 * Get the state of the joystick button.
 */
bool button_isJoystickPressed(void);

/** 
 * Get the state of the rotary button.
 */
bool button_isRotaryPressed(void);
/**
 * For debouncing the buttons
 */ 
void button_checkState(void);
/**
 *  Sets a function to be called when rotary button is pressed
 *
 * @param callback  pointer to the callback function; if NULL, no callback
 *                  will be executed.
 */ 
void button_setRotaryButtonCallback(pButtonCallback callback);
/**
 *  Sets a function to be called when joystick button is pressed
 *
 * @param callback  pointer to the callback function; if NULL, no callback
 *                  will be executed.
 */ 
void button_setJoystickButtonCallback(pButtonCallback callback);

#endif /* SES_BUTTON_H_ */
