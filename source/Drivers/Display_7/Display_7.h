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
#include "../GPIO/gpio.h"

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//		CONSTANT AND MACRO DEFINITIONS USING #DEFINE 		 	//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#define DISPLAY_MAX_AMOUNT  	10
#define REFRESH_FREQUENCY_HZ 	60
#define DIGITS 					4
#define MUX_PINS 				2
#define ROTATION_TIME_S 		0.4
#define EXT_BUF_LEN 			30

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			ENUMERATIONS AND STRUCTURES AND TYPEDEFS			//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

typedef uint8_t display_id_t;
typedef enum {
	DISPLAY_MODE_TEMPORARY,
	DISPLAY_MODE_PERSISTANT,
	DISPLAY_MODE_BLINK
} display_mode_t;

typedef enum{
	BRIGHT_MIN,
	BRIGHT_LOW,
	BRIGHT_HIGH,
	BRIGHT_MAX
} display_brightness_level_t;

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
 * @brief: Intialize the Pins of the seven segment display.
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

void display_set_string(char * string);
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

void display_temp_message(char * message, uint8_t seconds);

void display_set_brightness_level(display_brightness_level_t level);

void display_enable_soft_highlight(uint8_t digit);

void display_enable_hard_highlight(uint8_t digit);

void display_disable_highlight();

void display_clear_buffer(void);

void display_set_single_number(uint8_t number, uint8_t index);

void display_set_single_char(char character, uint8_t index);

void display_enable_auto_rotation();

void display_disable_auto_rotation();

void display_stop_rotation();

void display_rotate_left();

void display_rotate_right();

void display_set_index(uint8_t index);

uint8_t display_get_index();

uint8_t display_get_brightness(void);

#endif
