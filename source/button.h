//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//	@file		Button.h										//
//	@brief		Button management services.						//
//	@author		Grupo	4										//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#ifndef _BUTTON_H_
#define _BUTTON_H_

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//							Headers								//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"		//No se si meter gpio aca esta bien, pero necesitaria
						//el tipo de dato pin_t, que hacemos?

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//		CONSTANT AND MACRO DEFINITIONS USING #DEFINE 		 	//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#define BUTTONS_MAX_AMOUNT  16
#define BUTTON_INVALID_ID   255

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			ENUMERATIONS AND STRUCTURES AND TYPEDEFS			//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

typedef enum {BUTTON_MODE_SINGLE, BUTTON_MODE_DOUBLE, BUTTON_MODE_LONG} pressed_types_t;
typedef void (*button_callback_t)(void);
typedef uint8_t button_id_t;

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			FUNCTION PROTOTYPES WITH GLOBAL SCOPE				//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void button_init(void);
/*****************************************************************
 * @brief: Initialize the button Driver
 * **************************************************************/

button_id_t button_get_new_button(pin_t pin, uint8_t mode);
/*****************************************************************
 * @brief: Create new button
 * @param pin: Pin of the button connected, (gpioMode will be called
 *				with this pin).
 * @param mode: Mode for the gpioMode (Only available INPUT, INPUT_PULLUP
 *				INPUT_PULLDOWN).
 * @return: button_id_t with the id number of the button configured.
 * **************************************************************/

void button_configure(button_id_t button_id, button_callback_t callback_single, button_callback_t callback_long, button_callback_t callback_double);
/*****************************************************************
 * @brief: Configure the callbacks of the selected button
 * @param button_id: Button id of the button to configure its callbacks.
 * @param callback_single: Callback for the single press of the button.
 * @param callback_long: Callback for the long press of the button.
 * @param callback_double: Callback for the double press of the button.
 * 
 * @brief: If you dont want to specify a certain callback, just send NULL.
 * **************************************************************/


void button_disable(button_id_t button_id);
/*****************************************************************
 * @brief: Disable a specific button for not to take into account
 * 			its actions
 * @param button_id: Button id of the button to be disabled.
 * **************************************************************/


#endif	// _BUTTON_H_