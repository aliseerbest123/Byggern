#include "mcp2515.h"

#define F_CPU 4915200 //clockspeed
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef CAN_H_
#define CAN_H_

typedef struct can_message can_message;
struct can_message {
	
};

uint8_t mcp2515_init();
uint8_t can_read(uint8_t adress);
// void can_write();
// uint8_t can_request_to_send();
// void can_




#endif