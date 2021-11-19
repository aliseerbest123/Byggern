/*
 * pong_driver.c
 *
 * Created: 16.11.2021 09:48:41
 *  Author: trygvru
 */

#include "pong_driver.h"

void play_pong(uint8_t x_speed_ball)
{
	printf("helllo gamers");
	PONG_PLAYER player_1 = {4, 64 / 2, 1, 10, 1};
	PONG_PLAYER player_2 = {128 - 1 - 4, 64 / 2, 1, 10, 1};

	PONG_BALL ball = {64 / 2, 128 / 2, 3, x_speed_ball, 1, 0, 0};

	while (1)
	{
		OLED_reset();
		OLED_reset_sram();

		printf("Slider = %d, %d\n", get_slider_left(), get_slider_right());
		printf("players = %d, %d\n", player_1.y_pos, player_2.y_pos);

		move_player(&player_1, get_slider_left());
		move_player(&player_2, get_slider_right());

		draw_player(player_1);
		draw_player(player_2);

		move_ball(&ball, player_1, player_2);
		draw_ball(ball);

		_delay_ms(10);
		char score[11];
		char score1[5];
		char score2[5];

		sprintf(score1, "%d", ball.score_p1);
		sprintf(score2, "%d", ball.score_p2);
		strcpy(score, score1);
		strcat(score, ":");
		strcat(score, score2);

		OLED_print_to_sram(score, 0, 3);
		OLED_draw_from_sram(1);
		_delay_ms(40);

		if (get_btn_left())
		{
			for (uint8_t i = n_highscore_elements*2; i > 1; i--)
				pong_score_sram[i] = pong_score_sram[i-2];
			pong_score_sram[0] = score1;
			pong_score_sram[1] = score2;
			return;
		}
	}
}

print_score_history() {
	OLED_reset_sram();
	_delay_ms(20);
	OLED_print_to_sram('Highscores', 0, 0);
	for (uint8_t i = 0; i < n_highscore_elements*2; i += 2)
	{
		char score[11];
		char score1[5];
		char score2[5];
		
		sprintf(score1, "%d", pong_score_sram[i]);
		sprintf(score2, "%d", pong_score_sram[i+1]);
		strcpy(score, score1);
		strcat(score, ":");
		strcat(score, score2);
		
		OLED_print_to_sram(score, i/2 + 1, 3);
		OLED_draw_from_sram(1);
	}
}