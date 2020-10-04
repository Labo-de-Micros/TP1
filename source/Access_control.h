#ifndef _ACCESSCONTROL_H
#define _ACCESSCONTROL_H



#include "StateMachine/DataTypes.h"
#include "StateMachine/State_machine.h"
#include <stdint.h>
#include <stdbool.h>



#define MAX_NUM_ATTEMPTS 3

#define MAX_DIGIT_DISPLAY 9
#define MIN_DIGIT_DISPLAY 0


typedef enum{
	NEW_ID,
	ID,
	PIN4,
    PIN5
} word_option;


// Access control object structure
typedef struct
{
    int current_id; // ID actual 
    int number_of_ids; //Cantidad de ids 

    int current_brightness; //Nivel de brillo actual

    //Manejo de introduccion de palabra
    int word_introduced[8]; //Palabra de 4,5 o 8 digitos
    int current_num; //Numero que se esta mostrando en el display
    int digits_introduced; //Cantidad de numeros introducidos 
    word_option current_option;

    int PIN_attempts;

    //Lista de IDs


} AccessControl;


typedef struct 
{
    uint8_t number; //Numero de ID (8 digitos)
    uint8_t PIN; //Contraseña del ID (4 o 5 digitos)
    bool blocked_status; //Si el ID esta bloquedo es TRUE

} IDData;


// Event data structure
typedef struct
{
    uint8_t id;
    word_option option;

} AccessControlData;


typedef struct
{
    uint8_t id;
    
} CardReaderData;





// State machine event functions

EVENT_DECLARE(Encoder_Click, NoEventData);
EVENT_DECLARE(Encoder_Double_Click, NoEventData);
EVENT_DECLARE(Encoder_CW, NoEventData);
EVENT_DECLARE(Encoder_CCW, NoEventData);
EVENT_DECLARE(Encoder_Long_Click, NoEventData);
EVENT_DECLARE(Card_Reader, CardReaderData);

#endif // _ACCESSCONTROL_H
