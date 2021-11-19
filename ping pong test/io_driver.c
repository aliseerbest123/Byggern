/*
 * CFile1.c
 *
 * Created: 21.09.2021 09:51:57
 *  Author: ingvode
 */

#include "io_driver.h"


/**
 * \brief Coverts from binary between 0 and 255 to decimal value between 0 and 100
 *
 * \param value The value to convert from binary
 *
 * \retval The converted value between 0 and 100
 */
uint8_t slider_binary_to_decimal(uint8_t value)
{
	return (100 * value) / 255;
}

/**
 * \brief Convert the joystick values to the corresponding values in the range of 0-100
 *
 * \param value The value to convert from binary
 * \param elevated_zero The value to use as the zero point
 *
 * \retval The converted value between 0 and 100
 */
int16_t joy_axis_binary_to_decimal(uint8_t value, uint8_t elevated_zero)
{
	uint8_t devider = (value < elevated_zero) ? elevated_zero : (255 - elevated_zero);

	return (100 * (value - elevated_zero)) / devider;
}


/**
 * \brief Converts binary joystick values to the corresponding values in a joytick_vector 
 *
 * \param x value of the x-axis
 * \param y value of the y-axis
 *
 * \retval joystick_vector
 */
joystick_vector get_vector_direction_from_binary(uint8_t x, uint8_t y)
{
	joystick_vector vec;

	vec.x = joy_axis_binary_to_decimal(x, 166);
	vec.y = joy_axis_binary_to_decimal(y, 162);

	return vec;
};


/**
 * \brief Get the direction as enum from joystick_vector
 *
 * \param vec The joystick_vector to get the direction from
 * \param threshold The threshold to use for the direction
 *
 * \retval The direction as enum
 */
direction get_dir_from_joystick_vector(joystick_vector vec, uint8_t threshold)
{
	if (vec.x >= threshold && vec.x >= abs(vec.y))
		return RIGHT;
	if (vec.x <= -threshold && vec.x <= -abs(vec.y))
		return LEFT;
	if (vec.y >= threshold && vec.y >= abs(vec.x))
		return UP;
	if (vec.y <= -threshold && vec.y <= -abs(vec.x))
		return DOWN;
	return NEUTRAL;
};


/**
 * \brief Get left button
 */
uint8_t get_btn_left()
{
	if (PINB & (1 << PB0))
		return 1;
	return 0;
};

/**
 * \brief Get right button
 */
uint8_t get_btn_right()
{
	if (PINB & (1 << PB1))
		return 1;
	return 0;
};


/**
 * \brief Get left slider
 */
uint8_t get_slider_left()
{
	ADC[0] = 0b10000011;
	//_delay_ms(20);
	_delay_ms(1);
	uint8_t value = ADC[0];
	//_delay_ms(50);
	return value;
};

/**
 * \brief Get right slider
 */
uint8_t get_slider_right()
{
	ADC[0] = 0b10000010;
	_delay_ms(1);
	uint8_t value = ADC[0];
	//_delay_ms(1);
	return value;
};


/**
 * \brief Get x joystick
 */
uint8_t get_joystick_x()
{
	ADC[4] = 0b10000001;
	_delay_ms(20);
	uint8_t x_joy = ADC[0];
	_delay_ms(50);
	return x_joy;
};


/**
 * \brief Get y joystick
 */
uint8_t get_joystick_y()
{
	ADC[4] = 0b10000000;
	_delay_ms(20);
	uint8_t y_joy = ADC[0];
	_delay_ms(50);
	return y_joy;
};


/**
 * \brief Init for IO
 */
void init_IO()
{
	DDRD |= (1 << PD5); // set pin D5 to output

	// mode set CTC
	// WGMn3	WGMn2	WGMn1	WGMn0
	// 0		1		0		0		CTC		OCRnA	Imediate	Max
	//	TCCR1A = 0b00000000;
	//	TCCR1B = 0b00001010;

	TCCR1B |= (1 << WGM12);
	//prescaler
	TCCR1B |= (1 << CS10);
	TCCR1B &= ~(1 << CS11);
	TCCR1B &= ~(1 << CS12);

	TCCR1A |= (1 << COM1A0);

	OCR1A = 0b00000001;
};


/**
 * \brief Get when down button pressed
 */
int get_down()
{
	uint16_t x = get_joystick_x();
	uint16_t y = get_joystick_y();

	joystick_vector vev;
	direction dir;

	int temp = 0;

	vev = get_vector_direction_from_binary(x, y);
	dir = get_dir_from_joystick_vector(vev, 10);
	if (dir = DOWN)
	{
		temp = 0;
	}
	else
	{
		temp = 0;
	}
	return temp;
}