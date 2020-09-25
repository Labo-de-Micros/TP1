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
#include "timer.h"

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
#define DISP_CLEAR	0x00
#define DISP_A		0x77
#define DISP_a		0x7D
#define DISP_B		0x7F
#define DISP_b		0x1F
#define DISP_C		0x4E
#define DISP_c		0x0D
#define DISP_D		0x7E
#define DISP_d		0x3D
#define DISP_E		0x4F
#define DISP_e		0x6f
#define DISP_F		0x47
#define DISP_f		0x47
#define DISP_G		0x5E
#define DISP_g		0x7B
#define DISP_H		0x37
#define DISP_h		0x17
#define DISP_I		0x30
#define DISP_i		0x10
#define DISP_J		0x3C
#define DISP_j		0x38
#define DISP_K		0x37
#define DISP_k		0x17
#define DISP_L		0x0E
#define DISP_l		0x06
#define DISP_M		0x55
#define DISP_m		0x55
#define DISP_N		0x15
#define DISP_n		0x15
#define DISP_O		0x7E
#define DISP_o		0x1D
#define DISP_P		0x67
#define DISP_p		0x67
#define DISP_Q		0x73
#define DISP_q		0x73
#define DISP_R		0x77
#define DISP_r		0x05
#define DISP_S		0x5B
#define DISP_s		0x5B
#define DISP_T		0x46
#define DISP_t		0x0F
#define DISP_U		0x3E
#define DISP_u		0x1C
#define DISP_V		0x27
#define DISP_v		0x23
#define DISP_W		0x3F
#define DISP_w		0x2B
#define DISP_X		0x25
#define DISP_x		0x25
#define DISP_Y		0x3B
#define DISP_y		0x33
#define DISP_Z		0x6D
#define DISP_z		0x6D

#define DISP_MASK 0x01

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			ENUMERATIONS AND STRUCTURES AND TYPEDEFS			//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

typedef enum {
	DISPLAY_MODE_TEMPORARY,
	DISPLAY_MODE_PERSISTANT,
	DISPLAY_MODE_BLINK
} display_mode_t;

typedef struct{
	uint8_t digits;
	pin_t pins[7];
	pin_t mux_control_pins[2];
	tim_id_t timer;
	tim_id_t temp_timer;
	uint8_t buf[4];
	uint8_t aux_buf[4];
	display_mode_t mode;
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
static void load_buffer(uint8_t pins, uint8_t digit);
void return_from_temp(void);
uint8_t get_7_segments_char(char character);
uint8_t get_7_segments_number(uint8_t num);
void display_clear_buffer(void);
void digit_select(uint8_t digit);
void display_refresh_callback();
void split_number(uint16_t num, uint16_t * buffers);

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//					FUNCTION DEFINITIONS						//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void display_init(void){
/*****************************************************************
 * @brief: Initialize the Seven segment display driver
 * **************************************************************/
	return;
}



void display_configure_pins(pin_t a,pin_t b,pin_t c,pin_t d,pin_t e,pin_t f,pin_t g){
/*****************************************************************
 * @brief: Initialize the pins of the seven segment display.
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


void display_configure_mux(pin_t pin0, pin_t pin1){
	uint8_t index;
	
	display.mux_control_pins[0] = pin0;
	display.mux_control_pins[1] = pin1;
	
	for(index=0; index<MUX_PINS; index++){
		gpioMode(display.mux_control_pins[index], OUTPUT);
	}
	return;
}

void display_set_string(char * string){
	uint8_t index;
	for(index=0; string[index]!='\0' && index<DIGITS; index++){
		load_buffer(get_7_segments_char(string[index]), index);
	}
	return;
}

void display_set_number(uint16_t number){
	uint8_t buffer[4];
	split_number(number, buffer);
	
	uint8_t index;
	for(index=0; index<4; index++){
		load_buffer(get_7_segments_number(buffer[index]), index);
	}

	return;
}

void display_on(){
	uint16_t ticks=(uint16_t)(1000/(REFRESH_FREQUENCY_HZ*DIGITS));
	timerStart(display.timer, ticks, TIM_MODE_PERIODIC, display_refresh_callback);
	return;
}

void display_off(){
	timerStop(display.timer);
	return;
}

void display_temp_message(char * message, uint8_t seconds){
	uint8_t i;
	for(i = 0; i < DIGITS; i++)
		display.aux_buf[i]=display.buf[i];
	display.mode=DISPLAY_MODE_TEMPORARY;
	display_set_string(message);
	timerStart(display.temp_timer, 1000*seconds, TIM_MODE_SINGLESHOT, return_from_temp);
}



//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//					LOCAL FUNCTION DEFINITIONS					//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


void return_from_temp(void){
	display.mode=DISPLAY_MODE_PERSISTANT;
	uint8_t i;
	for(i = 0; i < 29;i++)
		display.buf[i]=display.aux_buf[i];
}

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

static void load_buffer(uint8_t pins, uint8_t digit){
/*****************************************************************
 * @brief: Set the buffer pins abcdefg HIGH or LOW depending on the pins 
 * parameter
 * @param pins: An 8 bit param indicating the state of the pins to be
 * 				buffered. (The most significant bit doesnt change anything.)
 * 				Ex:		pins = 01010111
 * 							g -> HIGH	1
 * 							f -> LOW	0
 * 							e -> HIGH	1
 * 							d -> LOW	0
 * 							c -> HIGH	1
 * 							b -> HIGH	1
 * 							a -> HIGH	1
 *  @param digit: buffer digit to be modified
 * **************************************************************/
	if(digit<DIGITS)
		for(uint8_t i = 0; i<7; i++)
			display.buf[digit]=pins;
	return;
}

uint8_t get_7_segments_char(char character){
/*****************************************************************
 * @brief: Return 7 segment binary value for the character.
 * @param character: A character to be converted.
 * **************************************************************/	
	uint8_t return_val = DISP_E;

	switch(character){
		case 'A':
			return_val = DISP_A;
			break;
		case 'a':
			return_val = DISP_a;
			break;
		case 'B':
			return_val = DISP_B;
			break;
		case 'b':
			return_val = DISP_b;
			break;
		case 'C':
			return_val = DISP_C;
			break;
		case 'c':
			return_val = DISP_c;
			break;
		case 'D':
			return_val = DISP_D;
			break;
		case 'd':
			return_val = DISP_d;
			break;
		case 'E':
			return_val = DISP_E;
			break;
		case 'e':
			return_val = DISP_e;
			break;
		case 'F':
			return_val = DISP_F;
			break;
		case 'f':
			return_val = DISP_f;
			break;
		case 'G':
			return_val = DISP_G;
			break;
		case 'g':
			return_val = DISP_g;
			break;
		case 'H':
			return_val = DISP_H;
			break;
		case 'h':
			return_val = DISP_h;
			break;
		case 'I':
			return_val = DISP_I;
			break;
		case 'i':
			return_val = DISP_i;
			break;
		case 'J':
			return_val = DISP_J;
			break;
		case 'j':
			return_val = DISP_j;
			break;
		case 'K':
			return_val = DISP_K;
			break;
		case 'k':
			return_val = DISP_k;
			break;
		case 'L':
			return_val = DISP_L;
			break;
		case 'l':
			return_val = DISP_l;
			break;
		case 'M':
			return_val = DISP_M;
			break;
		case 'm':
			return_val = DISP_m;
			break;
		case 'N':
			return_val = DISP_N;
			break;
		case 'n':
			return_val = DISP_n;
			break;
		case 'O':
			return_val = DISP_O;
			break;
		case 'o':
			return_val = DISP_o;
			break;
		case 'P':
			return_val = DISP_P;
			break;
		case 'p':
			return_val = DISP_p;
			break;
		case 'Q':
			return_val = DISP_Q;
			break;
		case 'q':
			return_val = DISP_q;
			break;
		case 'R':
			return_val = DISP_R;
			break;
		case 'r':
			return_val = DISP_r;
			break;
		case 'S':
			return_val = DISP_S;
			break;
		case 's':
			return_val = DISP_s;
			break;
		case 'T':
			return_val = DISP_T;
			break;
		case 't':
			return_val = DISP_t;
			break;
		case 'U':
			return_val = DISP_U;
			break;
		case 'u':
			return_val = DISP_u;
			break;
		case 'V':
			return_val = DISP_V;
			break;
		case 'v':
			return_val = DISP_v;
			break;
		case 'W':
			return_val = DISP_W;
			break;
		case 'w':
			return_val = DISP_w;
			break;
		case 'X':
			return_val = DISP_X;
			break;
		case 'x':
			return_val = DISP_x;
			break;
		case 'Y':
			return_val = DISP_Y;
			break;
		case 'y':
			return_val = DISP_y;
			break;
		case 'Z':
			return_val = DISP_Z;
			break;
		case 'z':
			return_val = DISP_z;
			break;
		default:
	}	
}

uint8_t get_7_segments_number(uint8_t num){
/*****************************************************************
 * @brief: Return 7 segment value for the number.
 * @param num: A number from 0-9 to be converted.
 * **************************************************************/	
	uint8_t return_val = DISP_E;
	switch(num){
		case 0:
			return_val = DISP_0;
			break;
		case 1:
			return_val = DISP_1;
			break;
		case 2:
			return_val = DISP_2;
			break;
		case 3:
			return_val = DISP_3;
			break;
		case 4:
			return_val = DISP_4;
			break;
		case 5:
			return_val = DISP_5;
			break;
		case 6:
			return_val = DISP_6;
			break;
		case 7:
			return_val = DISP_7;
			break;
		case 8:
			return_val = DISP_8;
			break;
		case 9:
			return_val = DISP_9;
			break;
		default:
	}
	return return_val;
}

void display_clear_buffer(void){
/*****************************************************************
 * @brief: Clears the screen of the display (nothing will be displayed).
 * **************************************************************/
	uint8_t index;
	for(index=0;index,DIGITS;index++)
		load_buffer(DISP_CLEAR, index);
}

void digit_select(uint8_t digit){
	switch (digit){
		case 0:
			gpioWrite(display.mux_control_pins[0],0);
			gpioWrite(display.mux_control_pins[1],0);
			break;
		case 1:
			gpioWrite(display.mux_control_pins[0],0);
			gpioWrite(display.mux_control_pins[1],1);
			break;
		case 2:
			gpioWrite(display.mux_control_pins[0],1);
			gpioWrite(display.mux_control_pins[1],0);
			break;
		case 3:
			gpioWrite(display.mux_control_pins[0],1);
			gpioWrite(display.mux_control_pins[1],1);
			break;
		default:
	}
	return;
}

void display_refresh_callback(){
	static uint8_t digit = 0;
	if (digit>=DIGITS) digit=0;
	digit_select(digit);
	set_pins(display.buf[digit++]);
	return;
}

void split_number(uint16_t num, uint16_t * buffers){
	uint8_t index=3;
	while(index >= 0) //do till num greater than  0
	{
		buffers[index--] = num % 10;  //split last digit from number
		num = num / 10;    //divide num by 10. num /= 10 also a valid one 
	}
	return;
}