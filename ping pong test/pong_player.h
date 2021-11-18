/*
 * pong_player.h
 *
 * Created: 16.11.2021 09:17:26
 *  Author: trygvru
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef PONG_PLAYER_H_
#define PONG_PLAYER_H_

typedef struct PONG_PLAYER
{
	uint8_t x_pos;
	uint8_t y_pos;
	uint8_t width;
	uint8_t height;
} PONG_PLAYER;

void draw_player(PONG_PLAYER player);
void move_player(PONG_PLAYER *player, uint8_t slider);

#endif /* PONG_PLAYER_H_ */