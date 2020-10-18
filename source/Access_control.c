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

    //NARANJA
    ST_ELIMINATE_ID,
    ST_CONFIRMATION,
    ST_ID_ELIMINATION
};

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			     STATE MACHINE FUNCTION PROTOTYPES    	        //
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

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
//NARANJA
STATE_DECLARE(EliminateID, NoEventData)
STATE_DECLARE(Confirmation, NoEventData)
STATE_DECLARE(IDElimination, NoEventData)


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

    //NARANJA
    STATE_MAP_ENTRY(ST_EliminateID)
    STATE_MAP_ENTRY(ST_Confirmation)
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

    BEGIN_TRANSITION_MAP                                        // - Current State -
    
        TRANSITION_MAP_ENTRY(ST_ENTER_DIGITS_REQUEST)           //ST_ADMIN,
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

        //NARANJA
        TRANSITION_MAP_ENTRY(ST_ID_ENTERING_BY_ENCODER)         //ST_ELIMINATE_ID,
        TRANSITION_MAP_ENTRY(ST_ID_ELIMINATION)                 //ST_CONFIRMATION,
        TRANSITION_MAP_ENTRY(ST_ELIMINATE_ID)                   //ST_ID_ELIMINATION

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

        //NARANJA
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ELIMINATE_ID,
        TRANSITION_MAP_ENTRY(ST_ELIMINATE_ID)                   //ST_CONFIRMATION,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_ELIMINATION

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

        //NARANJA   
        TRANSITION_MAP_ENTRY(ST_CHANGE_BRIGHTNESS)              //ST_ELIMINATE_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CONFIRMATION,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_ELIMINATION

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

        //NARANJA               
        TRANSITION_MAP_ENTRY(ST_ADD_ID)                         //ST_ELIMINATE_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CONFIRMATION,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_ELIMINATION

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
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                 //ST_CHANGE_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_SET_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_LOWER_BRIGHTNESS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_HIGHER_BRIGHTNESS
        //VIOLETA                   
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                 //ST_ADD_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ALREADY_EXISTS,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_ADDITION,

        //NARANJA                   
        TRANSITION_MAP_ENTRY(ST_ACCESS_REQUEST)                 //ST_ELIMINATE_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CONFIRMATION,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_ELIMINATION

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

        //NARANJA                   
        TRANSITION_MAP_ENTRY(ST_ID_ENTERING_BY_CARD)            //ST_ELIMINATE_ID,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_CONFIRMATION,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)                     //ST_ID_ELIMINATION

    END_TRANSITION_MAP(AccessControl, pEventData)   
}



//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			     STATE MACHINE FUNCTION DEFINITIONS    	        //
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

STATE_DEFINE(Admin, NoEventData)
{
    char message[]=" Admin ";
	display_set_string(message);
    access_control.current_option = ADMIN_PIN;

}

STATE_DEFINE(AccessRequest, NoEventData)
{
    access_control.current_option = ID;
    char message[]="Access request";
	display_set_string(message);
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
        if(access_control.IDsList[index].card_id == card_data.pan){
            id_exists = true;
            access_control.current_ID_index=index;
            break;
        }
	}

    //Dependiendo si se quiere eliminar el id, agregar o acceder se hace lo siguiente;
    switch (access_control.current_option){
		case DELETE_ID:
			if(id_exists) 
				SM_InternalEvent(ST_CONFIRMATION, NULL); 
			else 
				SM_InternalEvent(ST_ID_NON_EXISTENT, NULL);
			break;

		case NEW_ID:
			if(id_exists) 
				SM_InternalEvent(ST_ALREADY_EXISTS, NULL); 
			else{   
				//Se guarda el Id en el usuario nuevo
				access_control.IDsList[access_control.total_of_IDs+1].card_id=card_data.pan;
				SM_InternalEvent(ST_PIN_REQUEST, NULL);  
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
        if(access_control.IDsList[index].number==entered_id){
            id_exists=true;
            access_control.current_ID_index=index;
            break;
        }
    //Dependiendo si se quiere eliminar el id, agregar o acceder se hace lo siguiente;
    switch (access_control.current_option)
    {
    case DELETE_ID:
        if(id_exists) SM_InternalEvent(ST_CONFIRMATION, NULL); 
        else SM_InternalEvent(ST_ID_NON_EXISTENT, NULL);
        break;
    
    case NEW_ID:
        if(id_exists) 
            SM_InternalEvent(ST_ALREADY_EXISTS, NULL); 
        else
        {   
            //Se guarda el Id en el usuario nuevo
            access_control.IDsList[access_control.total_of_IDs+1].number = entered_id;
            SM_InternalEvent(ST_PIN_REQUEST, NULL);  
        }
        
        break;

    case ID: default:
        if(id_exists)
        {
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

STATE_DEFINE(IdNonExistent, NoEventData)
{
    display_set_string(" Id NO EXISTS ");
    //SM_InternalEvent(ST_ACCESS_REQUEST, NULL); 
}

STATE_DEFINE(PinRequest, NoEventData)
{
    display_set_string(" enter PIN ");
    //veo el largo del pin segun el ID

    switch (access_control.current_option)
    {
        case NEW_ID:
            access_control.current_option = ADMIN_PIN;
            break;

        case ID: default:
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
    }   

    //SM_InternalEvent(ST_ENTER_DIGITS_REQUEST, NULL);
}

STATE_DEFINE(CheckPin, NoEventData)
{
    //PIN es correcto segun el ID ?
    uint32_t pin = array_to_int(access_control.word_introduced,access_control.IDsList[access_control.current_ID_index].PIN_length);
    uint32_t new_pin;
    switch (access_control.current_option)
    {
    case ADMIN_PIN:
        if(access_control.IDsList[0].PIN==pin)
            SM_InternalEvent(ST_CHANGE_BRIGHTNESS, NULL); 
        else 
            SM_InternalEvent(ST_INVALID_PIN, NULL);
        break;
    
    case NEW_ID_PIN:
            //VER ESTP PORQUE SOLO PUEDO PONER UN PIN DE 4 CUANDO AGRAGO UN ID
    		new_pin = array_to_int(access_control.word_introduced,4);
            access_control.IDsList[access_control.total_of_IDs+1].PIN = new_pin;
            SM_InternalEvent(ST_ID_ADDITION, NULL);
            break;

    case PIN4: case PIN5: default:

        if(access_control.IDsList[access_control.current_ID_index].PIN==pin) 
            SM_InternalEvent(ST_ACCESS_GRANTED, NULL);
        else 
            SM_InternalEvent(ST_INVALID_PIN, NULL);
        break;
    }
}

STATE_DEFINE(AccessGranted, NoEventData)
{
    display_set_string(" access granted ");
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
    
	display_set_string(" incorrect PIN ");

    //Retardo de unos segundos

    switch (access_control.current_option)
    {
    case ADMIN_PIN:
        //Si se introduce mal el PIn del administrador se vuelve al menu del ADMIN
        SM_InternalEvent(ST_ADMIN, NULL); 
        break;
    
    case PIN4: case PIN5: default:

        if((access_control.IDsList[access_control.current_ID_index].PIN_attempts++) == MAX_NUM_ATTEMPTS)
            SM_InternalEvent(ST_BLOCK_ID, NULL);
        else
            SM_InternalEvent(ST_PIN_REQUEST, NULL); 
        break;
    }  
}

STATE_DEFINE(BlockId, NoEventData)
{
    //Muestro ID BLOCKED
	display_set_string(" Id Ban ");
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
    
    if((access_control.current_option == PIN5 && access_control.digits_introduced == 5) ||
       (access_control.current_option == PIN4 && access_control.digits_introduced == 4) ||
       (access_control.current_option == ADMIN_PIN && (access_control.digits_introduced == 5 ||      
        access_control.digits_introduced == 5)))
    {
        hide_digit(0);
        SM_InternalEvent(ST_CHECK_PIN, NULL); 
    }
    else if((access_control.current_option == ID || access_control.current_option == NEW_ID || access_control.current_option == DELETE_ID) &&
        access_control.digits_introduced == 8){
        
		SM_InternalEvent(ST_CHECK_ID_ENTERING_BY_ENCODER, NULL);
	}
	else
	{
		// Entra aca si todavia no se terminaron de ingresar los digitos necesarios.
		access_control.index++;
		if(access_control.index-display_get_index()>3) 
			display_rotate_right();
		SM_InternalEvent(ST_ENTER_DIGIT_DISPLAY, NULL);  
	}     
}

STATE_DEFINE(PreviousDigit, NoEventData)
{
	display_disable_highlight();
    if(access_control.index != 0){
		if(access_control.index-display_get_index()==0) 
			display_rotate_left();
		access_control.index --;
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
	display_set_string(" Brightness ");
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
	display_set_string(" Add Id ");
    access_control.current_option = NEW_ID;

    //Si no hay mas lugar para agregar IDs genero un evento interno para que nunca llegue este menu
    if(access_control.total_of_IDs == MAX_IDS)
        SM_InternalEvent(ST_CHANGE_BRIGHTNESS, NULL); 

}

STATE_DEFINE(AlreadyExists, NoEventData)
{
    display_set_string(" Already exists ");
}

STATE_DEFINE(IDAddition, NoEventData)
{
    display_set_string(" Id added ");
    access_control.total_of_IDs++;
    access_control.IDsList[access_control.current_ID_index].blocked_status = false; //Si el ID esta bloquedo es TRUE
    access_control.IDsList[access_control.current_ID_index].valid = true;

}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			    			 NARANJA    				        //
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

STATE_DEFINE(EliminateID, NoEventData)
{
	display_set_string(" Delete Id ");
    access_control.current_option = DELETE_ID;
}

STATE_DEFINE(Confirmation, NoEventData)
{
    display_set_string(" Confirm ? ");
     
}

STATE_DEFINE(IDElimination, NoEventData)
{
    int ID_index = access_control.current_ID_index;
    access_control.IDsList[ID_index].valid = false;

	display_set_string(" Id deleted ");
   

}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			     			FUNCIONES 				   	        //
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
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

    //access_control.current_num=0;
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

void hide_digit(uint8_t digit){
    display_set_single_char('-', digit);
}









