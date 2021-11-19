/*
 * Joystick.c
 *
 * Created: 04.11.2021 11:26:33
 *  Author: ingvode
 */

#include "Joystick.h"

/*
 * \brief sends the joystick position over CAN
 *
 * \param void
 *
 * \retval void.
 */
void Joystick_send_pos()
{
	uint16_t x = get_joystick_x();
	uint16_t y = get_joystick_y();

	joystick_vector vev;

	vev = get_vector_direction_from_binary(x, y);

	can_message pos;
	pos.ID = 1;
	pos.length = 2;
	pos.data[0] = vev.x;
	pos.data[1] = vev.y;

	CAN_send_message(pos);
}