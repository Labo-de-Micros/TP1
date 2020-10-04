#include "Access_control.h"

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

// State machine state functions
STATE_DECLARE(Welcome, NoEventData);

//AZUL
STATE_DECLARE(AccessRequest, NoEventData);
STATE_DECLARE(IdEnteringByCard, NoEventData);
STATE_DECLARE(CheckIdEnteringByCard, NoEventData);
STATE_DECLARE(IdEnteringByEncoder, NoEventData);
STATE_DECLARE(ReadError, NoEventData);
STATE_DECLARE(IdNonExistent, NoEventData);
STATE_DECLARE(PinRequest, NoEventData);
STATE_DECLARE(CheckPin, NoEventData);
STATE_DECLARE(AccessGranted, NoEventData);
STATE_DECLARE(InvalidPin, NoEventData);
STATE_DECLARE(BlockId, NoEventData);
STATE_DECLARE(CheckIdEnteringByEncoder, NoEventData);



// State map to define state function order
BEGIN_STATE_MAP(AccessControl)
    STATE_MAP_ENTRY(ST_WELCOME)
    //AZUL
    STATE_MAP_ENTRY(ST_ACCESS_REQUEST)
    STATE_MAP_ENTRY(ST_ID_ENTERING_BY_CARD)
    STATE_MAP_ENTRY(ST_CHECK_ID_ENTERING_BY_CARD)
    STATE_MAP_ENTRY(ST_ID_ENTERING_BY_ENCODER)
    STATE_MAP_ENTRY(ST_READ_ERROR)
    STATE_MAP_ENTRY(ST_ID_NON_EXISTENT)
    STATE_MAP_ENTRY(ST_PIN_REQUEST)
    STATE_MAP_ENTRY(ST_CHECK_PIN)
    STATE_MAP_ENTRY(ST_ACCESS_GRANTED)
    STATE_MAP_ENTRY(ST_INVALID_PIN)
    STATE_MAP_ENTRY(ST_BLOCK_ID)
    STATE_MAP_ENTRY(ST_CHECK_ID_ENTERING_BY_ENCODER)

    //INGRESO DE PALABRA
    STATE_MAP_ENTRY(ST_ENTER_DIGITS_REQUEST)
    STATE_MAP_ENTRY(ST_ENTER_DIGIT_DISPLAY_1)
    STATE_MAP_ENTRY(ST_CHANGE_DIGIT_DISPLAY_1A)
    STATE_MAP_ENTRY(ST_CHANGE_DIGIT_DISPLAY_1B)
    STATE_MAP_ENTRY(ST_DIGITS_RECOUNT_1)
    STATE_MAP_ENTRY(ST_ENTER_DIGIT_DISPLAY_2)
    STATE_MAP_ENTRY(ST_CHANGE_DIGIT_DISPLAY_2A)
    STATE_MAP_ENTRY(ST_CHANGE_DIGIT_DISPLAY_2B)
    STATE_MAP_ENTRY(ST_DIGITS_RECOUNT_2)
    STATE_MAP_ENTRY(ST_ENTER_DIGIT_DISPLAY_3)
    STATE_MAP_ENTRY(ST_CHANGE_DIGIT_DISPLAY_3A)
    STATE_MAP_ENTRY(ST_CHANGE_DIGIT_DISPLAY_3B)
    STATE_MAP_ENTRY(ST_DIGITS_RECOUNT_3)
    STATE_MAP_ENTRY(ST_ENTER_DIGIT_DISPLAY_4)
    STATE_MAP_ENTRY(ST_CHANGE_DIGIT_DISPLAY_4A)
    STATE_MAP_ENTRY(ST_CHANGE_DIGIT_DISPLAY_4B)
    STATE_MAP_ENTRY(ST_DIGITS_RECOUNT_4)
    //VERDE
    STATE_MAP_ENTRY(ST_CHANGE_BRIGHTNESS)
    STATE_MAP_ENTRY(ST_SET_BRIGHTNESS)
    STATE_MAP_ENTRY(ST_LOWER_BRIGHTNESS)
    STATE_MAP_ENTRY(ST_HIGHER_BRIGHTNESS)
    
    //VIOLETA
    STATE_MAP_ENTRY(ST_ADD_ID)
    STATE_MAP_ENTRY(ST_SET_NEW_ID)
    STATE_MAP_ENTRY(ST_CHECK_NEW_ID)
    //NARANJA
    STATE_MAP_ENTRY(ST_ELIMINATE_ID)
    STATE_MAP_ENTRY(ST_SHOW_ID)
    STATE_MAP_ENTRY(ST_PREVIOUS_ID)
    STATE_MAP_ENTRY(ST_NEXT_ID)
    STATE_MAP_ENTRY(ST_ID_ELIMINATION)

END_STATE_MAP(AccessControl)

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






STATE_DEFINE(Welcome, NoEventData)
{
    //Muestro welcome en el display
}

STATE_DEFINE(IdEnteringByCard, NoEventData)
{
    //No hago nada 
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




STATE_DECLARE(IdEnteringByEncoder, NoEventData);
STATE_DECLARE(ReadError, NoEventData);
STATE_DECLARE(IdNonExistent, NoEventData);
STATE_DECLARE(PinRequest, NoEventData);
STATE_DECLARE(CheckPin, NoEventData);
STATE_DECLARE(AccessGranted, NoEventData);
STATE_DECLARE(InvalidPin, NoEventData);
STATE_DECLARE(BlockId, NoEventData);
STATE_DECLARE(CheckIdEnteringByEncoder, NoEventData);













STATE_DEFINE(Stop, NoEventData)
{
    // Get pointer to the instance data and update currentSpeed
    Motor* pInstance = SM_GetInstance(Motor);
    pInstance->currentSpeed = 0;

    // Perform the stop motor processing here
    printf("%s ST_Stop: %d\n", self->name, pInstance->currentSpeed);

    // Transition to ST_Idle via an internal event
    SM_InternalEvent(ST_IDLE, NULL);
}

// Start the motor going
STATE_DEFINE(Start, MotorData)
{
    ASSERT_TRUE(pEventData);

    // Get pointer to the instance data and update currentSpeed
    Motor* pInstance = SM_GetInstance(Motor);
    pInstance->currentSpeed = pEventData->speed;

    // Set initial motor speed processing here
    printf("%s ST_Start: %d\n", self->name, pInstance->currentSpeed);
}

// Changes the motor speed once the motor is moving
STATE_DEFINE(ChangeSpeed, MotorData)
{
    ASSERT_TRUE(pEventData);

    // Get pointer to the instance data and update currentSpeed
    Motor* pInstance = SM_GetInstance(Motor);
    pInstance->currentSpeed = pEventData->speed;

    // Perform the change motor speed here
    printf("%s ST_ChangeSpeed: %d\n", self->name, pInstance->currentSpeed);
}







