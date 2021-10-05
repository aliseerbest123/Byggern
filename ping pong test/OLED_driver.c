/*
 * OLED_driver.c
 *
 * Created: 23.09.2021 16:12:05
 *  Author: Trygv
 */ 

#include "OLED_driver.h"
#include "fonts.h"

void write_c(uint8_t i) {
	OLED_Command[0] = i;
}

void OLED_init() {
	write_c(0xae); // display off
	write_c(0xa1); //segment remap
	write_c(0xda); //common pads hardware: alternative
	write_c(0x12);
	write_c(0xc8); //common output scan direction:com63~com0
	write_c(0xa8); //multiplex ration mode:63
	write_c(0x3f);
	write_c(0xd5); //display divide ratio/osc. freq. mode
	write_c(0x80);
	write_c(0x81); //contrast control
	write_c(0x50);
	write_c(0xd9); //set pre-charge period
	write_c(0x21);
	write_c(0x20); //Set Memory Addressing Mode
	write_c(0x02);
	write_c(0xdb); //VCOM deselect level mode
	write_c(0x30);
	write_c(0xad); //master configuration
	write_c(0x00);
	write_c(0xa4); //out follows RAM content
	write_c(0xa6); //set normal display
	write_c(0xaf); // display on
	
	_delay_ms(10);
	//write_c(0xa5);
	OLED_pos(0,0);
	OLED_reset();
}

void OLED_reset() {
	for (uint8_t i = 0; i < 8; i++) {
		OLED_clear_line(i);
	}
	OLED_goto_line(0);
}

void OLED_set_brightness(uint8_t lvl) {
	write_c(0x81); // set contrast control
	write_c(lvl);
}

void OLED_home() {
	OLED_goto_line(0);
};

void OLED_goto_line(uint8_t line) {
	_delay_ms(1);
	write_c(0xB0 | line);
}

void OLED_goto_col(uint8_t col) {
	write_c(0x00 | (col & 0b00001111));
	write_c(0x10 | ((col & 0b11110000) >> 4));
}

void OLED_clear_line(uint8_t line) {
	OLED_goto_line(line);
	
	for (int i = 0; i < 128; i++) {
		OLED_goto_col(i);
		OLED_Data[0] = 0b00000000; // clears col on page
		//_delay_ms(5);
		//OLED_write_data('D');
	}
}

void OLED_pos(uint8_t row, uint8_t col) {
	OLED_goto_line(row);
	OLED_goto_col(col);
}

void volatile OLED_write_data(char c) {
	//OLED_pos(0,0);

	for (int i = 0; i < 8; i++) {
		//OLED_goto_col(i);
		unsigned char byte = pgm_read_byte(&(font8[c-32][i]));
		//printf("value= %d\n", byte);
		_delay_us(1);
		OLED_Data[0] = byte;
	}
}

void OLED_print(char * c) {
	for (int i = 0; i < strlen(c); i++) {
		OLED_write_data(c[i]);
	}
}

void OLED_print_emoji(uint8_t c) {
	for (int i = 0; i < 5; i++) {
		unsigned char byte = pgm_read_byte(&(emoji[c][i]));
		_delay_us(1);
		OLED_Data[0] = byte;
	}
}

void OLED_invert(uint8_t i) {
	write_c(0xA6 | i);
}