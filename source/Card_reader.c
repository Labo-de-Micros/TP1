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
//						GLOBAL VARIABLES						//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

uint8_t card_data_decompressed[CARD_CHARACTERS_LENGTH];
uint8_t pan[CARD_PAN_LENGHT];
uint8_t exp_year;
uint8_t exp_month;
uint8_t service_code[CARD_SERVICE_LENGHT];
uint8_t PVKI;
uint8_t PVV[CARD_PVV_LENGHT];
uint8_t CVV[CARD_CVV_LENGHT];


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//						STATIC VARIABLES						//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

static uint32_t card_data_compressed[CARD_DATA_LENGTH_32];
static uint8_t index;
static card_callback_t	call;
static card_states_t current_state;

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
	gpioIRQ (CARD_CLOCK, GPIO_IRQ_MODE_FALLING_EDGE , clock_callback);	//Asumo que el clock no tirara nada cuando no haya leido nada.
	gpioIRQ (CARD_ENABLE, GPIO_IRQ_MODE_BOTH_EDGES , enable_callback);
	call = callback;
	current_state = CARD_WAIT_FOR_START;
	index = 0;
	clear_buffer();
	return;
}

uint8_t * get_data(void){
/*****************************************************************
 * @brief: When the callback is called, one must obtain the data readed
 * 			by the driver, so this function returns the data.
 * 			(It must be called in the callback specified above).
 * @return: An array of lenght CARD_CHARACTERS_LENGTH containing the data
 * 			readed in the form of uint8_t.
 ****************************************************************/
//Por ahora asumo que siempre la targeta se lee en la direccion correcta.
	uint8_t start_index = search_for_start() + 5;
	uint8_t i = 0;
	uint8_t num = 0;
	for(i = 0; i < CARD_CHARACTERS_LENGTH && num != CARD_FS ; i++){
		num = get_char_num(get_current_char(start_index+i*5));
		if(num != CARD_SS && num != CARD_FS && num != CARD_ES)
			pan[i] = num;
	}
	num = get_char_num(get_current_char(start_index+i*5));
	i++;
	exp_year = num*10 + get_char_num(get_current_char(start_index+i*5));
	i++;
	num = get_char_num(get_current_char(start_index+i*5));
	i++;
	exp_month = num*10 + get_char_num(get_current_char(start_index+i*5));
	
	for(uint8_t u = 0; u < CARD_SERVICE_LENGHT; u++){
		i++;
		uint8_t num = get_char_num(get_current_char(start_index+i*5));
		service_code[i] = num;
	}
	i++;
	PVKI = get_char_num(get_current_char(start_index+i*5));
	for(uint8_t u = 0; u < CARD_PVV_LENGHT; u++){
		i++;
		uint8_t num = get_char_num(get_current_char(start_index+i*5));
		PVV[i] = num;
	}
	for(uint8_t u = 0; u < CARD_CVV_LENGHT; u++){
		i++;
		uint8_t num = get_char_num(get_current_char(start_index+i*5));
		CVV[i] = num;
	}
	i++;
	return card_data_decompressed;
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
	uint8_t data = gpioRead(CARD_DATA);	//reads the data pin from the card reader.
	switch(current_state){
		case CARD_WAIT_FOR_START:	// Estado inicial, si me llega un Falling de enable, esta leyendo targeta.
			if(ev == ENABLE_FALLING_EV){
				current_state = CARD_WAIT_DATA;
				index = 0;
				clear_buffer();
			}
			break;
		case CARD_WAIT_DATA:
			if(ev == CLK_EV){
				if(data == 0)
					card_data_compressed[index/32] |= CARD_32_MASK << (31-(index % 32));
				index++;
			}
			else if(ev == ENABLE_RISING_EV){
				call();
				get_current_char(15);
				current_state = CARD_WAIT_FOR_START;
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
	for(uint16_t i = 0; i < CARD_DATA_LENGTH_32; i++)
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
	uint8_t i = ind %32;
	uint8_t index_2 = ind/32;
	curr_char.data_0 = (card_data_compressed[index_2] & (CARD_32_MASK<<(31-i))) != 0;
	i++;
	curr_char.data_1 = (card_data_compressed[index_2] & (CARD_32_MASK<<(31-i))) != 0;
	i++;
	curr_char.data_2 = (card_data_compressed[index_2] & (CARD_32_MASK<<(31-i))) != 0;
	i++;
	curr_char.data_3 = (card_data_compressed[index_2] & (CARD_32_MASK<<(31-i))) != 0;
	i++;
	curr_char.data_p = (card_data_compressed[index_2] & (CARD_32_MASK<<(31-i))) != 0;
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
	uint8_t i = 0;
	for(uint8_t ind = 0 ; ind < CARD_DATA_LENGTH ; ind++){
		if(get_char_num(get_current_char(ind)) == CARD_SS && is_char_valid(get_current_char(ind))){
			i=ind;
			break;
		}
	}
	return i;
}
