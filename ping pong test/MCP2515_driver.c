/*
 * MCP2515_driver.c
 *
 * Created: 12.10.2021 13:47:53
 *  Author: ingvode
 */

#include "MCP2515_driver.h"

//READ
char MCP_read(char adress)
{
	PORTB &= ~(1 << PB4);

	//send read instruction
	SPI_transmission(MCP_READ);
	SPI_transmission(adress);

	char data = SPI_transmission(0xFF);

	//terminate read instruction
	PORTB |= (1 << PB4);

	return data;
}

//WRITE
void MCP_write(char adress, char text)
{
	PORTB &= ~(1 << PB4);

	SPI_transmission(MCP_WRITE);
	SPI_transmission(adress);
	_delay_ms(10);
	SPI_transmission(text);

	//terminate
	PORTB |= (1 << PB4);
}

//REQUEST TO SEND
void MCP_request_to_send(char TXB_number)
{
	PORTB &= ~(1 << PB4);
	if ((TXB_number < 0) | TXB_number > 0b111)
	{
		printf('invalid number');
	}
	else
	{
		char out = 0x80 | TXB_number;
		SPI_transmission(out);
	}

	PORTB |= (1 << PB4);
}

//READ STATUS
char MCP_read_status()
{
	PORTB &= ~(1 << PB4);

	char status;
	SPI_transmission(MCP_READ_STATUS);
	status = SPI_transmission(0xFF);

	PORTB |= (1 << PB4);
	return status;
}

//BIT MODIFY
void MCP_bit_modify(char adress, char mask, char data)
{
	PORTB &= ~(1 << PB4);
	SPI_transmission(MCP_BITMOD);
	SPI_transmission(adress);
	SPI_transmission(mask);
	SPI_transmission(data);
	PORTB |= (1 << PB4);
}

//RESET
void MCP_reset()
{
	PORTB &= ~(1 << PB4);
	SPI_transmission(MCP_RESET);
	PORTB |= (1 << PB4);
}