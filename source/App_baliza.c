/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "board.h"
#include "gpio.h"
#include "SysTick.h"
#define DEBUG_OSCILLOSCOPE
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define BLINK_FREQ_HZ 2U
#if SYSTICK_ISR_FREQUENCY_HZ % (2*BLINK_FREQ_HZ) != 0
#warning BLINK cannot implement this exact frequency.
		Using floor(SYSTICK_ISR_FREQUENCY_HZ/BLINK_FREQ_HZ/2) instead.
#endif
/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static void systick_callback(void);
static bool toggle(bool temp);
static bool baliza_encendida = false;
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void) {
	SysTick_Init(systick_callback);
	gpioMode(PIN_SW3,INPUT);
	gpioMode(DEBUG_PIN, OUTPUT);
	gpioMode(PIN_LED_RED, OUTPUT);
	return;
}

void App_Run (void){
	while (true) {
		if(gpioRead(PIN_SW3) == LOW)
			baliza_encendida = toggle(baliza_encendida);
	}
	return;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static void systick_callback(void){
#ifdef DEBUG_OSCILLOSCOPE
	gpioToggle(DEBUG_PIN);
#endif

	static uint32_t counter = 0;
	counter++;
	if (counter >= SYSTICK_ISR_FREQUENCY_HZ/(BLINK_FREQ_HZ*2) && baliza_encendida) {
		gpioToggle(PIN_LED_RED);
		counter = 0;
	}
	else if(!baliza_encendida)
		gpioWrite(PIN_LED_RED,HIGH);

#ifdef DEBUG_OSCILLOSCOPE
	gpioToggle(DEBUG_PIN);
#endif
return;
}

static bool toggle(bool temp){
	if(temp == true)
		return false;
	else
		return true;
}
