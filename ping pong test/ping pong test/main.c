/*
 * ping pong test.c
 *
 * Created: 06.09.2021 11:22:01
 * Author : ingvode
 */ 

#include <avr/io.h>
#include <util/delay.h>
#define FOSC 4915200//clockspeed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1


void init_UART (unsigned int ubrr) {
	// set baud rate
	UBRR1H = (unsigned char) (ubrr>>8);
	UBRR1L = (unsigned char) ubrr;
	
	UCSR1B = (1<<RXEN1) | (1<<TXEN1);
	
	UCSR1C = (1<<URSEL1) | (1<<USBS1) | (3<<UCSZ10);
}

void init_UART_new (unsigned int ubrr) {
	// set baud rate
	UBRR0H = (unsigned char) (ubrr>>8);
	UBRR0L = (unsigned char) ubrr;
	
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	
	UCSR0C = (1<<URSEL0) | (1<<USBS0) | (3<<UCSZ00);
}

void uart_send(unsigned char letter) {
	
	while (! (UCSR1A & (1<<UDRE1))){}
	/*while (UDRE1){
		UDR1=letter;
	}*/
	
	UDR1 = letter;
}

void uart_send_new(unsigned char letter) {
	
	while (! (UCSR0A & (1<<UDRE0)))
	
	UDR0 = letter;
}

char uart_recieve() {
	while (! (UCSR1A & (1<<RXC1)));
	
	return UDR1;
}



int main(void)
{
	DDRA |= (1 << PA0);
	
	
	init_UART(MYUBRR);
	//init_UART_new(MYUBRR);
	
	uint8_t i = 0;
	
	while(1) {
		uart_send('F');
		//uart_send_new('F');
		
		PORTA ^= 0x1 << PA0;
		_delay_ms(200);
		/*					
		i = (i + 1) % 100;
		if (i == 1) {
			//uart_send_new('F');
			uart_send('F');
			PORTA ^= 0x1 << PA0;
		}
		*/
	}
}