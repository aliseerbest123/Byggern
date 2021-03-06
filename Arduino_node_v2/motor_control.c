/*
 * CFile1.c
 *
 * Created: 11.11.2021 11:26:39
 *  Author: ingvode
 */

#include "motor_control.h"

/**
 * \brief Initializes the motor control
 *
 * \param void
 *
 * \retval void
 */
void enable_motor_encoder()
{
	// set EN-port on shield, MJ1

	// PIOD P10 = DIR
	// port D p9 = EN
	// port D p2 = select
	// port D p1 = !RST
	// port D p0 = !OE

	PMC->PMC_PCER1 |= PMC_PCER1_PID38;
	PMC->PMC_PCER0 |= PMC_PCDR0_PID14;
	PMC->PMC_PCER0 |= PMC_PCDR0_PID13;
	DACC->DACC_CHER |= DACC_CHER_CH1;
	DACC->DACC_MR |= DACC_MR_USER_SEL_CHANNEL1;

	//Enable MJ1 pins
	PIOD->PIO_PER |= PIO_PER_P10; //DIR
	PIOD->PIO_PER |= PIO_PER_P9;  //Enable
	PIOD->PIO_PER |= PIO_PER_P2;  //Sel
	PIOD->PIO_PER |= PIO_PER_P1;  //!RST
	PIOD->PIO_PER |= PIO_PER_P0;  //!OE

	//Define MJ1 pins, output
	PIOD->PIO_OER |= PIO_OER_P10; //DIR
	PIOD->PIO_OER |= PIO_OER_P9;  //Enable
	PIOD->PIO_OER |= PIO_OER_P2;  //Sel
	PIOD->PIO_OER |= PIO_OER_P1;  //!RST
	PIOD->PIO_OER |= PIO_OER_P0;  //!OE

	//Enable MJ2 pins
	PIOC->PIO_PER |= PIO_PER_P1; //D0
	PIOC->PIO_PER |= PIO_PER_P2; //D1
	PIOC->PIO_PER |= PIO_PER_P3; //D2
	PIOC->PIO_PER |= PIO_PER_P4; //D3
	PIOC->PIO_PER |= PIO_PER_P5; //D4
	PIOC->PIO_PER |= PIO_PER_P6; //D5
	PIOC->PIO_PER |= PIO_PER_P7; //D6
	PIOC->PIO_PER |= PIO_PER_P8; //D7

	//Define MJ2 input
	PIOC->PIO_ODR |= PIO_ODR_P1;
	PIOC->PIO_ODR |= PIO_ODR_P2;
	PIOC->PIO_ODR |= PIO_ODR_P3;
	PIOC->PIO_ODR |= PIO_ODR_P4;
	PIOC->PIO_ODR |= PIO_ODR_P5;
	PIOC->PIO_ODR |= PIO_ODR_P6;
	PIOC->PIO_ODR |= PIO_ODR_P7;
	PIOC->PIO_ODR |= PIO_ODR_P8;
}

/**
 * \brief Just a test for the motor control
 */

void test_motor()
{
	PIOD->PIO_SODR |= PIO_PER_P9;
	PIOD->PIO_SODR |= PIO_PER_P10;
	DACC->DACC_CDR = DACC_CDR_DATA(5000);
}

/**
 * \brief Let the motor go in the same direction as joystick
 *
 * \param x_joy The x-axis of the joystick
 *
 * \retval void
 */

void joystick_movement(int8_t x_joy)
{
	PIOD->PIO_SODR |= PIO_SODR_P9;
	if (x_joy > 50)
	{
		PIOD->PIO_SODR |= PIO_SODR_P10;
		DACC->DACC_CDR = DACC_CDR_DATA(6000);
	}
	else if (x_joy < -50)
	{
		PIOD->PIO_CODR |= PIO_CODR_P10;
		DACC->DACC_CDR = DACC_CDR_DATA(6000);
	}
	else
	{
		PIOD->PIO_CODR |= PIO_CODR_P9;
	}
}

/**
 * \brief gives the motor position
 *
 * \param void
 *
 * \retval motor position, typicaly between 0 and 16000
 */
uint16_t feedback_encoder()
{
	PIOD->PIO_SODR |= PIO_SODR_P1;
	PIOD->PIO_CODR |= PIO_CODR_P0;
	PIOD->PIO_CODR |= PIO_CODR_P2;
	_elay_ms(25);
	uint8_t msb = 0b00000000;
	msb |= PIOC->PIO_PDSR;
	PIOD->PIO_SODR |= PIO_SODR_P2;
	_elay_ms(25);
	uint8_t lsb = 0b00000000;
	lsb |= PIOC->PIO_PDSR;
	int16_t number = ((msb << 8) | (lsb));
	if (number < 0)
	{
		number = (~number) - 1;
	}
	PIOD->PIO_SODR |= PIO_SODR_P0;
	printf("encoder: %d\n", number);
	return number;
}

void calibrate_encoder()
{
	PIOD->PIO_SODR |= PIO_PER_P9;
	PIOD->PIO_SODR |= PIO_PER_P10;
	DACC->DACC_CDR = DACC_CDR_DATA(15000);
	_elay_ms(500);
	PIOD->PIO_CODR |= PIO_CODR_P9;
	PIOD->PIO_CODR |= PIO_CODR_P1;
	_elay_ms(25);
}

int16_t slider_to_encoder(int8_t slider)
{
	return slider * 70;
}

int16_t pid_controller(int32_t reference, int32_t state, PID_DATA *pid)
{
	int32_t error = reference - state;

	int32_t p_term;
	if (error > INT32_MAX / (pid->p + 1))
		p_term = INT32_MAX;
	else if (error < -INT32_MAX / (pid->p + 1))
		p_term = -INT32_MAX;
	else
		p_term = pid->p * error;

	int32_t i_term;
	int32_t temp_pid = pid->integral_sum_error + error;
	if (temp_pid > INT32_MAX / (2 * (pid->i + 1)))
	{
		i_term = INT32_MAX / 2;
		pid->integral_sum_error = INT32_MAX / (2 * (pid->i + 1));
	}
	else if (temp_pid < -INT32_MAX / (2 * (pid->i + 1)))
	{
		i_term = -INT32_MAX / 2;
		pid->integral_sum_error = -INT32_MAX / (2 * (pid->i + 1));
	}
	else
	{
		pid->integral_sum_error = temp_pid;
		i_term = pid->i * pid->integral_sum_error;
	}

	int32_t d_term = pid->d * (pid->last_state_value - state);
	pid->last_state_value = state;

	int32_t return_value = (p_term + i_term + d_term) / 128;

	if (return_value > INT16_MAX)
		return_value = INT16_MAX;
	else if (return_value < -INT16_MAX)
		return_value = -INT16_MAX;

	return return_value;
}

void set_motor(int32_t speed)
{
	uint16_t threshold = 0;
	PIOD->PIO_SODR |= PIO_PER_P9;
	if (speed > 0)
		PIOD->PIO_CODR |= PIO_CODR_P10;
	else
		PIOD->PIO_SODR |= PIO_PER_P10;

	DACC->DACC_CDR = DACC_CDR_DATA(abs(speed) + 400);
}

PID_DATA pid_init(int32_t p, int32_t i, int32_t d)
{
	PID_DATA pid;
	pid.p = p;
	pid.i = i;
	pid.d = d;

	pid.integral_sum_error = 0;
	pid.last_state_value = 0;

	return pid;
}

void _elay_ms(int t)
{
	for (int a = 0; a < 20000 * t; a++)
	{
		__NOP();
	}
}