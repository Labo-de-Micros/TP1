/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "./board.h"
#include "./Drivers/Timer/timer.h"
#include "./Drivers/SysTick/SysTick.h"
#include "./Drivers/Encoder/encoder.h"
#include "./Drivers/Display_7/Display_7.h"
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
static void ccw(void);
static void cw(void);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void App_Init (void) {
	//encoder_init();
    display_init();
    char text[]="alto display";
	display_set_string(text);
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

static void click(void){
	return;
}

static void ccw(void){
	return;
}

static void cw(void){
	return;
}
