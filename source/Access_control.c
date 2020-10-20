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

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//		CONSTANT AND MACRO DEFINITIONS USING #DEFINE 			//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#define TIMEOUT_TIMER_TICKS		TIMER_MS2TICKS(TIMEOUT_TIMER_MS)

// Phrases
#define ACCESS_REQUEST_PH   "    Access Request    "
#define ADMIN_PH            "Admin    "
#define ID_NO_EXISTS_PH     "Id NO EXISTS    "
#define ENTER_PIN_PH        "Enter PIN    "
#define ACCESS_GRANTED_PH	"Access granted    "
#define INCORRECT_PIN_PH	"Incorrect PIN    "
#define ID_BAN_PH			"Id Ban    "
#define BRIGHTNESS_PH		"Brightness    "
#define ADD_ID_PH			"Add Id    "
#define ALREADY_EXISTS_PH	"Already Exists    "
#define ID_ADDED_PH			"Id Added    "
#define DELETE_ID_PH		"Delete Id    "
#define CONFIRM_PH			"Confirm ?    "
#define ID_DELETED_PH		"Id deleted    "
#define MODIFY_ID_PH		"Modify Id    "
#define ID_MODIFIED_PH		"Id modified    "


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			ENUMERATIONS AND STRUCTURES AND TYPEDEFS	  		//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

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

typedef struct{
    uint16_t current_ID_index; 	// index ID actual 0,1,2,3 
    uint16_t total_of_IDs; 		//Cantidad de ids en la lista de IDS 
    							//Manejo de introduccion de palabra
    uint8_t word_introduced[8]; //Palabra de 4,5 o 8 digitos
    uint8_t index; 
    uint8_t digits_introduced; 	//Cantidad de numeros introducidos 
    word_option_t current_option;
    ID_data_t IDsList[MAX_IDS];
	tim_id_t timer;
}access_control_t;

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
    ST_ID_MODIFICATION

};

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//						STATIC VARIABLES						//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

static access_control_t access_control;

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

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//					FUNCTION DEFINITIONS						//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void access_control_init(){
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
	char message[]= ADMIN_PH;
	display_set_string(message);
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
    STATE_MAP_ENTRY(ST_Confirmation2)

    //NARANJA
    STATE_MAP_ENTRY(ST_EliminateID)
    STATE_MAP_ENTRY(ST_Confirmation1)
    STATE_MAP_ENTRY(ST_IDElimination)

    //ROJO
    STATE_MAP_ENTRY(ST_ModifyID)
    STATE_MAP_ENTRY(ST_Confirmation2)
    STATE_MAP_ENTRY(ST_IDModification)

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
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CONFIRMATION_2,

        //NARANJA   
        TRANSITION_MAP_ENTRY(ST_CHANGE_BRIGHTNESS)              //ST_ELIMINATE_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CONFIRMATION_1,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_ELIMINATION

        //ROJO
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_MODIFY_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CONFIRMATION_2,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_MODIFICATION

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
        TRANSITION_MAP_ENTRY(ST_ELIMINATE_ID)                     //ST_CHANGE_BRIGHTNESS,
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
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_MODIFY_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CONFIRMATION_2,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_MODIFICATION

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
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                 //ST_ENTER_DIGIT_DISPLAY,		//ESTE HAY Q VERLO BIEN
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHANGE_DIGIT_DISPLAY_A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CHANGE_DIGIT_DISPLAY_B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_NEXT_DIGIT,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_PREVIOUS_DIGIT,
        //VERDE                 
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                 //ST_CHANGE_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_SET_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_LOWER_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_HIGHER_BRIGHTNESS
        //VIOLETA                   
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                 //ST_ADD_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                    	//ST_ALREADY_EXISTS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                 	//ST_ID_ADDITION,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_RECOUNT_NEW_ID_PIN
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CONFIRMATION_2,

        //NARANJA                   
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                 //ST_ELIMINATE_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                   	//ST_CONFIRMATION_1,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                   	//ST_ID_ELIMINATION

        //ROJO
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                 //ST_MODIFY_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CONFIRMATION_2,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_MODIFICATION

    END_TRANSITION_MAP(AccessControl, pEventData)
}

//Lector de tarjetas external event
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
        
    END_TRANSITION_MAP(AccessControl, pEventData)   
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			     STATE MACHINE FUNCTION DEFINITIONS    	        //
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

STATE_DEFINE(Admin, NoEventData)
{
    char message[]= ADMIN_PH;
	display_set_string(message);
    access_control.current_option = ADMIN_PIN;

}

STATE_DEFINE(AccessRequest, NoEventData)
{
    access_control.current_option = ID;
    char message[]= ACCESS_REQUEST_PH;
	display_set_string(message);
	start_timeout();
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
					access_control.IDsList[index].PIN_attempts=0;
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
                access_control.IDsList[index].PIN_attempts=0;
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
    display_set_string(ID_NO_EXISTS_PH);
}

STATE_DEFINE(PinRequest, NoEventData)
{
    display_set_string(ENTER_PIN_PH);
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
	return;
    //SM_InternalEvent(ST_ENTER_DIGITS_REQUEST, NULL);
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
    display_set_string(ACCESS_GRANTED_PH);
	// TODO
	//Muestro ACCESS GRANTED
	//Prendo LED 
	//Espero 5 seg
	//Apago LED
	//SM_InternalEvent(ST_ACCESS_REQUEST, NULL);
}

STATE_DEFINE(InvalidPin, NoEventData)
{
    //Muestro INCORRECT PIN
    
	display_set_string(INCORRECT_PIN_PH);

    timerDelay(5000);	// Esto es bloqueante, OJO

	switch (access_control.current_option){
	case ADMIN_PIN:
		//Si se introduce mal el Pin del administrador se vuelve al menu del ADMIN
		SM_InternalEvent(ST_ADMIN, NULL); 
		break;
	case PIN4: case PIN5: default:
		access_control.IDsList[access_control.current_ID_index].PIN_attempts++;
		if((access_control.IDsList[access_control.current_ID_index].PIN_attempts) == MAX_NUM_ATTEMPTS)
			SM_InternalEvent(ST_BLOCK_ID, NULL);
		else
			SM_InternalEvent(ST_PIN_REQUEST, NULL); 
		break;
	}
}

STATE_DEFINE(BlockId, NoEventData)
{
    //Muestro ID BLOCKED
	display_set_string(ID_BAN_PH);
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
    for(count = 0; count < 9; count ++)
        access_control.word_introduced[count] = 0;    

    display_clear_buffer();
    SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY, NULL); 
}

STATE_DEFINE(EnterDigitDisplay, NoEventData)
{
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
	display_set_string(BRIGHTNESS_PH);
}

STATE_DEFINE(SetBrightness, NoEventData)
{
    // TODO incorporar el fascinante driver de ftm para que nadie nunca regule el brillo
    display_set_number(display_get_brightness());
}

STATE_DEFINE(LowerBrightness, NoEventData)
{
    display_set_brightness_level(display_get_brightness()-1);
    SM_InternalEvent(ST_SET_BRIGHTNESS, NULL); 
}

STATE_DEFINE(HigherBrightness, NoEventData)
{
    display_set_brightness_level(display_get_brightness()+1);
    SM_InternalEvent(ST_SET_BRIGHTNESS, NULL); 
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			    			VIOLETA				    	        //
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

STATE_DEFINE(AddID, NoEventData)
{
	display_set_string(ADD_ID_PH);
    access_control.current_option = NEW_ID;

    //Si no hay mas lugar para agregar IDs genero un evento interno para que nunca llegue este menu
    if(access_control.total_of_IDs == MAX_IDS)
        SM_InternalEvent(ST_CHANGE_BRIGHTNESS, NULL); 

}

STATE_DEFINE(AlreadyExists, NoEventData)
{
    display_set_string(ALREADY_EXISTS_PH);
}

STATE_DEFINE(IDAddition, NoEventData)
{
    display_set_string(ID_ADDED_PH);
	
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

}

STATE_DEFINE(Confirmation0, NoEventData)
{
    display_set_string(CONFIRM_PH);
}
 

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			    			 NARANJA    				        //
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

STATE_DEFINE(EliminateID, NoEventData)
{
	display_set_string(DELETE_ID_PH);
    access_control.current_option = DELETE_ID;
}

STATE_DEFINE(Confirmation1, NoEventData)
{
    display_set_string(CONFIRM_PH);
     
}

STATE_DEFINE(IDElimination, NoEventData)
{
    access_control.IDsList[access_control.current_ID_index].valid = false;
	display_set_string(ID_DELETED_PH);
}



//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			    			 NARANJA    				        //
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

STATE_DEFINE(ModifyID, NoEventData)
{
	display_set_string(MODIFY_ID_PH);
    access_control.current_option = MODIFY_ID;
}

STATE_DEFINE(Confirmation2, NoEventData)
{
    display_set_string(CONFIRM_PH);
     
}

STATE_DEFINE(IDModification, NoEventData)
{
    display_set_string(ID_DELETED_PH);

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
	//Falta raisear un timeout event a la maquina de estados
	//SM_Event(ACC, Encoder_Click, NULL);
	return;
}
