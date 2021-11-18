/*
 * CFile1.c
 *
 * Created: 21.09.2021 09:42:07
 *  Author: ingvode
 */

#include "uart_driver.h"

void uart_send(unsigned char letter)
{

	while (!(UCSR1A & (1 << UDRE1)))
	{
	}

	UDR1 = letter;
};

char uart_recieve()
{
	while (!(UCSR1A & (1 << RXC1)))
	{
	}

	return UDR1;
};

void init_UART(unsigned int ubrr)
{
	// set baud rate
	UBRR1H = (unsigned char)(ubrr >> 8);
	UBRR1L = (unsigned char)ubrr;

	UCSR1B = (1 << RXEN1) | (1 << TXEN1);

	UCSR1C = (1 << URSEL1) | (1 << USBS1) | (3 << UCSZ10);

	// setup printf
	fdevopen(uart_send, uart_recieve);
};