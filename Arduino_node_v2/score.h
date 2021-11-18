/*
 * score.h
 *
 * Created: 09.11.2021 15:23:11
 *  Author: trygvru
 */

#ifndef SCORE_H_
#define SCORE_H_

#include <stdint.h>

#define upper_score_threshold 3000
#define lower_score_threshold 1000

uint8_t global_score = 0;
uint8_t IR_blocked = 0;

uint8_t score_handler();

#endif /* SCORE_H_ */