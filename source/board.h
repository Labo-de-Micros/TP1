/***************************************************************************//**
  @file     board.h
  @brief    Board management
  @author   Nicolás Magliola
 ******************************************************************************/

#ifndef _BOARD_H_
#define _BOARD_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "gpio.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/***** BOARD defines **********************************************************/

// On Board User LEDs
#define PIN_LED_RED     PORTNUM2PIN(PB,22) 	// PTB22
#define PIN_LED_GREEN   PORTNUM2PIN(PE,26)	// PTB26
#define PIN_LED_BLUE    PORTNUM2PIN(PB,21) 	// PTB21



#define INTERRUPT_PIN	  PORTNUM2PIN(PC,5)		//PTC5
#define DEBUG_PIN       PORTNUM2PIN(PE,25)	//PTE25
#define SIGNAL_PIN		  PORTNUM2PIN(PC,12)	//PTC12

#define ENCODER_PIN_A   PORTNUM2PIN(PC,5)	//PTC5
#define ENCODER_PIN_B   PORTNUM2PIN(PC,17)//PTC17
#define ENCODER_PIN_SW  PORTNUM2PIN(PC,16)//PTC16

#define DISPLAY_PIN_A   PORTNUM2PIN(PC,7)
#define DISPLAY_PIN_B   PORTNUM2PIN(PC,0)
#define DISPLAY_PIN_C   PORTNUM2PIN(PC,9)
#define DISPLAY_PIN_D   PORTNUM2PIN(PC,8)
#define DISPLAY_PIN_E   PORTNUM2PIN(PC,1)
#define DISPLAY_PIN_F   PORTNUM2PIN(PC,12)
#define DISPLAY_PIN_G   PORTNUM2PIN(PC,4)

#define CARD_DATA   0
#define CARD_CLOCK  0
#define CARD_ENABLE 0



#define LED_ACTIVE			LOW

// On Board User Switches
#define PIN_SW2         PORTNUM2PIN(PC,6)		// PTC6
#define PIN_SW3         PORTNUM2PIN(PA,4)		// PTA4

/*******************************************************************************
 ******************************************************************************/

#endif // _BOARD_H_
