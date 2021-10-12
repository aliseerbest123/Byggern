/*
 * CAN_drive.c
 *
 * Created: 12.10.2021 15:07:53
 *  Author: ingvode
 */ 

#include "CAN_driver.h"
#include <avr/io.h>
uint8_t mcp2515_init () {
	uint8_t value ;

	spi_master_init(); // Initialize SPI
	MCP_reset() ; // Send reset - command
	
	//SKAL LEGGE TIL MER HER VED NESTE LAB!
	
	//Setter Mode til slutt fordi vi vil først være i CONFIGURASTION MODE for å konfigurere diverse (Se side 59 om configuration mode).
	MCP_write(MCP_CANCTRL,(char)(0b01000000)); //Set loopback mode.

	// Self - test
	/*MCP_read ( MCP_CANSTAT , & value );
	if (( value & MODE_MASK ) != MODE_CONFIG ) {
		printf("MCP2515 is NOT in configuration mode after reset !\n");
		return 1;
	}*/
	// More initialization

	// set loopback mode
	MCP_write(MCP_CANCTRL, (char)(1 << 6)); // REQOP1 = 6-bit // REQOP<2:0> = 010 = loopback mode
	
	// reset
	//MCP_reset();
	

	return 0;
}

void CAN_send_message(can_message msg){
	
	//Filling transmit buffers
	MCP_write(MCP_TXB0CTRL+5,msg->length);
	MCP_write(MCP_TXB0CTRL+1,(msg->ID>>3));
	int temp=msg->ID;
	temp &=0b00000111;
	temp=(temp<<5);
	MCP_write(MCP_TXB0CTRL+2,(temp));
	//MCP_write(MCP_CANINTE,(0<<2));
	
	//Faktisk skriv data!!
	//Skriver bare et byte om gangen, dermed for løkke med lengde!
	int adr=0b00000000; //vi må velge en adresse!!
	for (int i=0;i<msg->length;++i)
	{
		MCP_write(adr+i,msg->data[i]);
	}
	
	//request to send
	MCP_request_to_send(0x1);
}

void CAN_recive_message(){
	int beginning_id=MCP_read(MCP_RXB0SIDH);
	int end_id=MCP_read((MCP_RXB0CTRL+2));
	end_id=(end_id>>5);
	beginning_id=(beginning_id<<3);
	can_message recived;
	recived->ID=(beginning_id|end_id);
	int length=MCP_read(MCP_RXB0CTRL+5);
	length &= 0b00001111;
	recived->length=length;
	
	int adr=0b00000000;
	for(int i=0;i<recived->length;++i){
		recived->data[i]=MCP_read(adr+i);
	}
	
	MCP_write((MCP_CANINTF<<2),0x0);
	
}

/*
uint8_t can_read(uint8_t adress) {
	uint8_t result ;
	
	PORTB &= ~(1 < < CAN_CS ); // Select CAN - controller
	
	SPI_MasterTransmit ( MCP_READ ); // Send read instruction
	SPI_MasterTransmit ( address ); // Send address
	result = SPI_SlaveReceive () ; // Read result
	
	PORTB |= (1 < < CAN_CS ); // Deselect CAN - controller
	
	return result ;
}

void can_write(char data, uint8_t adress) {
	PORTB &= ~(1 << CAN_CS ); // Select CAN - controller

	SPI_write( MCP_WRITE ); // send write instruction
	SPI_write( adress );
	SPI_write( data );

	PORTB |= (1 << CAN_CS ); // Deselect CAN - controller
}

void can_request_to_send(uint8_t instruction) {
	PORTB &= ~(1 << CAN_CS ); // Select CAN - controller

	SPI_write( 0b10000000 | instruction );

	PORTB |= (1 << CAN_CS ); // Deselect CAN - controller
}

void can_bit_modify(uint8_t adress, uint8_t mask, uint8_t data) {
	PORTB &= ~(1 << CAN_CS ); // Select CAN - controller

	SPI_write( MCP_BITMOD ); // send BIT MODIFY INSTRUCTION
	SPI_write( adress );
	SPI_write( mask );
	SPI_write( data );

	PORTB |= (1 < < CAN_CS ); // Deselect CAN - controller
}

void can_reset() {
	PORTB &= ~(1 << CAN_CS ); // Select CAN - controller

	PORTB &= ~(1 << CAN_RESET); // it is highly recommended that the Reset command be sent (or the RESET pin be lowered) as part of the power-on initialization sequence

	PORTB |= (1 << CAN_CS ); // Deselect CAN - controller
}

char can_read_status() {
	PORTB &= ~(1 << CAN_CS ); // Select CAN - controller

	SPI_write( MCP_READ_STATUS ); // send BIT MODIFY INSTRUCTION
	char result_0 = SPI_read();
	char result_1 = SPI_read();

	PORTB |= (1 << CAN_CS ); // Deselect CAN - controller

	if (result_0 != result_1 ) printf("An error occured while reading status of can.\n");
	
	return result_0;
}*/