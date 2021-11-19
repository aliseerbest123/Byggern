/*
 * pong_driver.h
 *
 * Created: 16.11.2021 09:48:54
 *  Author: trygvru
 */

#define pong_score_sram ((char *)0x2000) // Start address for the SRAM

#define n_highscore_elements 5

#include "pong_ball.h"

#ifndef PONG_DRIVER_H_
#define PONG_DRIVER_H_

void play_pong(uint8_t x_speed_ball);
void print_score_history();

#endif /* PONG_DRIVER_H_ */