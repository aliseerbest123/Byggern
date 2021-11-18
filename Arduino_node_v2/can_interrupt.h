/*
 * can_interrupt.h
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */
#include "motor_control.h"

#ifndef CAN_INTERRUPT_H_
#define CAN_INTERRUPT_H_
void CAN0_Handler(void);
void pid_init_can();

#endif /* CAN_INTERRUPT_H_ */