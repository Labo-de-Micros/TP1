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
#define DISP_B		0x7C
#define DISP_C		0x39
#define DISP_D		0x5E
#define DISP_E		0x79
#define DISP_F		0x71
#define DISP_G		0x3D
#define DISP_H		0x76
#define DISP_I		0x30
#define DISP_J		0x1E
#define DISP_K		0x75
#define DISP_L		0x38
#define DISP_M		0x15
#define DISP_N		0x37
#define DISP_O		0x3F
#define DISP_P		0x73
#define DISP_Q		0x6B
#define DISP_R		0x33
#define DISP_S		0x6D
#define DISP_T		0x78
#define DISP_U		0x3E
#define DISP_V		0x3E
#define DISP_W		0x2A
#define DISP_X		0x76
#define DISP_Y		0x6E
#define DISP_Z		0x5B
#define DISP_a		0x5F
#define DISP_b		0x7C
#define DISP_c		0x58
#define DISP_d		0x5E
#define DISP_e		0x7B
#define DISP_f		0x71
#define DISP_g		0x6F
#define DISP_h		0x74
#define DISP_i		0x10
#define DISP_j		0x0C
#define DISP_k		0x75
#define DISP_l		0x30
#define DISP_m		0x14
#define DISP_n		0x54
#define DISP_o		0x5C
#define DISP_p		0x73
#define DISP_q		0x67
#define DISP_r		0x50
#define DISP_s		0x6D
#define DISP_t		0x78
#define DISP_u		0x1C
#define DISP_v		0x1C
#define DISP_w		0x14
#define DISP_x		0x76
#define DISP_y		0x6E
#define DISP_z		0x5B
#define DISP_bar	0x80
#define DISP_END	0xFF




#define DISP_MASK 0x01
#define BRIGHTNESS_LEVELS 4


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			ENUMERATIONS AND STRUCTURES AND TYPEDEFS			//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


typedef struct{
	uint8_t digits;
	pin_t pins[7];
	pin_t mux_control_pins[2];
	tim_id_t timer;
	tim_id_t temp_timer;
	tim_id_t pwm_timer;
	tim_id_t rotation_timer;
	uint8_t buf[EXT_BUF_LEN];
	uint8_t aux_buf[EXT_BUF_LEN];
	uint8_t ext_index;
	display_brightness_level_t brightness[BRIGHTNESS_LEVELS];	
	display_brightness_level_t brightness_level;
	display_mode_t mode;
	bool auto_rotation;
	bool queued_return;
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

void digit_select(uint8_t digit);
void refresh_callback();
void split_number(uint16_t num, uint8_t * buffers);
void set_blank();
void set_brightness_level(display_brightness_level_t level);
void set_digit_brightness_level(display_brightness_level_t level, uint8_t digit);
void rotate_callback();
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//					FUNCTION DEFINITIONS						//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void display_init(void){
/*****************************************************************
 * @brief: Initialize the Seven segment display driver
 * **************************************************************/
	timerInit();
	display.timer=timerGetId();
	display.temp_timer=timerGetId();
	display.pwm_timer=timerGetId();
	display.brightness_level= LOW;
	display.ext_index=0;
	display.auto_rotation=true;
	display.queued_return=false;
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
	/*****************************************************************
 * @brief: Initialize the pins of the seven segment display multiplexor.
 * **************************************************************/
	uint8_t index;
	
	display.mux_control_pins[1] = pin0;
	display.mux_control_pins[0] = pin1;
	
	for(index=0; index<MUX_PINS; index++){
		gpioMode(display.mux_control_pins[index], OUTPUT);
	}
	return;
}

void display_set_string(char * string){
/*****************************************************************
 * @brief: Writes a string to the main display buffer
 * **************************************************************/
	display_clear_buffer();
	uint8_t index;
	uint8_t done=false;
	for(index=0; index<EXT_BUF_LEN; index++){
		if(string[index]=='\0') load_buffer(DISP_END, index);
		else load_buffer(get_7_segments_char(string[index]), index);
	}
	return;
}

void display_set_number(uint16_t number){
/*****************************************************************
 * @brief: Writes a number to the main display buffer
 * **************************************************************/
	display_clear_buffer();
	uint8_t buffer[EXT_BUF_LEN];
	split_number(number, buffer);
	
	uint8_t index;
	for(index=0; index<EXT_BUF_LEN; index++)
		load_buffer(get_7_segments_number(buffer[index]), index);
	return;
}

void display_set_single_number(uint8_t number, uint8_t index){
/*****************************************************************
 * @brief: Writes a number to a single digit of the display
 * **************************************************************/
	load_buffer(get_7_segments_number(number), index);
}

void display_set_single_char(char character, uint8_t index){
/*****************************************************************
 * @brief: Writes a character to a single digit of the display
 * **************************************************************/
	load_buffer(get_7_segments_char(character), index);
}

void display_set_brightness_level(display_brightness_level_t level){
/*****************************************************************
 * @brief: Sets a brightness level for the display
 * **************************************************************/
	display_brightness_level_t checked_level;
	switch(level){
		case BRIGHT_MIN: case BRIGHT_LOW:
			checked_level=BRIGHT_LOW;
			break;
		case BRIGHT_HIGH: case BRIGHT_MAX:
			checked_level=BRIGHT_HIGH;
			break;
	}
	uint8_t index;
	for(index=0; index<EXT_BUF_LEN; index++)
		display.brightness[index]=checked_level;
}

void display_enable_soft_highlight(uint8_t digit){
/*****************************************************************
 * @brief: Sets high brightness for a digit and normal brightness
 * for the rest of them
 * **************************************************************/
	set_digit_brightness_level(BRIGHT_MAX, digit);
}

void display_enable_hard_highlight(uint8_t digit){
/*****************************************************************
 * @brief: Sets high brightness for a digit and low brightness
 * for the rest of them
 * **************************************************************/
	set_brightness_level(BRIGHT_MIN);
	set_digit_brightness_level(BRIGHT_MAX, digit);
}

void display_disable_highlight(){
/*****************************************************************
 * @brief: Sets brightness back to nominal level
 * **************************************************************/
	set_brightness_level(display.brightness_level);	
}

void display_on(){
/*****************************************************************
 * @brief: Turns display refresh on
 * **************************************************************/
	uint16_t ticks=(uint16_t)(1000/(REFRESH_FREQUENCY_HZ*DIGITS));
	timerStart(display.timer, ticks, TIM_MODE_PERIODIC, refresh_callback);
	return;
}

void display_off(){
/*****************************************************************
 * @brief: Turns display refresh off
 * **************************************************************/
	timerStop(display.timer);
	return;
}

void display_temp_message(char * message, uint8_t seconds){
/*****************************************************************
 * @brief: Displays a string on the display for a number of seconds.
 * If message length exceeds that of the display, the delay starts 
 * after the message completes a rotation.
 * **************************************************************/
	uint8_t i;
	for(i = 0; i < EXT_BUF_LEN; i++)
		display.aux_buf[i]=display.buf[i];
	display_set_string(message);
	timerStart(display.temp_timer, 1000*seconds, TIM_MODE_SINGLESHOT, return_from_temp);
}

void display_clear_buffer(void){
/*****************************************************************
 * @brief: Clears the screen of the display (nothing will be displayed).
 * **************************************************************/
	uint8_t index;
	for(index=0; index<EXT_BUF_LEN; index++)
		load_buffer(DISP_END, index);
}

void display_enable_auto_rotation(){
/*****************************************************************
 * @brief: Turns automatic rotation on. When a string is loaded, 
 * it will rotate once automatically.
 * **************************************************************/
	display.auto_rotation=true;
}

void display_disable_auto_rotation(){
/*****************************************************************
 * @brief: Turns automatic rotation off.
 * **************************************************************/
	display.auto_rotation=false;
}

void display_stop_rotation(){
/*****************************************************************
 * @brief: Stops current rotation (if happening)
 * **************************************************************/
	if(timerRunning(display.rotation_timer)) 
	{
		timerStop(display.rotation_timer);
		if(display.queued_return) timerStart(display.temp_timer, 1000, TIM_MODE_SINGLESHOT, return_from_temp);
		display.ext_index=0;
	}
}

void display_rotate_left(){
/*****************************************************************
 * @brief: Shifts digits on the display to the left.
 * **************************************************************/
	display_stop_rotation();
	if(display.ext_index!=0) 
		display.ext_index--;
}

void display_rotate_right(){
/*****************************************************************
 * @brief: Shifts digits on the display to the right.
 * **************************************************************/
	display_stop_rotation();
	rotate_callback();
}


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//					LOCAL FUNCTION DEFINITIONS					//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void return_from_temp(void){
/*****************************************************************
 * @brief: If the display is not rotating, transfers contents back
 * from aux to main buffer, returning from a temp message. If it is,
 * queue the transfer for when its done.
 * **************************************************************/
	uint8_t i;
	if(!timerRunning(display.rotation_timer)){	
		for(i = 0; i < EXT_BUF_LEN; i++)
			display.buf[i]=display.aux_buf[i];
		display.queued_return=false;
	}
	else display.queued_return=true;
	return;
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
	if(pins==DISP_END) pins=DISP_CLEAR;
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
	bool extended=false;
	display_stop_rotation();
	if(digit<EXT_BUF_LEN)
		if(digit>3) extended=true;
		display.buf[digit]=pins;
	
	if(extended && display.auto_rotation && !timerRunning(display.rotation_timer)) timerStart(display.rotation_timer, ROTATION_TIME_S*1000, TIM_MODE_PERIODIC, rotate_callback);

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
		case '-':
			return_val = DISP_bar;
			break;
		case ' ':
			return_val = DISP_CLEAR;
			break;
		case '\0':
			return_val = DISP_END;
			break;
			
		default:
			break;
	}	
	return return_val;
}

uint8_t get_7_segments_number(uint8_t num){
/*****************************************************************
 * @brief: Return 7 segment value for the number.
 * @param num: A number from 0-9 to be converted.
 * **************************************************************/	
	uint8_t return_val;
	return_val = DISP_E;
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
		case DISP_END:
			return_val=DISP_END;
			break;
		default:
			break;
	}
	return return_val;
}

void digit_select(uint8_t digit){
/*****************************************************************
 * @brief: Configures display multiplexer to write to a certain 
 * digit.
 * **************************************************************/
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
			break;
	}
	return;
}

void refresh_callback(){
/*****************************************************************
 * @brief: Callback called every time the display is refreshed.
 * **************************************************************/
	static uint8_t digit = 0;
	if (digit>=DIGITS) digit=0;
	digit_select(digit);
	uint8_t pwm_ticks=(uint8_t)(1000/(REFRESH_FREQUENCY_HZ*DIGITS*(BRIGHTNESS_LEVELS-display.brightness[display.ext_index+digit])));
	timerStart(display.pwm_timer, pwm_ticks, TIM_MODE_SINGLESHOT, set_blank);
	set_pins(display.buf[display.ext_index+(digit++)]);
	return;
}

void set_blank(){
/*****************************************************************
 * @brief: Sets selected digit to blank.
 * **************************************************************/
	set_pins(DISP_CLEAR);
}

void split_number(uint16_t num, uint8_t * buffers){
/*****************************************************************
 * @brief: Splits a number into digits and stores the digits 
 * in a buffer.
 * **************************************************************/
	int8_t index=EXT_BUF_LEN-1;
	while(index >= 0) //do till num greater than  0
	{
		if(num>0){
			buffers[index--] = num % 10;  //split last digit from number
			num = num / 10;    //divide num by 10. num /= 10 also a valid one 
		}
		else buffers[index--] = DISP_END;
	}
	return;
}

void set_brightness_level(display_brightness_level_t level){
/*****************************************************************
 * @brief: Sets the brightness level for the entire display.
 * **************************************************************/
	uint8_t index;
	for(index=0; index<EXT_BUF_LEN; index++)
		display.brightness[index]=level;
}

void set_digit_brightness_level(display_brightness_level_t level, uint8_t digit){
/*****************************************************************
 * @brief: Sets the brightness level for a digit.
 * **************************************************************/
	if (digit<EXT_BUF_LEN) display.brightness[digit]=level;
}

void rotate_callback(){
/*****************************************************************
 * @brief: Callback for auto rotation.
 * **************************************************************/
	display.ext_index++;
	if(display.ext_index>EXT_BUF_LEN-DIGITS || display.buf[display.ext_index+3]==DISP_END) 
	{
		display.ext_index=0;
		if(timerRunning(display.rotation_timer)) timerStop(display.rotation_timer);
		if(display.queued_return) timerStart(display.temp_timer, 1000, TIM_MODE_SINGLESHOT, return_from_temp);
	}
}