/*
 * CAN_drive.c
 *
 * Created: 12.10.2021 15:07:53
 *  Author: ingvode
 */

#include "CAN_driver.h"
#include <avr/io.h>

void CAN_init()
{
	spi_master_init(); // Initialize SPI
	MCP_reset();	   // Send reset - command
	_delay_ms(10);

	if ((MCP_read(MCP_CANSTAT) & MODE_MASK) != MODE_CONFIG)
	{
		printf("\n\nMCP2515 is NOT in configuration mode after reset !\n");
	}

	MCP_bit_modify(MCP_CNF3, 0x07, 0b00000111); // set Length = T_q and BRP
	MCP_bit_modify(MCP_CNF2, 0xFF, 0b10101100); // BTLMODE determined by CNF3, PS1 length and Propagation length
	MCP_bit_modify(MCP_CNF1, 0xFF, 0b00000011); // PS2 length
	
	// interrupts
	MCP_write(MCP_CANINTE, 0x01);

	MCP_write(MCP_CANCTRL, 0); // REQOP<2:0> = 000 = normal mode

	if ((MCP_read(MCP_CANSTAT) & MODE_MASK) != MODE_NORMAL)
	{
		printf("\n\nMCP2515 is NOT in normal mode !\n");
	}

	// 	// set loopback mode
	// 	MCP_write(MCP_CANCTRL, (char)(1 << 6)); // REQOP1 = bit 6 // REQOP<2:0> = 010 = loopback mode
	//
	//
	// 	//_delay_ms(100);
	// 	if (( MCP_read( MCP_CANSTAT ) & MODE_MASK ) != MODE_LOOPBACK ) {
	// 		printf("\n\nMCP2515 is NOT in loop !\n");
	// 	}
}

void CAN_send_message(can_message msg)
{
	// send length
	MCP_write(MCP_TXB0CTRL + 5, msg.length);
	// send ID
	MCP_write(MCP_TXB0CTRL + 1, msg.ID >> 3);
	MCP_write(MCP_TXB0CTRL + 2, (msg.ID & 0b111) << 5);

	// send data
	for (uint8_t i = 0; i < msg.length; i++)
	{
		MCP_write(CAN_data_address + i, msg.data[i]);
	}

	MCP_request_to_send(0x1);
}

int CAN_recive_message(can_message *msg, uint16_t id)
{
	if (!MCP_read(MCP_CANINTF) & MCP_RX0IF)
	{
		return 0;
	}

	// get ID
	int beginning_id = MCP_read(MCP_RXB0SIDH) << 3;
	int end_id = MCP_read(MCP_RXB0SIDL) >> 5;
	msg->ID = beginning_id | end_id;

	// get length
	msg->length = MCP_read(MCP_RXB0DLC);

	// get data
	for (uint8_t i = 0; i < msg->length; i++)
	{
		msg->data[i] = MCP_read(MCP_RXB0D0 + i);
	}

	MCP_bit_modify(MCP_CANINTF, MCP_RX0IF, 0);

	msg->data[msg->length] = '\0'; // add null terminator

	return msg->length;
}