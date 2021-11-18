/*
 * adc_driver.h
 *
 * Created: 09.11.2021 11:12:18
 *  Author: trygvru
 */ 


#ifndef ADC_DRIVER_H_
#define ADC_DRIVER_H_

#include <stdint.h>

void adc_init();
uint16_t get_adc();
uint16_t digital_filter(uint16_t prev_value, uint16_t new_value, uint32_t strength);


#endif /* ADC_DRIVER_H_ */