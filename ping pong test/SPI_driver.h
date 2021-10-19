/*
 * SPI_driver.h
 *
 * Created: 12.10.2021 13:39:28
 *  Author: ingvode
 */ 


#ifndef SPI_DRIVER_H_
#define SPI_DRIVER_H_

#include <avr/io.h>
#include <util/delay.h>

void spi_master_init();
char SPI_SlaveReceive();
void SPI_MasterTransmit(char cData);
char SPI_transmission(char data);

#endif /* SPI_DRIVER_H_ */