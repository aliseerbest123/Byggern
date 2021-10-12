/*
 * ping pong test.c
 *
 * Created: 06.09.2021 11:22:01
 * Author : ingvode
 */ 

#define F_CPU 4915200 //clockspeed
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include <time.h>

#include "uart_driver.h"
//#include "io_driver.h"
#include "tests.h"


int main(void)
{
	time_t t = time(NULL);
	srand((unsigned) time(&t));
	
	// Init sram
	DDRA |= (1 << PA0);
	DDRA = 0xfff;
	DDRE |= 1 << PE1;
	DDRC |= 0b1111;
	
	DDRD |= 1 << PD7; // enable Read
	DDRD |= 1 << PD6; // enable write
	
	MCUCR |= (1 << SRE);
	SFIOR |= (1<<XMM2);
	// end init
	
	init_UART(MYUBRR);
	init_IO();
	OLED_init();
	
	//SRAM_test();


	printf("\nRun main, code = %i\n", rand());
	
	//OLED_goto_line(0);
	//OLED_print("1234567891234567");
	//OLED_pos(4, 0);
	//OLED_print("how are you");
	
	//OLED_print_to_sram("AB", 1, 0);
	//OLED_draw_from_sram();
	OLED_draw_point_sram(127,64);
	menu_init();
	display();
	
	while (1)
	{
		/*for (int i = 0; i < 8; i++)
		{
			//OLED_goto_line(i);0123456789ABCDEF
			OLED_print_to_sram("JESUS<3DEG", i, 0);
			OLED_draw_from_sram();
			_delay_ms(500);
		}*/
		if (get_btn_right())
		{
			check_child();
			//update_display_prev();
			_delay_ms(200);
		}
		else if (get_btn_left())
		{
			update_display_next();
			_delay_ms(200);
		}
	}
	
	//spi_slave_init();
	/*while (1)
	{
		PORTB=0b1000000;
	}
	PORTB=(1<<PB6);
	//SPI_SlaveReceive();*/
	
	while (1) {
		for (int i = 0; i < 8; i++)
		{
			//OLED_goto_line(i);0123456789ABCDEF
			OLED_print_to_sram("JESUS<3DEG", i, 0);
			OLED_draw_from_sram();
			_delay_ms(500);
		}
		for (int i = 0; i < 8; i++)
		{
			//OLED_goto_line(i);
			OLED_clear_line_sram(i);
			OLED_print_emoji_sram(4, i, 50);
			OLED_draw_from_sram();
			_delay_ms(500);
		}
	}
	
	OLED_draw_from_sram();
	
	//OLED_set_brightness(100);
	//menu_init();
	
	
	//display();
	
	//Opp når du klikker høyre, ned når du klikker venstre
	while (0)
	{
		if (get_btn_right())
		{
			//check_child();
			update_display_prev();
			_delay_ms(200);
		}
		else if (get_btn_left())
		{
			update_display_next();
			_delay_ms(200);
		}
	}
	
	
	
	
	/*for (uint8_t i = 0; ; i++) {
		OLED_invert(i%2);
		_delay_ms(200);
	}*/
	
	//test_OLED();
	
	//uint8_t i = 0;
	
	//adresse 0x1567;
	
	//volatile char *ext_ram = (char *) 0x1400;
	
	/*while(1){
	ext_ram[5] = 0b101010101010;}*/
	
	
	// SRAM_test();
	
	//DDRD |= 1 << PD5;
	
	//while(1) {
	//	PORTD ^= 1 << 5;
	//	_delay_ms(10);
	//}
	
	//test_slider();
	
	//PORTB |= 1 << PB1; // enable Read
	
	
	//while(1) print_IO();
	
	/*while(1) { // testing av knapper
		printf("Hallo = %i\n", i++);
		printf("Btn_L = %d\n", PINB & (1 << PB0));
		printf("Btn_R = %d\n", PINB & (1 << PB1));
		_delay_ms(2000);
	};*/
	
	
	/*while(1) {
		enable_Latch();
		//testPrintf(i);
		
		_delay_ms(4000);
		
		printf("Hello\n");
	}*/
}


// DRIVER FOR SPI
/*
void spi_master_init() {
	//port b pin 4
	//Set MISO output, all others input
	DDRB = (1<<PB5) | (1<<PB7) | (1<<PB4);
	//Enable SPI
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0);
	
	PORTB|=(1<<PB6);
}

char SPI_SlaveReceive() {
	//Wait for reception complete
	while (!(SPSR & (1<<SPIF))) {}
	return SPDR;
}

void SPI_MasterTransmit(char cData){
	SPDR=cData;
	while (!(SPSR & (1<<SPIF))){}
}

//DRIVER FOR MCP2515

//READ
char MCP_read(char adress){
	PORTB &= !(1<<PB4);
	
	//send read instruction
	SPI_MasterTransmit(0b0011);
	_delay_ms(10);
	SPI_MasterTransmit(adress);
	char data = SPI_SlaveReceive();
	
	//terminate read instruction
	PORTB |= (1<<PB4);
	
	return data;
}

//WRITE
void MCP_write(char adress, char text) {
	PORTB &= !(1<<PB4);
	
	SPI_MasterTransmit(0b10);
	SPI_MasterTransmit(adress);
	_delay_ms(10);
	SPI_MasterTransmit(text);
	
	//terminate
	PORTB |= (1<<PB4);
}

//REQUEST TO SEND
void MCP_request_to_send(char TXB_number){
	PORTB &= !(1<<PB4);
	if ((TXB_number<0)|TXB_number>0b111)
	{
		printf('invalid number');	
	}
	else{
		char out=0b01000000|TXB_number;
		SPI_MasterTransmit(out);
	}
	
	PORTB |= (1<<PB4);
}


//READ STATUS
char MCP_read_status(){
	PORTB &= !(1<<PB4);
	
	char status;
	SPI_MasterTransmit(0b10100000);
	status=SPI_SlaveReceive();
	
	PORTB |= (1<<PB4);
	return status;
}

//BIT MODIFY
void MCP_bit_modify(char adress, char mask, char data){
	PORTB &= !(1<<PB4);
	SPI_MasterTransmit(0b00000101);
	SPI_MasterTransmit(adress);
	SPI_MasterTransmit(mask);
	SPI_MasterTransmit(data);
	PORTB |=(1<<PB4);
}

//RESET
void MCP_reset(){
	PORTB &= !(1<<PB4);
	SPI_MasterTransmit(0b11000000);
	PORTB |=(1<<PB4);
}*/