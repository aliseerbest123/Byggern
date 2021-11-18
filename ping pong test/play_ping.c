/*
 * play_ping.c
 *
 * Created: 16.11.2021 15:41:54
 *  Author: trygvru
 */ 

#include "play_ping.h"
#include "CAN_driver.h"
#include "io_driver.h"

void play_ping(int16_t ID) {
	int i = 0;
	int a = 0;
	
	while (1)
	{
		_delay_ms(1);
		uint16_t x = get_joystick_x();
		uint16_t y = get_joystick_y();
		
		joystick_vector j_vec = get_vector_direction_from_binary(x,y);
		
		can_message msg;
		msg.ID = ID;
		
		msg.data[0] = j_vec.x;
		msg.data[1] = j_vec.y;
		msg.data[2] = get_btn_left();
		msg.data[3] = get_btn_right();
		msg.data[4] = slider_binary_to_decimal(get_slider_left());
		msg.data[5] = slider_binary_to_decimal(get_slider_right());
		
		msg.length = 6;
		CAN_send_message(msg);
		
		_delay_ms(50);
		if (get_btn_left())
		{
			break;
		}
	}
}