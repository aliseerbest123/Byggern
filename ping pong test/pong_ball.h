/*
 * pong_ball.h
 *
 * Created: 16.11.2021 09:33:39
 *  Author: trygvru
 */ 

#include "pong_player.h"

#ifndef PONG_BALL_H_
#define PONG_BALL_H_

typedef struct PONG_BALL
{
	uint8_t x_pos;
	uint8_t y_pos;
	uint8_t r;
	int8_t x_speed;
	int8_t y_speed;
	uint8_t score_p1;
	uint8_t score_p2;
} PONG_BALL;

void draw_ball(PONG_BALL ball);
void move_ball(PONG_BALL * ball, PONG_PLAYER player_1, PONG_PLAYER player_2);
uint8_t collision(PONG_PLAYER player, PONG_BALL * ball);


#endif /* PONG_BALL_H_ */