//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//	@file		Display_7.h										//
//	@brief		Seven segment display services.					//
//	@author		Grupo	4										//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#ifndef _DISP_7_SEG_H
#define _DISP_7_SEG_H

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//							Headers								//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "gpio.h"

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//		CONSTANT AND MACRO DEFINITIONS USING #DEFINE 		 	//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#define DISPLAY_MAX_AMOUNT  10
#define REFRESH_FREQUENCY_HZ 60
#define DIGITS 4
#define MUX_PINS 2
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			ENUMERATIONS AND STRUCTURES AND TYPEDEFS			//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

typedef uint8_t display_id_t;


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			FUNCTION PROTOTYPES WITH GLOBAL SCOPE				//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void display_init(void);
/*****************************************************************
 * @brief: Initialize the Seven segment display driver
 * **************************************************************/

void display_set_mode(display_mode_t mode);

void display_configure_pins(pin_t a,pin_t b,pin_t c,pin_t d,pin_t e,pin_t f,pin_t g);
/*****************************************************************
 * @brief: Intialize de Pins of the seven segment display.
 * @param a: Is the pin number of the 'a' pin of the display
 * @param b: Is the pin number of the 'b' pin of the display
 * @param c: Is the pin number of the 'c' pin of the display
 * @param d: Is the pin number of the 'd' pin of the display
 * @param e: Is the pin number of the 'e' pin of the display
 * @param f: Is the pin number of the 'f' pin of the display
 * @param g: Is the pin number of the 'g' pin of the display
 * 
 * @brief: gpioMode() will be called with those pins numbers.
 * **************************************************************/

void display_configure_mux(pin_t pin0, pin_t pin1);
/*****************************************************************
 * @brief: Initialize mux pins for digit select.
 * **************************************************************/

void display_set_string(char character);
/*****************************************************************
 * @brief: Set a specified number on the screen of the seven segment display
 * @param character: A character from a-z to be displayed on the screen.
 * **************************************************************/

void display_set_number(uint16_t number);
/*****************************************************************
 * @brief: Sets a four digit number into display buffers.
 * **************************************************************/

void display_on();
/*****************************************************************
 * @brief: Starts updating display.
 * **************************************************************/

void display_off();
/*****************************************************************
 * @brief: Stops updating display.
 * **************************************************************/

void display_temp_message();

#endif