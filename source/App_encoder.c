/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "board.h"
#include "timer.h"
#include "SysTick.h"
#include "encoder.h"
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

static void click_call(void);
static void ccw_call(void);
static void cw_call(void);
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void App_Init (void) {
	encoder_init();
	encoder_set_callback(ccw_call,cw_call,click_call);
	gpioMode(PIN_LED_RED,OUTPUT);
	gpioMode(PIN_LED_BLUE,OUTPUT);
	gpioMode(PIN_LED_GREEN,OUTPUT);
	return;
}

void App_Run (void){
	while (true);
	return;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void click_call(void){
	gpioToggle(PIN_LED_GREEN);
	return;
}

static void ccw_call(void){
	gpioToggle(PIN_LED_BLUE);
	return;
}

static void cw_call(void){
	gpioToggle(PIN_LED_RED);
	return;
}
