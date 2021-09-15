/*
 * ping pong test.c
 *
 * Created: 06.09.2021 11:22:01
 * Author : ingvode
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#define FOSC 4915200//clockspeed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1


void uart_send(unsigned char letter) {
	
	while (! (UCSR1A & (1<<UDRE1))){}
	
	UDR1 = letter;
}

char uart_recieve() {
	while (! (UCSR1A & (1<<RXC1))){}
	
	return UDR1;
}


void init_UART (unsigned int ubrr) {
	// set baud rate
	UBRR1H = (unsigned char) (ubrr>>8);
	UBRR1L = (unsigned char) ubrr;
	
	UCSR1B = (1<<RXEN1) | (1<<TXEN1);
	
	UCSR1C = (1<<URSEL1) | (1<<USBS1) | (3<<UCSZ10);


	fdevopen(uart_send, uart_recieve);

}


int main(void)
{
	DDRA |= (1 << PA0);
	DDRA = 0xff;
	DDRE |= 1 << PE1;
	DDRC |= 0b1111;
	
	DDRD |= 1 << PD7; // enable Read
	DDRD |= 1 << PD6; // enable write
	
	MCUCR |= (1 << SRE);
	
	
	init_UART(MYUBRR);
	
	uint8_t i = 0;
	
	//adresse 0x1567;
	
	//volatile char *ext_ram = (char *) 0x1400;
	
	/*while(1){
	ext_ram[5] = 0b101010101010;}*/
	
	
	//SRAM_test();
	
	DDRD |= 1 << PD5;
	
	while(1) {
		PORTD ^= 1 << 5;
		_delay_ms(10);
	}
	
	
	
	/*while(1) {
		enable_Latch();
		//testPrintf(i);
		
		_delay_ms(4000);
		
		printf("Hello\n");
	}*/
}

void enable_Latch() {
	
	PORTE |= 1 << PE1;
	PORTA = 0b11111111;
	
	_delay_ms(1000);
	
	PORTA = 0b01010101;
	//PORTA = 0b00000000;
	
	_delay_ms(1000);
	
	PORTE &= !(1 << PE1);
	
	PORTA = 0b00000000;
	_delay_ms(1000);
}

void testPrintf(int i) {
		printf("Write something: %i\n", i++);
		
		
		PORTA ^= 0x1 << PA0;
		_delay_ms(1000);
		
		
		char c = uart_recieve();
		
		printf("You wrote: %c\n", c);
		
		
		/*					
		i = (i + 1) % 100;
		if (i == 1) {
			//uart_send_new('F');
			uart_send('F');
			PORTA ^= 0x1 << PA0;
		}
		*/
}




void SRAM_test(void)
{
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	uint16_t ext_ram_size = 0x800;
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	printf("Starting SRAM test...\n");
	// rand() stores some internal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before this function)
	uint16_t seed = rand();
	// Write phase: Immediately check that the correct value was stored
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			write_errors++;
		}
	}
	// Retrieval phase: Check that no values were changed during or after the write phase
	srand(seed);
	// reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			retrieval_errors++;
		}
	}
	printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
}


uint8_t binary_to_decimal_range(uint8_t from, uint8_t to, uint8_t binary, uint8_t max_input = 0xFF) {
	return ((to - from )binary / max_input) + from;
}

struct joystick_vector {
	uint8_t x;
	uint8_t y;
}

joystick_vector get_vector_direction_from_binary(uint8_t x, uint8_t y) {
	joystick_vector vec;
	vec.x = binary_to_decimal_range(-100, 100, x);
	vec.y = binary_to_decimal_range(-100, 100, y);
	return vec;
}

enum direction
{
	LEFT, RIGHT, UP, DOWN, NEUTRAL;
}

direction get_dir_from_joystick_vector(joystick_vector vec, uint8_t threshold = 10) {
	if (vec.x > threshold && vec.x > abs(vec.y)) return RIGHT;
	if (vec.x < - threshold && vec.x < - abs(vec.y)) return LEFT;
	if (vec.y > threshold && vec.y > abs(vec.x)) return UP;
	if (vec.y < - threshold && vec.y < - abs(vec.x)) return DOWN;
	return NEUTRAL;
}