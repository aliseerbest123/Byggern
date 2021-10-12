#include "can.h"

uint8_t mcp2515_init () {
	uint8_t value ;

	SPI_init () ; // Initialize SPI
	mcp2515_reset () ; // Send reset - command

	// Self - test
	mcp2515_read ( MCP_CANSTAT , & value );
	if (( value & MODE_MASK ) != MODE_CONFIG ) {
		printf("MCP2515 is NOT in configuration mode after reset !\n");
		return 1;
	}
	// More initialization

	// set loopback mode
	MCP_write(MCP_CANCTRL, (char)(1 << REQOP1)); // REQOP1 = 6-bit // REQOP<2:0> = 010 = loopback mode
	
	// reset
	can_reset();
	

	return 0;
}

uint8_t can_read(uint8_t adress) {
	uint8_t result ;
	
	PORTB &= ~(1 < < CAN_CS ); // Select CAN - controller
	
	SPI_write ( MCP_READ ); // Send read instruction
	SPI_write ( address ); // Send address
	result = SPI_read () ; // Read result
	
	PORTB |= (1 < < CAN_CS ); // Deselect CAN - controller
	
	return result ;
}

void can_write(char data, uint8_t adress) {
	PORTB &= ~(1 < < CAN_CS ); // Select CAN - controller

	SPI_write( MCP_WRITE ); // send write instruction
	SPI_write( adress );
	SPI_write( data );

	PORTB |= (1 < < CAN_CS ); // Deselect CAN - controller
}

void can_request_to_send(uint8_t instruction) {
	PORTB &= ~(1 < < CAN_CS ); // Select CAN - controller

	SPI_write( 0b10000000 | instruction );

	PORTB |= (1 < < CAN_CS ); // Deselect CAN - controller
}

void can_bit_modify(uint8_t adress, uint8_t mask, uint8_t data) {
	PORTB &= ~(1 < < CAN_CS ); // Select CAN - controller

	SPI_write( MCP_BITMOD ); // send BIT MODIFY INSTRUCTION
	SPI_write( adress );
	SPI_write( mask );
	SPI_write( data );

	PORTB |= (1 < < CAN_CS ); // Deselect CAN - controller
}

void can_reset() {
	PORTB &= ~(1 < < CAN_CS ); // Select CAN - controller

	PORTB &= ~(1 < < CAN_RESET); // it is highly recommended that the Reset command be sent (or the RESET pin be lowered) as part of the power-on initialization sequence

	PORTB |= (1 < < CAN_CS ); // Deselect CAN - controller
}

char can_read_status() {
	PORTB &= ~(1 < < CAN_CS ); // Select CAN - controller

	SPI_write( MCP_READ_STATUS ); // send BIT MODIFY INSTRUCTION
	char result_0 = SPI_read();
	char result_1 = SPI_read();

	PORTB |= (1 < < CAN_CS ); // Deselect CAN - controller

	if (result_0 != result_1 ) printf("An error occured while reading status of can.\n");
	
	return result_0;
}