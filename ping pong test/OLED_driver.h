/*
 * OLED_driver.h
 *
 * Created: 23.09.2021 16:12:22
 *  Author: Trygv
 */ 

#define F_CPU 4915200 //clockspeed
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

// #define volatile char *OLED_Command = (char *) 0x1000;
#define OLED_Command ((char*)0x1000)
#define OLED_Data ((char*)0x1200)


#define external_ram ((char *)0x1800) // Start address for the SRAM

#ifndef OLED_DRIVER_H_
#define OLED_DRIVER_H_

void OLED_init();
void OLED_reset();
void OLED_home();
void OLED_goto_line(uint8_t line);
void OLED_goto_col(uint8_t col);
void OLED_clear_line(uint8_t line);
void OLED_pos(uint8_t row, uint8_t col);
void volatile OLED_write_data(char c);
void OLED_print(char * c);
void OLED_set_brightness(uint8_t lvl);
void OLED_print_emoji(uint8_t c);
void OLED_invert(uint8_t i);


void OLED_draw_from_sram();
void OLED_write_data_to_sram(char c, uint8_t row , uint8_t col);
void OLED_print_to_sram(char * c, uint8_t row, uint8_t col);
void OLED_reset_sram();
void OLED_clear_line_sram(uint8_t line);
void OLED_print_emoji_sram(uint8_t c, uint8_t row, uint8_t col);

void OLED_draw_point_sram(uint8_t x, uint8_t y);
void OLED_clear_point_sram(uint8_t x, uint8_t y);
void OLED_draw_circle(uint8_t x, uint8_t y, uint8_t r, uint8_t clear);
void OLED_draw_box(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t clear);
void OLED_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t clear); //not implemented yet

#endif /* OLED_DRIVER_H_ */