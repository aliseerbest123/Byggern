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
	void MCP_write(MCP_CANCTRL, (char)(1 << REQOP1)); // REQOP1 = 6-bit // REQOP<2:0> = 010 = loopback mode
	
	

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