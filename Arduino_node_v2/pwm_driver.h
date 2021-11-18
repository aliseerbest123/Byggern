/*
 * pwm_driver.h
 *
 * Created: 05.11.2021 09:51:31
 *  Author: trygvru
 */

#ifndef PWM_DRIVER_H_
#define PWM_DRIVER_H_

#include <stdint.h>

#define DIVA 42
#define PREA 2

#define CLKA 0b1011

#define channel_period_counter 10000

void pwm_init();
void pwm_set_relative_duty(int8_t percentage);

#endif /* PWM_DRIVER_H_ */