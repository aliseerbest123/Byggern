/*
 * MCP2515_driver.c
 *
 * Created: 12.10.2021 13:47:53
 *  Author: ingvode
 */

#include "MCP2515_driver.h"

/**
 * \brief Sends a read signal to the CAN controller followed by an adress.
 *
 * \param adress adress of the data
 *
 * \retval data the data we wished to read
 */
char MCP_read(char adress)
{
	PORTB &= ~(1 << PB4); //Sets CS low

	//send read instruction
	SPI_transmission(MCP_READ);
	SPI_transmission(adress);

	char data = SPI_transmission(0xFF);

	//terminate read instruction
	PORTB |= (1 << PB4); // Sets CS high

	return data;
}

// 0000 0011 0000 1010

/**
 * \brief Sends a write signal to the CAN controller followed by an adress and at lease one byte of data
 *
 * \param adress adress we want to write to
 * \param text the data we want to send
 *
 * \retval void.
 */
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

/**
 * \brief initilize message transimssion
 *
 * \param TXB_number indicates which transmit buffer are enabled to send
 *
 * \retval void.
 */
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

/**
 * \brief reads the status of a pin
 *
 * \param void
 *
 * \retval status the status of the pin.
 */
char MCP_read_status()
{
	PORTB &= ~(1 << PB4);

	char status;
	SPI_transmission(MCP_READ_STATUS);
	status = SPI_transmission(0xFF);

	PORTB |= (1 << PB4);
	return status;
}

/*
 * \brief sets or clears individual bits in specific status and control registers
 *
 * \param adress adress of the register
 * \param mask which bit in the register we want to change
 * \param data what the modified bit is changed to
 *
 * \retval void.
 */
void MCP_bit_modify(char adress, char mask, char data)
{
	PORTB &= ~(1 << PB4);
	SPI_transmission(MCP_BITMOD);
	SPI_transmission(adress);
	SPI_transmission(mask);
	SPI_transmission(data);
	PORTB |= (1 << PB4);
}

/*
 * \brief Re-inizilizes the internal registers of the CAN controller and sets configuration mode
 *
 * \param void
 *
 * \retval void.
 */
void MCP_reset()
{
	PORTB &= ~(1 << PB4);
	SPI_transmission(MCP_RESET);
	PORTB |= (1 << PB4);
}