/*
 * ping pong test.c
 *
 * Created: 06.09.2021 11:22:01
 * Author : ingvode
 */

#define F_CPU 4915200 //clockspeed
#define BAUD 9600
#define MYUBRR F_CPU / 16 / BAUD - 1

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include <time.h>

#include "uart_driver.h"
#include "tests.h"
#include "Joystick.h"

int main(void)
{
	// enable interrupts // er dette nødvendig?
	SREG |= 1 << 7;
	DDRD &= ~(1 << PD2);
	// er dette nødvendig?

	// Init sram
	DDRA |= (1 << PA0);
	DDRA = 0xfff;
	DDRE |= 1 << PE1;
	DDRC |= 0b1111;

	DDRD |= 1 << PD7; // enable Read
	DDRD |= 1 << PD6; // enable write

	MCUCR |= (1 << SRE);
	SFIOR |= (1 << XMM2);
	// end init

	init_UART(MYUBRR);
	printf("\n\nRun main:\n");
	init_IO();
	OLED_init();
	CAN_init();
	menu_init();
	display();

	// menu
	while (1)
	{
		int16_t joy_y = joy_axis_binary_to_decimal(get_joystick_y(), 165);
		uint8_t threshold = 10;
		if (joy_y > threshold)
		{
			update_display_prev();
			_delay_ms(200);
		}
		else if (joy_y < -threshold)
		{
			update_display_next();
			_delay_ms(200);
		}
		else if (get_btn_right())
		{
			printf("button pressed\n");
			check_child();
			_delay_ms(500);
		}
		else if (get_btn_left())
		{
			go_back();
			_delay_ms(500);
		}
	}
}