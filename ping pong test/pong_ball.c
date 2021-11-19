/*
 * pong_ball.c
 *
 * Created: 16.11.2021 09:34:01
 *  Author: trygvru
 */

#include "pong_ball.h"

void draw_ball(PONG_BALL ball)
{
	//OLED_draw_circle(ball.x_pos, ball.y_pos, ball.r, 0);
	OLED_draw_box(ball.x_pos - 1, ball.y_pos - 1, 3, 3, 0);
}

void move_ball(PONG_BALL *ball, PONG_PLAYER player_1, PONG_PLAYER player_2)
{
	int16_t temp_x = ball->x_pos + ball->x_speed;
	int16_t temp_y = ball->y_pos + ball->y_speed;

	if (temp_x < 0 || temp_x > 128)
	{
		ball->x_pos = 64;
		ball->y_pos = 32;
		ball->y_speed = 0;

		ball->score_p1 += temp_x > 128;
		ball->score_p2 += temp_x < 0;
	}

	if (collision(player_1, ball) || collision(player_2, ball))
		ball->x_speed *= -1;
	if (temp_y < 0 || temp_y > 64)
		ball->y_speed *= -1;

	ball->x_pos += ball->x_speed;
	ball->y_pos += ball->y_speed;
}

uint8_t collision(PONG_PLAYER player, PONG_BALL *ball)
{
	if (abs(ball->x_pos - player.x_pos) <= 1 && abs(ball->y_pos - player.y_pos - player.height / 2) < player.height)
	{
		ball->y_speed += (ball->y_pos - player.y_pos - player.height / 2) / 5;
		return 1;
	}
	return 0;
}