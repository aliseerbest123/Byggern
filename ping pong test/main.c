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
	
	
	while (1) {
		for (int i = 0; i < 8; i++)
		{
			//OLED_goto_line(i);
			OLED_print_to_sram("0123456789ABCDEF", i, 0);
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