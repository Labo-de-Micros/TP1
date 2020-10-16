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
#define BLINK_FREQ_HZ 10U
#if SYSTICK_ISR_FREQUENCY_HZ % (2*BLINK_FREQ_HZ) != 0
#warning BLINK cannot implement this exact frequency.
		Using floor(SYSTICK_ISR_FREQUENCY_HZ/BLINK_FREQ_HZ/2) instead.
#endif
/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static void systick_callback(void);
static void irq_callback(void);
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void){
	//gpioMode(PIN_LED_GREEN, OUTPUT);
	//gpioMode(PIN_LED_BLUE, OUTPUT);
	//SysTick_Init(systick_callback);
	gpioMode(DEBUG_PIN, OUTPUT);
	gpioMode(SIGNAL_PIN, OUTPUT);
	gpioMode(INTERRUPT_PIN, INPUT_PULLDOWN);
	gpioIRQ (INTERRUPT_PIN, GPIO_IRQ_MODE_RISING_EDGE, irq_callback);
	return;
}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void){
	while (1) { ; }
	return;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void systick_callback(void){
#ifdef DEBUG_OSCILLOSCOPE
	gpioToggle(DEBUG_PIN);
#endif
	static uint32_t counter = 0;
	counter++;
	if (counter == SYSTICK_ISR_FREQUENCY_HZ/(BLINK_FREQ_HZ*2)) {
		gpioToggle(SIGNAL_PIN);
		counter = 0;
	}
#ifdef DEBUG_OSCILLOSCOPE
	gpioToggle(DEBUG_PIN);
#endif
return;
}

void irq_callback(void){
#ifdef DEBUG_OSCILLOSCOPE
	gpioWrite(DEBUG_PIN, HIGH);
#endif
	gpioToggle(SIGNAL_PIN);
#ifdef DEBUG_OSCILLOSCOPE
	gpioWrite(DEBUG_PIN, LOW);
#endif
return;
}
