/*
 * CAN_drive.c
 *
 * Created: 12.10.2021 15:07:53
 *  Author: ingvode
 */ 

#include "CAN_driver.h"
#include <avr/io.h>
void CAN_init () {
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

	
	/*MCP_write(MCP_CANCTRL,MODE_CONFIG);
	MCP_write(MCP_CANCTRL, MODE_LOOPBACK);*/
	
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
}

void CAN_send_message(can_message msg){
	
	//Filling transmit buffers
	MCP_write(MCP_TXB0CTRL+5,msg.length);
	MCP_write(MCP_TXB0CTRL+1,(msg.ID>>3));
	int temp=msg.ID;
	temp &=0b00000111;
	temp=(temp<<5);
	MCP_write(MCP_TXB0CTRL+2,(temp));
	//MCP_write(MCP_CANINTE,(0<<2));
	
	//Faktisk skriv data!!
	//Skriver bare et byte om gangen, dermed for løkke med lengde!
	int adr=0b00110000; //vi må velge en adresse!!
	/*for (int i=0;i<msg.length;++i)
	{
		MCP_write(adr+i,msg.data[i]);
	}*/
	
	MCP_write(0b00010000,msg.data[0]);
	MCP_write(0b00010001,msg.data[1]);
	MCP_write(0b00010010,msg.data[2]);
	MCP_write(0b00010011,msg.data[3]);
	MCP_write(0b00010100,msg.data[4]);
	MCP_write(0b00010101,msg.data[5]);
	MCP_write(0b00010110,msg.data[6]);
	MCP_write(0b00010111,msg.data[7]);
	
	//request to send
	MCP_request_to_send(0x1);
}

void CAN_recive_message(){
	int beginning_id=MCP_read(MCP_RXB0SIDH);
	printf("Begin: %d\n",beginning_id);
	int end_id=MCP_read((MCP_RXB0CTRL+2 ));
	end_id=(end_id>>5);
	printf("End: %d\n",end_id);
	beginning_id=(beginning_id<<3);
	can_message recived;
	recived.ID=(beginning_id|end_id);
	int length=MCP_read(MCP_RXB0CTRL+5);
	length &= 0b00001111;
	recived.length=length;
	
	int adr=0b00110000;
	/*for(int i=0;i<recived.length;++i){
		recived.data[i]=MCP_read(adr+i);
	}*/
	
	recived.data[0]=MCP_read(0b00010000);
	recived.data[1]=MCP_read(0b00010001);
	recived.data[2]=MCP_read(0b00010010);
	recived.data[3]=MCP_read(0b00010011);
	recived.data[4]=MCP_read(0b00010100);
	recived.data[5]=MCP_read(0b00010101);
	recived.data[6]=MCP_read(0b00010110);
	recived.data[7]=MCP_read(0b00010111);
	
	MCP_write((MCP_CANINTF<<2),0x0);
	

	printf("My ID = %d\n", recived.ID);
	printf("My length = %d\n",recived.length);
	printf("My val0= %d\n", recived.data[0]);
	printf("My val1= %d\n", recived.data[1]);
	printf("My val2= %d\n", recived.data[2]);
	printf("My val3= %d\n", recived.data[3]);
	printf("My val4= %d\n", recived.data[4]);
	printf("My val5= %d\n", recived.data[5]);
	printf("My val6= %d\n", recived.data[6]);
	printf("My val7= %d\n", recived.data[7]);
	//for (uint8_t i = 0; i < 8; i++) printf("Value = %d\n",recived.data[i]);
	
}

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




int decimal_to_binary(int n) {
	int return_value = 0;
	for (int i = 50; i >= 0; i--) {
		if (n >= pow(2, i)) {
			return_value += pow(10,i);
			n -= pow(2, i);
		}
	}
	return return_value;
}