//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//	@file		Card_reader.h									//
//	@brief		Driver for usage of the card reader 			//
//	@author		Grupo	4										//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#ifndef _CARD_READER_H_
#define _CARD_READER_H_

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//							Headers								//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <stdbool.h>

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//		CONSTANT AND MACRO DEFINITIONS USING #DEFINE 		 	//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#define CARD_DATA_LENGTH		256		//Bits totales en la targeta, midiendolo llegue a que eran 256
#define CARD_DATA_REGISTER		32		//Uso registros de 32 bits para guardar la data.
#define CARD_CHARACTERS_LENGTH	40		//Segun la consigna.
#define CARD_PAN_LENGHT			19		//Maxima cantidad de caracteres de PAN
#define	CARD_EXP_LENGHT			4		//Maxima cantidad de caracteres de expiration date (YYMM)
#define CARD_SERVICE_LENGHT		3		//Maxima cantidad de caracteres de Service Code
#define CARD_PVKI_LENGHT		1		//Maxima cantidad de caracteres de PVKI
#define CARD_PVV_LENGHT			4		//Maxima cantidad de caracteres de PVV
#define CARD_CVV_LENGHT			3		//Maxima cantidad de caracteres de CVV
#define CARD_DATA_REGISTERS_LENGTH	(CARD_DATA_LENGTH/CARD_DATA_REGISTER)	//Cantidad de registros de 32 bits necesarios para guardar la CARD_DATA_LENGTH
													//IMPORTANTE: es necesario que CARD_DATA_LENGTH sea multiplo de 32. (No sirve otra cosa).
#if ((CARD_DATA_LENGTH%CARD_DATA_REGISTER) != 0)
#error CARD_DATA_LENGTH must be multiple of CARD_DATA_REGISTER! Cannot be otherwise! Please change CARD_DATA_LENGTH or CARD_DATA_REGISTER.
#endif

#if (CARD_PAN_LENGHT+CARD_EXP_LENGHT+CARD_EXP_LENGHT+CARD_SERVICE_LENGHT+CARD_PVKI_LENGHT+CARD_PVV_LENGHT+CARD_CVV_LENGHT) > CARD_CHARACTERS_LENGTH
#error The ammount of characters per card specified is not correct. Please check the defines:CARD_PAN_LENGHT, CARD_EXP_LENGHT, CARD_SERVICE_LENGHT, CARD_PVKI_LENGHT, CARD_PVV_LENGHT, CARD_CVV_LENGHT and CARD_CHARACTERS_LENGTH.
#endif

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			ENUMERATIONS AND STRUCTURES AND TYPEDEFS			//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

typedef void (*card_callback_t)(void);

typedef struct{
	uint64_t pan;
	uint16_t service_code;
	uint16_t PVV;
	uint16_t CVV;
    uint8_t exp_year;
	uint8_t exp_month;
    uint8_t PVKI;
}card_t;

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			FUNCTION PROTOTYPES WITH GLOBAL SCOPE				//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void card_init(card_callback_t callback);
/*****************************************************************
 * @brief: Initializer the card reader driver and its components
 * @param callback: callback to be called when a new card is readed.
 ****************************************************************/

card_t get_data(void);
/*****************************************************************
 * @brief: When the callback is called, one must obtain the data readed
 * 			by the driver, so this function returns the card data.
 * @return: A struct card_t containing the information readed in the card.
 ****************************************************************/

#endif	//_CARD_READER_H_
