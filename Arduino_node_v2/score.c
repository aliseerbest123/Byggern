/*
 * score.c
 *
 * Created: 09.11.2021 15:23:40
 *  Author: trygvru
 */

#include "score.h"
#include <stdio.h>
#include "sam.h"

uint8_t score_handler()
{
	if (get_adc() < lower_score_threshold && !IR_blocked)
	{
		IR_blocked = 1;
		global_score++;
	}
	else if (get_adc() > upper_score_threshold && IR_blocked)
	{
		IR_blocked = 0;
	}
	return global_score;
}