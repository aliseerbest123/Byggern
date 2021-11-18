/*
 * pwm_driver.c
 *
 * Created: 05.11.2021 09:51:41
 *  Author: trygvru
 */

#include "pwm_driver.h"
#include "sam.h"

void pwm_init()
{
	PIOC->PIO_PDR |= PIO_PDR_P19;	// Disable PIO
	PIOC->PIO_ABSR |= PIO_ABSR_P19; // AB Peripheral select
	// caution: before using PWM, enable PWM clock in Power Managment Controller (PMC)
	PMC->PMC_PCER1 |= PMC_PCER1_PID36;		  // PWM, Table 9-1
	PWM->PWM_CLK = DIVA | PWM_CLK_PREA(PREA); // set clock freq = clkA / DIVA
	// You can see channel ID = 5 for PMC in Table 9-1.
	REG_PWM_CMR5 = CLKA | PWM_CMR_CPOL;				   // Set clock properties. We here use clock A and flip signal
	REG_PWM_CPRD5 = channel_period_counter;			   // set period
	REG_PWM_CDTY5 = channel_period_counter * 1.5 / 20; // set duty cycle to mid value
	PWM->PWM_ENA = PWM_ENA_CHID5;
}

void pwm_set_relative_duty(int8_t percentage)
{
	if (percentage > 100)
		percentage = 100;
	else if (percentage < -100)
		percentage = -100;

	REG_PWM_CDTYUPD5 = ((1.5 + (percentage / 200.)) * channel_period_counter) / 20; // set duty between 1/20 and 2/20, where 1/20*channel_period = 1ms
}