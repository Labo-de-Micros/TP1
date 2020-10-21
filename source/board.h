//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//	@file		board.h										    //
//	@brief		Board Management								//
//	@author		Grupo	4										//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#ifndef _BOARD_H_
#define _BOARD_H_

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//							Headers								//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#include "./Drivers/GPIO/gpio.h"


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//		CONSTANT AND MACRO DEFINITIONS USING #DEFINE 		 	//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#define __DEBUG__

// On Board User LEDs
#define PIN_LED_RED     	PORTNUM2PIN(PB,22) 	//PTB22
#define PIN_LED_GREEN   	PORTNUM2PIN(PE,26)	//PTB26
#define PIN_LED_BLUE    	PORTNUM2PIN(PB,21) 	//PTB21
// Debug Pins
#define DEBUG_PIN			PORTNUM2PIN(PE,25)	//PTE25
#define TEST                PORTNUM2PIN(PE,26)	//PTE26
// Encoder Pins
#define ENCODER_PIN_A   	PORTNUM2PIN(PC,5)	//PTC5
#define ENCODER_PIN_B   	PORTNUM2PIN(PC,17)	//PTC17
#define ENCODER_PIN_SW  	PORTNUM2PIN(PC,16)	//PTC16
// Display pins
#define DISPLAY_PIN_A   	PORTNUM2PIN(PC,7)	//PTC7
#define DISPLAY_PIN_B   	PORTNUM2PIN(PC,0)	//PTC0
#define DISPLAY_PIN_C   	PORTNUM2PIN(PC,9)	//PTC9
#define DISPLAY_PIN_D   	PORTNUM2PIN(PC,8)	//PTC8
#define DISPLAY_PIN_E   	PORTNUM2PIN(PD,0)	//PTD0
#define DISPLAY_PIN_F   	PORTNUM2PIN(PC,12)	//PTC12
#define DISPLAY_PIN_G   	PORTNUM2PIN(PC,4)	//PTC4
#define DISPLAY_MUX_PIN_0   PORTNUM2PIN(PA,1)	//PTA1
#define DISPLAY_MUX_PIN_1   PORTNUM2PIN(PB,9)	//PTB9
//PCB Led pins
#define PCB_LED_STATUS_1    PORTNUM2PIN(PC,10)	//PTC10
#define PCB_LED_STATUS_2    PORTNUM2PIN(PC,11)	//PTC11
// FTM Debug Pin
#define FTM_PIN         	PORTNUM2PIN(PC,1)	//PTC1
// Card reader pins
#define CARD_DATA   		PORTNUM2PIN(PB,2)	//PTB2
#define CARD_CLOCK  		PORTNUM2PIN(PB,3)	//PTB3
#define CARD_ENABLE 		PORTNUM2PIN(PB,10)	//PTB10

#define LED_ACTIVE			LOW
// On Board User Switches
#define PIN_SW2         	PORTNUM2PIN(PC,6)	//PTC6
#define PIN_SW3         	PORTNUM2PIN(PA,4)	//PTA4

#endif // _BOARD_H_
