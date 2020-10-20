//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//	@file		App_access.c								    //
//	@brief		Acces Control main								//
//	@author		Grupo	4										//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//							Headers								//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#include "./Drivers/Timer/timer.h"
#include "./Drivers/SysTick/Systick.h"
#include "./Drivers/Display_7/Display_7.h"
#include "./Drivers/Card_reader/Card_reader.h"
#include "./Drivers/Encoder/encoder.h"
#include "StateMachine/DataTypes.h"
#include "StateMachine/State_machine.h"
#include "Access_control.h"

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//		CONSTANT AND MACRO DEFINITIONS USING #DEFINE 		 	//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#define MAX_EVENTS				100

#if (MAX_EVENTS > 100)
#warning MAX_EVENTS too high! Maybe an unnecesary ammount of memory is used to catch events!
#elif (MAX_EVENTS <= 5)
#warning MAX_EVENTS too low! Maybe it is not enough space to store the events!
#endif

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			ENUMERATIONS AND STRUCTURES AND TYPEDEFS			//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

typedef enum {EV_CCW, EV_CW, EV_CLICK, EV_LONG_CLICK, EV_DOUBLE_CLICK, EV_CARD, EV_NO_EV}events_t;

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//						STATIC VARIABLES						//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

static events_t evs[MAX_EVENTS];
static uint32_t index_push = 0;
static uint32_t index_pull = 0;
SM_DEFINE(ACC, NULL)

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS W FILE LEVEL SCOPE//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

static void get_new_event(void);
/*****************************************************************
 * @brief: It runs trough all the event generator modules and gets
 * 			the event of every module. It appends the events arrived in
 * 			the variable evs. The evs buffer works as a circular array
 * 			in the FIFO form.
 * **************************************************************/

static void push_event(events_t ev);
/*****************************************************************
 * @brief: Function to push a new event to the events buffer evs.
 * @param ev: Event to be appended in the buffer.
 * **************************************************************/

static events_t pull_ev(void);
/*****************************************************************
 * @brief: Function to pull an event from the events buffer evs.
 * @returns: Event from the events buffer.
 * **************************************************************/

static void reset_buffer(void);
/*****************************************************************
 * @brief: Resets the buffer and sets no events in it.
 * **************************************************************/

static void run_state_machine(void);
/*****************************************************************
 * @brief: Function that gets the events from the events buffer and
 * 			runs the FSM with the correponding events.
 * **************************************************************/

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//					FUNCTION DEFINITIONS						//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void App_Init (void) {
	timerInit();
    encoder_init();
    card_init();
    display_init();
    display_on();
    access_control_init();
	reset_buffer();
	return;
}

void App_Run (void){
	while(true){
		get_new_event();
		run_state_machine();
	}
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//					LOCAL FUNCTION DEFINITIONS					//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

static void get_new_event(void){
/*****************************************************************
 * @brief: It runs trough all the event generator modules and gets
 * 			the event of every module. It appends the events arrived in
 * 			the variable evs.
 * **************************************************************/
	if(card_was_read())
		push_event(EV_CARD);
	switch(encoder_get_event()){
		case ENC_SINGLE_PRESS:
			push_event(EV_CLICK);
			break;
		case ENC_DOUBLE_PRESS:
			push_event(EV_DOUBLE_CLICK);
			break;
		case ENC_LONG_PRESS:
			push_event(EV_LONG_CLICK);
			break;
		case ENC_CLOCKWISE_TURN:
			push_event(EV_CW);
			break;
		case ENC_COUNTER_CLOCKWISE_TURN:
			push_event(EV_CCW);
			break;
		default:
			break;
    }

}

static void push_event(events_t ev){
/*****************************************************************
 * @brief: Function to push a new event to the events buffer evs.
 * @param ev: Event to be appended in the buffer.
 * **************************************************************/
	evs[index_push] = ev;	//Pusheo evento
	index_push++;					//Incremento el indice
	if(index_push>=MAX_EVENTS)
		index_push = 0;				// Si el indice es mayor al largo del buffer seteo el indice en 0 
											// porque es un buffer circular.
	return;
}

static events_t pull_ev(void){
/*****************************************************************
 * @brief: Function to pull an event from the events buffer evs.
 * @returns: Event from the events buffer.
 * **************************************************************/
	events_t temp = evs[index_pull];	//Guardo el ultimo evento recibido
	if(temp == EV_NO_EV)	//Si no hubo evento, salgo de la funcion.
		return temp;
	evs[index_pull] = EV_NO_EV;	//caso contrario, seteo ENC_NO_EV y incremento el indice actual
	index_pull++;							//teniendo en cuenta el buffer circular.
	if(index_pull >= MAX_EVENTS)
		index_pull = 0;
	return temp;
}

static void reset_buffer(void){
/*****************************************************************
 * @brief: Resets the buffer and sets no events in it.
 * **************************************************************/
	for(uint32_t i = 0 ; i < MAX_EVENTS; i++)
		evs[i] = EV_NO_EV;
	index_push = 0;
	index_pull = 0;
	return;
}

static void run_state_machine(void){
/*****************************************************************
 * @brief: Function that gets the events from the events buffer and
 * 			runs the FSM with the correponding events.
 * **************************************************************/
	switch(pull_ev()){
		case EV_CCW:
			SM_Event(ACC, Encoder_CCW, NULL);
			break;
		case EV_CW:
			SM_Event(ACC, Encoder_CW, NULL);
			break;
		case EV_CLICK:
			SM_Event(ACC, Encoder_Click, NULL);
			break;
		case EV_DOUBLE_CLICK:
			SM_Event(ACC, Encoder_Double_Click, NULL);
			break;
		case EV_LONG_CLICK:
			SM_Event(ACC, Encoder_Long_Click, NULL);
			break;
		case EV_CARD:
			SM_Event(ACC, Card_Reader, NULL);
			break;
		default:
			break;
	}
}
