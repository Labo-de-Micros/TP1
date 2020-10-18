/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "./board.h"
#include "./Drivers/Timer/timer.h"
#include "./Drivers/SysTick/SysTick.h"
#include "./Drivers/Encoder/encoder.h"
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

static void click(void);
static void click_double(void);
static void click_long(void);
static void ccw(void);
static void cw(void);
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void App_Init (void) {
	encoder_init();
	//encoder_set_callback(ccw_call,cw_call,click_call,click_double_call,click_long_call);
	gpioMode(PIN_LED_RED,OUTPUT);
	gpioMode(PIN_LED_BLUE,OUTPUT);
	gpioMode(PIN_LED_GREEN,OUTPUT);
	gpioWrite(PIN_LED_RED,HIGH);
	gpioWrite(PIN_LED_BLUE,HIGH);
	gpioWrite(PIN_LED_GREEN,HIGH);
	return;
}

void App_Run (void){
	//while (true);
	while(true){
		enc_events_t temp = encoder_get_event();
		switch(temp){
		case ENC_SINGLE_PRESS:
			click();
			break;
		case ENC_DOUBLE_PRESS:
			click_double();
			break;
		case ENC_LONG_PRESS:
			click_long();
			break;
		case ENC_CLOCKWISE_TURN:
			cw();
			break;
		case ENC_COUNTER_CLOCKWISE_TURN:
			ccw();
			break;
		case ENC_NO_EV:
			break;
		default:
			break;
		}
	}
	return;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void click(void){
	gpioToggle(PIN_LED_GREEN);
	return;
}

static void click_double(void){
	gpioToggle(PIN_LED_GREEN);
	return;
}

static void click_long(void){
	gpioToggle(PIN_LED_GREEN);
	return;
}

static void ccw(void){
	gpioToggle(PIN_LED_BLUE);
	return;
}

static void cw(void){
	gpioToggle(PIN_LED_RED);
	return;
}
