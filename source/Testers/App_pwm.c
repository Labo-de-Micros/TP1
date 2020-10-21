/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "./board.h"
#include "./Drivers/PWM/PWM.h"
#include "./Drivers/GPIO/gpio.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define DEBUG_OSCILLOSCOPE

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
void callback(void);
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void App_Init (void) {
	pwm_init(10000-1);
	pwm_start_timer(PWM_MS_TO_TICKS(1),callback);
	gpioMode(TEST,OUTPUT);
	return;
}

void App_Run (void){
    while (true);
	return;
}

void callback(void){
	gpioToggle(TEST);
	//pwm_start_timer(PWM_MS_TO_TICKS(1),callback);
	return;
}
