#ifndef SES_ROTARY_H_
#define SES_ROTARY_H_

/* INCLUDES ******************************************************************/

#include "ses_common.h"
/************Type Definitions****************************/

typedef void (*pTypeRotaryCallback)();//defining function pointer type

/* FUNCTION PROTOTYPES *******************************************************/

/**
 * Initializes rotary encoder 
 */
void rotary_init();

/** 
 * Get the state of the Rotary Input A.
 * @return a bool indicating either Rotary Input A is on or off
 */
bool button_isAPressed(void);

/** 
 * Get the state of the Rotary Input B.
 * @return a bool indicating either Rotary Input B is on or off
 */

bool button_isBPressed(void);

/**
 * For handling mechanical detents
 */ 
void check_rotary();
/**
 *  Sets a function to be called when Rotary is rotated clockwise
 *
 * @param callback  pointer to the callback function; if NULL, no callback
 *                  will be executed.
 */ 
void rotary_setClockwiseCallback(pTypeRotaryCallback callback);
/**
 *  Sets a function to be called when Rotary is rotated Anticlockwise
 *
 * @param callback  pointer to the callback function; if NULL, no callback
 *                  will be executed.
 */ 
void rotary_setCounterClockwiseCallback(pTypeRotaryCallback callback);

#endif /* SES_ROTARY_H_ */
