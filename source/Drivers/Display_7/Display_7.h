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

#if (REFRESH_FREQUENCY_HZ < 30)
#warning The refresh frequency of the display, is too low and it may be seen by the naked eye!
#elif (REFRESH_FREQUENCY_HZ > 60)
#warning The refresh frequency of the display is too high, and might be using unnecesary processor power!
#endif	// (REFRESH_FREQUENCY_HZ < 30)

#if (MUX_PINS != 2)
#error	This library only works with 2 MUX pins, in other words, only 4 display are allowed!
#endif	// (MUX_PINS != 2)

#if (DIGITS != 4)
#error This library only works with only 4 digits!
#endif

#if (EXT_BUF_LEN <= 10)
#error The buffer display is too short! Some messages cant be shown.abort
#elif (EXT_BUF_LEN >= 70)
#warning The buffer seems too big and inncesesary memory might be being used!
#endif	// (EXT_BUF_LEN <= 10)

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

void display_set_string(char * string);
/*****************************************************************
 * @brief: Writes a string to the main display buffer.
 * @param string: String of characters to be written.
 * **************************************************************/

void display_set_number(uint16_t number);
/*****************************************************************
 * @brief: Writes a number to the main display buffer
 * @param number: A number of maximum 4 digits to be written in the
 * 					display.
 * **************************************************************/

void display_on(void);
/*****************************************************************
 * @brief: Turns display refresh on
 * **************************************************************/

void display_off(void);
/*****************************************************************
 * @brief: Turns display refresh off
 * **************************************************************/

void display_temp_message(char * message, uint8_t seconds);
/*****************************************************************
 * @brief: Displays a string on the display for a number of seconds.
 * If message length exceeds that of the display, the delay starts 
 * after the message completes a rotation.
 * @param message: string of character with the message to show
 * @param seconds amount of seconds to show the message.
 * **************************************************************/

void display_set_brightness_level(display_brightness_level_t level);
/*****************************************************************
 * @brief: Sets a brightness level for the display
 * @param level: New level of brightness.
 * **************************************************************/

void display_enable_soft_highlight(uint8_t digit);
/*****************************************************************
 * @brief: Sets high brightness for a digit and normal brightness
 * for the rest of them
 * @param digit: Index of the digit to enable the soft Highlight.
 * **************************************************************/

void display_enable_hard_highlight(uint8_t digit);
/*****************************************************************
 * @brief: Sets high brightness for a digit and low brightness
 * for the rest of them
 * @param digit: Index of the digit to enable the hard Highlight.
 * **************************************************************/

void display_disable_highlight(void);
/*****************************************************************
 * @brief: Sets brightness back to nominal level
 * **************************************************************/

void display_clear_buffer(void);
/*****************************************************************
 * @brief: Clears the screen of the display (nothing will be displayed).
 * **************************************************************/

void display_set_single_number(uint8_t number, uint8_t index);
/*****************************************************************
 * @brief: Writes a number to a single digit of the display
 * @param number: The digit to be displayed.
 * @param index: Index in the buffer where the digit will be displayed.
 * **************************************************************/

void display_set_single_char(char character, uint8_t index);
/*****************************************************************
 * @brief: Writes a character to a single digit of the display
 * @param character: The character to be displayed.
 * @param index: Index in the buffer where the digit will be displayed. 
 * **************************************************************/

void display_enable_auto_rotation(void);
/*****************************************************************
 * @brief: Turns automatic rotation on. When a string is loaded, 
 * it will rotate once automatically.
 * **************************************************************/

void display_disable_auto_rotation(void);
/*****************************************************************
 * @brief: Turns automatic rotation off.
 * **************************************************************/

void display_stop_rotation(void);
/*****************************************************************
 * @brief: Stops current rotation (if happening)
 * **************************************************************/

void display_rotate_left(void);
/*****************************************************************
 * @brief: Shifts digits on the display to the left.
 * **************************************************************/

void display_rotate_right(void);
/*****************************************************************
 * @brief: Shifts digits on the display to the right.
 * **************************************************************/

void display_set_index(uint8_t index);
/*****************************************************************
 * @brief: Sets the current index for the display.
 * **************************************************************/

uint8_t display_get_index(void);
/*****************************************************************
 * @brief: Returns the current index of the display
 * @returns: The current index.
 * **************************************************************/

uint8_t display_get_brightness(void);
/*****************************************************************
 * @brief: Gets the current brightness of the whole display.
 * @returns: Current display brightness.
 * **************************************************************/

#endif
