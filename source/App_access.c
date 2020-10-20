/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "board.h"
#include "./Drivers/Timer/timer.h"
#include "./Drivers/SysTick/Systick.h"
#include "./Drivers/Display_7/Display_7.h"
#include "./Drivers/Card_reader/Card_reader.h"
#include "./Drivers/Encoder/encoder.h"
#include "StateMachine/DataTypes.h"
#include "StateMachine/State_machine.h"
#include "Access_control.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define DEBUG_OSCILLOSCOPE
#define BUTTON_CHECK_MS			10
#define BLINK_FREQ_HZ 			2U
#define BUTTON_CHECK_TICKS		TIMER_MS2TICKS(10)
#define BLINK_FREQ_TICKS 		TIMER_MS2TICKS( 1000/BLINK_FREQ_HZ )
#define MAX_EVENTS				100

#if SYSTICK_ISR_FREQUENCY_HZ % (2*BLINK_FREQ_HZ) != 0
#warning BLINK cannot implement this exact frequency.
		Using floor(SYSTICK_ISR_FREQUENCY_HZ/BLINK_FREQ_HZ/2) instead.
#endif
/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

typedef enum {EV_CCW, EV_CW, EV_CLICK, EV_LONG_CLICK, EV_DOUBLE_CLICK, EV_CARD, EV_NO_EV}events_t;
static void get_new_event(void);
static void push_event(events_t ev);
static events_t pull_ev(void);
static void reset_buffer(void);
static void run_state_machine(void);

static events_t evs[MAX_EVENTS];
static uint32_t index_push = 0;
static uint32_t index_pull = 0;
SM_DEFINE(ACC, NULL)

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void App_Init (void) {
	timerInit();
    encoder_init();
    card_init();
    display_init();
    display_configure_pins(DISPLAY_PIN_A, DISPLAY_PIN_B, DISPLAY_PIN_C, DISPLAY_PIN_D, DISPLAY_PIN_E, DISPLAY_PIN_F, DISPLAY_PIN_G);
    display_configure_mux(DISPLAY_MUX_PIN_0, DISPLAY_MUX_PIN_1);
    display_on();
    access_control_init(ACCObj);
	reset_buffer();
	return;
}

void App_Run (void){
	while(true){
		get_new_event();
		run_state_machine();
	}
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static void get_new_event(void){
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
 * @brief: Pushes an event to the buffer.
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
 * @brief: Pulls an event from the buffer, removing it.
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
	for(uint32_t i = 0 ; i < MAX_EVENTS; i++)
		evs[i] = EV_NO_EV;
	index_push = 0;
	index_pull = 0;
	return;
}

static void run_state_machine(void){
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
