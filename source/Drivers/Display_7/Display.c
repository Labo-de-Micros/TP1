//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//	@file		Display.c	    								//
//	@brief	    Display Screen Driver           				//
//	@author		Grupo	4										//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//							Headers								//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#include "./Display.h"
#include "./Disp_single.h"
#include "../Timer/timer.h"
#include "../../board.h"
#include "../GPIO/gpio.h"

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//		CONSTANT AND MACRO DEFINITIONS USING #DEFINE 			//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#define DISPLAY_MAX_AMOUNT  	10
#define REFRESH_FREQUENCY_HZ 	60
#define DIGITS 					4
#define MUX_PINS 				2
#define ROTATION_TIME_S 		1
#define EXT_BUF_LEN 			15
#define BRIGHTNESS_LEVELS 4

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			ENUMERATIONS AND STRUCTURES AND TYPEDEFS			//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

typedef struct{
	uint8_t curr_num;
	uint8_t brightness;
}display_t;

typedef struct{
    display_t displays[4];
	uint8_t buf[EXT_BUF_LEN];
}screen_t;

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//						STATIC VARIABLES						//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

static screen_t screen;
static tim_id_t display_timer;

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS W FILE LEVEL SCOPE//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

static void display_on();
/*****************************************************************
 * @brief: Turns display refresh on
 * **************************************************************/
static void refresh_callback();
/*****************************************************************
 * @brief: Callback called every time the display is refreshed.
 * **************************************************************/


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//					FUNCTION DEFINITIONS						//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void display_init(void){
/*****************************************************************
 * @brief: Initialize the Display Screen Driver
 * **************************************************************/
	static ya_init = false;
	if(ya_init)
		return;
	single_disp_init();
	gpioMode(DISPLAY_MUX_PIN_0, OUTPUT);
	gpioMode(DISPLAY_MUX_PIN_1, OUTPUT);
	display_timer = timerGetId();
	display_on();
	ya_init = true;
	return;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//					LOCAL FUNCTION DEFINITIONS					//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

static void display_on(void){
/*****************************************************************
 * @brief: Turns display refresh on
 * **************************************************************/
	uint16_t ticks=(uint16_t)(1000/(REFRESH_FREQUENCY_HZ*DIGITS));
	timerStart(display_timer, ticks, TIM_MODE_PERIODIC, refresh_callback);
	return;
}

static void refresh_callback(){
/*****************************************************************
 * @brief: Callback called every time the display is refreshed.
 * **************************************************************/
	static uint8_t digit = 0;
	if (digit>=DIGITS) digit=0;
	digit_select(digit);
	uint8_t pwm_ticks=(uint8_t)(1000/(REFRESH_FREQUENCY_HZ*DIGITS*(BRIGHTNESS_LEVELS-display.brightness[display.ext_index+digit])));
	timerStart(display.pwm_timer, pwm_ticks, TIM_MODE_SINGLESHOT, set_blank);
	set_pins(display.buf[display.ext_index+(digit++)]);
	return;
}