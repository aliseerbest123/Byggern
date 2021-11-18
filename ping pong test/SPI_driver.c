/*
 * SPI_driver.c
 *
 * Created: 12.10.2021 13:38:56
 *  Author: ingvode
 */

#include "SPI_driver.h"

void spi_master_init()
{
	//port b pin 4
	//Set MISO output, all others input
	DDRB = (1 << PB5) | (1 << PB7) | (1 << PB4);

	//Enable SPI
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);

	PORTB &= ~(1 << PB6);
}

char SPI_SlaveReceive()
{
	//Wait for reception complete
	while (!(SPSR & (1 << SPIF)))
		;
	return SPDR;
}

void SPI_MasterTransmit(char cData)
{
	// 0000 0011
	printf("cData = %d\n", cData);
	SPDR = cData;
	printf("SPDR = %d\n", SPDR);
	_delay_ms(10);
	while (!(SPSR & (1 << SPIF)))
		;

	printf("SPDR = %d\n", SPDR);
	_delay_ms(10);
}

char SPI_transmission(char data)
{
	SPDR = data;
	while (!(SPSR & (1 << SPIF)))
		;
	return SPDR;
}