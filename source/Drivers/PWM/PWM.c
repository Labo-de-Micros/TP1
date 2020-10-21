//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//	@file		PWM.c											//
//	@brief		PWM driver. Advance Implementation			//
//	@author		Grupo 4											//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//							Headers								//
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#include "./Drivers/PWM/PWM.h"











void PWM_Init (uint16_t modulus, FTM_Prescal_t prescaler, uint16_t duty)
{

	PWM_modulus=modulus;
	PWM_duty=duty;
	//seteo los contadores

	FTM0->CNTIN = 0X00;
	FTM0->MOD = FTM_MOD_MOD(PWM_modulus);

	PORTC->PCR[PIN2NUM(PORTNUM2PIN(PC,1))]=0x00;
	PORTC->PCR[PIN2NUM(PORTNUM2PIN(PC,1))] |= PORT_PCR_DSE(1);
	PORTC->PCR[PIN2NUM(PORTNUM2PIN(PC,1))] |= PORT_PCR_MUX(4);
	PORTC->PCR[PIN2NUM(PORTNUM2PIN(PC,1))] |= PORT_PCR_IRQC(0);

	//	PORT_Configure2 (PORTC,1,UserPCR);

	FTM0->CNT = 0X00;


	FTM_SetPrescaler(FTM0, prescaler);
	FTM_SetModulus(FTM0, PWM_modulus);
	FTM_SetOverflowMode(FTM0, true);
	//FTM_SetInterruptMode (FTM0,FTM_CH_0, true);
	FTM_SetWorkingMode(FTM0, 0, FTM_mPulseWidthModulation);			// MSA  / B
	FTM_SetPulseWidthModulationLogic(FTM0, 0, FTM_lAssertedHigh);   // ELSA / B
	set_DutyPWM(FTM0, 0, PWM_duty);
	//FTM_SetCounter(FTM0, 0, PWM_duty);
	//FTM_StartClock(FTM0);
}

void PWM_ISR (void)
{

	callBack();
	//FTM_ClearOverflowFlag (FTM0);
	//FTM_ClearInterruptFlag(FTM0,FTM_CH_0);
	//gpioToggle(TEST);
	//set_DutyPWM(FTM0, 0, percent);
	//percent +=10;
	//percent= percent%100;
	//PWM_duty %= PWM_modulus;
}



void pwm_start_timer(uint16_t ticks,uint16_t duty_cycle,FTM_callback_t callback) //tener en cuenta que el tiempo se obtiene como ticks*preescaler/Sysclock == T
{
	FTM_StopClock(FTM0);
	FTM_ClearOverflowFlag(FTM0);
	FTM_SetModulus(FTM0,ticks);
	set_DutyPWM(FTM0,0,duty_cycle);

	FTM_StartClock(FTM0);

}


void set_DutyPWM(FTM_t ftm,FTMChannel_t Chn, uint16_t  percent)
{
	//double duty_per=(percent/100.0)*(PWM_modulus+1);
	PWM_duty=percent*(PWM_modulus+1)/100;
	FTM_SetCounter(ftm, Chn, PWM_duty);
}