//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//	@file		Display_7.h										//
//	@brief		Seven segment display services.					//
//	@author		Grupo	4										//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//							Headers								//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#include "Display_7.h"

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//		CONSTANT AND MACRO DEFINITIONS USING #DEFINE 		 	//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#define DISP_0		0x3F
#define DISP_1		0x06
#define DISP_2		0x5B
#define DISP_3		0x4F
#define DISP_4		0x66
#define DISP_5		0x6D
#define DISP_6		0x7D
#define DISP_7		0x07
#define DISP_8		0x7F
#define DISP_9		0x6F
#define DISP_E		0x79
#define DISP_CLEAR	0x00

#define DISP_MASK 0x01

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			ENUMERATIONS AND STRUCTURES AND TYPEDEFS			//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

typedef struct{
	pin_t pins[7];
}display_t;

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//						STATIC VARIABLES						//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

static display_t display;

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS W FILE LEVEL SCOPE//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

static void set_pins(uint8_t pins);

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//					FUNCTION DEFINITIONS						//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void initDisplay(void){
/*****************************************************************
 * @brief: Initialize the Seven segment display driver
 * **************************************************************/
	return;
}

void set_display(pin_t a,pin_t b,pin_t c,pin_t d,pin_t e,pin_t f,pin_t g){
/*****************************************************************
 * @brief: Intialize de Pins of the seven segment display.
 * @param a: Is the pin number of the 'a' pin of the display
 * @param b: Is the pin number of the 'b' pin of the display
 * @param c: Is the pin number of the 'c' pin of the display
 * @param d: Is the pin number of the 'd' pin of the display
 * @param e: Is the pin number of the 'e' pin of the display
 * @param f: Is the pin number of the 'f' pin of the display
 * @param g: Is the pin number of the 'g' pin of the display
 * 
 * @brief: gpioMode() will be called with those pins numbers.
 * **************************************************************/
	display.pins[0] = a;
	display.pins[1] = b;
	display.pins[2] = c;
	display.pins[3] = d;
	display.pins[4] = e;
	display.pins[5] = f;
	display.pins[6] = g;
    for(uint8_t i = 0 ; i < 7; i++){
    	gpioMode(display.pins[i],OUTPUT);
    }
    return;
}

void set_number(uint8_t num){
/*****************************************************************
 * @brief: Set a specified number on the screen of the seven segment display
 * @param num: A number from 0-9 to be displayed on the screen.
 * **************************************************************/	
	switch(num){
		case 0:
			set_pins(DISP_0);
			break;
		case 1:
			set_pins(DISP_1);
			break;
		case 2:
			set_pins(DISP_2);
			break;
		case 3:
			set_pins(DISP_3);
			break;
		case 4:
			set_pins(DISP_4);
			break;
		case 5:
			set_pins(DISP_5);
			break;
		case 6:
			set_pins(DISP_6);
			break;
		case 7:
			set_pins(DISP_7);
			break;
		case 8:
			set_pins(DISP_8);
			break;
		case 9:
			set_pins(DISP_9);
			break;
		default:
			set_pins(DISP_E);
			break;
	}
}

void clear_display(void){
/*****************************************************************
 * @brief: Clears the screen of the display (nothing will be displayed).
 * **************************************************************/
	set_pins(DISP_CLEAR);
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//					LOCAL FUNCTION DEFINITIONS					//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

static void set_pins(uint8_t pins){
/*****************************************************************
 * @brief: Set the pins abcdefg HIGH or LOW depending on the pins parameter
 * @param pins: An 8 bit param indicating the state of the pins to be
 * 				displayed. (The most significant bit doesnt change anything.)
 * 				Ex:		pins = 01010111
 * 							g -> HIGH	1
 * 							f -> LOW	0
 * 							e -> HIGH	1
 * 							d -> LOW	0
 * 							c -> HIGH	1
 * 							b -> HIGH	1
 * 							a -> HIGH	1
 * **************************************************************/
	for(uint8_t i = 0; i<7; i++){	// Get the pins and starts shifting the mask
									// 00000001 i spaces left to OR and set the pin 
									// HIGH or LOW.
		if(((DISP_MASK << i) & pins) != 0x00)
			gpioWrite(display.pins[i],HIGH);
		else
			gpioWrite(display.pins[i],LOW);
	}
	return;
}
