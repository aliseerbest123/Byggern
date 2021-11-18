/*
 * IncFile1.h
 *
 * Created: 21.09.2021 09:42:32
 *  Author: ingvode
 */

#define F_CPU 4915200 //clockspeed
#define BAUD 9600
#define MYUBRR F_CPU / 16 / BAUD - 1

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef INCFILE1_H_
#define INCFILE1_H_

void uart_send(unsigned char letter);
char uart_recieve();
void init_UART(unsigned int ubrr);

#endif /* INCFILE1_H_ */