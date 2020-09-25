/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "board.h"
#include "timer.h"
#include "SysTick.h"
#include "encoder.h"
#include "Display_7.h"
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
    display_init();
    display_configure_pins(DISPLAY_PIN_A, DISPLAY_PIN_B, DISPLAY_PIN_C, DISPLAY_PIN_D, DISPLAY_PIN_E, DISPLAY_PIN_F, DISPLAY_PIN_G);
    display_configure_mux(DISPLAY_MUX_PIN_0, DISPLAY_MUX_PIN_1);
    char text[4];
	//text[0] = 'h';
	//text[1] = 'o';
	//text[2] = 'l';

	display_set_string(text);
	display_on();
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

static uint8_t number=10;

static void click_call(void){
	char text[4];
	text[0] = 'p';
	text[1] = 'e';
	text[2] = 't';
	text[3] = 'e';
	display_temp_message(text, 2);
	return;
}

static void ccw_call(void){
	number--;
	if (number<1) number=1;
	display_set_pwm_level(number);
	return;
}

static void cw_call(void){
	number++;
	if (number>10) number=10;
	display_set_pwm_level(number);
	return;
}
