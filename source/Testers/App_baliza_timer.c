/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "./board.h"
#include "./Drivers/Timer/timer.h"
#include "./Drivers/SysTick/SysTick.h"
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
static bool toggle(bool temp);
static void button_callback(void);
static void baliza_callback(void);
static bool baliza_encendida = false;
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void App_Init (void) {
	gpioMode(PIN_SW3,INPUT_PULLDOWN);
	gpioMode(DEBUG_PIN, OUTPUT);
	gpioMode(PIN_LED_RED, OUTPUT);
	timerInit();
	tim_id_t timer_button = timerGetId();
	tim_id_t timer_baliza = timerGetId();
	timerStart(timer_button,BUTTON_CHECK_TICKS, TIM_MODE_PERIODIC, button_callback);
	timerStart(timer_baliza,BLINK_FREQ_TICKS, TIM_MODE_PERIODIC, baliza_callback);
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

static bool toggle(bool temp){
	if(temp == true)
		return false;
	else
		return true;
}

static void button_callback(void){
	if(gpioRead(PIN_SW3) == LOW)
		baliza_encendida = toggle(baliza_encendida);
	return;
}

static void baliza_callback(void){
	if(baliza_encendida){
		gpioToggle(PIN_LED_RED);
	}
	else
		gpioWrite(PIN_LED_RED,HIGH);
}
