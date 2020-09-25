//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//	@file		Card_reader.c									//
//	@brief		Driver for usage of the card reader 			//
//	@author		Grupo	4										//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//							Headers								//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#include <stddef.h>
#include "Card_reader.h"
#include "gpio.h"
#include "board.h"

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//		CONSTANT AND MACRO DEFINITIONS USING #DEFINE 			//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#define CARD_SS				0b1011
#define	CARD_FS				0b1101
#define	CARD_ES				0b1111
#define	CARD_LRC			0

#define _CARD_DEBUG_

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			ENUMERATIONS AND STRUCTURES AND TYPEDEFS	  		//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

typedef enum{CARD_IDLE, 
			CARD_WAIT_DATA, 
			CARD_0_ARRIVED,
			CARD_1_ARRIVED,
			CARD_2_ARRIVED,
			CARD_3_ARRIVED,
			CARD_P_ARRIVED, 
			CARD_WAIT_LRC, 
			CARD_ERROR,
			CARD_WAIT_FOR_START,
			CARD_ENABLE_ACTIVATED
}card_states_t;

typedef enum{
	CLK_EV,
	ENABLE_RISING_EV,
	ENABLE_FALLING_EV
}card_events_t;

typedef struct{
	uint8_t	data_0 : 1;
	uint8_t	data_1 : 1;
	uint8_t	data_2 : 1;
	uint8_t	data_3 : 1;
	uint8_t	data_p : 1;
}card_char_t;


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//						STATIC VARIABLES						//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

int16_t card_data[CARD_DATA_LENGTH];
uint8_t index = 0;
static card_callback_t	call;
static card_states_t current_state;
static card_char_t current_char;
static uint8_t enable_prev_state;
#ifdef	_CARD_DEBUG_
static uint32_t current_step = 0;
#endif


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS W FILE LEVEL SCOPE//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

static void card_callback(void);
static void clean_current_char(void);
static uint8_t get_binary_from_current_char(void);
static bool check_data_arrived(void);
static void clear_buffer(void);
static void save_data(void);
static bool check_entire_card(uint8_t lrc);
static card_events_t get_new_event(void);


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//					FUNCTION DEFINITIONS						//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void card_init(card_callback_t callback){
/*****************************************************************
 * @brief: Initializer the card reader driver and its components
 * @param callback: callback to be called when a new card is readed.
 ****************************************************************/
	gpioMode(CARD_DATA, INPUT);
	gpioMode(CARD_CLOCK, INPUT);
	gpioMode(CARD_ENABLE, INPUT);
	gpioIRQ (CARD_CLOCK, GPIO_IRQ_MODE_FALLING_EDGE , card_callback);	//Asumo que el clock no tirara nada cuando no haya leido nada.
	gpioIRQ (CARD_ENABLE, GPIO_IRQ_MODE_BOTH_EDGES , card_callback);
	call = callback;
	current_state = CARD_WAIT_FOR_START;
	enable_prev_state = gpioRead(CARD_ENABLE);
	return;
}

uint8_t * get_data(void){
/*****************************************************************
 * @brief: When the callback is called, one must obtain the data readed
 * 			by the driver, so this function returns the data.
 * 			(It must be called in the callback specified above).
 * @return: An array of lenght CARD_DATA_LENGTH containing the data
 * 			readed.
 ****************************************************************/
	return card_data;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//					LOCAL FUNCTION DEFINITIONS					//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

static void card_callback(void){
/*****************************************************************
 * @brief: Runs the state machine for the card reader protocol
 * 			(ISO/IEC 7811-2)
 ****************************************************************/
	card_events_t ev = get_new_event();		//gets the event
	uint8_t data = gpioRead(CARD_DATA);	//reads the data pin from the card reader.
	if(ev == ENABLE_RISING_EV){
		call();
		current_state = CARD_WAIT_FOR_START;
	}

	switch(current_state){
		case CARD_WAIT_FOR_START:	// Estado inicial, si me llega un Falling de enable, esta leyendo targeta.
			if(ev == ENABLE_FALLING_EV){
				//current_state = CARD_ENABLE_ACTIVATED;
				current_state = CARD_WAIT_DATA;
				clean_current_char();	//Limpio todo
				clear_buffer();
			}
			break;
		case CARD_ENABLE_ACTIVATED:	//Estado de lectura de targeta, si llega un rising en enable, termino de leer y esta listo
									//para enviarme los datos.
			if(ev == ENABLE_RISING_EV)
				current_state = CARD_WAIT_DATA;
			break;
		case CARD_WAIT_DATA:	//El sistema esta listo para empezar a recibir los datos.
			if(ev == CLK_EV){
				if(get_binary_from_current_char() == CARD_SS)	// me fijo si previamente llego un LS o un ES.
					current_state = CARD_WAIT_DATA;
				//else if(get_binary_from_current_char() == CARD_ES)
				//	current_state = CARD_WAIT_LRC;
				else{
					current_char.data_0 = data;//Guardo el primer dato que me llega en el primer bit del caracter.
					current_state = CARD_0_ARRIVED;
				}
			}
			//else	//No deberia llegar un cambio en enable
			//	current_state = CARD_ERROR;
			break;
		case CARD_0_ARRIVED://Estado de espera del segundo bit del caracter actual.
			if(ev == CLK_EV){
				current_char.data_0 = data;//Guardo el segundo dato que me llega en el segundo bit del caracter.
				current_state = CARD_1_ARRIVED;
			}
			//else
			//	current_state = CARD_ERROR;
			break;
		case CARD_1_ARRIVED://Estado de espera del tercer bit del caracter actual.
			if(ev == CLK_EV){
				current_char.data_1 = data;
				current_state = CARD_2_ARRIVED;
			}
			//else
			//	current_state = CARD_ERROR;
			break;
		case CARD_2_ARRIVED://Estado de espera del cuarto bit del caracter actual.
			if(ev == CLK_EV){
				current_char.data_2 = data;
				current_state = CARD_3_ARRIVED;
			}
			//else
			//	current_state = CARD_ERROR;
			break;
		case CARD_3_ARRIVED://Estado de espera del quinto bit del caracter actual.
			if(ev == CLK_EV){
				current_char.data_3 = data;
				current_state = CARD_P_ARRIVED;
			}
			//else
			//	current_state = CARD_ERROR;
			break;
		case CARD_P_ARRIVED://LLegaron todos los bits del caracter acutal, se procede a confirmar que la lectura
							//se haya hecho correctamente, y en caso de que asi sea, se la procede a guardar.
			if(ev == CLK_EV){
				current_char.data_p = data;
				if(check_data_arrived()){
					current_state = CARD_WAIT_DATA;//Se vuelve al estado de espera de nuevo cartacter.
					save_data();
				}
				//else
				//	current_state = CARD_ERROR;
			}
			//else
			//	current_state = CARD_ERROR;
			break;
		case CARD_WAIT_LRC://Estado de espera del ultimo caracter de la secuencia.
			if(ev == CLK_EV){
				if(check_entire_card(data))//Se guarda el ultimo caracter y se chequea que toda la targeta
											//se haya guardado correctamente, en caso de ser asi, se llama al callback.
					call();
				current_state = CARD_WAIT_FOR_START;
			}
			//else
			//	current_state = CARD_ERROR;
			break;
		default:
			current_state = CARD_ERROR;
			break;
	}
#ifdef	_CARD_DEBUG_
	current_step++;
#endif
	return;
}

static card_events_t get_new_event(void){
/*****************************************************************
 * @brief: Given that the callback has been called, this function
 * 			returns the event that produced that callback.
 * 			There are 3 events possible:
 * 				- CLK RISING EDGE EVENT
 * 				- ENABLE RISING EDGE EVENT
 * 				- ENABLE FALLING EDGE EVENT
 ****************************************************************/
	uint8_t enable = gpioRead(CARD_ENABLE);
	if(enable != enable_prev_state){
		enable_prev_state = enable;
		if(enable == HIGH)
			return ENABLE_RISING_EV;
		return ENABLE_FALLING_EV;
	}
	return CLK_EV;
}

static void clean_current_char(void){
/*****************************************************************
 * @brief: Set current_char in 0.
 ****************************************************************/	
	current_char.data_0 = 0;
	current_char.data_1 = 0;
	current_char.data_2 = 0;
	current_char.data_3 = 0;
	current_char.data_p = 0;
	return;
}

static bool check_data_arrived(void){
/*****************************************************************
 * @brief: Checks if the data arrived is a valid array or not.
 * @return: Returns 'true' if the data arrived is valid and 'false' otherwise.
 ****************************************************************/
	uint8_t data = get_binary_from_current_char();
	if(data % 2 != 1 && current_char.data_p != 0)
		return false;
	else if(data % 2 == 1 && current_char.data_p != 1)
		return false;
	return true;
}

static void clear_buffer(void){
/*****************************************************************
 * @brief: Clears the 'card_data' buffer
 ****************************************************************/
	for(uint8_t i = 0; i < CARD_DATA_LENGTH; i++)
		card_data[i] = -1;
	index = 0;
	return;
}

static void save_data(void){
/*****************************************************************
 * @brief: Saves the 'current_char' to the 'card_data'.
 ****************************************************************/
	card_data[index] = get_binary_from_current_char();
	index++;
	return;
}

static bool check_entire_card(uint8_t lrc){// Hay que cambiarla
/*****************************************************************
 * @brief: Checks if the entire card was readed correctly.
 ****************************************************************/
	for(uint8_t i = 0 ;i < index; i++){
		if(card_data[i] == -1)
			return false;
	}
	return true;
}

static uint8_t get_binary_from_current_char(void){
/*****************************************************************
 * @brief: Gets the binary number ob the obtained char.
 ****************************************************************/
	uint8_t data = current_char.data_3<<3;
	data |= current_char.data_2<<2;
	data |= current_char.data_1<<1;
	data |= current_char.data_0<<0;
	return data;
}
