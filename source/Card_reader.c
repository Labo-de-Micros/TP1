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

#define CARD_SS				11
#define	CARD_FS				13
#define	CARD_ES				15

#define CARD_32_MASK		0x00000001
#define _CARD_DEBUG_

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			ENUMERATIONS AND STRUCTURES AND TYPEDEFS	  		//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

typedef enum{CARD_IDLE, 
			CARD_WAIT_DATA, 
			CARD_ERROR,
			CARD_WAIT_FOR_START,
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

static uint32_t card_data_compressed[CARD_DATA_REGISTERS_LENGTH];
static uint8_t index;
static card_callback_t	call;
static card_states_t current_state;
static card_t card;

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS W FILE LEVEL SCOPE//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

static void card_machine(card_events_t);
static void clear_buffer(void);
static void enable_callback(void);
static void clock_callback(void);
static card_char_t get_current_char(uint8_t ind);
static uint8_t get_char_num(card_char_t character);
static bool is_char_valid(card_char_t character);
static uint8_t search_for_start(void);
static void clear_card(void);

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
	gpioMode(CARD_DATA, INPUT);	// Inicializo los pines correspondientemente
	gpioMode(CARD_CLOCK, INPUT);
	gpioMode(CARD_ENABLE, INPUT);
	gpioIRQ (CARD_CLOCK, GPIO_IRQ_MODE_FALLING_EDGE , clock_callback);	//Inicializo las interrupciones
	gpioIRQ (CARD_ENABLE, GPIO_IRQ_MODE_BOTH_EDGES , enable_callback);
	call = callback;
	current_state = CARD_WAIT_FOR_START;	//Seteo el estado de la maquina de estados como 'esperando targeta'.
	index = 0;
	clear_buffer();	// Limpio el buffer para que todo quede seteado en 0.
	return;
}

card_t get_data(void){
/*****************************************************************
 * @brief: When the callback is called, one must obtain the data readed
 * 			by the driver, so this function returns the card data.
 * @return: A struct card_t containing the information readed in the card.
 ****************************************************************/
//Por ahora asumo que siempre la targeta se lee en la direccion correcta.
	clear_card();
	uint8_t start_index = search_for_start();
	uint8_t i = 0;
	uint8_t num = 0;
	for(i = 0; i < CARD_CHARACTERS_LENGTH && num != CARD_FS ; i++){
		card_char_t chara = get_current_char(start_index+i*5);
		num = get_char_num(chara);
		if(num != CARD_SS && num != CARD_FS && num != CARD_ES){
			card.pan = card.pan * 10 + num;
		}
	}

	num = get_char_num(get_current_char(start_index+i*5));
	i++;
	card.exp_year = num*10 + get_char_num(get_current_char(start_index+i*5));
	i++;
	num = get_char_num(get_current_char(start_index+i*5));
	i++;
	card.exp_month = num*10 + get_char_num(get_current_char(start_index+i*5));
	
	for(uint8_t u = 0; u < CARD_SERVICE_LENGHT; u++){
		i++;
		uint8_t num = get_char_num(get_current_char(start_index+i*5));
		card.service_code = card.service_code * 10 + num;
	}
	i++;
	card.PVKI = get_char_num(get_current_char(start_index+i*5));
	for(uint8_t u = 0; u < CARD_PVV_LENGHT; u++){
		i++;
		uint8_t num = get_char_num(get_current_char(start_index+i*5));
		card.PVV = card.PVV * 10 + num;
	}
	for(uint8_t u = 0; u < CARD_CVV_LENGHT; u++){
		i++;
		uint8_t num = get_char_num(get_current_char(start_index+i*5));
		card.CVV = card.CVV*10 +  num;
	}
	i++;

	return card;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//					LOCAL FUNCTION DEFINITIONS					//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

static void card_machine(card_events_t ev){
/*****************************************************************
 * @brief: Runs the state machine for the card reader protocol
 * 			(ISO/IEC 7811-2)
 ****************************************************************/
	uint8_t data = gpioRead(CARD_DATA);	//Leo el dato del pin apenas entra a la funcion para que no me afecte el tiempo
										// de ejecucion de las instrucciones.
	switch(current_state){
		case CARD_WAIT_FOR_START:		// Estado inicial, esperando a que se empieze a leer una targeta.
			if(ev == ENABLE_FALLING_EV){// Si llega un flanco negativo del pin ENABLE, se empezo a leer una targeta
										// entonces pasamos al estado WAIT_DATA.
				current_state = CARD_WAIT_DATA;
				index = 0;
				clear_buffer();
			}
			break;
		case CARD_WAIT_DATA:	// Estado de espera de arribo de nuevo dato.
			if(ev == CLK_EV){	// Si llega un flanco negativo de clock, guardo el dato leido.
				if(data == 0)	// Es importante notar que el dato leido en el pin esta negado, por eso este data == 0.
								// ya que en caso de que asi sea, escribo un 1 en el lugar correspondiente de los 8 registros de 32 bits.
					card_data_compressed[index/CARD_DATA_REGISTER] |= CARD_32_MASK << ((CARD_DATA_REGISTER-1)-(index % CARD_DATA_REGISTER));
				index++;		// incremento el indice del dato actual.
			}
			else if(ev == ENABLE_RISING_EV){		// Si llega un flanco positivo del pin ENABLE, se termino de leer la targeta
				call();								// entonces llamo al callback, y seteo el estado como el estado inicial, por si se lee
				current_state = CARD_WAIT_FOR_START;// otra trageta.
			}
			break;
		default:
			current_state = CARD_ERROR;
			break;
	}
	return;
}

static void enable_callback(void){
/*****************************************************************
 * @brief: Generates the events of the enable pin for the state machine 
 ****************************************************************/
	if(gpioRead(CARD_ENABLE) == HIGH)
		card_machine(ENABLE_RISING_EV);
	else
		card_machine(ENABLE_FALLING_EV);
	return;
}

static void clock_callback(void){
/*****************************************************************
 * @brief: Generates the events of the clock pin for the state machine 
 ****************************************************************/
	card_machine(CLK_EV);
	return;
}

static void clear_buffer(void){
/*****************************************************************
 * @brief: Clears the 'card_data_compressed' buffer
 ****************************************************************/
	for(uint16_t i = 0; i < CARD_DATA_REGISTERS_LENGTH; i++)
		card_data_compressed[i] = 0;
	index = 0;
	return;
}

static card_char_t get_current_char(uint8_t ind){
/*****************************************************************
 * @brief: Generates the char associated with the index given.
 * @param ind: index for the character start.
 * @return card_char_t containing the information of the character in the
 * 			selected position.
 * @example: Given the sequence 01100101111000101 and ind = 2, the returned char
 * 				will be composed of:
 * 					data_0 = 1
 * 					data_1 = 0
 * 					data_2 = 0
 * 					data_3 = 1
 * 					data_p = 0
 ****************************************************************/
	card_char_t curr_char;
	uint8_t i = ind % CARD_DATA_REGISTER;      // indice de que bit tengo que agarrar.
	uint8_t index_2 = ind/ CARD_DATA_REGISTER; // Indice de que registro tengo que agarrar.
	// Agarro el registro correspondiente y realizo una operacion AND con un 1 ubicado en
	// la posición i-esima. Finalmente me fijo si el numero resultante es 0 o no, en caso
	// de que no sea cero, guardo un 1, caso contrario un 0.
	// Recordemos que la posición 0 es el bit mas significativo y la posicion 31 (para el caso de
	// registros de 32 bits) es el bit menos significativo.
	curr_char.data_0 = (card_data_compressed[index_2] & (CARD_32_MASK<<((CARD_DATA_REGISTER-1)-i))) != 0;
	i++;
	curr_char.data_1 = (card_data_compressed[index_2] & (CARD_32_MASK<<((CARD_DATA_REGISTER-1)-i))) != 0;
	i++;
	curr_char.data_2 = (card_data_compressed[index_2] & (CARD_32_MASK<<((CARD_DATA_REGISTER-1)-i))) != 0;
	i++;
	curr_char.data_3 = (card_data_compressed[index_2] & (CARD_32_MASK<<((CARD_DATA_REGISTER-1)-i))) != 0;
	i++;
	curr_char.data_p = (card_data_compressed[index_2] & (CARD_32_MASK<<((CARD_DATA_REGISTER-1)-i))) != 0;
	return curr_char;
}

static uint8_t get_char_num(card_char_t character){
/*****************************************************************
 * @brief: Gets the hexadecimal number corresponding to the character given
 * @param character: caharacter to get its hexadecimal value.
 * @returns: hexadecimal value.
 ****************************************************************/
	uint8_t temp = character.data_3 << 3;
	temp |= character.data_2 << 2;
	temp |= character.data_1 << 1;
	temp |= character.data_0 << 0;
	return temp;
}

static bool is_char_valid(card_char_t character){
/*****************************************************************
 * @brief: Checks if a given character is valid (parity code)
 * @param character: caharacter to check.
 * @returns: true if the character is a valid one, false otherwise.
 ****************************************************************/
	if(get_char_num(character) % 2 == 0 && character.data_p != 1)
		return false;
	else if(get_char_num(character) % 2 != 0 && character.data_p != 0)
		return false;
	else
		return true;
}

static uint8_t search_for_start(void){
/*****************************************************************
 * @brief: Search for the start of the Card Sequence.
 * @returns: uint8_t index of the start of the card sequence.
 ****************************************************************/
	// Busco que indice se corresponde con el caracter SS (inicio de targeta).
	uint8_t i = 0;
	for(uint8_t ind = 0 ; ind < CARD_DATA_LENGTH ; ind++){
		if(get_char_num(get_current_char(ind)) == CARD_SS && is_char_valid(get_current_char(ind))){
			i=ind;
			break;
		}
	}
	// Una vez que lo encuentro, devuelvo la posicion del siguiente caracter.
	return i+5;
}

static void clear_card(void){
/*****************************************************************
 * @brief: Clears all data from the variable card.
 ****************************************************************/
	card.pan = 0;
	card.CVV = 0;
	card.exp_month = 0;
	card.exp_year = 0;
	card.PVKI = 0;
	card.PVV = 0;
	card.service_code = 0;
	return;
}