#include "State_machine.h"
#include <stdint.h>
#include <stdbool.h>


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
    uint8_t word_introduced; //Palabra de 4,5 o 8 digitos
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
    
} AccessControlData;


typedef struct
{
    uint8_t id;
    
} CardReaderData;





// State machine event functions
EVENT_DECLARE(MTR_SetSpeed, MotorData)
EVENT_DECLARE(MTR_Halt, NoEventData)