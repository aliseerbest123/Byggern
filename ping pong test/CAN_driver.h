/*
 * CAN_driver.h
 *
 * Created: 12.10.2021 15:09:03
 *  Author: ingvode
 */ 


#define F_CPU 4915200 //clockspeed
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include "MCP2515_driver.h"

#ifndef CAN_H_
#define CAN_H_

typedef struct can_message can_message;
struct can_message {
	uint16_t ID;
	char length;
	char data[8];
};

void CAN_init ();
void CAN_send_message(can_message msg);
void CAN_recive_message();
//uint8_t can_read(uint8_t adress);
// void can_write();
// uint8_t can_request_to_send();
// void can_




#endif