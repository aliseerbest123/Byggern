/*
 * motor_control.h
 *
 * Created: 11.11.2021 11:27:28
 *  Author: ingvode
 */ 

#include <sam.h>

#ifndef MOTOR_CONTROL_H_
#define MOTOR_CONTROL_H_

typedef struct PID_DATA PID_DATA;
struct PID_DATA {
	int32_t last_state_value, integral_sum_error, p, i, d;
};


void enable_motor_encoder();
void test_motor();
void joystick_movement(int8_t x_joy);
uint16_t feedback_encoder();
void calibrate_encoder();
int16_t pid_controller(int32_t reference, int32_t state, PID_DATA* pid);
void set_motor(int32_t speed);

PID_DATA pid_init(int32_t p, int32_t i, int32_t d);
#endif /* MOTOR_CONTROL_H_ */