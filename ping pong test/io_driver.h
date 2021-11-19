/*
 * IncFile1.h
 *
 * Created: 21.09.2021 09:51:13
 *  Author: ingvode
 */

#define F_CPU 4915200 //clockspeed
#define BAUD 9600
#define MYUBRR F_CPU / 16 / BAUD - 1

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#define ADC ((char *)0x1400)

#ifndef INCFILE1_H_
#define INCFILE1_H_

typedef struct joystick_vector joystick_vector;
struct joystick_vector
{
	int16_t x;
	int16_t y;
};

typedef enum direction direction;
enum direction
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NEUTRAL,
};

//int16_t binary_to_decimal_range(uint8_t input_binary, uint8_t min_out, uint8_t max_out, uint8_t max_in, uint8_t elevated_zero);
uint8_t slider_binary_to_decimal(uint8_t value);
int16_t joy_axis_binary_to_decimal(uint8_t value, uint8_t elevated_zero);
joystick_vector get_vector_direction_from_binary(uint8_t x, uint8_t y);
direction get_dir_from_joystick_vector(joystick_vector vec, uint8_t threshold);
uint8_t get_btn_left();
uint8_t get_btn_right();
uint8_t get_slider_left();
uint8_t get_slider_right();
uint8_t get_joystick_x();
uint8_t get_joystick_y();
void init_IO();

#endif /* INCFILE1_H_ */