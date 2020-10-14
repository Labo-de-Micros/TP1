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

#define CARD_DATA_LENGTH		256		//Bits totales en la targeta, midiendolo llegue a que eran 256
#define CARD_DATA_REGISTER		32		//Uso registros de 32 bits para guardar la data.
#define CARD_CHARACTERS_LENGTH	40		//Segun la consigna.
#define CARD_PAN_LENGHT			19		//Maxima cantidad de caracteres de PAN
#define	CARD_EXP_LENGHT			4		//Maxima cantidad de caracteres de expiration date (YYMM)
#define CARD_SERVICE_LENGHT		3		//Maxima cantidad de caracteres de Service Code
#define CARD_PVKI_LENGHT		1		//Maxima cantidad de caracteres de PVKI
#define CARD_PVV_LENGHT			4		//Maxima cantidad de caracteres de PVV
#define CARD_CVV_LENGHT			3		//Maxima cantidad de caracteres de CVV
#define CARD_DATA_REGISTERS_LENGTH	(CARD_DATA_LENGTH/CARD_DATA_REGISTER)	//Cantidad de registros de CARD_DATA_REGISTER bits necesarios para guardar la CARD_DATA_LENGTH
													//IMPORTANTE: es necesario que CARD_DATA_LENGTH sea multiplo de CARD_DATA_REGISTER. (No sirve otra cosa).
#if ((CARD_DATA_LENGTH%CARD_DATA_REGISTER) != 0)
#error CARD_DATA_LENGTH must be multiple of CARD_DATA_REGISTER! Cannot be otherwise! Please change CARD_DATA_LENGTH or CARD_DATA_REGISTER.
#endif

#if (CARD_PAN_LENGHT+CARD_EXP_LENGHT+CARD_EXP_LENGHT+CARD_SERVICE_LENGHT+CARD_PVKI_LENGHT+CARD_PVV_LENGHT+CARD_CVV_LENGHT) > CARD_CHARACTERS_LENGTH
#error The ammount of characters per card specified is not correct. Please check the defines:CARD_PAN_LENGHT, CARD_EXP_LENGHT, CARD_SERVICE_LENGHT, CARD_PVKI_LENGHT, CARD_PVV_LENGHT, CARD_CVV_LENGHT and CARD_CHARACTERS_LENGTH.
#endif

#define CARD_SS				11
#define	CARD_FS				13
#define	CARD_ES				15

#define CARD_32_MASK		0x1
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

typedef uint32_t UINT_REGISTER;

#if CARD_DATA_REGISTER != 32
#error Please correct the typedef for the data register UINT_REGISTER for the selected in CARD_DATA_REGISTER!
#endif

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//						STATIC VARIABLES						//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

static UINT_REGISTER card_data_compressed[CARD_DATA_REGISTERS_LENGTH];
static uint8_t index;
static card_states_t current_state;
static card_t card;
static bool card_read;

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
static uint8_t get_pan_number(void);
static uint8_t get_exp_date(uint8_t ind);
static uint8_t get_service_code(uint8_t ind);
static uint8_t get_pvki(uint8_t ind);
static uint8_t get_pvv(uint8_t ind);
static uint8_t get_cvv(uint8_t ind);
static UINT_REGISTER reverseBits(UINT_REGISTER n);

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//					FUNCTION DEFINITIONS						//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void card_init(void){
/*****************************************************************
 * @brief: Initializer the card reader driver and its components
 ****************************************************************/
	static bool yaInit = false;
	if(yaInit)
		return;
	gpioMode(CARD_DATA, INPUT);	// Inicializo los pines correspondientemente
	gpioMode(CARD_CLOCK, INPUT);
	gpioMode(CARD_ENABLE, INPUT);
	gpioIRQ (CARD_CLOCK, GPIO_IRQ_MODE_FALLING_EDGE , clock_callback);	//Inicializo las interrupciones
	gpioIRQ (CARD_ENABLE, GPIO_IRQ_MODE_BOTH_EDGES , enable_callback);
	current_state = CARD_WAIT_FOR_START;	//Seteo el estado de la maquina de estados como 'esperando targeta'.
	index = 0;
	card_read = false;
	clear_buffer();	// Limpio el buffer para que todo quede seteado en 0.
	yaInit = true;
	return;
}

card_t card_get_data(void){
/*****************************************************************
 * @brief: When the callback is called, one must obtain the data read
 * 			by the driver, so this function returns the card data.
 * @return: A struct card_t containing the information read in the card.
 ****************************************************************/
	clear_card();
	if(!card_read)
		return card;
	uint8_t ind = get_pan_number();
	ind = get_exp_date(ind);
	ind = get_service_code(ind);
	ind = get_pvki(ind);
	ind = get_pvv(ind);
	ind = get_cvv(ind);
	return card;
}

bool card_was_read(void){
	return card_read;
}

void card_data_read(void){
	clear_card();
	card_read = false;
	return;
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
				card_read = false;
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
				card_read = true;								// entonces llamo al callback, y seteo el estado como el estado inicial, por si se lee
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
 * The sequence of data checked must be stored in card_data_compressed.
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
	uint8_t count = 0;
	count += character.data_0;
	count += character.data_1;
	count += character.data_2;
	count += character.data_3;
	if(count % 2 == 0 && character.data_p == 1 )
		return true;
	else if(count % 2 != 0 && character.data_p == 0)
		return true;
	else
		return false;
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
	/*if(i+5 >= 30){//Significa que la tarjeta fue pasada en sentido contrario, debo swapear el array.
		UINT_REGISTER _temp_[CARD_DATA_REGISTERS_LENGTH];
		for(uint8_t i = 0; i<CARD_DATA_REGISTERS_LENGTH; i++){
			_temp_[i] = card_data_compressed[CARD_DATA_REGISTERS_LENGTH-i-1];
			_temp_[i] = reverseBits(_temp_[i]);
		}
		for(uint8_t i = 0; i<CARD_DATA_REGISTERS_LENGTH; i++){
			card_data_compressed[i] = _temp_[i];
		}
		for(uint8_t ind = 0 ; ind < CARD_DATA_LENGTH ; ind++){
			if(get_char_num(get_current_char(ind)) == CARD_SS && is_char_valid(get_current_char(ind))){
				i=ind;
				break;
			}
		}
	}*/
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
	card.everything_ok = false;
	return;
}

static uint8_t get_pan_number(void){
/*****************************************************************
 * @brief: Reads the pan number, and returns the index for the next
 * 			data to be taken.
 ****************************************************************/
	uint8_t start_index = search_for_start();
	uint8_t i = 0;
	uint8_t num = 0;
	bool error = false;
	for(i = 0; i < CARD_CHARACTERS_LENGTH && num != CARD_FS ; i++){
		card_char_t chara = get_current_char(start_index+i*5);
		num = get_char_num(chara);
		if(num != CARD_SS && num != CARD_FS && num != CARD_ES){
			card.pan = card.pan * 10 + num;
		}
		if(!is_char_valid(chara)){
			error = true;
		}
	}
	if(!error)
		card.everything_ok = true;
	return start_index+i*5;
}

static uint8_t get_exp_date(uint8_t ind){
/*****************************************************************
 * @brief: Reads the expiration date, and returns the index for the next
 * 			data to be taken.
 ****************************************************************/
	uint8_t num = 0;
	uint8_t i = 0;
	num = get_char_num(get_current_char(ind+i*5));
	i++;
	card.exp_year = num*10 + get_char_num(get_current_char(ind+i*5));
	i++;
	num = get_char_num(get_current_char(ind+i*5));
	i++;
	card.exp_month = num*10 + get_char_num(get_current_char(ind+i*5));
	i++;
	return ind+i*5;
}

static uint8_t get_service_code(uint8_t ind){
/*****************************************************************
 * @brief: Reads the service code, and returns the index for the next
 * 			data to be taken.
 ****************************************************************/
	uint8_t i = 0;
	for(i = 0; i < CARD_SERVICE_LENGHT; i++){
		uint8_t num = get_char_num(get_current_char(ind+i*5));
		card.service_code = card.service_code * 10 + num;
	}
	return ind+i*5;
}

static uint8_t get_pvki(uint8_t ind){
/*****************************************************************
 * @brief: Reads the PVKI, and returns the index for the next
 * 			data to be taken.
 ****************************************************************/
	card.PVKI = get_char_num(get_current_char(ind));
	return ind+5;
}

static uint8_t get_pvv(uint8_t ind){
/*****************************************************************
 * @brief: Reads the PVV, and returns the index for the next
 * 			data to be taken.
 ****************************************************************/
	uint8_t i = 0;
	for(i = 0; i < CARD_PVV_LENGHT; i++){
		uint8_t num = get_char_num(get_current_char(ind+i*5));
		card.PVV = card.PVV * 10 + num;
	}
	return ind+i*5;
}

static uint8_t get_cvv(uint8_t ind){
/*****************************************************************
 * @brief: Reads the CVV, and returns the index for the next
 * 			data to be taken.
 ****************************************************************/
	uint8_t i = 0;
	for(i = 0; i < CARD_CVV_LENGHT; i++){
		uint8_t num = get_char_num(get_current_char(ind+i*5));
		card.CVV = card.CVV*10 +  num;
	}
	return ind+i*5;
}

static UINT_REGISTER reverseBits(UINT_REGISTER n) {
/*****************************************************************
 * @brief: Given a sequence of bits it reverse it.
 * 			Ex:
 * 				Input: 011101
 * 				Output 101110
 ****************************************************************/
    for(int i = 0, j = CARD_DATA_REGISTER-1; i < j; i++, j--) {
        bool iSet = (bool)(n & (1 << i));
        bool jSet = (bool)(n & (1 << j));
        n &= ~(1 << j);
        n &= ~(1 << i);
        if(iSet) n |= (1 << j);
        if(jSet) n |= (1 << i);
    }
    return n;
}
