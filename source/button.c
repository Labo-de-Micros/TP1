//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//	@file		Button.h									    //
//	@brief		Button management services.						//
//	@author		Grupo	4										//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//							Headers								//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#include "button.h"
#include "timer.h"
#include "gpio.h"

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//		CONSTANT AND MACRO DEFINITIONS USING #DEFINE 			//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#define BUTTON_TIMER_TICK	10
#define BUTTON_LONG_TICKS	100
#define BUTTON_SINGLE_TICKS	10
#define BUTTON_DOUBLE_TICK	10

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			ENUMERATIONS AND STRUCTURES AND TYPEDEFS	  		//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

enum {BUT_IDLE, BUT_PRESSED_ON, BUT_PRESSED_OFF, BUT_PRESSED_DOUBLE, BUT_ERROR}; 	//Estados de la maquina de Estados
enum {BUT_PRESS, BUT_REL, BUT_NO_EV}; 												//Eventos de la maquina de estados
typedef struct{
	button_callback_t callback_single;
	button_callback_t callback_long;
	button_callback_t callback_double;
	pin_t pin;
	uint8_t cnt;
	uint8_t enabled  	: 1;
	uint8_t is_pressed: 1;
	uint8_t sm_state  : 3;
	uint8_t sm_event  : 3;
}button_t;

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//						STATIC VARIABLES						//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

static button_t buttons[BUTTONS_MAX_AMOUNT];
static button_id_t id_count = 0;
static tim_id_t button_timer;

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS W FILE LEVEL SCOPE//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

static void button_callback(void);
static void get_events(void);
static void run_state_machine(button_id_t i);

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//					FUNCTION DEFINITIONS						//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void buttonInit(void){
/*****************************************************************
 * @brief: Initialize the button Driver
 * **************************************************************/

	static bool yaInit = false;
	if (yaInit)
			return;
	timerInit();
	button_timer = timerGetId();
	timerStart(button_timer, BUTTON_TIMER_TICK, TIM_MODE_PERIODIC, button_callback);
	yaInit = true;
	return;
}

button_id_t get_new_button(pin_t pin, uint8_t mode){
/*****************************************************************
 * @brief: Create new button
 * @param pin: Pin of the button connected, (gpioMode will be called
 *				with this pin).
 * @param mode: Mode for the gpioMode (Only available INPUT, INPUT_PULLUP
 *				INPUT_PULLDOWN).
 * @return: button_id_t with the id number of the button configured.
 * **************************************************************/

	button_id_t id = id_count++;
	gpioMode(pin,mode);
	buttons[id].pin = pin;
	buttons[id].enabled = true;
	buttons[id].is_pressed = gpioRead(pin);
	return id;
}

void configure_button(button_id_t button_id, button_callback_t callback_single, button_callback_t callback_long, button_callback_t callback_double){
/*****************************************************************
 * @brief: Configure the callbacks of the selected button
 * @param button_id: Button id of the button to configure its callbacks.
 * @param callback_single: Callback for the single press of the button.
 * @param callback_long: Callback for the long press of the button.
 * @param callback_double: Callback for the double press of the button.
 * 
 * @brief: If you dont want to specify a certain callback, just send NULL.
 * **************************************************************/
	buttons[button_id].callback_single = callback_single;
	buttons[button_id].callback_long = callback_long;
	buttons[button_id].callback_double = callback_double;
	buttons[button_id].sm_state = BUT_IDLE;
	buttons[button_id].sm_event = BUT_NO_EV;
	return;
}

void disable(button_id_t button_id){
/*****************************************************************
 * @brief: Disable a specific button for not to take into account
 * 			its actions
 * @param button_id: Button id of the button to be disabled.
 * **************************************************************/
	buttons[button_id].enabled = false;
	return;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//					LOCAL FUNCTION DEFINITIONS					//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void button_callback(void){
/*****************************************************************
 * @brief: Callback of the timer pooling, that runs the state machine
 * **************************************************************/
	get_events();	//Get the event for every button configured to run its own
					// state machine
	for(button_id_t i = 0; i<id_count; i++){	//For every button run its state machine
		if(buttons[i].enabled)
			run_state_machine(i);	//run machine
	}
	return;
}

void get_events(void){
/*****************************************************************
 * @brief: Update the events of each button configured
 * **************************************************************/
	for(button_id_t i = 0; i <= id_count; i++){
			if(buttons[i].is_pressed != gpioRead(buttons[i].pin)){
				//Checks the previous state of the button and compares with the
				//actual state, if it changes, then an event has occured, otherwise
				//BUT_NO_EV to the button evaluated.
					buttons[i].is_pressed = gpioRead(buttons[i].pin);
					if(buttons[i].is_pressed)
						buttons[i].sm_event = BUT_REL;	//Event button released
					else
						buttons[i].sm_event = BUT_PRESS;//Event button pressed.
			}
			else
				buttons[i].sm_event = BUT_NO_EV;
	}
	return;
}

void run_state_machine(button_id_t i){
/*****************************************************************
 * @brief: Runs the state machine for a given button
 * @param i: Button id of the button to run the state machine.
 * **************************************************************/
	uint8_t ev = buttons[i].sm_event;
	switch(buttons[i].sm_state){
		case BUT_IDLE:						//State IDLE
			if(ev == BUT_PRESS){
				buttons[i].sm_state = BUT_PRESSED_ON;
				buttons[i].cnt = 0;
			}
			break;
		case BUT_PRESSED_ON:				//State Button pressed First time after IDLE
			if(ev == BUT_PRESS){
				buttons[i].sm_state = BUT_ERROR;
				buttons[i].cnt = 0;
			}
			else if ((ev == BUT_NO_EV) && (buttons[i].cnt <= BUTTON_LONG_TICKS)){
				buttons[i].cnt++;
			}
			else if((ev == BUT_REL) && (buttons[i].cnt > BUTTON_LONG_TICKS)){
				buttons[i].cnt = 0;
				buttons[i].sm_state = BUT_IDLE;
				if(buttons[i].callback_long != NULL)
					buttons[i].callback_long();
			}
			else if((ev == BUT_REL) && (buttons[i].cnt <= BUTTON_LONG_TICKS)){
				buttons[i].cnt = 0;
				buttons[i].sm_state = BUT_PRESSED_OFF;
			}
			break;
		case BUT_PRESSED_OFF:				//Button released First time
			if((ev == BUT_NO_EV) && (buttons[i].cnt <= BUTTON_SINGLE_TICKS)){
				buttons[i].cnt++;
			}
			else if((ev == BUT_NO_EV) && (buttons[i].cnt > BUTTON_SINGLE_TICKS)){
				buttons[i].cnt = 0;
				buttons[i].sm_state = BUT_IDLE;
				if(buttons[i].callback_single != NULL)
					buttons[i].callback_single();
			}
			else if((ev == BUT_PRESS) && (buttons[i].cnt <= BUTTON_SINGLE_TICKS)){
				buttons[i].sm_state = BUT_PRESSED_DOUBLE;
				buttons[i].cnt = 0;
			}
			else if(ev == BUT_REL){
				buttons[i].sm_state = BUT_ERROR;
				buttons[i].cnt = 0;
			}
			break;
		case BUT_PRESSED_DOUBLE:			//Button pressed second time
			if(ev == BUT_REL){
				buttons[i].cnt = 0;
				buttons[i].sm_state = BUT_IDLE;
				if(buttons[i].callback_double != NULL)
					buttons[i].callback_double();
			}
			else if(ev == BUT_PRESS){
				buttons[i].sm_state = BUT_ERROR;
				buttons[i].cnt=0;
			}
			break;
		case BUT_ERROR:						//Button error
			break;
		default:
			break;
	}
	return;
}
