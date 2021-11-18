/*
 * OLED_driver.c
 *
 * Created: 23.09.2021 16:12:05
 *  Author: Trygv
 */

#include "OLED_driver.h"
#include "fonts.h"

void write_c(uint8_t i)
{
	OLED_Command[0] = i;
}

void OLED_init()
{
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
	OLED_pos(0, 0);
	OLED_reset();
	OLED_reset_sram();
}

void OLED_reset()
{
	for (uint8_t i = 0; i < 8; i++)
	{
		OLED_clear_line(i);
	}
	OLED_pos(0, 0);
}

void OLED_set_brightness(uint8_t lvl)
{
	write_c(0x81); // set contrast control
	write_c(lvl);
}

void OLED_home()
{
	OLED_goto_line(0);
};

void OLED_goto_line(uint8_t line)
{
	OLED_pos(line, 0);
}

void OLED_goto_col(uint8_t col)
{
	write_c(0x00 | (col & 0b00001111));
	write_c(0x10 | ((col & 0b11110000) >> 4));
	_delay_us(1);
}

void OLED_clear_line(uint8_t line)
{
	OLED_goto_line(line);

	for (int i = 0; i < 128; i++)
	{
		OLED_goto_col(i);
		OLED_Data[0] = 0b00000000; // clears col on page
								   //_delay_ms(5);
								   //OLED_write_data('D');
	}
}

void OLED_pos(uint8_t row, uint8_t col)
{
	//OLED_goto_line(row);
	//OLED_goto_col(col);

	write_c(0xB0 | row);
	_delay_us(1);

	//write_c(0b00001111);
	//write_c(0b00010000);

	write_c(0x00 | (col & 0b00001111));
	write_c(0b00010000 | ((col & 0b11110000) >> 4));
	_delay_us(1);
}

void volatile OLED_write_data(char c)
{
	//OLED_pos(0,0);

	for (int i = 0; i < 8; i++)
	{
		//OLED_goto_col(i);
		unsigned char byte = pgm_read_byte(&(font8[c - 32][i]));
		//printf("value= %d\n", byte);
		_delay_us(1);
		OLED_Data[0] = byte;
	}
}

void OLED_print(char *c)
{
	for (int i = 0; i < strlen(c); i++)
	{
		OLED_write_data(c[i]);
		_delay_us(1);
	}
}

void OLED_print_emoji(uint8_t c)
{
	for (int i = 0; i < 5; i++)
	{
		unsigned char byte = pgm_read_byte(&(emoji[c][i]));
		_delay_us(1);
		OLED_Data[0] = byte;
	}
}

void OLED_invert(uint8_t i)
{
	write_c(0xA6 | i);
}

/*void OLED_draw_from_sram() {
	for (uint8_t line = 0; line < 8; line++) {
		OLED_goto_line(line);

		for (int col = 0; col < 128; col++) {
			OLED_goto_col(col);
			OLED_Data[0] = external_ram[line*128 + col];
		}
	}
}*/

void OLED_draw_from_sram()
{ // flipped
	for (uint8_t line = 0; line < 8; line++)
	{
		OLED_goto_line(line);

		for (int col = 0; col < 128; col++)
		{
			OLED_goto_col(col);

			uint8_t byte = external_ram[(7 - line) * 128 + (127 - col)];
			byte = reverse_byte(byte);
			OLED_Data[0] = byte; // write byte to screen
		}
	}
}

void OLED_write_data_to_sram(char c, uint8_t row, uint8_t col)
{
	for (int i = 0; i < 8; i++)
	{
		unsigned char byte = pgm_read_byte(&(font8[c - 32][i]));
		_delay_us(1);
		external_ram[row * 128 + col + i] = byte;
	}
}

void OLED_print_to_sram(char *c, uint8_t row, uint8_t col)
{
	for (int i = 0; i < strlen(c); i++)
	{
		OLED_write_data_to_sram(c[i], row, (col + i) * 8);
		_delay_us(1);
	}
}

void OLED_reset_sram()
{
	for (int i = 0; i < 128 * 8; i++)
		external_ram[i] = 0;
}

void OLED_clear_line_sram(uint8_t line)
{
	for (int i = line * 128; i < (line + 1) * 128; i++)
		external_ram[i] = 0;
}

void OLED_print_emoji_sram(uint8_t c, uint8_t row, uint8_t col)
{
	for (int i = 0; i < 5; i++)
	{
		unsigned char byte = pgm_read_byte(&(emoji[c][i]));
		_delay_us(1);
		external_ram[row * 128 + col + i] = byte;
	}
}

void OLED_draw_point_sram(uint8_t x, uint8_t y)
{
	uint8_t row = y / 8;
	uint8_t col = x;

	external_ram[row * 128 + col] |= 1 << (y % 8);
}

void OLED_clear_point_sram(uint8_t x, uint8_t y)
{
	uint8_t row = y / 8;
	uint8_t col = x;

	external_ram[row * 128 + col] &= ~(1 << (y % 8));
}

void OLED_draw_circle(uint8_t x, uint8_t y, uint8_t r, uint8_t clear)
{
	for (uint8_t iy = y - r; iy < y + r; iy++)
	{
		for (uint8_t ix = x - r; ix < x + r; ix++)
		{
			if (ix * ix + iy * iy <= r * r && 0 <= ix && ix < 128 && 0 <= iy && iy < 64)
			{
				if (clear)
				{
					OLED_clear_point_sram(ix, iy);
				}
				else
				{
					OLED_draw_point_sram(ix, iy);
				}
			}
		}
	}
}

void OLED_draw_box(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t clear)
{
	x = max(0, min(127, x));
	y = max(0, min(63, y));
	for (uint8_t iy = y; iy < min(y + h, 63); iy++)
	{
		for (uint8_t ix = x; ix < min(x + w, 127); ix++)
		{
			if (clear)
			{
				OLED_clear_point_sram(ix, iy);
			}
			else
			{
				OLED_draw_point_sram(ix, iy);
			}
		}
	}
}

/**
 * Find maximum between two numbers.
 */
int max(int num1, int num2)
{
	return (num1 > num2) ? num1 : num2;
}

/**
 * Find minimum between two numbers.
 */
int min(int num1, int num2)
{
	return (num1 > num2) ? num2 : num1;
}

uint8_t reverse_byte(uint8_t byte)
{
	byte = (byte & 0xF0) >> 4 | (byte & 0x0F) << 4;
	byte = (byte & 0xCC) >> 2 | (byte & 0x33) << 2;
	byte = (byte & 0xAA) >> 1 | (byte & 0x55) << 1;
	return byte;
}