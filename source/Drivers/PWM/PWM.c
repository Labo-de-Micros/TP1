//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//	@file		PWM.c											//
//	@brief		PWM driver. Advance Implementation				//
//	@author		Grupo 4											//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//							Headers								//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#include "./PWM.h"
#include "./FTM.h"
#include "../GPIO/gpio.h"

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//		CONSTANT AND MACRO DEFINITIONS USING #DEFINE 			//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#define INTERNAL_DUTY_CYCLE		90		//Creo que no cambia nada lo que ponga.

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//			ENUMERATIONS AND STRUCTURES AND TYPEDEFS	  		//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//						STATIC VARIABLES						//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

static PWM_callback_t callback;
static uint16_t mod;

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS W FILE LEVEL SCOPE//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

static void pwm_set_duty(FTM_t ftm, FTMChannel_t Chn, uint16_t  percent);
/*****************************************************************
 * @brief set the PWM duty_cycle percentage
 * @param ftm FTM module to use
 * @param Chn channel of the FTM module to use
 * @param percent percentage of the Duty Cycle. Values between 1 and 99
 *****************************************************************/

static void PWM_ISR(void);
/*****************************************************************
 * @brief: Interruption Function called by the FTM PWM.
 *****************************************************************/

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//					FUNCTION DEFINITIONS						//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void pwm_init(uint16_t modulus){
/*****************************************************************
 * @brief: Funcion para inicializar el modulo PWM, esta inicializa
 *          internamente el modulo de FTM.
 *****************************************************************/
	FTM_Init(FTM0, PWM_ISR);
	mod = modulus;
	//seteo los contadores
	FTM0->CNTIN = 0X00;
	FTM0->MOD = FTM_MOD_MOD(modulus);
	PORTC->PCR[PIN2NUM(PORTNUM2PIN(PC,1))]=0x00;
	PORTC->PCR[PIN2NUM(PORTNUM2PIN(PC,1))] |= PORT_PCR_DSE(1);
	PORTC->PCR[PIN2NUM(PORTNUM2PIN(PC,1))] |= PORT_PCR_MUX(4);
	PORTC->PCR[PIN2NUM(PORTNUM2PIN(PC,1))] |= PORT_PCR_IRQC(0);

	FTM0->CNT = 0X00;
	FTM_SetPrescaler(FTM0, FTM_PSC_x32);	//SI SE CAMBIA EL DEFINE DE PRESCALER HAY QUE CAMBIAR ESTO.
	FTM_SetModulus(FTM0, modulus);
	FTM_SetOverflowMode(FTM0, true);
	FTM_SetWorkingMode(FTM0, 0, FTM_mPulseWidthModulation);			// MSA  / B
	FTM_SetPulseWidthModulationLogic(FTM0, 0, FTM_lAssertedHigh);   // ELSA / B
	return;
}

void pwm_start_timer(uint16_t ticks,PWM_callback_t call){
/*****************************************************************
 * @brief: Utilizando el modulo de PWM para generar un timer preciso
 * 			sin la utilizacion de Systick(muy eficiente ya que no consume
 * 			recursos del procesador realizando pooling periodicamente)
 * @param ticks: Ticks que debe contar hasta finalizar e interrumpir.
 * 				Los ticks deben ser calculados de manera que:
 * 				Ticks = time * SYS_CLOCK / PreScaler
 * 				donde time es el tiempo en segundos.
 * @param call: callback to be called whe the amount of ticks are finished.
 *****************************************************************/
	//tener en cuenta que el tiempo se obtiene como ticks*preescaler/Sysclock == T
	FTM_StopClock(FTM0);
	FTM_ClearOverflowFlag(FTM0);
	FTM_SetModulus(FTM0,ticks);
	pwm_set_duty(FTM0,0,INTERNAL_DUTY_CYCLE);
	FTM_StartClock(FTM0);
	callback = call;
	return;
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//					LOCAL FUNCTION DEFINITIONS					//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

static void pwm_set_duty(FTM_t ftm, FTMChannel_t Chn, uint16_t  percent){
/*****************************************************************
 * @brief set the PWM duty_cycle percentage
 * @param ftm FTM module to use
 * @param Chn channel of the FTM module to use
 * @param percent percentage of the Duty Cycle. Values between 1 and 99
 *****************************************************************/
	//double duty_per=(percent/100.0)*(PWM_modulus+1);
	uint16_t PWM_duty= INTERNAL_DUTY_CYCLE *(mod+1)/100;
	FTM_SetCounter(ftm, Chn, PWM_duty);
	return;
}

static void PWM_ISR (void){
/*****************************************************************
 * @brief: Interruption Function called by the FTM PWM.
 *****************************************************************/
	FTM_ClearOverflowFlag(FTM0);
	FTM_StopClock(FTM0);
	callback();
	//FTM_ClearInterruptFlag(FTM0,FTM_CH_0);
	return;
}
