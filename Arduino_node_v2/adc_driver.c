/*
 * adc_driver.c
 *
 * Created: 09.11.2021 11:12:06
 *  Author: trygvru
 */

#include "adc_driver.h"
#include <stdio.h>
#include "sam.h"

/**
 * \brief ADC initialization function
 *
 * \param void
 *
 * \retval void
 */
void adc_init()
{
	PMC->PMC_PCER1 |= PMC_PCER1_PID37; // ADC Instance ID = 37
	ADC->ADC_MR |= ADC_MR_FREERUN; // ADC Mode Register;	"Freerun mode is the easiest"
	ADC->ADC_CHER |= ADC_CHER_CH10; // Channel Enable Register;	Apparently Channel 10 = A8
}

/**
 * \brief ADC read function
 *
 * \param void
 *
 * \retval void
 */
uint16_t get_adc()
{
	return ADC->ADC_CDR[10];
}

/**
 * \brief Works as a simple low pas filter
 *
 * \param prev_value: The previous value
 * \param new_value: The new value
 * \param strength: The strength of the filter
 *
 * \retval 
 */
uint16_t digital_filter(uint16_t prev_value, uint16_t new_value, uint32_t strength)
{
	return (strength * prev_value + new_value) / (strength + 1);
}