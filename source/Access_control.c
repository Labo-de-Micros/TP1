#include "Access_control.h"
#include "StateMachine/State_machine.h"
#include <stdio.h>

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
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_SET_BRIGHTNESS,
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
      
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_WELCOME
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
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_SET_BRIGHTNESS,
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
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                         // ST_SET_BRIGHTNESS,
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
EVENT_DEFINE(Card_Reader, CardReaderData)
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
    //Muestro welcome en el display
}

STATE_DEFINE(IdEnteringByCard, NoEventData)
{
    SM_InternalEvent(ST_ID_ENTERING_BY_CARD, NULL);
}

STATE_DEFINE(CheckIdEnteringByCard, AccessControlData)
{
    ASSERT_TRUE(pEventData);

    // Get pointer to the instance data and update id
    AccessControl* pInstance = SM_GetInstance(AccessControl);
    pInstance->current_id = pEventData->id;

    //Existe el ID?
    //NO SM_InternalEvent(ST_ID_NON_EXISTENT, NULL);
    //SI SM_InternalEvent(ST_PIN_REQUEST, NULL);

}

STATE_DEFINE(IdEnteringByEncoder, NoEventData)
{
    AccessControl* pInstance = SM_GetInstance(AccessControl);
    pInstance->current_option = ID;

    SM_InternalEvent(ST_ENTER_DIGITS_REQUEST, NULL);
}


STATE_DEFINE(ReadError, NoEventData)
{
    //Muesto READ ERROR
    SM_InternalEvent(ST_ACCESS_REQUEST, NULL);
}
STATE_DEFINE(IdNonExistent, NoEventData)
{
    //Muestro ID NON EXISTENT
    SM_InternalEvent(ST_ACCESS_REQUEST, NULL);
}
STATE_DEFINE(PinRequest, NoEventData)
{
    //Muestro ENTER PIN
    //veo el largo del pin segun el ID

    AccessControl* pInstance = SM_GetInstance(AccessControl);
    
    pInstance->current_option = PIN4; //El id tiene un pin de 4 digitos 
    pInstance->current_option = PIN5; //El id tiene un pin de 5 digitos 
    
    SM_InternalEvent(ST_ENTER_DIGITS_REQUEST, NULL);

}
STATE_DEFINE(CheckPin, NoEventData)
{
    //PIN es correcto segun el ID ?

    //SI SM_InternalEvent(ST_ACCESS_GRANTED, NULL);
    //NO SM_InternalEvent(ST_INVALID_PIN, NULL);

    
}
STATE_DEFINE(AccessGranted, NoEventData)
{
    //Muestro ACCESS GRANTED
    //Prendo LED 
    //Espero 5 seg
    //Apago LED
    
}
STATE_DEFINE(InvalidPin, NoEventData)
{
    //Muestro INCORRECT PIN
    AccessControl* pInstance = SM_GetInstance(AccessControl);
    pInstance->PIN_attempts ++; //El id tiene un pin de 4 

    //Retardo de unos segundos

    if(pInstance->PIN_attempts == MAX_NUM_ATTEMPTS)
        SM_InternalEvent(ST_BLOCK_ID, NULL);
    else
        SM_InternalEvent(ST_PIN_REQUEST, NULL); 
    
}
STATE_DEFINE(BlockId, NoEventData)
{
    //Muestro ID BLOCKED
    //Agrego a la lista de bloqueados el ID actual

    SM_InternalEvent(ST_WELCOME, NULL); 
    
}
STATE_DEFINE(CheckIdEnteringByEncoder, NoEventData)
{
    AccessControl* pInstance = SM_GetInstance(AccessControl);
    pInstance->PIN_attempts = 0; 

    //EXISTE EL ID ?
    //NO 
    SM_InternalEvent(ST_ID_NON_EXISTENT, NULL); 

    //SI, ESTA BLOQUEDO?

    //NO 
    SM_InternalEvent(ST_PIN_REQUEST, NULL); 

    //SI
    SM_InternalEvent(ST_BLOCK_ID, NULL); 

}

//INGRESO DE PALABRA

STATE_DEFINE(EnterDigitsRequest, NoEventData)
{
    AccessControl* pInstance = SM_GetInstance(AccessControl);
    pInstance->current_num = 0;
    pInstance->digits_introduced = 0;
    pInstance->word_introduced = 0;
     
    SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY_1, NULL); 
}
STATE_DEFINE(EnterDigitDisplay1, NoEventData)
{
   //Muestra el ccurrent_num 
}
STATE_DEFINE(ChangeDigitDisplay1A, NoEventData)
{
    AccessControl* pInstance = SM_GetInstance(AccessControl);
    if(pInstance->current_num = MIN_DIGIT_DISPLAY)
       pInstance->current_num = MAX_DIGIT_DISPLAY; 
    else
       pInstance->current_num --; 

    SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY_1, NULL); 
}
STATE_DEFINE(ChangeDigitDisplay1B, NoEventData)
{
    AccessControl* pInstance = SM_GetInstance(AccessControl);
    if(pInstance->current_num = MAX_DIGIT_DISPLAY)
       pInstance->current_num = MIN_DIGIT_DISPLAY; 
    else
       pInstance->current_num ++; 

    SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY_1, NULL); 
 
}
STATE_DEFINE(DigitsRecount1, NoEventData)
{
    AccessControl* pInstance = SM_GetInstance(AccessControl);
    pInstance->digits_introduced ++;
    pInstance->word_introduced[pInstance->digits_introduced] = pInstance->current_num;
    pInstance->current_num = 0;

    if(pInstance->current_option == PIN5 && pInstance->digits_introduced == 5)
    {
        //Se ocula el display *****
        SM_InternalEvent(ST_CHECK_PIN, NULL); 
    }
    else
    {
        if(pInstance->current_option == PIN4)
            //Se ocula el display ****
        
        SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY_2, NULL);
        
    }
}
STATE_DEFINE(EnterDigitDisplay2, NoEventData)
{
    //Muestra el current_num en el segundo display
}
STATE_DEFINE(ChangeDigitDisplay2A, NoEventData)
{
    AccessControl* pInstance = SM_GetInstance(AccessControl);
    if(pInstance->current_num = MIN_DIGIT_DISPLAY)
       pInstance->current_num = MAX_DIGIT_DISPLAY; 
    else
       pInstance->current_num --; 

    SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY_2, NULL);     
}
STATE_DEFINE(ChangeDigitDisplay2B, NoEventData)
{
    AccessControl* pInstance = SM_GetInstance(AccessControl);
    if(pInstance->current_num = MAX_DIGIT_DISPLAY)
       pInstance->current_num = MIN_DIGIT_DISPLAY; 
    else
       pInstance->current_num ++; 

    SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY_2, NULL); 

}
STATE_DEFINE(DigitsRecount2, NoEventData)
{
    AccessControl* pInstance = SM_GetInstance(AccessControl);
    pInstance->digits_introduced ++;
    pInstance->word_introduced[pInstance->digits_introduced] = pInstance->current_num;
    pInstance->current_num = 0;

    if(pInstance->current_option == PIN4 || pInstance->current_option == PIN5)
        //Se ocula el display *****
    
    SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY_3, NULL);
        
    
}
STATE_DEFINE(EnterDigitDisplay3, NoEventData)
{
    //Muestra el current_num en el segundo display
}
STATE_DEFINE(ChangeDigitDisplay3A, NoEventData)
{
    AccessControl* pInstance = SM_GetInstance(AccessControl);
    if(pInstance->current_num = MIN_DIGIT_DISPLAY)
       pInstance->current_num = MAX_DIGIT_DISPLAY; 
    else
       pInstance->current_num --; 

    SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY_3, NULL); 
}
STATE_DEFINE(ChangeDigitDisplay3B, NoEventData)
{
    AccessControl* pInstance = SM_GetInstance(AccessControl);
    if(pInstance->current_num = MAX_DIGIT_DISPLAY)
       pInstance->current_num = MIN_DIGIT_DISPLAY; 
    else
       pInstance->current_num ++; 

    SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY_3, NULL); 
}
STATE_DEFINE(DigitsRecount3, NoEventData)
{
    AccessControl* pInstance = SM_GetInstance(AccessControl);
    pInstance->digits_introduced ++;
    pInstance->word_introduced[pInstance->digits_introduced] = pInstance->current_num;
    pInstance->current_num = 0;

    if(pInstance->current_option == PIN4 || pInstance->current_option == PIN5)
        //Se ocula el display *****
    
    SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY_4, NULL);
}
STATE_DEFINE(EnterDigitDisplay4, NoEventData)
{
    //MUESTRA EL CURRENT_NUM EN el cuarto display
}
STATE_DEFINE(ChangeDigitDisplay4A, NoEventData)
{
    AccessControl* pInstance = SM_GetInstance(AccessControl);
    if(pInstance->current_num = MIN_DIGIT_DISPLAY)
       pInstance->current_num = MAX_DIGIT_DISPLAY; 
    else
       pInstance->current_num --; 

    SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY_4, NULL); 
}
STATE_DEFINE(ChangeDigitDisplay4B, NoEventData)
{
    AccessControl* pInstance = SM_GetInstance(AccessControl);
    if(pInstance->current_num = MAX_DIGIT_DISPLAY)
       pInstance->current_num = MIN_DIGIT_DISPLAY; 
    else
       pInstance->current_num ++; 

    SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY_4, NULL); 

}
STATE_DEFINE(DigitsRecount4, NoEventData)
{
    AccessControl* pInstance = SM_GetInstance(AccessControl);
    pInstance->digits_introduced ++;
    pInstance->word_introduced[pInstance->digits_introduced] = pInstance->current_num;
    pInstance->current_num = 0;

    if(pInstance->current_option == PIN4 || pInstance->current_option == PIN5)
        //Se ocula el display *****
    
    if((pInstance->current_option == ID || pInstance->current_option == NEW_ID PIN5) &&
    pInstance->word_introduced == 4) 
        SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY_1, NULL); //Es un ID y se requieren 4 digitos mas 
    else
        if((pInstance->current_option == ID || pInstance->current_option == NEW_ID PIN5) &&
    pInstance->word_introduced == 8)
            if(pInstance->current_option == ID)
                SM_InternalEvent(ST_CHECK_ID_ENTERING_BY_ENCODER, NULL); //Se trata de un ID y ya se tienen los 8 digitos
            else
                SM_InternalEvent(ST_CHECK_NEW_ID, NULL); //Se trata de un nuevo ID y se tienen los 8 digitos
        else
            if(pInstance->current_option == PIN5)       
                SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY_1, NULL); //Es un pin de 5 y falta un digito mas 
            else
                SM_InternalEvent(ST_CHECK_PIN, NULL); //Es un pin de 4 y ya se tienen los 4 digitos
              
}
//VERDE

    

STATE_DEFINE(ChangeBrightness, NoEventData)
{
    //Se muestra BRIGHTNESS
}
STATE_DEFINE(SetBrightness, NoEventData)
{
    AccessControl* pInstance = SM_GetInstance(AccessControl);
    
    //Se pone el current brighness en el display
    pInstance->current_brightness; 
    
}
STATE_DEFINE(LowerBrightness, NoEventData)
{
    AccessControl* pInstance = SM_GetInstance(AccessControl);
    if(pInstance->current_brightness != MIN_BRIGHTNESS)
        pInstance->current_brightness--;

    SM_InternalEvent(ST_SET_BRIGHTNESS, NULL); 
}
STATE_DEFINE(HigherBrightness, NoEventData)
{
    AccessControl* pInstance = SM_GetInstance(AccessControl);
    if(pInstance->current_brightness != MAX_BRIGHTNESS)
        pInstance->current_brightness++;

    SM_InternalEvent(ST_SET_BRIGHTNESS, NULL); 
}
//VIOLETA

STATE_DEFINE(AddID, NoEventData)
{
    //Muestra NEW ID
}
STATE_DEFINE(SetNewID, NoEventData)
{
    AccessControl* pInstance = SM_GetInstance(AccessControl);
    pInstance->current_option = NEW_ID;
    SM_InternalEvent(ST_SET_BRIGHTNESS, NULL); 
}
STATE_DEFINE(CheckNewID, NoEventData)
{
    //Existe el ID?
    //Si ---> Muestro ID ALREADY EXISTS
    //NO ---> Muestro NEW ID INTRODUCED, se agrega a la lista de IDs

    SM_InternalEvent(ST_WELCOME, NULL); 


}
//NARANJA

STATE_DEFINE(EliminateID, NoEventData)
{
    //Se muestra ELIMINATE ID
}
STATE_DEFINE(ShowID, NoEventData)
{
    AccessControl* pInstance = SM_GetInstance(AccessControl);
    //Se muestra el ID actual
    pInstance->current_ID_index;
}
STATE_DEFINE(PreviousID, NoEventData)
{
    AccessControl* pInstance = SM_GetInstance(AccessControl);
    //Se muestra el ID actual
    if(pInstance->current_ID_index == 0)
        pInstance->current_ID_index = pInstance->total_of_IDs;
    else
        pInstance->current_ID_index --;

    SM_InternalEvent(ST_SHOW_ID, NULL); 

}
STATE_DEFINE(NextID, NoEventData)
{
    
    AccessControl* pInstance = SM_GetInstance(AccessControl);
    //Se muestra el ID actual
    if(pInstance->current_ID_index == pInstance->total_of_IDs)
        pInstance->current_ID_index = 0;
    else
        pInstance->current_ID_index --;

    SM_InternalEvent(ST_SHOW_ID, NULL); 

}
STATE_DEFINE(IDElimination, NoEventData)
{

    AccessControl* pInstance = SM_GetInstance(AccessControl);
    int ID_index = pInstance->current_ID_index;
    pInstance->IDs[ID_index].blocked_status = TRUE;

    //Mostrar ID ELIMINATED;

    SM_InternalEvent(ST_WELCOME, NULL); 

}

















