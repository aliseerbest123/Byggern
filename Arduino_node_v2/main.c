/*
 * Arduino_node_v2.c
 *
 * Created: 26.10.2021 10:25:57
 * Author : trygvru
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sam.h"
#include "uart.h"
#include "printf-stdarg.h"
#include "can_controller.h"
#include "pwm_driver.h"
#include "motor_control.h"

int main(void)
{
	SystemInit(); // Initialize the SAM system

	WDT->WDT_MR = WDT_MR_WDDIS; // turn off watchdog

	configure_uart();
	adc_init();
	can_init_def_tx_rx_mb(0x00290457);


	PMC->PMC_PCER0 |= 1 << 11; // Enable peripheral clock for PWM // Usikker på om denne trengs eller ikke?

	pwm_init();

	enable_motor_encoder();
	calibrate_encoder();

	int g = 0;

	// init pin 41
	PIOC->PIO_PER |= PIO_PC9;
	PIOC->PIO_OER |= PIO_PC9;
	PIOC->PIO_SODR = PIO_PC9;
	// end init

	while (1)
		;
}