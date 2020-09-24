#include "Screen.h"

static uint8_t buffer[4];

void split_number(uint16_t num){
	uint8_t index=3;
	while(index >= 0) //do till num greater than  0
	{
		buffer[index--] = num % 10;  //split last digit from number

		num = num / 10;    //divide num by 10. num /= 10 also a valid one 
	}
}

void select_display()


void display_number(uint16_t num){
/*****************************************************************
 * @brief: Set a specified number on the screen of the seven segment display
 * @param num: A number from 0-9999 to be displayed on the screen.
 * **************************************************************/
	split_number(num);

	uint8_t index;

	for (index=3;index>=0;index++)
		
		set_number(buffer[index]);

}