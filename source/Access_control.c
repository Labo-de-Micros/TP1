#include "Access_control.h"
#include "StateMachine/State_machine.h"
#include <stdio.h>

#include "./Drivers/Display_7/Display_7.h"
#include "./Drivers/Encoder/encoder.h"
#include "./Drivers/Card_reader/Card_reader.h"

uint64_t array_to_int(uint8_t* array, uint8_t length);
void error_msg();
void done_msg();
void hide_digit(uint8_t digit);


static access_control_t access_control;

enum States
{
    ST_WELCOME,
    //AZUL
    ST_ACCESS_REQUEST,
    ST_ID_ENTERING_BY_CARD,
    ST_CHECK_ID_ENTERING_BY_CARD,
    ST_ID_ENTERING_BY_ENCODER,
    ST_READ_ERROR,
    ST_ID_NON_EXISTENT,
    ST_PIN_REQUEST,
    ST_CHECK_PIN,
    ST_ACCESS_GRANTED,
    ST_INVALID_PIN,
    ST_BLOCK_ID,
    ST_CHECK_ID_ENTERING_BY_ENCODER,

    //INGRESO DE PALABRA
    ST_ENTER_DIGITS_REQUEST,
    ST_ENTER_DIGIT_DISPLAY_1,
    ST_CHANGE_DIGIT_DISPLAY_1A,
    ST_CHANGE_DIGIT_DISPLAY_1B,
    ST_DIGITS_RECOUNT_1,
    ST_ENTER_DIGIT_DISPLAY_2,
    ST_CHANGE_DIGIT_DISPLAY_2A,
    ST_CHANGE_DIGIT_DISPLAY_2B,
    ST_DIGITS_RECOUNT_2,
    ST_ENTER_DIGIT_DISPLAY_3,
    ST_CHANGE_DIGIT_DISPLAY_3A,
    ST_CHANGE_DIGIT_DISPLAY_3B,
    ST_DIGITS_RECOUNT_3,
    ST_ENTER_DIGIT_DISPLAY_4,
    ST_CHANGE_DIGIT_DISPLAY_4A,
    ST_CHANGE_DIGIT_DISPLAY_4B,
    ST_DIGITS_RECOUNT_4,

    //VERDE
    ST_CHANGE_BRIGHTNESS,
    ST_SET_BRIGHTNESS,
    ST_LOWER_BRIGHTNESS,
    ST_HIGHER_BRIGHTNESS,

    //VIOLETA
    ST_ADD_ID,
    ST_SET_NEW_ID,
    ST_CHECK_NEW_ID,

    //NARANJA
    ST_ELIMINATE_ID,
    ST_SHOW_ID,
    ST_PREVIOUS_ID,
    ST_NEXT_ID,
    ST_ID_ELIMINATION
};

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			     STATE MACHINE FUNCTION PROTOTYPES    	        //
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

STATE_DECLARE(Welcome, NoEventData)
//AZUL
STATE_DECLARE(AccessRequest, NoEventData)
STATE_DECLARE(IdEnteringByCard, NoEventData)
STATE_DECLARE(CheckIdEnteringByCard, NoEventData)
STATE_DECLARE(IdEnteringByEncoder, NoEventData)
STATE_DECLARE(ReadError, NoEventData)
STATE_DECLARE(IdNonExistent, NoEventData)
STATE_DECLARE(PinRequest, NoEventData)
STATE_DECLARE(CheckPin, NoEventData)
STATE_DECLARE(AccessGranted, NoEventData)
STATE_DECLARE(InvalidPin, NoEventData)
STATE_DECLARE(BlockId, NoEventData)
STATE_DECLARE(CheckIdEnteringByEncoder, NoEventData)
//INGRESO DE PALABRA
STATE_DECLARE(EnterDigitsRequest, NoEventData)
STATE_DECLARE(EnterDigitDisplay1, NoEventData)
STATE_DECLARE(ChangeDigitDisplay1A, NoEventData)
STATE_DECLARE(ChangeDigitDisplay1B, NoEventData)
STATE_DECLARE(DigitsRecount1, NoEventData)
STATE_DECLARE(EnterDigitDisplay2, NoEventData)
STATE_DECLARE(ChangeDigitDisplay2A, NoEventData)
STATE_DECLARE(ChangeDigitDisplay2B, NoEventData)
STATE_DECLARE(DigitsRecount2, NoEventData)
STATE_DECLARE(EnterDigitDisplay3, NoEventData)
STATE_DECLARE(ChangeDigitDisplay3A, NoEventData)
STATE_DECLARE(ChangeDigitDisplay3B, NoEventData)
STATE_DECLARE(DigitsRecount3, NoEventData)
STATE_DECLARE(EnterDigitDisplay4, NoEventData)
STATE_DECLARE(ChangeDigitDisplay4A, NoEventData)
STATE_DECLARE(ChangeDigitDisplay4B, NoEventData)
STATE_DECLARE(DigitsRecount4, NoEventData)
//VERDE
STATE_DECLARE(ChangeBrightness, NoEventData)
STATE_DECLARE(SetBrightness, NoEventData)
STATE_DECLARE(LowerBrightness, NoEventData)
STATE_DECLARE(HigherBrightness, NoEventData)
//VIOLETA
STATE_DECLARE(AddID, NoEventData)
STATE_DECLARE(SetNewID, NoEventData)
STATE_DECLARE(CheckNewID, NoEventData)
//NARANJA
STATE_DECLARE(EliminateID, NoEventData)
STATE_DECLARE(ShowID, NoEventData)
STATE_DECLARE(PreviousID, NoEventData)
STATE_DECLARE(NextID, NoEventData)
STATE_DECLARE(IDElimination, NoEventData)


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			                STATE MAP                   	    //
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

BEGIN_STATE_MAP(AccessControl)
    STATE_MAP_ENTRY(ST_Welcome)
    //AZUL
    STATE_MAP_ENTRY(ST_AccessRequest)
    STATE_MAP_ENTRY(ST_IdEnteringByCard)
    STATE_MAP_ENTRY(ST_CheckIdEnteringByCard)
    STATE_MAP_ENTRY(ST_IdEnteringByEncoder)
    STATE_MAP_ENTRY(ST_ReadError)
    STATE_MAP_ENTRY(ST_IdNonExistent)
    STATE_MAP_ENTRY(ST_PinRequest)
    STATE_MAP_ENTRY(ST_CheckPin)
    STATE_MAP_ENTRY(ST_AccessGranted)
    STATE_MAP_ENTRY(ST_InvalidPin)
    STATE_MAP_ENTRY(ST_BlockId)
    STATE_MAP_ENTRY(ST_CheckIdEnteringByEncoder)

    //INGRESO DE PALABRA
    STATE_MAP_ENTRY(ST_EnterDigitsRequest)
    STATE_MAP_ENTRY(ST_EnterDigitDisplay1)
    STATE_MAP_ENTRY(ST_ChangeDigitDisplay1A)
    STATE_MAP_ENTRY(ST_ChangeDigitDisplay1B)
    STATE_MAP_ENTRY(ST_DigitsRecount1)
    STATE_MAP_ENTRY(ST_EnterDigitDisplay2)
    STATE_MAP_ENTRY(ST_ChangeDigitDisplay2A)
    STATE_MAP_ENTRY(ST_ChangeDigitDisplay2B)
    STATE_MAP_ENTRY(ST_DigitsRecount2)
    STATE_MAP_ENTRY(ST_EnterDigitDisplay3)
    STATE_MAP_ENTRY(ST_ChangeDigitDisplay3A)
    STATE_MAP_ENTRY(ST_ChangeDigitDisplay3B)
    STATE_MAP_ENTRY(ST_DigitsRecount3)
    STATE_MAP_ENTRY(ST_EnterDigitDisplay4)
    STATE_MAP_ENTRY(ST_ChangeDigitDisplay4A)
    STATE_MAP_ENTRY(ST_ChangeDigitDisplay4B)
    STATE_MAP_ENTRY(ST_DigitsRecount4)
   
    //VERDE
    STATE_MAP_ENTRY(ST_ChangeBrightness)
    STATE_MAP_ENTRY(ST_SetBrightness)
    STATE_MAP_ENTRY(ST_LowerBrightness)
    STATE_MAP_ENTRY(ST_HigherBrightness)
    
    //VIOLETA
    STATE_MAP_ENTRY(ST_AddID)
    STATE_MAP_ENTRY(ST_SetNewID)
    STATE_MAP_ENTRY(ST_CheckNewID)

    //NARANJA
    STATE_MAP_ENTRY(ST_EliminateID)
    STATE_MAP_ENTRY(ST_ShowID)
    STATE_MAP_ENTRY(ST_PreviousID)
    STATE_MAP_ENTRY(ST_NextID)
    STATE_MAP_ENTRY(ST_IDElimination)

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

    BEGIN_TRANSITION_MAP                                            // - Current State -
      
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_WELCOME
        TRANSITION_MAP_ENTRY(ST_ID_ENTERING_BY_ENCODER)             // ST_ACCESS_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ID_ENTERING_BY_CARD,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHECK_ID_ENTERING_BY_CARD,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ID_ENTERING_BY_ENCODER,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_READ_ERROR,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ID_NON_EXISTENT,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_PIN_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHECK_PIN,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ACCESS_GRANTED,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_INVALID_PIN,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_BLOCK_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHECK_ID_ENTERING_BY_ENCODER           
        // //INGRESO DE PALABRA
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ENTER_DIGITS_REQUEST,
        TRANSITION_MAP_ENTRY(ST_DIGITS_RECOUNT_1)                   // ST_ENTER_DIGIT_DISPLAY_1,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_1A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_1B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_DIGITS_RECOUNT_1,
        TRANSITION_MAP_ENTRY(ST_DIGITS_RECOUNT_2)                   // ST_ENTER_DIGIT_DISPLAY_2,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_2A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_2B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_DIGITS_RECOUNT_2,
        TRANSITION_MAP_ENTRY(ST_DIGITS_RECOUNT_3)                   // ST_ENTER_DIGIT_DISPLAY_3,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_3A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_3B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_DIGITS_RECOUNT_3,
        TRANSITION_MAP_ENTRY(ST_DIGITS_RECOUNT_4)                   // ST_ENTER_DIGIT_DISPLAY_4,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_4A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_4B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_DIGITS_RECOUNT_4       
        // //VERDE
        TRANSITION_MAP_ENTRY(ST_SET_BRIGHTNESS)                     // ST_CHANGE_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(ST_WELCOME)                         // ST_SET_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_LOWER_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_HIGHER_BRIGHTNESS     
        // //VIOLETA
        TRANSITION_MAP_ENTRY(ST_SET_NEW_ID)                         // ST_ADD_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_SET_NEW_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHECK_NEW_ID      
        // //NARANJA
        TRANSITION_MAP_ENTRY(ST_SHOW_ID)                            // ST_ELIMINATE_ID,
        TRANSITION_MAP_ENTRY(ST_ID_ELIMINATION)                     // ST_SHOW_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_PREVIOUS_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_NEXT_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ID_ELIMINATION

    END_TRANSITION_MAP(AccessControl, pEventData)
}

// Encoder doble click external event
EVENT_DEFINE(Encoder_Double_Click, NoEventData)
{
    //Dado el evento de Encoder_Click, se transiciona a un nuevo estado basado en el estado actual 

    BEGIN_TRANSITION_MAP                                            // - Current State -
      
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_WELCOME
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ACCESS_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ID_ENTERING_BY_CARD,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHECK_ID_ENTERING_BY_CARD,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ID_ENTERING_BY_ENCODER,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_READ_ERROR,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ID_NON_EXISTENT,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_PIN_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHECK_PIN,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ACCESS_GRANTED,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_INVALID_PIN,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_BLOCK_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHECK_ID_ENTERING_BY_ENCODER           
        // //INGRESO DE PALABRA
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ENTER_DIGITS_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ENTER_DIGIT_DISPLAY_1,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_1A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_1B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_DIGITS_RECOUNT_1,
        TRANSITION_MAP_ENTRY(ST_ENTER_DIGIT_DISPLAY_1)              // ST_ENTER_DIGIT_DISPLAY_2,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_2A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_2B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_DIGITS_RECOUNT_2,
        TRANSITION_MAP_ENTRY(ST_ENTER_DIGIT_DISPLAY_2)              // ST_ENTER_DIGIT_DISPLAY_3,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_3A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_3B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_DIGITS_RECOUNT_3,
        TRANSITION_MAP_ENTRY(ST_ENTER_DIGIT_DISPLAY_3)              // ST_ENTER_DIGIT_DISPLAY_4,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_4A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_4B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_DIGITS_RECOUNT_4       
        // //VERDE
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_SET_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_LOWER_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_HIGHER_BRIGHTNESS     
        // //VIOLETA
        TRANSITION_MAP_ENTRY(ST_SET_NEW_ID)                         // ST_ADD_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_SET_NEW_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHECK_NEW_ID      
        // //NARANJA
        TRANSITION_MAP_ENTRY(ST_SHOW_ID)                            // ST_ELIMINATE_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_SHOW_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_PREVIOUS_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_NEXT_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ID_ELIMINATION

    END_TRANSITION_MAP(AccessControl, pEventData)
}

// Encoder cw external event
EVENT_DEFINE(Encoder_CW, NoEventData)
{
    //Dado el evento de Encoder_Click, se transiciona a un nuevo estado basado en el estado actual 

    BEGIN_TRANSITION_MAP                                            // - Current State -
      
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                         // ST_WELCOME
        TRANSITION_MAP_ENTRY(ST_CHANGE_BRIGHTNESS)                  // ST_ACCESS_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ID_ENTERING_BY_CARD,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHECK_ID_ENTERING_BY_CARD,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ID_ENTERING_BY_ENCODER,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_READ_ERROR,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ID_NON_EXISTENT,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_PIN_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHECK_PIN,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ACCESS_GRANTED,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_INVALID_PIN,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_BLOCK_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHECK_ID_ENTERING_BY_ENCODER           
        // //INGRESO DE PALABRA
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ENTER_DIGITS_REQUEST,
        TRANSITION_MAP_ENTRY(ST_CHANGE_DIGIT_DISPLAY_1B)            // ST_ENTER_DIGIT_DISPLAY_1,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_1A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_1B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_DIGITS_RECOUNT_1,
        TRANSITION_MAP_ENTRY(ST_CHANGE_DIGIT_DISPLAY_2B)            // ST_ENTER_DIGIT_DISPLAY_2,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_2A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_2B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_DIGITS_RECOUNT_2,
        TRANSITION_MAP_ENTRY(ST_CHANGE_DIGIT_DISPLAY_3B)            // ST_ENTER_DIGIT_DISPLAY_3,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_3A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_3B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_DIGITS_RECOUNT_3,
        TRANSITION_MAP_ENTRY(ST_CHANGE_DIGIT_DISPLAY_4B)                         // ST_ENTER_DIGIT_DISPLAY_4,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_4A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_4B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_DIGITS_RECOUNT_4       
        // //VERDE
        TRANSITION_MAP_ENTRY(ST_ADD_ID)                             // ST_CHANGE_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(ST_HIGHER_BRIGHTNESS)                         // ST_SET_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_LOWER_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_HIGHER_BRIGHTNESS     
        // //VIOLETA
        TRANSITION_MAP_ENTRY(ST_ELIMINATE_ID)                       // ST_ADD_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_SET_NEW_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHECK_NEW_ID      
        // //NARANJA
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                     // ST_ELIMINATE_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_SHOW_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_PREVIOUS_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_NEXT_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ID_ELIMINATION

    END_TRANSITION_MAP(AccessControl, pEventData)
}

// Encoder ccw external event
EVENT_DEFINE(Encoder_CCW, NoEventData)
{
    //Dado el evento de Encoder_Click, se transiciona a un nuevo estado basado en el estado actual 

    BEGIN_TRANSITION_MAP                                            // - Current State -
      
        TRANSITION_MAP_ENTRY(ST_ELIMINATE_ID)                       // ST_WELCOME
        TRANSITION_MAP_ENTRY(ST_ELIMINATE_ID)                       // ST_ACCESS_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ID_ENTERING_BY_CARD,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHECK_ID_ENTERING_BY_CARD,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ID_ENTERING_BY_ENCODER,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_READ_ERROR,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ID_NON_EXISTENT,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_PIN_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHECK_PIN,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ACCESS_GRANTED,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_INVALID_PIN,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_BLOCK_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHECK_ID_ENTERING_BY_ENCODER           
        // //INGRESO DE PALABRA
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ENTER_DIGITS_REQUEST,
        TRANSITION_MAP_ENTRY(ST_CHANGE_DIGIT_DISPLAY_1A)            // ST_ENTER_DIGIT_DISPLAY_1,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_1A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_1B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_DIGITS_RECOUNT_1,
        TRANSITION_MAP_ENTRY(ST_CHANGE_DIGIT_DISPLAY_2A)            // ST_ENTER_DIGIT_DISPLAY_2,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_2A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_2B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_DIGITS_RECOUNT_2,
        TRANSITION_MAP_ENTRY(ST_CHANGE_DIGIT_DISPLAY_3A)            // ST_ENTER_DIGIT_DISPLAY_3,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_3A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_3B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_DIGITS_RECOUNT_3,
        TRANSITION_MAP_ENTRY(ST_CHANGE_DIGIT_DISPLAY_4A)            // ST_ENTER_DIGIT_DISPLAY_4,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_4A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_4B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_DIGITS_RECOUNT_4       
        // //VERDE
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                     // ST_CHANGE_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(ST_LOWER_BRIGHTNESS)                         // ST_SET_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_LOWER_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_HIGHER_BRIGHTNESS     
        // //VIOLETA
        TRANSITION_MAP_ENTRY(ST_CHANGE_BRIGHTNESS)                  // ST_ADD_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_SET_NEW_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHECK_NEW_ID      
        // //NARANJA
        TRANSITION_MAP_ENTRY(ST_ADD_ID)                             // ST_ELIMINATE_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_SHOW_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_PREVIOUS_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_NEXT_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ID_ELIMINATION

    END_TRANSITION_MAP(AccessControl, pEventData)
}

// Encoder long click external event
EVENT_DEFINE(Encoder_Long_Click, NoEventData)
{
    //Dado el evento de Encoder_Click, se transiciona a un nuevo estado basado en el estado actual 

    BEGIN_TRANSITION_MAP                                            // - Current State -
      
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_WELCOME
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ACCESS_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ID_ENTERING_BY_CARD,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHECK_ID_ENTERING_BY_CARD,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ID_ENTERING_BY_ENCODER,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_READ_ERROR,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ID_NON_EXISTENT,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_PIN_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHECK_PIN,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ACCESS_GRANTED,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_INVALID_PIN,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_BLOCK_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHECK_ID_ENTERING_BY_ENCODER           
        // //INGRESO DE PALABRA
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ENTER_DIGITS_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ENTER_DIGIT_DISPLAY_1,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_1A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_1B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_DIGITS_RECOUNT_1,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ENTER_DIGIT_DISPLAY_2,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_2A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_2B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_DIGITS_RECOUNT_2,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ENTER_DIGIT_DISPLAY_3,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_3A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_3B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_DIGITS_RECOUNT_3,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ENTER_DIGIT_DISPLAY_4,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_4A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_4B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_DIGITS_RECOUNT_4       
        // //VERDE
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_SET_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_LOWER_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_HIGHER_BRIGHTNESS     
        // //VIOLETA
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ADD_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_SET_NEW_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHECK_NEW_ID      
        // //NARANJA
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                            // ST_ELIMINATE_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_SHOW_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_PREVIOUS_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_NEXT_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ID_ELIMINATION

    END_TRANSITION_MAP(AccessControl, pEventData)
}

//Lector de tarjetas external event
EVENT_DEFINE(Card_Reader, NoEventData)
{
    
    BEGIN_TRANSITION_MAP                                            // - Current State -
      
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_WELCOME
        TRANSITION_MAP_ENTRY(ST_ID_ENTERING_BY_CARD)                // ST_ACCESS_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ID_ENTERING_BY_CARD,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHECK_ID_ENTERING_BY_CARD,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ID_ENTERING_BY_ENCODER,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_READ_ERROR,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ID_NON_EXISTENT,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_PIN_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHECK_PIN,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ACCESS_GRANTED,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_INVALID_PIN,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_BLOCK_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHECK_ID_ENTERING_BY_ENCODER           
        // //INGRESO DE PALABRA
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ENTER_DIGITS_REQUEST,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ENTER_DIGIT_DISPLAY_1,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_1A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_1B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_DIGITS_RECOUNT_1,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ENTER_DIGIT_DISPLAY_2,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_2A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_2B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_DIGITS_RECOUNT_2,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ENTER_DIGIT_DISPLAY_3,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_3A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_3B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_DIGITS_RECOUNT_3,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ENTER_DIGIT_DISPLAY_4,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_4A,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_DIGIT_DISPLAY_4B,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_DIGITS_RECOUNT_4       
        // //VERDE
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHANGE_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_SET_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_LOWER_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_HIGHER_BRIGHTNESS     
        // //VIOLETA
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ADD_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_SET_NEW_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_CHECK_NEW_ID      
        // //NARANJA
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                            // ST_ELIMINATE_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_SHOW_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_PREVIOUS_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_NEXT_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_ID_ELIMINATION

    END_TRANSITION_MAP(AccessControl, pEventData)
}



//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			     STATE MACHINE FUNCTION DEFINITIONS    	        //
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

STATE_DEFINE(Welcome, NoEventData)
{
    char message[]="welcome";
	display_set_string(message);
}

STATE_DEFINE(AccessRequest, NoEventData)
{
    char message[]="access request";
	display_set_string(message);
}

STATE_DEFINE(IdEnteringByCard, NoEventData)
{
    SM_InternalEvent(ST_CHECK_ID_ENTERING_BY_CARD, NULL);
}

STATE_DEFINE(CheckIdEnteringByCard, NoEventData)
{
    // Get pointer to the instance data and update id
    card_t card_data = card_get_data();
    bool id_exists=false;
    
    uint16_t index;
    for(index=0; index<access_control.total_of_IDs; index++)
        if(access_control.IDsList[index].card_id==card_data.pan){
            id_exists=true;
            access_control.current_ID_index=index;
            break;
        }

    if(id_exists) SM_InternalEvent(ST_PIN_REQUEST, NULL); 
    else SM_InternalEvent(ST_ID_NON_EXISTENT, NULL);

    return;
}

STATE_DEFINE(IdEnteringByEncoder, NoEventData)
{
    access_control.current_option = ID;

    SM_InternalEvent(ST_ENTER_DIGITS_REQUEST, NULL);

    return;
}


STATE_DEFINE(ReadError, NoEventData)
{
    //Muesto READ ERROR
    error_msg();
    SM_InternalEvent(ST_ACCESS_REQUEST, NULL);
}

STATE_DEFINE(IdNonExistent, NoEventData)
{
    //Muestro ID NON EXISTENT
    display_set_string("ID NO EXISTS");
    SM_InternalEvent(ST_ACCESS_REQUEST, NULL);
}

STATE_DEFINE(PinRequest, NoEventData)
{
    char message[]="enter PIN";
	display_set_string(message);
    //veo el largo del pin segun el ID
    switch(access_control.IDsList[access_control.current_ID_index].PIN_length){
        case 4 :
            access_control.current_option = PIN4;
            break;
        case 5 :
            access_control.current_option = PIN5;
            break;
        default :
            //error
            break;
    }
    SM_InternalEvent(ST_ENTER_DIGITS_REQUEST, NULL);
}

STATE_DEFINE(CheckPin, NoEventData)
{
    //PIN es correcto segun el ID ?
    uint32_t pin = array_to_int(access_control.word_introduced,access_control.IDsList[access_control.current_ID_index].PIN_length);

    if(access_control.IDsList[access_control.current_ID_index].PIN==pin) SM_InternalEvent(ST_ACCESS_GRANTED, NULL);
    else SM_InternalEvent(ST_INVALID_PIN, NULL);
    
}

STATE_DEFINE(AccessGranted, NoEventData)
{
    char message[]="access granted";
	display_temp_message(message,2);

    // TODO
    //Muestro ACCESS GRANTED
    //Prendo LED 
    //Espero 5 seg
    //Apago LED
    SM_InternalEvent(ST_WELCOME, NULL);
}


STATE_DEFINE(InvalidPin, NoEventData)
{
    //Muestro INCORRECT PIN
    char message[]="incorrect PIN";
	display_set_string(message);

    //Retardo de unos segundos

    if((access_control.IDsList[access_control.current_ID_index].PIN_attempts++) == MAX_NUM_ATTEMPTS)
        SM_InternalEvent(ST_BLOCK_ID, NULL);
    else
        SM_InternalEvent(ST_PIN_REQUEST, NULL); 
    
}
STATE_DEFINE(BlockId, NoEventData)
{
    //Muestro ID BLOCKED

     char message[]="Id Ban";
	display_set_string(message);

    //display_temp_message(message, ERR_MSG_TIME);

    //Agrego a la lista de bloqueados el ID actual
    access_control.IDsList[access_control.current_ID_index].blocked_status=true;

    SM_InternalEvent(ST_WELCOME, NULL); 
    
}

STATE_DEFINE(CheckIdEnteringByEncoder, NoEventData)
{
    //EXISTE EL ID ?
    uint32_t entered_id = array_to_int(access_control.word_introduced,ID_LENGTH);

    bool id_exists=false;
    
    uint16_t index;
    for(index=0; index<access_control.total_of_IDs; index++)
        if(access_control.IDsList[index].number==entered_id){
            id_exists=true;
            access_control.current_ID_index=index;
            break;
        }

    if(!id_exists) 
        SM_InternalEvent(ST_ID_NON_EXISTENT, NULL); 
    else
        if(access_control.IDsList[index].blocked_status)
            SM_InternalEvent(ST_BLOCK_ID, NULL);
        else{
            access_control.IDsList[index].PIN_attempts=0;
            SM_InternalEvent(ST_PIN_REQUEST, NULL); 
        }
}

//INGRESO DE PALABRA
STATE_DEFINE(EnterDigitsRequest, NoEventData)
{
    access_control.current_num = 0;
    access_control.digits_introduced = 0;
    display_clear_buffer();
    SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY_1, NULL); 
}

//1


STATE_DEFINE(EnterDigitDisplay1, NoEventData)
{
   //Muestra el current_num 
   display_enable_hard_highlight(0);
   display_set_single_number(access_control.current_num, 0);
}

STATE_DEFINE(ChangeDigitDisplay1A, NoEventData)
{
    if(access_control.current_num == MIN_DIGIT_DISPLAY)
       access_control.current_num = MAX_DIGIT_DISPLAY; 
    else
       access_control.current_num--; 

    SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY_1, NULL); 
}
STATE_DEFINE(ChangeDigitDisplay1B, NoEventData)
{
    if(access_control.current_num == MAX_DIGIT_DISPLAY)
       access_control.current_num = MIN_DIGIT_DISPLAY; 
    else
       access_control.current_num++; 

    SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY_1, NULL); 
}

STATE_DEFINE(DigitsRecount1, NoEventData)
{
    access_control.word_introduced[access_control.digits_introduced] = access_control.current_num;
    access_control.digits_introduced++;
    access_control.current_num = 0;

    if(access_control.current_option == PIN5 && access_control.digits_introduced == 5)
    {
        hide_digit(0);
        SM_InternalEvent(ST_CHECK_PIN, NULL); 
    }
    else
    {
        if(access_control.current_option == PIN4)
            hide_digit(0);
        SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY_2, NULL);
        
    }
}


//2

STATE_DEFINE(EnterDigitDisplay2, NoEventData)
{
    //Muestra el current_num en el segundo display
    display_enable_hard_highlight(1);
    display_set_single_number(access_control.current_num, 1);
}

STATE_DEFINE(ChangeDigitDisplay2A, NoEventData)
{
    if(access_control.current_num == MIN_DIGIT_DISPLAY)
       access_control.current_num = MAX_DIGIT_DISPLAY; 
    else
       access_control.current_num--; 

    SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY_2, NULL);     
}
STATE_DEFINE(ChangeDigitDisplay2B, NoEventData)
{
    if(access_control.current_num == MAX_DIGIT_DISPLAY)
       access_control.current_num = MIN_DIGIT_DISPLAY; 
    else
       access_control.current_num++; 

    SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY_2, NULL); 

}
STATE_DEFINE(DigitsRecount2, NoEventData)
{
    
    access_control.word_introduced[access_control.digits_introduced] = access_control.current_num;
    access_control.digits_introduced++;
    access_control.current_num = 0;

    if(access_control.current_option == PIN4 || access_control.current_option == PIN5)
        hide_digit(1);
    
    SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY_3, NULL);
        
    
}

//3

STATE_DEFINE(EnterDigitDisplay3, NoEventData)
{
    //Muestra el current_num en el tercer display
    display_enable_hard_highlight(2);
    display_set_single_number(access_control.current_num, 2);
}

STATE_DEFINE(ChangeDigitDisplay3A, NoEventData)
{
    if(access_control.current_num == MIN_DIGIT_DISPLAY)
       access_control.current_num = MAX_DIGIT_DISPLAY; 
    else
       access_control.current_num--; 

    SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY_3, NULL); 
}

STATE_DEFINE(ChangeDigitDisplay3B, NoEventData)
{
    if(access_control.current_num == MAX_DIGIT_DISPLAY)
       access_control.current_num = MIN_DIGIT_DISPLAY; 
    else
       access_control.current_num++; 

    SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY_3, NULL); 
}

STATE_DEFINE(DigitsRecount3, NoEventData)
{
    access_control.word_introduced[access_control.digits_introduced] = access_control.current_num;
    access_control.digits_introduced++;
    access_control.current_num = 0;

    if(access_control.current_option == PIN4 || access_control.current_option == PIN5)
        hide_digit(2);
    
    SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY_4, NULL);
}

//4

STATE_DEFINE(EnterDigitDisplay4, NoEventData)
{
    //MUESTRA EL CURRENT_NUM EN el cuarto display
    display_enable_hard_highlight(3);
    display_set_single_number(access_control.current_num, 3);
}

STATE_DEFINE(ChangeDigitDisplay4A, NoEventData)
{
    if(access_control.current_num == MIN_DIGIT_DISPLAY)
       access_control.current_num = MAX_DIGIT_DISPLAY; 
    else
       access_control.current_num --; 

    SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY_4, NULL); 
}

STATE_DEFINE(ChangeDigitDisplay4B, NoEventData)
{
    if(access_control.current_num == MAX_DIGIT_DISPLAY)
       access_control.current_num = MIN_DIGIT_DISPLAY; 
    else
       access_control.current_num++; 

    SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY_4, NULL); 
}

STATE_DEFINE(DigitsRecount4, NoEventData)
{
    access_control.word_introduced[access_control.digits_introduced] = access_control.current_num;
    access_control.digits_introduced++;
    access_control.current_num = 0;

    if(access_control.current_option == PIN4 || access_control.current_option == PIN5)
        hide_digit(3);
    
    if((access_control.current_option == ID || access_control.current_option == NEW_ID) &&
    access_control.digits_introduced == 4) 
        SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY_1, NULL); //Es un ID y se requieren 4 digitos mas 
    else
        if((access_control.current_option == ID || access_control.current_option == NEW_ID) &&
    access_control.digits_introduced == 8)
            if(access_control.current_option == ID)
                SM_InternalEvent(ST_CHECK_ID_ENTERING_BY_ENCODER, NULL); //Se trata de un ID y ya se tienen los 8 digitos
            else
                SM_InternalEvent(ST_CHECK_NEW_ID, NULL); //Se trata de un nuevo ID y se tienen los 8 digitos
        else
            if(access_control.current_option == PIN5)       
                SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY_1, NULL); //Es un pin de 5 y falta un digito mas 
            else
                SM_InternalEvent(ST_CHECK_PIN, NULL); //Es un pin de 4 y ya se tienen los 4 digitos
              
}

//VERDE

    

STATE_DEFINE(ChangeBrightness, NoEventData)
{
    char message[]="Brightness";
	display_set_string(message);
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

//VIOLETA

STATE_DEFINE(AddID, NoEventData)
{
    char message[]="Add Id";
	display_set_string(message);
}

STATE_DEFINE(SetNewID, NoEventData)
{
    access_control.current_option = NEW_ID;
    SM_InternalEvent(ST_SET_BRIGHTNESS, NULL); 
}

STATE_DEFINE(CheckNewID, NoEventData)
{
    uint32_t entered_id = array_to_int(access_control.word_introduced,ID_LENGTH);

    bool id_exists=false;
    
    uint16_t index;

    for(index=0; index<access_control.total_of_IDs; index++)
        if(access_control.IDsList[index].number==entered_id){
            id_exists=true;
            break;
        }

    
    if (id_exists) error_msg();
    else{
        done_msg();
        // TODO terminar esto MANUEL

    }

    //Existe el ID?
    //Si ---> Muestro ID ALREADY EXISTS
    //NO ---> Muestro NEW ID INTRODUCED, se agrega a la lista de IDs

    SM_InternalEvent(ST_WELCOME, NULL); 

}

//NARANJA

STATE_DEFINE(EliminateID, NoEventData)
{
    char message[]="Delete Id";
	display_set_string(message);

    access_control.current_ID_index=0;

}

STATE_DEFINE(ShowID, NoEventData)
{
    display_set_number(access_control.IDsList[access_control.current_ID_index].number);
    //Se muestra el ID actual
    
}

STATE_DEFINE(PreviousID, NoEventData)
{
    
    if(access_control.current_ID_index == 0)
        access_control.current_ID_index = access_control.total_of_IDs;
    else
        access_control.current_ID_index --;

    SM_InternalEvent(ST_SHOW_ID, NULL); 

}

STATE_DEFINE(NextID, NoEventData)
{
    
    if(access_control.current_ID_index == access_control.total_of_IDs)
        access_control.current_ID_index = 0;
    else
        access_control.current_ID_index --;

    SM_InternalEvent(ST_SHOW_ID, NULL); 

}

STATE_DEFINE(IDElimination, NoEventData)
{
    int ID_index = access_control.current_ID_index;
    access_control.IDsList[ID_index].valid = false;

    //Mostrar ID ELIMINATED;
    char message[]="Id deleted";
	display_set_string(message);
    SM_InternalEvent(ST_WELCOME, NULL); 

}


// init function

void access_control_init(){
    ID_data_t sample_id;
    sample_id.blocked_status=false;
    sample_id.valid=true;
    sample_id.number=12345678;
    sample_id.card_id=6391300355831573;
    sample_id.PIN=1234;
    sample_id.PIN_attempts=0;
    sample_id.PIN_length=4;

    access_control.current_num=0;
    access_control.current_ID_index=0;
    access_control.IDsList[0]=sample_id;
    access_control.total_of_IDs=1;
    access_control.digits_introduced=0;
}


// auxiliary functions

uint64_t array_to_int(uint8_t* array, uint8_t length){
    uint8_t index;
    uint64_t return_value=0;
    for(index=0; index<length; index++){
        return_value*=10;
        return_value+=array[index]; 
    }
    return return_value;
}



void error_msg(){
    char message[4];
    message[0]='E';
    message[1]='r';
    message[2]='r';
    message[3]='\0';
    //display_temp_message(message, ERR_MSG_TIME);
}

void done_msg(){
    char message[4];
    message[0]='D';
    message[1]='o';
    message[2]='n';
    message[3]='e';
    //message[4]='\0';
    //display_temp_message(message, ERR_MSG_TIME);
}


void hide_digit(uint8_t digit){
    display_set_single_char('-', digit);
}









