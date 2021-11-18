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

	while (1);

	// alt under her kan fjernes

// 	while (1)
// 	{
// 		CAN_MESSAGE msg;
// 		can_receive(&msg, 0);
// 		int8_t x_joy = msg.data[0];
// 		int32_t ref = (x_joy + 128) * 16000 / 256 + 1000;
// 		// printf("x_joy = %d\r", x_joy);
// 		// printf("ref = %d\r", ref);

// 		//printf("joy =%d", x_joy);
// 		// set_motor(0);
// 		//set_motor(pid_controller(ref, feedback_encoder(), &my_pid ));
// 		// feedback_encoder();
// 		// _delay_ms(10);
// 		g = (g + 1) % 10;
// 		if (g == 1)
// 			printf("Value = %d\r", feedback_encoder());
// 		//pwm_set_relative_duty(100);
// 	}

// 	while (0)
// 	{
// 		CAN_MESSAGE msg;
// 		can_receive(&msg, 0);
// 		int8_t x_joy = msg.data[0];
// 		int8_t y_joy = msg.data[1];
// 		joystick_movement(y_joy);
// 		_delay_ms(10);
// 		pwm_set_relative_duty(x_joy);
// 	}
// 	//test_motor();

// 	uint8_t i = 0;

// 	while (0)
// 	{

// 		CAN_MESSAGE msg;
// 		msg.id = i;
// 		msg.data_length = 5;
// 		strcpy(msg.data, "Alise");

// 		printf("Send msg %d (%d : %s):", i++, msg.id, msg.data);

// 		can_send(&msg, 0);

// 		_delay_ms(500);

// 		printf(" \r");
// 		// printf("Message '%s' was sent\n", msg.data);
// 	}

// 	uint16_t adc_value = get_adc();

// 	while (1)
// 	{
// 		i = (++i) % 10;

// 		CAN_MESSAGE msg;
// 		can_receive(&msg, 0);
// 		int8_t x_joy = msg.data[0];
// 		int8_t y_joy = msg.data[1];
// 		if (i == 1)
// 		{
// 			printf("length: %d\n", msg.data_length);
// 			printf("id: %d\n", msg.id);
// 			printf("Joystick (x, y): (%d, %d)\n", x_joy, y_joy);
// 		}
// 		pwm_set_relative_duty(x_joy);
// 		joystick_movement(y_joy);

// 		adc_value = digital_filter(adc_value, get_adc(), 1);
// 		uint8_t score = score_handler();
// 		if (i % 2)
// 			printf("ADC = %d, \t Score = %d\n", adc_value, score);
// 		_delay_ms(50);
// 	}

// 	while (1)
// 	{
// 		CAN_MESSAGE msg;
// 		msg.id = 100;
// 		printf("Pre ID = %d\r", msg.id);
// 		if (!can_receive(&msg, 0))
// 		{
// 			printf("Message received\r");
// 		}
// 		else
// 		{
// 			//printf("Expect 255, ID = %d\r", msg.id);
// 		}

// 		int a = 0;
// 		for (int i = 1; i < 2000000; i++)
// 		{
// 			a = 1;
// 		}
// 		_delay_ms(1000);
// 	}

// 	while (1)
// 	{
// 	}
// }

// void _delay_ms(int t)
// {
// 	for (int a = 0; a < 20000 * t; a++)
// 	{
// 		__NOP();
// 	}
}