//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//	@file		Encoder.c										//
//	@brief		Encoder driver. Advance Implementation			//
//	@author		Grupo	4										//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//							Headers								//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#include <stddef.h>
#include "encoder.h"
#include "gpio.h"
#include "board.h"
#include "timer.h"
#include "button.h"

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//		CONSTANT AND MACRO DEFINITIONS USING #DEFINE 			//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			ENUMERATIONS AND STRUCTURES AND TYPEDEFS	  		//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

typedef uint8_t enc_status_t;
enum {INIT, CW, CCW};

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//						STATIC VARIABLES						//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

static enc_status_t status;
static tim_id_t encoder_timer;
static button_id_t encoder_button;
static uint8_t A_measured;
static enc_callback_t  callback_ccw;
static enc_callback_t  callback_cw;
static enc_callback_t  callback_click;

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS W FILE LEVEL SCOPE//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

static void callback_B2(void);
static void callback_B1(void);

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//					FUNCTION DEFINITIONS						//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void encoder_init(void){
/*****************************************************************
 * @brief: Initialize the encoder Driver
 * **************************************************************/
	static bool yaInit = false;
	if (yaInit)
		return;
	timerInit();
	encoder_timer = timerGetId();
	gpioMode(ENCODER_PIN_A, INPUT);
	gpioMode(ENCODER_PIN_B, INPUT);
	gpioIRQ (ENCODER_PIN_B, GPIO_IRQ_MODE_FALLING_EDGE, callback_B1);
	buttonInit();
	encoder_button = get_new_button(ENCODER_PIN_SW, INPUT);
	status=INIT;
	yaInit = true;
	return;
}

void encoder_set_callback(enc_callback_t  ccw, enc_callback_t  cw, enc_callback_t  click){
/*****************************************************************
 * @brief: Set the callbacks corresponding to the different types
 *          of modes
 * @param ccw: Callback for Counter-Clockwise turn.
 * @param cw: Callback for Clockwise turn.
 * @param click: Callback for button pressed.
 * **************************************************************/
	callback_ccw=ccw;
	callback_cw=cw;
	callback_click=click;
	configure_button(encoder_button, callback_cw,callback_click,callback_ccw);
	return;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//					LOCAL FUNCTION DEFINITIONS					//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void callback_B1(void){
/*****************************************************************
 * @brief: Funcion que se llama en cada flanco descendente del pin
 * 			B del encoder
 ****************************************************************/
	// Lo siguiente es una logíca para que los rebotes de las señales
	// no se tengan en cuenta.
	if (!timerRunning(encoder_timer)){
	// Si es la primera vez que entra al callback, se inicializa el timer
	// si vuelve a entrar al callback antes de que finalize el timer, no hace nada
	// por lo tanto, cuando termina el timer se llama a callback B2.
		timerStart(encoder_timer, 60, TIM_MODE_SINGLESHOT, callback_B2);
		A_measured = gpioRead(ENCODER_PIN_A);	//Se guarda el valor de la medicion del
												//pin A la primera vez que se llama a 
												//Callback_B1.
	}
	return;
}

void callback_B2(void){
/*****************************************************************
 * @brief: Funcion  que realiza la logica para determinar para
 * 			que lado se giró el encoder y se procede a llamar a
 * 			su respectivo callback.
 ****************************************************************/
	if(A_measured)		//Dado que esto entra con un flanco negativo de B
		callback_cw();	//se puede determinar el sentido de giro solo con el
	else				//valor de A medido.
		callback_ccw();
	return;
}
