//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//	@file		Access_control.h							    //
//	@brief		Acces Control application						//
//	@author		Grupo	4										//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//							Headers								//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#include "Access_control.h"
#include "StateMachine/State_machine.h"
#include <stdio.h>
#include "./Drivers/Display_7/Display_7.h"
#include "./Drivers/Encoder/encoder.h"
#include "./Drivers/Card_reader/Card_reader.h"
#include "./board.h"

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//		CONSTANT AND MACRO DEFINITIONS USING #DEFINE 			//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#define TIMEOUT_TIMER_TICKS			TIMER_MS2TICKS(TIMEOUT_TIMER_MS)
#define TIMEOUT_OPEN_DOOR_TICKS 	TIMER_MS2TICKS(TIMEOUT_OPEN_DOOR_MS)
#define TIMEOUT_INCORRECT_PIN_MS	5000
#define TIMEOUT_INCORRECT_PIN_TICKS	TIMER_MS2TICKS(TIMEOUT_INCORRECT_PIN_MS)
#define MAX_WORD_INTRODUCED     	8
#define MAX_LANGUAGES 4


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			ENUMERATIONS AND STRUCTURES AND TYPEDEFS	  		//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
typedef enum{
    ACCESS_REQUEST_PH,  
    ADMIN_PH,           
    ID_NO_EXISTS_PH,    
    ENTER_PIN_PH,       
    ACCESS_GRANTED_PH,	
    INCORRECT_PIN_PH,	
    ID_BAN_PH,			
    BRIGHTNESS_PH,		
    ADD_ID_PH,			
    ALREADY_EXISTS_PH,	
    ID_ADDED_PH,			
    DELETE_ID_PH,		
    CONFIRM_PH,			
    ID_DELETED_PH,		
    MODIFY_ID_PH,		
    ID_MODIFIED_PH,		
    PIN_MODIFIED_PH    
}disp_strings_t;

typedef enum{ 
	NEW_ID,
	ID,
    DELETE_ID,
    MODIFY_ID,
	PIN4,
    PIN5,
    ADMIN_PIN,
    NEW_ID_PIN,
    MODIFY_PIN
}word_option_t;

typedef struct{
    uint64_t card_id;
    uint32_t number; 			//Numero de ID (8 digitos)
    uint32_t PIN; 				//Contraseña del ID (4 o 5 digitos)
    uint8_t PIN_length;
    bool blocked_status; 		//Si el ID esta bloquedo es TRUE
    uint8_t PIN_attempts;
    bool valid;
}ID_data_t;

typedef enum {ES, EN, PT, FR} language_t;

typedef struct{
    uint16_t current_ID_index; 	// index ID actual 0,1,2,3 
    uint16_t total_of_IDs; 		//Cantidad de ids en la lista de IDS 
    							//Manejo de introduccion de palabra
    uint8_t word_introduced[MAX_WORD_INTRODUCED]; //Palabra de 4,5 o 8 digitos
    uint8_t index; 
    uint8_t digits_introduced; 	//Cantidad de numeros introducidos 
    word_option_t current_option;
    ID_data_t IDsList[MAX_IDS];
	tim_id_t timer;
    language_t language;
}access_control_t;

typedef struct{
	uint8_t pin_led_blue	:1;
	uint8_t pin_led_red		:1;
	uint8_t pin_led_green	:1;
	uint8_t pin_led_status_1:1;
	uint8_t pin_led_status_2:1;
}led_status_t;

typedef enum {DOOR_LOCKED, DOOR_OPEN, DOOR_ADMIN, DOOR_ERROR} door_modes_t;

enum States
{   
    ST_ADMIN,
    //AZUL
    ST_ACCESS_REQUEST,
    ST_ID_ENTERING_BY_CARD,
    ST_ID_ENTERING_BY_ENCODER,  
    ST_CHECK_ID_ENTERING_BY_CARD,
    ST_CHECK_ID_ENTERING_BY_ENCODER,
    ST_ID_NON_EXISTENT,
    ST_PIN_REQUEST,
    ST_CHECK_PIN,
    ST_ACCESS_GRANTED,
    ST_INVALID_PIN,
    ST_BLOCK_ID,
    
    //INGRESO DE PALABRA
    ST_ENTER_DIGITS_REQUEST,
    ST_ENTER_DIGIT_DISPLAY,
    ST_CHANGE_DIGIT_DISPLAY_A,
    ST_CHANGE_DIGIT_DISPLAY_B,
    ST_NEXT_DIGIT,
    ST_PREVIOUS_DIGIT,
 
    //VERDE
    ST_CHANGE_BRIGHTNESS,
    ST_SET_BRIGHTNESS,
    ST_LOWER_BRIGHTNESS,
    ST_HIGHER_BRIGHTNESS,

    //VIOLETA
    ST_ADD_ID,
    ST_ALREADY_EXISTS,
    ST_ID_ADDITION,
    ST_RECOUNT_NEW_ID_PIN,
    ST_CONFIRMATION_0,

    //NARANJA
    ST_ELIMINATE_ID,
    ST_CONFIRMATION_1,
    ST_ID_ELIMINATION,

    //ROJO
    ST_MODIFY_ID,
    ST_CONFIRMATION_2,
    ST_ID_MODIFICATION,

    //AMARILLO
    ST_CHANGE_LANGUAGE,
    ST_SET_LANGUAGE,
    ST_PREVIOUS_LANGUAGE,
    ST_NEXT_LANGUAGE

};

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//						STATIC VARIABLES						//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

static access_control_t access_control;

static tim_id_t door_timer;

static tim_id_t led_refresh_timer;

static tim_id_t led_off_timer;

static ttick_t led_off_timer_ticks;

static led_status_t leds;

static char * EN_strings[17]={
    "    Access Request     ",         // ACCESS_REQUEST_PH  
    "Admin    ",                       // ADMIN_PH           
    "Id NO EXISTS    ",                // ID_NO_EXISTS_PH  
    "Enter PIN    ",                   // ENTER_PIN_PH       
    "Access granted     ",             // ACCESS_GRANTED_PH	
    "INC",                             // INCORRECT_PIN_PH
    "Id Ban    ",                      // ID_BAN_PH			
    "Brightness    ",                  // BRIGHTNESS_PH		
    "Add Id    ",                      // ADD_ID_PH			
    "Already Exists    ",              // ALREADY_EXISTS_PH
    "Id Added    ",                    // ID_ADDED_PH		
    "Delete Id    ",                   // DELETE_ID_PH	
    "Confirm ?    ",                   // CONFIRM_PH		
    "Id deleted    ",                  // ID_DELETED_PH		
    "Modify Id    ",                   // MODIFY_ID_PH	
    "Id modified    ",                 // ID_MODIFIED_PH	
    "Pin Modified    "                 // PIN_MODIFIED_PH   
};
 
static char * ES_strings[17]={
    "    Pedido de acceso     ",         // ACCESS_REQUEST_PH  
    "Admin    ",                       // ADMIN_PH           
    "Id NO EXISTE    ",                // ID_NO_EXISTS_PH  
    "Ingresar PIN    ",                   // ENTER_PIN_PH       
    "Acceso correcto     ",             // ACCESS_GRANTED_PH	
    "INC",                             // INCORRECT_PIN_PH
    "Id Ban    ",                      // ID_BAN_PH			
    "Brillo    ",                  // BRIGHTNESS_PH		
    "Agregar Id    ",                      // ADD_ID_PH			
    "Ya existe    ",              // ALREADY_EXISTS_PH
    "Id agregado    ",                    // ID_ADDED_PH		
    "Borrar Id    ",                   // DELETE_ID_PH	
    "Confirmar ?    ",                   // CONFIRM_PH		
    "Id borrado    ",                  // ID_DELETED_PH		
    "Modificar Id    ",                   // MODIFY_ID_PH	
    "Id modificado    ",                 // ID_MODIFIED_PH	
    "Pin Modificado    "                 // PIN_MODIFIED_PH   
};
static char * PT_strings[17]={
    "    Pedido de acesso     ",         // ACCESS_REQUEST_PH  
    "Admin    ",                       // ADMIN_PH           
    "Id NAO EXISTE    ",                // ID_NO_EXISTS_PH  
    "Digite o PIN    ",                   // ENTER_PIN_PH       
    "Acesso correcto     ",             // ACCESS_GRANTED_PH	
    "INC",                             // INCORRECT_PIN_PH
    "Id Ban    ",                      // ID_BAN_PH			
    "Brilho    ",                  // BRIGHTNESS_PH		
    "Adicionar Id    ",                      // ADD_ID_PH			
    "ja existe    ",              // ALREADY_EXISTS_PH
    "Id adicionado    ",                    // ID_ADDED_PH		
    "deletar Id    ",                   // DELETE_ID_PH	
    "Confirmar ?    ",                   // CONFIRM_PH		
    "Id deletado    ",                  // ID_DELETED_PH		
    "Modificar Id    ",                   // MODIFY_ID_PH	
    "Id modificado    ",                 // ID_MODIFIED_PH	
    "Pin Modificado    "                 // PIN_MODIFIED_PH   
};

static char * FR_strings[17]={
    "    Demande d acess     ",         // ACCESS_REQUEST_PH  
    "Admin    ",                       // ADMIN_PH           
    "Id n existe pas    ",                // ID_NO_EXISTS_PH  
    "entrer le code PIN    ",                   // ENTER_PIN_PH       
    "acces autorise     ",             // ACCESS_GRANTED_PH	
    "INC",                             // INCORRECT_PIN_PH
    "Id Ban    ",                      // ID_BAN_PH			
    "luminosite    ",                  // BRIGHTNESS_PH		
    "ajouter l Id    ",                      // ADD_ID_PH			
    "existe deja    ",              // ALREADY_EXISTS_PH
    "Id ajoute    ",                    // ID_ADDED_PH		
    "supprimer Id    ",                   // DELETE_ID_PH	
    "Confirmer ?    ",                   // CONFIRM_PH		
    "Id supprime    ",                  // ID_DELETED_PH		
    "modifier l Id    ",                   // MODIFY_ID_PH	
    "Id modifie    ",                 // ID_MODIFIED_PH	
    "Pin Modifie    "                 // PIN_MODIFIED_PH   
};

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS W FILE LEVEL SCOPE//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

static uint64_t array_to_int(uint8_t* array, uint8_t length);
/*****************************************************************
 * @brief: It converts the digits from an array into an uint64_t 
 * 			number
 * @param array: The array containing the number.
 * @param length: Lenght of the array sent so that the array goes from
 * 					array[0] to array[lenght-1].
 * @example: If the input array is array = [1,2,3,4,5], then
 * 				lenght = 5
 * 				answer = (uint64_t) 12345
 * **************************************************************/

static void hide_digit(uint8_t digit);
/*****************************************************************
 * @brief: It hides a digit from the display in the index = digit
 * @param digit: index of the display to be hidden.
 * **************************************************************/

static void start_timeout(void);
/*****************************************************************
 * @brief: Function that restart the timeout from the previous action.
 * **************************************************************/

static void timeout_callback(void);
/*****************************************************************
 * @brief: Timeout callback, it raises a TIMEOUT event that returns
 * 			the state machine to the initial state.
 * **************************************************************/

static void door_timeout_callback(void);
/*****************************************************************
 * @brief: timeout for the door callback.
 * **************************************************************/

static void set_door_led_mode(door_modes_t mode);
/*****************************************************************
 * @brief: set the door pins corresponding to an input mode
 * @param mode: Mode of the current door.
 * **************************************************************/

static void led_set_brightness(uint8_t brightness);
/*****************************************************************
 * @brief: Function to set the brightness of the leds.
 * @param brightness: Number from 0-100 representing the percentage
 * 						of the brightness.
 * **************************************************************/

static void led_refresh_callback(void);
/*****************************************************************
 * @brief: Function called every 10 ms to refresh the PWM of the leds
 * **************************************************************/

static void led_off_callback(void);
/*****************************************************************
 * @brief: Function called by the timer led_off_timer, that turns the
 * 			leds off controling the duty cycle of the leds.
 * **************************************************************/

char * translate(disp_strings_t string);
/*****************************************************************
 * @brief: return a string in the current language
 * @param string: Id of the string to be translated.
 * **************************************************************/

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//					FUNCTION DEFINITIONS						//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void access_control_init(void){
/*****************************************************************
 * @brief: Initialization function for the Access control application
 *          It initializes the internal variables and the State machine
 *          in the corresponding state.
 * **************************************************************/
    ID_data_t sample_id;
    sample_id.blocked_status=false;
    sample_id.valid=true;
    sample_id.number=0;
    sample_id.card_id=6391300355831573;
    sample_id.PIN=1234;
    sample_id.PIN_attempts=0;
    sample_id.PIN_length=4;
    //access_control.current_num=0;
    access_control.current_ID_index=0;
    access_control.IDsList[0]=sample_id;
    access_control.total_of_IDs=1;
    access_control.digits_introduced=0;
	timerInit();
	access_control.timer = timerGetId();
	led_refresh_timer = timerGetId();
	led_off_timer = timerGetId();
	door_timer = timerGetId();
	led_set_brightness(50);
	gpioMode(PIN_LED_GREEN, OUTPUT);
	gpioMode(PIN_LED_BLUE, OUTPUT);
	gpioMode(PIN_LED_RED, OUTPUT);
    gpioMode(PCB_LED_STATUS_1, OUTPUT);    
    gpioMode(PCB_LED_STATUS_2, OUTPUT);    
	set_door_led_mode(DOOR_ADMIN);
	display_set_string(translate(ADMIN_PH));
    access_control.current_option = ADMIN_PIN;
	return;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//					LOCAL FUNCTION DEFINITIONS					//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			     STATE MACHINE FUNCTION PROTOTYPES    	        //
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

/***********************************************************************************************************************/

STATE_DECLARE(Admin, NoEventData)

//AZUL
STATE_DECLARE(AccessRequest, NoEventData)
STATE_DECLARE(IdEnteringByCard, NoEventData)
STATE_DECLARE(IdEnteringByEncoder, NoEventData)
STATE_DECLARE(CheckIdEnteringByCard, NoEventData)
STATE_DECLARE(CheckIdEnteringByEncoder, NoEventData)
STATE_DECLARE(IdNonExistent, NoEventData)
STATE_DECLARE(PinRequest, NoEventData)
STATE_DECLARE(CheckPin, NoEventData)
STATE_DECLARE(AccessGranted, NoEventData)
STATE_DECLARE(InvalidPin, NoEventData)
STATE_DECLARE(BlockId, NoEventData)

//INGRESO DE PALABRA
STATE_DECLARE(EnterDigitsRequest, NoEventData)
STATE_DECLARE(EnterDigitDisplay, NoEventData)
STATE_DECLARE(ChangeDigitDisplayA, NoEventData)
STATE_DECLARE(ChangeDigitDisplayB, NoEventData)
STATE_DECLARE(NextDigit, NoEventData)
STATE_DECLARE(PreviousDigit, NoEventData)

//VERDE
STATE_DECLARE(ChangeBrightness, NoEventData)
STATE_DECLARE(SetBrightness, NoEventData)
STATE_DECLARE(LowerBrightness, NoEventData)
STATE_DECLARE(HigherBrightness, NoEventData)

//VIOLETA
STATE_DECLARE(AddID, NoEventData)
STATE_DECLARE(AlreadyExists, NoEventData)
STATE_DECLARE(IDAddition, NoEventData)
STATE_DECLARE(RecountNewIdPIN, NoEventData)
STATE_DECLARE(Confirmation0, NoEventData)

//NARANJA
STATE_DECLARE(EliminateID, NoEventData)
STATE_DECLARE(Confirmation1, NoEventData)
STATE_DECLARE(IDElimination, NoEventData)

//ROJO
STATE_DECLARE(ModifyID, NoEventData)
STATE_DECLARE(Confirmation2, NoEventData)
STATE_DECLARE(IDModification, NoEventData)

//AMARILLO
STATE_DECLARE(ChangeLanguage, NoEventData)
STATE_DECLARE(SetLanguage, NoEventData)
STATE_DECLARE(PreviuosLanguage, NoEventData)
STATE_DECLARE(NextLanguage, NoEventData)


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			                STATE MAP                   	    //
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

BEGIN_STATE_MAP(AccessControl)
    STATE_MAP_ENTRY(ST_Admin)
    //AZUL
    STATE_MAP_ENTRY(ST_AccessRequest)
    STATE_MAP_ENTRY(ST_IdEnteringByCard)
    STATE_MAP_ENTRY(ST_IdEnteringByEncoder)
    STATE_MAP_ENTRY(ST_CheckIdEnteringByCard)
    STATE_MAP_ENTRY(ST_CheckIdEnteringByEncoder)
    STATE_MAP_ENTRY(ST_IdNonExistent)
    STATE_MAP_ENTRY(ST_PinRequest)
    STATE_MAP_ENTRY(ST_CheckPin)
    STATE_MAP_ENTRY(ST_AccessGranted)
    STATE_MAP_ENTRY(ST_InvalidPin)
    STATE_MAP_ENTRY(ST_BlockId)

    //INGRESO DE PALABRA
    STATE_MAP_ENTRY(ST_EnterDigitsRequest)
    STATE_MAP_ENTRY(ST_EnterDigitDisplay)
    STATE_MAP_ENTRY(ST_ChangeDigitDisplayA)
    STATE_MAP_ENTRY(ST_ChangeDigitDisplayB)
    STATE_MAP_ENTRY(ST_NextDigit)
    STATE_MAP_ENTRY(ST_PreviousDigit)

    //VERDE
    STATE_MAP_ENTRY(ST_ChangeBrightness)
    STATE_MAP_ENTRY(ST_SetBrightness)
    STATE_MAP_ENTRY(ST_LowerBrightness)
    STATE_MAP_ENTRY(ST_HigherBrightness)
    
    //VIOLETA
    STATE_MAP_ENTRY(ST_AddID)
    STATE_MAP_ENTRY(ST_AlreadyExists)
    STATE_MAP_ENTRY(ST_IDAddition)
    STATE_MAP_ENTRY(ST_RecountNewIdPIN)
    STATE_MAP_ENTRY(ST_Confirmation0)

    //NARANJA
    STATE_MAP_ENTRY(ST_EliminateID)
    STATE_MAP_ENTRY(ST_Confirmation1)
    STATE_MAP_ENTRY(ST_IDElimination)

    //ROJO
    STATE_MAP_ENTRY(ST_ModifyID)
    STATE_MAP_ENTRY(ST_Confirmation2)
    STATE_MAP_ENTRY(ST_IDModification)

    //AMARILLO
    STATE_MAP_ENTRY(ST_ChangeLanguage)
    STATE_MAP_ENTRY(ST_SetLanguage)
    STATE_MAP_ENTRY(ST_PreviuosLanguage)
    STATE_MAP_ENTRY(ST_NextLanguage)


END_STATE_MAP(AccessControl)

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			        EXTERNAL EVENTS DEFINITIONS				    //
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

// Encoder click external event
EVENT_DEFINE(Encoder_Click, NoEventData)
{
    //Dado el evento de Encoder_Click, se transiciona a un nuevo estado basado en el estado actual 

    BEGIN_TRANSITION_MAP                                        // - Current State -
    
        TRANSITION_MAP_ENTRY(ST_PIN_REQUEST)                    //ST_ADMIN,
        //AZUL
        TRANSITION_MAP_ENTRY(ST_ID_ENTERING_BY_ENCODER)         //ST_ACCESS_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_ENTERING_BY_CARD,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_ENTERING_BY_ENCODER,  
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHECK_ID_ENTERING_BY_CARD,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHECK_ID_ENTERING_BY_ENCODER,
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                 //ST_ID_NON_EXISTENT,
        TRANSITION_MAP_ENTRY(ST_ENTER_DIGITS_REQUEST)           //ST_PIN_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHECK_PIN,
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                 //ST_ACCESS_GRANTED,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_INVALID_PIN,
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                 //ST_BLOCK_ID,
        
        //INGRESO DE PALABRA
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ENTER_DIGITS_REQUEST,
        TRANSITION_MAP_ENTRY(ST_NEXT_DIGIT)                     //ST_ENTER_DIGIT_DISPLAY,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHANGE_DIGIT_DISPLAY_A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHANGE_DIGIT_DISPLAY_B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_NEXT_DIGIT,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_PREVIOUS_DIGIT,
    
        //VERDE
        TRANSITION_MAP_ENTRY(ST_SET_BRIGHTNESS)                 //ST_CHANGE_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(ST_CHANGE_BRIGHTNESS)              //ST_SET_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_LOWER_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_HIGHER_BRIGHTNESS,

        //VIOLETA
        TRANSITION_MAP_ENTRY(ST_ID_ENTERING_BY_ENCODER)         //ST_ADD_ID,
        TRANSITION_MAP_ENTRY(ST_ADD_ID)                         //ST_ALREADY_EXISTS,
        TRANSITION_MAP_ENTRY(ST_ADD_ID)                         //ST_ID_ADDITION,
        TRANSITION_MAP_ENTRY(ST_ENTER_DIGIT_DISPLAY)            //ST_RECOUNT_NEW_ID_PIN
        TRANSITION_MAP_ENTRY(ST_ID_ADDITION)                     //ST_CONFIRMATION_0,

        //NARANJA
        TRANSITION_MAP_ENTRY(ST_ID_ENTERING_BY_ENCODER)         //ST_ELIMINATE_ID,
        TRANSITION_MAP_ENTRY(ST_ID_ELIMINATION)                 //ST_CONFIRMATION_1,
        TRANSITION_MAP_ENTRY(ST_ELIMINATE_ID)                   //ST_ID_ELIMINATION

        //ROJO
        TRANSITION_MAP_ENTRY(ST_ID_ENTERING_BY_ENCODER)         //ST_MODIFY_ID,
        TRANSITION_MAP_ENTRY(ST_ID_MODIFICATION)                //ST_CONFIRMATION_2,
        TRANSITION_MAP_ENTRY(ST_MODIFY_ID)                      //ST_ID_MODIFICATION

        //AMARILLO
        TRANSITION_MAP_ENTRY(ST_SET_LANGUAGE)                   //ST_CHANGE_LANGUAGE,
        TRANSITION_MAP_ENTRY(ST_CHANGE_LANGUAGE)                //ST_SET_LANGUAGE,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_PREVIOUS_LANGUAGE,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_NEXT_LANGUAGE


    END_TRANSITION_MAP(AccessControl, pEventData)
}

// Encoder doble click external event
EVENT_DEFINE(Encoder_Double_Click, NoEventData)
{
    //Dado el evento de Encoder_Click, se transiciona a un nuevo estado basado en el estado actual 

    BEGIN_TRANSITION_MAP                                        // - Current State -
      
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ADMIN,
        //AZUL                  
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ACCESS_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_ENTERING_BY_CARD,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_ENTERING_BY_ENCODER,  
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHECK_ID_ENTERING_BY_CARD,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHECK_ID_ENTERING_BY_ENCODER,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_NON_EXISTENT,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_PIN_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHECK_PIN,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ACCESS_GRANTED,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_INVALID_PIN,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_BLOCK_ID,
        
        //INGRESO DE PALABRA
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ENTER_DIGITS_REQUEST,
        TRANSITION_MAP_ENTRY(ST_PREVIOUS_DIGIT)                 //ST_ENTER_DIGIT_DISPLAY,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHANGE_DIGIT_DISPLAY_A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHANGE_DIGIT_DISPLAY_B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_NEXT_DIGIT,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_PREVIOUS_DIGIT,

        //VERDE                 
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHANGE_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_SET_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_LOWER_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_HIGHER_BRIGHTNESS,

        //VIOLETA
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ADD_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ALREADY_EXISTS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_ADDITION,
        TRANSITION_MAP_ENTRY(ST_CHECK_PIN)                      //ST_RECOUNT_NEW_ID_PIN
        TRANSITION_MAP_ENTRY(ST_ADD_ID)                         //ST_CONFIRMATION_2,

        //NARANJA
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ELIMINATE_ID,
        TRANSITION_MAP_ENTRY(ST_ELIMINATE_ID)                   //ST_CONFIRMATION_1,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_ELIMINATION

        //ROJO
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_MODIFY_ID,
        TRANSITION_MAP_ENTRY(ST_MODIFY_ID)                      //ST_CONFIRMATION_2,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_MODIFICATION

        //AMARILLO
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHANGE_LANGUAGE,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_SET_LANGUAGE,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_PREVIOUS_LANGUAGE,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_NEXT_LANGUAGE

    END_TRANSITION_MAP(AccessControl, pEventData)
}

// Encoder cw external event
EVENT_DEFINE(Encoder_CW, NoEventData)
{
    //Dado el evento de Encoder_Click, se transiciona a un nuevo estado basado en el estado actual 

    BEGIN_TRANSITION_MAP                                        // - Current State -

        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ADMIN,
        //AZUL  
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ACCESS_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_ENTERING_BY_CARD,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_ENTERING_BY_ENCODER,  
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHECK_ID_ENTERING_BY_CARD,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHECK_ID_ENTERING_BY_ENCODER,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_NON_EXISTENT,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_PIN_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHECK_PIN,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ACCESS_GRANTED,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_INVALID_PIN,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_BLOCK_ID,
    
        //INGRESO DE PALABRA    
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ENTER_DIGITS_REQUEST,
        TRANSITION_MAP_ENTRY(ST_CHANGE_DIGIT_DISPLAY_B)         //ST_ENTER_DIGIT_DISPLAY,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHANGE_DIGIT_DISPLAY_A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHANGE_DIGIT_DISPLAY_B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_NEXT_DIGIT,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_PREVIOUS_DIGIT,

        //VERDE 
        TRANSITION_MAP_ENTRY(ST_ADD_ID)                         //ST_CHANGE_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(ST_HIGHER_BRIGHTNESS)              //ST_SET_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_LOWER_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_HIGHER_BRIGHTNESS,

        //VIOLETA
        TRANSITION_MAP_ENTRY(ST_ELIMINATE_ID)                   //ST_ADD_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ALREADY_EXISTS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_ADDITION,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_RECOUNT_NEW_ID_PIN
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CONFIRMATION_0,

        //NARANJA   
        TRANSITION_MAP_ENTRY(ST_MODIFY_ID)                      //ST_ELIMINATE_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CONFIRMATION_1,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_ELIMINATION

        //ROJO
        TRANSITION_MAP_ENTRY(ST_CHANGE_LANGUAGE)                //ST_MODIFY_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CONFIRMATION_2,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_MODIFICATION

        //AMARILLO
        TRANSITION_MAP_ENTRY(ST_CHANGE_BRIGHTNESS)              //ST_CHANGE_LANGUAGE,
        TRANSITION_MAP_ENTRY(ST_NEXT_LANGUAGE)                  //ST_SET_LANGUAGE,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_PREVIOUS_LANGUAGE,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_NEXT_LANGUAGE

    END_TRANSITION_MAP(AccessControl, pEventData)
}

// Encoder ccw external event
EVENT_DEFINE(Encoder_CCW, NoEventData)
{
    //Dado el evento de Encoder_Click, se transiciona a un nuevo estado basado en el estado actual 

    BEGIN_TRANSITION_MAP                                        // - Current State -

        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ADMIN,

        //AZUL                  
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ACCESS_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_ENTERING_BY_CARD,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_ENTERING_BY_ENCODER,  
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHECK_ID_ENTERING_BY_CARD,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHECK_ID_ENTERING_BY_ENCODER,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_NON_EXISTENT,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_PIN_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHECK_PIN,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ACCESS_GRANTED,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_INVALID_PIN,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_BLOCK_ID,

        //INGRESO DE PALABRA    
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ENTER_DIGITS_REQUEST,
        TRANSITION_MAP_ENTRY(ST_CHANGE_DIGIT_DISPLAY_A)         //ST_ENTER_DIGIT_DISPLAY,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHANGE_DIGIT_DISPLAY_A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHANGE_DIGIT_DISPLAY_B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_NEXT_DIGIT,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_PREVIOUS_DIGIT,
        //VERDE                 
        TRANSITION_MAP_ENTRY(ST_CHANGE_LANGUAGE)                      //ST_CHANGE_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(ST_LOWER_BRIGHTNESS)               //ST_SET_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_LOWER_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_HIGHER_BRIGHTNESS
        //VIOLETA                   
        TRANSITION_MAP_ENTRY(ST_CHANGE_BRIGHTNESS)              //ST_ADD_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ALREADY_EXISTS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_ADDITION,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_RECOUNT_NEW_ID_PIN
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CONFIRMATION_2,

        //NARANJA               
        TRANSITION_MAP_ENTRY(ST_ADD_ID)                         //ST_ELIMINATE_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CONFIRMATION_1,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_ELIMINATION

        //ROJO
        TRANSITION_MAP_ENTRY(ST_ELIMINATE_ID)                   //ST_MODIFY_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CONFIRMATION_2,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_MODIFICATION

        //AMARILLO
        TRANSITION_MAP_ENTRY(ST_ELIMINATE_ID)                   //ST_CHANGE_LANGUAGE,
        TRANSITION_MAP_ENTRY(ST_PREVIOUS_LANGUAGE)              //ST_SET_LANGUAGE,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_PREVIOUS_LANGUAGE,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_NEXT_LANGUAGE

    END_TRANSITION_MAP(AccessControl, pEventData)
}

// Encoder long click external event
EVENT_DEFINE(Encoder_Long_Click, NoEventData)
{
    //Dado el evento de Encoder_Click, se transiciona a un nuevo estado basado en el estado actual 

    BEGIN_TRANSITION_MAP                                        // - Current State -

        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                 //ST_ADMIN,

        //AZUL                  
        TRANSITION_MAP_ENTRY(ST_ADMIN)                          //ST_ACCESS_REQUEST,
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                 //ST_ID_ENTERING_BY_CARD,
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                 //ST_ID_ENTERING_BY_ENCODER,  
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHECK_ID_ENTERING_BY_CARD,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHECK_ID_ENTERING_BY_ENCODER,
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                 //ST_ID_NON_EXISTENT,
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                 //ST_PIN_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHECK_PIN,
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                 //ST_ACCESS_GRANTED,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_INVALID_PIN,
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                 //ST_BLOCK_ID,

        //INGRESO DE PALABRA    
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ENTER_DIGITS_REQUEST,
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                 //ST_ENTER_DIGIT_DISPLAY,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHANGE_DIGIT_DISPLAY_A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHANGE_DIGIT_DISPLAY_B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_NEXT_DIGIT,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_PREVIOUS_DIGIT,
        //VERDE                 
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                 //ST_CHANGE_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                     //ST_SET_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_LOWER_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_HIGHER_BRIGHTNESS
        //VIOLETA                   
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                 //ST_ADD_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                    	//ST_ALREADY_EXISTS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                 	//ST_ID_ADDITION,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_RECOUNT_NEW_ID_PIN
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CONFIRMATION_0,

        //NARANJA                   
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                 //ST_ELIMINATE_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                   	//ST_CONFIRMATION_1,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                   	//ST_ID_ELIMINATION

        //ROJO
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                 //ST_MODIFY_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CONFIRMATION_2,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_MODIFICATION

        //AMARILLO
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                 //ST_CHANGE_LANGUAGE,
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                 //ST_SET_LANGUAGE,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_PREVIOUS_LANGUAGE,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_NEXT_LANGUAGE

    END_TRANSITION_MAP(AccessControl, pEventData)
}

// Lector de tarjetas external event
EVENT_DEFINE(Card_Reader, NoEventData)
{
    
    BEGIN_TRANSITION_MAP                                        // - Current State -
      
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ADMIN,

        //AZUL                  
        TRANSITION_MAP_ENTRY(ST_ID_ENTERING_BY_CARD)            //ST_ACCESS_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_ENTERING_BY_CARD,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_ENTERING_BY_ENCODER,  
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHECK_ID_ENTERING_BY_CARD,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHECK_ID_ENTERING_BY_ENCODER,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_NON_EXISTENT,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_PIN_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHECK_PIN,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ACCESS_GRANTED,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_INVALID_PIN,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_BLOCK_ID,

        //INGRESO DE PALABRA    
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ENTER_DIGITS_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ENTER_DIGIT_DISPLAY,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHANGE_DIGIT_DISPLAY_A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHANGE_DIGIT_DISPLAY_B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_NEXT_DIGIT,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_PREVIOUS_DIGIT,
        //VERDE                 
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHANGE_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_SET_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_LOWER_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_HIGHER_BRIGHTNESS
        //VIOLETA                   
        TRANSITION_MAP_ENTRY(ST_ID_ENTERING_BY_CARD)            //ST_ADD_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ALREADY_EXISTS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_ADDITION,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_RECOUNT_NEW_ID_PIN
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CONFIRMATION_2,

        //NARANJA                   
        TRANSITION_MAP_ENTRY(ST_ID_ENTERING_BY_CARD)            //ST_ELIMINATE_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CONFIRMATION_1,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_ELIMINATION

        //ROJO
        TRANSITION_MAP_ENTRY(ST_ID_ENTERING_BY_CARD)            //ST_MODIFY_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CONFIRMATION_2,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_MODIFICATION

        //AMARILLO
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHANGE_LANGUAGE,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_SET_LANGUAGE,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_PREVIOUS_LANGUAGE,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_NEXT_LANGUAGE

    END_TRANSITION_MAP(AccessControl, pEventData)   
}

// Timeout internal event
EVENT_DEFINE(Time_Out, NoEventData)
{
    
    BEGIN_TRANSITION_MAP                                        // - Current State -
      
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                     //ST_ADMIN,

        //AZUL                  
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         //ST_ACCESS_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_ENTERING_BY_CARD,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_ENTERING_BY_ENCODER,  
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHECK_ID_ENTERING_BY_CARD,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHECK_ID_ENTERING_BY_ENCODER,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_NON_EXISTENT,
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                     //ST_PIN_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHECK_PIN,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ACCESS_GRANTED,
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                     //ST_INVALID_PIN,
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                     //ST_BLOCK_ID,

        //INGRESO DE PALABRA    
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ENTER_DIGITS_REQUEST,
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                     //ST_ENTER_DIGIT_DISPLAY,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHANGE_DIGIT_DISPLAY_A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHANGE_DIGIT_DISPLAY_B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_NEXT_DIGIT,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_PREVIOUS_DIGIT,
        //VERDE                 
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                     //ST_CHANGE_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                     //ST_SET_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_LOWER_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_HIGHER_BRIGHTNESS
        //VIOLETA                   
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                     //ST_ADD_ID,
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                     //ST_ALREADY_EXISTS,
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                     //ST_ID_ADDITION,
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                     //ST_RECOUNT_NEW_ID_PIN
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                     //ST_CONFIRMATION_2,

        //NARANJA                   
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                     //ST_ELIMINATE_ID,
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                     //ST_CONFIRMATION_1,
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                     //ST_ID_ELIMINATION

        //ROJO
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                     //ST_MODIFY_ID,
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                     //ST_CONFIRMATION_2,
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                     //ST_ID_MODIFICATION

        //AMARILLO
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                     //ST_CHANGE_LANGUAGE,
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                     //ST_SET_LANGUAGE,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_PREVIOUS_LANGUAGE,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_NEXT_LANGUAGE

    END_TRANSITION_MAP(AccessControl, pEventData)   
}




//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			     STATE MACHINE FUNCTION DEFINITIONS    	        //
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

STATE_DEFINE(Admin, NoEventData)
{
	display_set_string(translate(ADMIN_PH));
	set_door_led_mode(DOOR_ADMIN);
    access_control.current_option = ADMIN_PIN;
    start_timeout();

}

STATE_DEFINE(AccessRequest, NoEventData)
{
    access_control.current_option = ID;
	display_set_string(translate(ACCESS_REQUEST_PH));
	start_timeout();
	set_door_led_mode(DOOR_LOCKED);
    timerStop(door_timer);
	return;
}

STATE_DEFINE(IdEnteringByCard, NoEventData)
{
    SM_InternalEvent(ST_CHECK_ID_ENTERING_BY_CARD, NULL);
}

STATE_DEFINE(IdEnteringByEncoder, NoEventData)
{
    SM_InternalEvent(ST_ENTER_DIGITS_REQUEST, NULL);
}

STATE_DEFINE(CheckIdEnteringByCard, NoEventData)
{
    //Primero se chequea si el numero de la tarjeta coincide con un ID
    // Get pointer to the instance data and update id
    card_t card_data = card_get_data();
    bool id_exists = false;
    
    uint16_t index;
    for(index=0; index<access_control.total_of_IDs; index++){
        if(access_control.IDsList[index].card_id == card_data.pan && access_control.IDsList[index].valid==true){
            id_exists = true;
            access_control.current_ID_index=index;
            break;
        }
	}

    //Dependiendo si se quiere eliminar el id, agregar o acceder se hace lo siguiente;
    switch (access_control.current_option){
		case DELETE_ID:
			if(id_exists) 
				SM_InternalEvent(ST_CONFIRMATION_1, NULL); 
			else 
				SM_InternalEvent(ST_ID_NON_EXISTENT, NULL);
			break;

		case NEW_ID:
			if(id_exists) 
				SM_InternalEvent(ST_ALREADY_EXISTS, NULL); 
			else{   
				//Se guarda el Id en el usuario nuevo
				access_control.IDsList[access_control.total_of_IDs].card_id=card_data.pan;
				SM_InternalEvent(ST_PIN_REQUEST, NULL);  
			}
			break;

		case MODIFY_ID:
			if(id_exists) 
				SM_InternalEvent(ST_PIN_REQUEST, NULL); 
			else{   
				SM_InternalEvent(ST_ID_NON_EXISTENT, NULL);
			}
			break;

		case ID: default:
			if(id_exists){
				if(access_control.IDsList[index].blocked_status) 
					SM_InternalEvent(ST_BLOCK_ID, NULL); 
				else 
				{
					//access_control.IDsList[index].PIN_attempts=0;
					SM_InternalEvent(ST_PIN_REQUEST, NULL); 
				}
			}
			else SM_InternalEvent(ST_ID_NON_EXISTENT, NULL);    
			break;
    }
}

STATE_DEFINE(CheckIdEnteringByEncoder, NoEventData)
{
    
    uint32_t entered_id = array_to_int(access_control.word_introduced,ID_LENGTH);
    bool id_exists=false;
    uint16_t index;

    for(index=0; index<access_control.total_of_IDs; index++)
        if(access_control.IDsList[index].number==entered_id && access_control.IDsList[index].valid==true){
            id_exists=true;
            access_control.current_ID_index=index;
            break;
        }

    //Dependiendo si se quiere eliminar el id, agregar o acceder se hace lo siguiente;
    switch (access_control.current_option)
    {
    case DELETE_ID:
        if(id_exists) SM_InternalEvent(ST_CONFIRMATION_1, NULL); 
        else SM_InternalEvent(ST_ID_NON_EXISTENT, NULL);
        break;
    
    case NEW_ID:
        if(id_exists) 
            SM_InternalEvent(ST_ALREADY_EXISTS, NULL); 
        else
        {   
            //Se guarda el Id en el usuario nuevo
            access_control.IDsList[access_control.total_of_IDs].number = entered_id;
            SM_InternalEvent(ST_PIN_REQUEST, NULL);  
        }
        break;

    case MODIFY_ID:
        if(id_exists) 
            SM_InternalEvent(ST_PIN_REQUEST, NULL); 
        else{   
            SM_InternalEvent(ST_ID_NON_EXISTENT, NULL);
        }
        break;

    case ID: default:
        if(id_exists)
        {
            if(access_control.IDsList[index].blocked_status) 
                SM_InternalEvent(ST_BLOCK_ID, NULL); 
            else {
                //access_control.IDsList[index].PIN_attempts=0;
                SM_InternalEvent(ST_PIN_REQUEST, NULL); 
            }
        }
        else 
			SM_InternalEvent(ST_ID_NON_EXISTENT, NULL);    
        break;
    }      
}

STATE_DEFINE(IdNonExistent, NoEventData)
{
    display_set_string(translate(ID_NO_EXISTS_PH));
}

STATE_DEFINE(PinRequest, NoEventData)
{
    start_timeout();
    display_set_string(translate(ENTER_PIN_PH));
    //veo el largo del pin segun el ID

    switch (access_control.current_option)
    {
        case ID:  default:
            switch(access_control.IDsList[access_control.current_ID_index].PIN_length){
                case 4 :
                    access_control.current_option = PIN4;
                    break;
                case 5 :
                    access_control.current_option = PIN5;
                    break;
                default :
                
                    break;
            }
			break;
        case NEW_ID:
            access_control.current_option = NEW_ID_PIN;
            break;

        case MODIFY_ID:
            access_control.current_option = MODIFY_PIN;
            break;

        case ADMIN_PIN:
            access_control.current_option = ADMIN_PIN;
            break;
     
    }   
	
    
}

STATE_DEFINE(CheckPin, NoEventData)
{
    //PIN es correcto segun el ID ?
    uint32_t pin_introduced;
    
    switch (access_control.current_option)
    {
    case ADMIN_PIN:
        pin_introduced = array_to_int(access_control.word_introduced,access_control.IDsList[access_control.current_ID_index].PIN_length);
        if(access_control.IDsList[0].PIN==pin_introduced) 
			SM_InternalEvent(ST_CHANGE_BRIGHTNESS, NULL); 
        else 
			SM_InternalEvent(ST_INVALID_PIN, NULL);
        break;
    
    case NEW_ID_PIN:
        SM_InternalEvent(ST_CONFIRMATION_0, NULL);
        break;

   case MODIFY_PIN: 
        SM_InternalEvent(ST_CONFIRMATION_2, NULL);
        break;

    case PIN4: case PIN5: default:
        pin_introduced = array_to_int(access_control.word_introduced,access_control.IDsList[access_control.current_ID_index].PIN_length);
        if(access_control.IDsList[access_control.current_ID_index].PIN==pin_introduced) 
			SM_InternalEvent(ST_ACCESS_GRANTED, NULL);
        else 
			SM_InternalEvent(ST_INVALID_PIN, NULL);
        break;
    }
}

STATE_DEFINE(AccessGranted, NoEventData)
{
    display_set_string(translate(ACCESS_GRANTED_PH));
	access_control.IDsList[access_control.current_ID_index].PIN_attempts=0;
	timerStart(door_timer,TIMEOUT_OPEN_DOOR_TICKS,TIM_MODE_SINGLESHOT,door_timeout_callback);
	set_door_led_mode(DOOR_OPEN);
}

STATE_DEFINE(InvalidPin, NoEventData)
{
	//Muestro INCORRECT PIN
	start_timeout();
	display_set_string(translate(INCORRECT_PIN_PH));
	switch (access_control.current_option){
	case ADMIN_PIN:
		//Si se introduce mal el Pin del administrador se vuelve al menu del ADMIN
		timerDelay(TIMEOUT_INCORRECT_PIN_TICKS);
		SM_InternalEvent(ST_ADMIN, NULL); 
		break;
	case PIN4: case PIN5: default:
		access_control.IDsList[access_control.current_ID_index].PIN_attempts++;
		if((access_control.IDsList[access_control.current_ID_index].PIN_attempts) == MAX_NUM_ATTEMPTS)
			SM_InternalEvent(ST_BLOCK_ID, NULL);
		else{
			timerDelay(TIMEOUT_INCORRECT_PIN_TICKS);
			SM_InternalEvent(ST_PIN_REQUEST, NULL);
		}
		break;
	}
	return;
}

STATE_DEFINE(BlockId, NoEventData)
{
    //Muestro ID BLOCKED
    start_timeout();
	display_set_string(translate(ID_BAN_PH));
    access_control.IDsList[access_control.current_ID_index].blocked_status=true;
    
    //PONER UN TIMEOUT
    //SM_InternalEvent(ST_ACCESS_REQUEST, NULL);  
}




//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			    	 Ingreso de palabras	 		   	        //
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

STATE_DEFINE(EnterDigitsRequest, NoEventData)
{
    access_control.index = 0;
    access_control.digits_introduced = 0;
    int count;
    for(count = 0; count < MAX_WORD_INTRODUCED; count ++)
        access_control.word_introduced[count] = 0;    

    display_clear_buffer();
    SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY, NULL); 
}

STATE_DEFINE(EnterDigitDisplay, NoEventData)
{
    start_timeout();
    display_enable_hard_highlight(access_control.index);
    display_set_single_number(access_control.word_introduced[access_control.index], access_control.index);
   
}

STATE_DEFINE(ChangeDigitDisplayA, NoEventData)
{
    if(access_control.word_introduced[access_control.index] == MIN_DIGIT_DISPLAY)
       access_control.word_introduced[access_control.index] = MAX_DIGIT_DISPLAY; 
    else
       access_control.word_introduced[access_control.index]--; 

    SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY, NULL); 
}

STATE_DEFINE(ChangeDigitDisplayB, NoEventData)
{
    if(access_control.word_introduced[access_control.index] == MAX_DIGIT_DISPLAY)
       access_control.word_introduced[access_control.index] = MIN_DIGIT_DISPLAY; 
    else
       access_control.word_introduced[access_control.index]++; 

    SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY, NULL); 
}

STATE_DEFINE(NextDigit, NoEventData)
{
    display_disable_highlight();
    access_control.digits_introduced++;
    switch (access_control.current_option){
		case ID: case NEW_ID: case  DELETE_ID: case MODIFY_ID:
			if(access_control.digits_introduced == ID_LENGTH) 
				SM_InternalEvent(ST_CHECK_ID_ENTERING_BY_ENCODER, NULL);
			else{
				access_control.index++;
				if(access_control.index-display_get_index()>3) 	
					display_rotate_right();
				SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY, NULL);
			}
			break;
		case PIN4:
			if(access_control.digits_introduced == 4) 
				SM_InternalEvent(ST_CHECK_PIN, NULL);
			else{
				hide_digit(access_control.index);
				access_control.index++;
				if(access_control.index-display_get_index()>3) 	
					display_rotate_right();
				SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY, NULL);
			}
			break;
		case  PIN5:      
			if(access_control.digits_introduced == 5) 
				SM_InternalEvent(ST_CHECK_PIN, NULL);
			else{
				hide_digit(access_control.index);
				access_control.index++;
				if(access_control.index-display_get_index()>3) 	
					display_rotate_right();
				SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY, NULL);
			}
			break;
		case ADMIN_PIN:
			if(access_control.digits_introduced == access_control.IDsList[0].PIN_length) 
				SM_InternalEvent(ST_CHECK_PIN, NULL);
			else{
				hide_digit(access_control.index);
				access_control.index++;
				if(access_control.index-display_get_index()>3) 	
					display_rotate_right();
				SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY, NULL);
			}
			break;
		case  NEW_ID_PIN:
			//Se da la opcion de que el PIN del nuevo ID sea de 4 o 5
			if(access_control.digits_introduced == 5) {
				access_control.IDsList[access_control.total_of_IDs].PIN_length = 5;
				SM_InternalEvent(ST_CHECK_PIN, NULL); 
			}
			if(access_control.digits_introduced == 4){
				access_control.IDsList[access_control.total_of_IDs].PIN_length = 4;
				hide_digit(access_control.index);
				access_control.index++;
				if(access_control.index-display_get_index()>3) 	
					display_rotate_right();
				SM_InternalEvent(ST_RECOUNT_NEW_ID_PIN, NULL); 
			}
			else if(access_control.digits_introduced < 4){
				hide_digit(access_control.index);
				access_control.index++;
				if(access_control.index-display_get_index()>3) 	
					display_rotate_right();
				SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY, NULL);
			}
			break;

		case  MODIFY_PIN:
			//Se da la opcion de que el PIN del nuevo ID sea de 4 o 5
			if(access_control.digits_introduced == 5) {
				SM_InternalEvent(ST_CHECK_PIN, NULL); 
			}
			if(access_control.digits_introduced == 4){
				hide_digit(access_control.index);
				access_control.index++;
				if(access_control.index-display_get_index()>3) 	
					display_rotate_right();
				SM_InternalEvent(ST_RECOUNT_NEW_ID_PIN, NULL); 
			}
			else if(access_control.digits_introduced < 4){
				hide_digit(access_control.index);
				access_control.index++;
				if(access_control.index-display_get_index()>3) 	
					display_rotate_right();
				SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY, NULL);
			}
			break;

		default:
			break;
    }
	return;
}

STATE_DEFINE(PreviousDigit, NoEventData)
{
	display_disable_highlight();
    if(access_control.index != 0){
		if(access_control.index-display_get_index()==0) 
			display_rotate_left();
		if(access_control.current_option == PIN5||access_control.current_option == PIN4)
			hide_digit(access_control.index);
        access_control.index--;
        if(access_control.digits_introduced != 0)
        	access_control.digits_introduced--;
	}
    SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY, NULL); 
}




//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			    			 VERDE   				 	        //
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
    
STATE_DEFINE(ChangeBrightness, NoEventData)
{
    start_timeout();
	display_set_string(translate(BRIGHTNESS_PH));
}

STATE_DEFINE(SetBrightness, NoEventData)
{
    start_timeout();
    display_set_number(display_get_brightness());
	led_set_brightness(display_get_brightness());
	return;
}

STATE_DEFINE(LowerBrightness, NoEventData)
{
    if((display_get_brightness()-10)>0)
        display_set_brightness_level(display_get_brightness()-10);
    SM_InternalEvent(ST_SET_BRIGHTNESS, NULL); 
}

STATE_DEFINE(HigherBrightness, NoEventData)
{
    if((display_get_brightness()+10)<100)
        display_set_brightness_level(display_get_brightness()+10);
    SM_InternalEvent(ST_SET_BRIGHTNESS, NULL); 
}




//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			    			VIOLETA				    	        //
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

STATE_DEFINE(AddID, NoEventData)
{
    start_timeout();
	display_set_string(translate(ADD_ID_PH));
    access_control.current_option = NEW_ID;

    //Si no hay mas lugar para agregar IDs genero un evento interno para que nunca llegue este menu
    if(access_control.total_of_IDs == MAX_IDS)
        SM_InternalEvent(ST_CHANGE_BRIGHTNESS, NULL); 

}

STATE_DEFINE(AlreadyExists, NoEventData)
{
    start_timeout();
    display_set_string(translate(ALREADY_EXISTS_PH));
}

STATE_DEFINE(IDAddition, NoEventData)
{
    start_timeout();
    display_set_string(translate(ID_ADDED_PH));
	
    uint8_t pin_length = access_control.digits_introduced;
    uint32_t pin_introduced = array_to_int(access_control.word_introduced,pin_length);
    
    //El Id ya se guardo en Check ID. Ahora se guarda el PIN y el resto de las variable del nuevo Id
    access_control.IDsList[access_control.total_of_IDs].PIN = pin_introduced;
    access_control.IDsList[access_control.total_of_IDs].blocked_status = false; 
    access_control.IDsList[access_control.total_of_IDs].valid = true;
    access_control.IDsList[access_control.total_of_IDs].PIN_attempts=0; 
    
    access_control.total_of_IDs++;

}

STATE_DEFINE(RecountNewIdPIN, NoEventData)
{
    start_timeout();
}

STATE_DEFINE(Confirmation0, NoEventData)
{
    start_timeout();
    display_set_string(translate(CONFIRM_PH));
}




//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			    			 NARANJA    				        //
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

STATE_DEFINE(EliminateID, NoEventData)
{
    start_timeout();
	display_set_string(translate(DELETE_ID_PH));
    access_control.current_option = DELETE_ID;
}

STATE_DEFINE(Confirmation1, NoEventData)
{
    start_timeout();
    display_set_string(translate(CONFIRM_PH));
     
}

STATE_DEFINE(IDElimination, NoEventData)
{
    start_timeout();
    access_control.IDsList[access_control.current_ID_index].valid = false;
	display_set_string(translate(ID_DELETED_PH));
}




//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			    			 NARANJA    				        //
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

STATE_DEFINE(ModifyID, NoEventData)
{
    start_timeout();
	display_set_string(translate(MODIFY_ID_PH));
    access_control.current_option = MODIFY_ID;
}

STATE_DEFINE(Confirmation2, NoEventData)
{
    start_timeout();
    display_set_string(translate(CONFIRM_PH));
     
}

STATE_DEFINE(IDModification, NoEventData)
{
    start_timeout();
    display_set_string(translate(PIN_MODIFIED_PH));

    uint8_t pin_length = access_control.digits_introduced;
    uint32_t pin_introduced = array_to_int(access_control.word_introduced,pin_length);

    //Se modifica el PIN
    access_control.IDsList[access_control.current_ID_index].PIN_length = pin_length;
    access_control.IDsList[access_control.current_ID_index].PIN = pin_introduced;
    access_control.IDsList[access_control.current_ID_index].blocked_status = false;
    //access_control.IDsList[access_control.current_ID_index].PIN_attempts = 0;
	
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			    			AMARILLO   				 	        //
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
    
STATE_DEFINE(ChangeLanguage, NoEventData)
{
    start_timeout();
	display_set_string(LANGUAGE_PH);
}

STATE_DEFINE(SetLanguage, NoEventData)
{
    start_timeout();
    //muesto el language
    display_set_string("mati puto");
}

STATE_DEFINE(PreviuosLanguage, NoEventData)
{
    if(access_control.language == ES)
        access_control.language = MAX_LANGUAGES;
    else
        access_control.language--;

    SM_InternalEvent(ST_SET_LANGUAGE, NULL); 
}

STATE_DEFINE(NextLanguage, NoEventData)
{
    if(access_control.language == MAX_LANGUAGES)
        access_control.language = ES;
    else
        access_control.language++;
    SM_InternalEvent(ST_SET_LANGUAGE, NULL); 
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			     			FUNCIONES 				   	        //
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

uint64_t array_to_int(uint8_t* array, uint8_t length){
/*****************************************************************
 * @brief: It converts the digits from an array into an uint64_t 
 * 			number
 * @param array: The array containing the number.
 * @param length: Lenght of the array sent so that the array goes from
 * 					array[0] to array[lenght-1].
 * @example: If the input array is array = [1,2,3,4,5], then
 * 				lenght = 5
 * 				answer = (uint64_t) 12345
 * **************************************************************/
    uint8_t index;
    uint64_t return_value=0;
    for(index=0; index<length; index++){
        return_value*=10;
        return_value+=array[index]; 
    }
    return return_value;
}

static void hide_digit(uint8_t digit){
/*****************************************************************
 * @brief: It hides a digit from the display in the index = digit
 * @param digit: index of the display to be hidden.
 * **************************************************************/
    display_set_single_char('-', digit);
}

static void start_timeout(void){
/*****************************************************************
 * @brief: Function that restart the timeout from the previous action.
 * **************************************************************/
	timerStart(access_control.timer,TIMEOUT_TIMER_TICKS,TIM_MODE_SINGLESHOT,timeout_callback);
	return;
}

static void timeout_callback(void){
/*****************************************************************
 * @brief: Timeout callback, it raises a TIMEOUT event that returns
 * 			the state machine to the initial state.
 * **************************************************************/
	SM_Event(ACC, Time_Out, NULL);
	return;
}

static void door_timeout_callback(void){
/*****************************************************************
 * @brief: timeout for the door callback.
 * **************************************************************/
	set_door_led_mode(DOOR_LOCKED);
    SM_Event(ACC, Encoder_Click, NULL);
	return;
}

static void set_door_led_mode(door_modes_t mode){
/*****************************************************************
 * @brief: set the door pins corresponding to an input mode
 * @param mode: Mode of the current door.
 * **************************************************************/
    switch(mode){
        case DOOR_LOCKED:
			leds.pin_led_green = true;
			leds.pin_led_blue = true;
			leds.pin_led_red = false;
			leds.pin_led_status_1 = true;
			leds.pin_led_status_2 = false;
			break;
		case DOOR_ADMIN:
			leds.pin_led_green = true;
			leds.pin_led_blue = false;
			leds.pin_led_red = true;
			leds.pin_led_status_1 = false;
			leds.pin_led_status_2 = true;
			break;
		case DOOR_OPEN:
			leds.pin_led_green = false;
			leds.pin_led_blue = true;
			leds.pin_led_red = true;
			leds.pin_led_status_1 = true;
			leds.pin_led_status_2 = true;
			break;
		case DOOR_ERROR:
			leds.pin_led_green = true;
			leds.pin_led_blue = true;
			leds.pin_led_red = true;
			leds.pin_led_status_1 = false;
			leds.pin_led_status_2 = false;
			break;
		default:
			break;
    }
	return;
}


static void led_set_brightness(uint8_t brightness){
/*****************************************************************
 * @brief: Function to set the brightness of the leds.
 * @param brightness: Number from 0-100 representing the percentage
 * 						of the brightness.
 * **************************************************************/
	led_off_timer_ticks = 10-((100-brightness)/10);
	timerStart(led_refresh_timer,10,TIM_MODE_PERIODIC,led_refresh_callback);
	return;
}

static void led_refresh_callback(void){
/*****************************************************************
 * @brief: Function called every 10 ms to refresh the PWM of the leds
 * **************************************************************/
	if(leds.pin_led_blue == false)
		gpioWrite(PIN_LED_BLUE, LOW);
	
	if(leds.pin_led_green == false)
		gpioWrite(PIN_LED_GREEN, LOW);
	
	if(leds.pin_led_red == false)
		gpioWrite(PIN_LED_RED, LOW);

	if(leds.pin_led_status_1 == true)
		gpioWrite(PCB_LED_STATUS_1, HIGH);
	
	if(leds.pin_led_status_2 == true)
		gpioWrite(PCB_LED_STATUS_2, HIGH);
	timerStart(led_off_timer, led_off_timer_ticks, TIM_MODE_SINGLESHOT,led_off_callback);
	return;
}

static void led_off_callback(void){
/*****************************************************************
 * @brief: Function called by the timer led_off_timer, that turns the
 * 			leds off controling the duty cycle of the leds.
 * **************************************************************/
	gpioWrite(PIN_LED_BLUE, HIGH);
	gpioWrite(PIN_LED_GREEN, HIGH);
	gpioWrite(PIN_LED_RED, HIGH);
	gpioWrite(PCB_LED_STATUS_1, LOW);
	gpioWrite(PCB_LED_STATUS_2, LOW);
	return;
}
char * translate(disp_strings_t string){
    language_t lang=access_control.language;
    char * return_string;
    switch (lang){
        case PT:
            return_string=PT_strings[string];
            break;
        case FR:
            return_string=FR_strings[string];
            break;
        case ES:
            return_string=ES_strings[string];
            break;
        case EN: default:
            return_string=EN_strings[string];
            break;
    }
    return return_string;
}


    
