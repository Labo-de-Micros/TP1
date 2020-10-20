/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "./board.h"
#include "hardware.h"
#include "./Drivers/PWM/FTM.h"
#include "./Drivers/GPIO/gpio.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define DEBUG_OSCILLOSCOPE
#define BUTTON_CHECK_MS			10
#define BLINK_FREQ_HZ 			2U
#define BUTTON_CHECK_TICKS	TIMER_MS2TICKS(10)
#define BLINK_FREQ_TICKS 		TIMER_MS2TICKS( 1000/BLINK_FREQ_HZ )

#if SYSTICK_ISR_FREQUENCY_HZ % (2*BLINK_FREQ_HZ) != 0
#warning BLINK cannot implement this exact frequency.
		Using floor(SYSTICK_ISR_FREQUENCY_HZ/BLINK_FREQ_HZ/2) instead.
#endif
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

	hw_Init ();
	gpioMode(TEST,OUTPUT);
	FTM_Init(FTM0);
	pwm_start_timer(PWM_MS_TO_TICKS(0.001),99,callback);
	gpioMode(TEST,OUTPUT);
	return;
}

void App_Run (void){
    while (true);
	return;
}

void callback(void){
	gpioToggle(TEST);
	pwm_start_timer(PWM_MS_TO_TICKS(0.001),99,callback);
	return;
}
