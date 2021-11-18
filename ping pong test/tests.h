/*
 * IncFile1.h
 *
 * Created: 21.09.2021 09:47:37
 *  Author: ingvode
 */

#define F_CPU 4915200 //clockspeed
#define BAUD 9600
#define MYUBRR F_CPU / 16 / BAUD - 1

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include "io_driver.h"
#include "uart_driver.h"
#include "menu.h"
#include "CAN_driver.h"

#ifndef INCFILE1_H_
#define INCFILE1_H_

void SRAM_test(void);
void testPrintf(int i);
void enable_Latch();
void test_controller();
void test_slider();
void slider_setup_testing();
void test_OLED();
void print_IO();
void test_CAN();

#endif /* INCFILE1_H_ */