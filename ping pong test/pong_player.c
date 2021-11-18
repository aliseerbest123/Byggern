/*
 * pong_player.c
 *
 * Created: 16.11.2021 09:24:05
 *  Author: trygvru
 */ 

#include "pong_player.h"
#include "OLED_driver.h"

void draw_player(PONG_PLAYER player) {
	OLED_draw_box(player.x_pos, player.y_pos, player.width, player.height, 0);
}
void move_player(PONG_PLAYER * player, uint8_t slider) {
	player->y_pos = ( slider * (64 - player->height) ) / 256;
}