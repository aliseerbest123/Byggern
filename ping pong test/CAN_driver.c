/*
 * CAN_drive.c
 *
 * Created: 12.10.2021 15:07:53
 *  Author: ingvode
 */ 

#include "CAN_driver.h"
#include <avr/io.h>
/*void CAN_init () {
	//printf("Decimal 20 = %d\n", decimal_to_binary(20));
	//uint8_t value ;
	spi_master_init(); // Initialize SPI
	MCP_reset(); // Send reset - command
	_delay_ms(10);
	// MCP_write(MCP_CANCTRL,MODE_CONFIG);
	
	// Self - test
	char value = MCP_read( MCP_CANSTAT );
	
	//	MCP_read ( MCP_CANSTAT , & value );
	if (( value & MODE_MASK ) != MODE_CONFIG ) {
		printf("\n\nMCP2515 is NOT in configuration mode after reset !\n");
		//return 1;
	}

	
	// MCP_write(MCP_CANCTRL,MODE_CONFIG);
	// MCP_write(MCP_CANCTRL, MODE_LOOPBACK);
	
	//SKAL LEGGE TIL MER HER VED NESTE LAB!
	
	//Setter Mode til slutt fordi vi vil først være i CONFIGURASTION MODE for å konfigurere diverse (Se side 59 om configuration mode).
	//MCP_write(MCP_CANCTRL,(char)(0b01000000)); //Set loopback mode.


	// More initialization

	// set loopback mode
	MCP_write(MCP_CANCTRL, (char)(1 << 6)); // REQOP1 = 6-bit // REQOP<2:0> = 010 = loopback mode
	
	_delay_ms(100);
	value = MCP_read( MCP_CANSTAT );
	if (( value & MODE_MASK ) != MODE_LOOPBACK ) {
		printf("\n\nMCP2515 is NOT in loop !\n");
	}
	
	printf("Read status: %d\n", MCP_read_status());
	
	
	
	// reset
	//MCP_reset();
	

	//return 0;
}*/

void CAN_init () {
	spi_master_init(); // Initialize SPI
	MCP_reset(); // Send reset - command
	_delay_ms(10);
	
	if ((  MCP_read( MCP_CANSTAT ) & MODE_MASK ) != MODE_CONFIG ) {
		printf("\n\nMCP2515 is NOT in configuration mode after reset !\n");
	}
	
	
	// uint8_t BRP = 3; //17;
	uint8_t BRP = 17;
	printf("MYBRR = %d\n", MYUBRR);
	

// 		BRP = 3
// 		SJW = 1 * T_q
// 		PROPAG/PRSEG = 6
// 		PHASE1 = 4
// 		PHASE2 = 4

	
	// MCP_write(1 | BRP, MCP_CNF1); // set Length = T_q and BRP
	// MCP_write( (1 << 7) | (0b100 << 3) | 0b110 , MCP_CNF2 ); // BTLMODE determined by CNF3, PS1 length and Propagation length
	// MCP_write(0b100, MCP_CNF3); // PS2 length

	/*MCP_write(0x05, MCP_CNF3);
	MCP_write(0b10110001, MCP_CNF2);
	MCP_write(0x03, MCP_CNF1);
*/
	
	MCP_bit_modify(MCP_CNF3,0x07,0b00000111);
	MCP_bit_modify(MCP_CNF2,0xFF,0b10101100);
	MCP_bit_modify(MCP_CNF1,0xFF,0b00000011);	
	// interrupts
	//MCUCR |= 1 << ISC01;
	//GICR |= 1 << INT0;
 	MCP_write(MCP_CANINTE, 0x01);
	
	MCP_write(MCP_CANCTRL, 0); // REQOP<2:0> = 000 = normal mode
		
	if (( MCP_read( MCP_CANSTAT ) & MODE_MASK ) != MODE_NORMAL ) {
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

void CAN_send_message(can_message msg){
	// send length
	MCP_write(MCP_TXB0CTRL+5, msg.length);
	// send ID
	MCP_write(MCP_TXB0CTRL+1, msg.ID >> 3);
	MCP_write(MCP_TXB0CTRL+2, (msg.ID & 0b111) << 5);
	
	// send data
	for (uint8_t i = 0; i < msg.length; i++) {
		MCP_write(CAN_data_address + i, msg.data[i]);
	}
	
	MCP_request_to_send(0x1);
}

int CAN_recive_message(can_message * msg, uint16_t id) {
	//_delay_ms(1);
	// can_message msg;
	
	if(!MCP_read(MCP_CANINTF) & MCP_RX0IF){
		return 0;	
	}
	
	// get ID
	int beginning_id = MCP_read(MCP_RXB0SIDH) << 3;
	int end_id = MCP_read(MCP_RXB0SIDL) >> 5;
	msg->ID = beginning_id | end_id;

	// get length
	msg->length = MCP_read(MCP_RXB0DLC);
	
	// get data
	for(uint8_t i = 0; i < msg->length; i++){
		msg->data[i] = MCP_read(MCP_RXB0D0 + i);
	}
	//printf("Ballong");
	//_delay_ms(10);
	MCP_bit_modify(MCP_CANINTF, MCP_RX0IF, 0);
	//printf("Abbor");
	//_delay_ms(10);	
// 	printf("My ID = %d\n", msg.ID);
// 	printf("My length = %d\n", msg.length);
// 	
// 	for(int i=0; i < msg.length; i++) printf("My val%d = %d\n", i, msg.data[i]);
	
	msg->data[msg->length] = '\0';

	//printf("Citrus");
	//_delay_ms(10);
	// return msg;
	
	return msg->length;
}


/*void CAN_recive_message(){
	_delay_ms(1);
	//while(!(MCP_CANINTF & (1 << MCP_RX1IF)));
	can_message recived;
	
	int beginning_id = MCP_read(MCP_RXB0SIDH) << 3;
	//int beginning_id = MCP_read(MCP_RXB0CTRL + 1) << 3;
	// beginning_id = (beginning_id << 3);
	int end_id = MCP_read(MCP_RXB0CTRL + 2) >> 5;
	
	printf("first = %d, second = %d\n", beginning_id, end_id);
	
	recived.ID = beginning_id | end_id;
	
	int length=MCP_read(MCP_RXB0CTRL+5);
	length &= 0b00001111;
	recived.length=length;
	
	int adr = 0x36; //0b00110000;
	for(int i=0;i<recived.length;++i){
		recived.data[i] = MCP_read(adr+i);
	}

	
	MCP_write((MCP_CANINTF<<2),0x0);
	

	printf("My ID = %d\n", recived.ID);
	printf("My length = %d\n",recived.length);
	for(int i=0;i<recived.length;++i) printf("My val%d = %d\n", i, recived.data[i]);
	
}*/

/*
uint8_t can_read(uint8_t adress) {
	uint8_t result ;
	
	PORTB &= ~(1 < < CAN_CS ); // Select CAN - controller
	
	SPI_transmission ( MCP_READ ); // Send read instruction
	SPI_transmission ( address ); // Send address
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




/*int decimal_to_binary(int n) {
	int return_value = 0;
	for (int i = 50; i >= 0; i--) {
		if (n >= pow(2, i)) {
			return_value += pow(10,i);
			n -= pow(2, i);
		}
	}
	return return_value;
}*/