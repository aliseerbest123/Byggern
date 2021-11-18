/*
 * CFile1.c
 *
 * Created: 21.09.2021 09:47:20
 *  Author: ingvode
 */

#include "tests.h"

void SRAM_test(void)
{
	volatile char *ext_ram = (char *)0x1800; // Start address for the SRAM
	uint16_t ext_ram_size = 0x800;
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	printf("Starting SRAM test...\n");
	// rand() stores some internal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before this function)
	uint16_t seed = rand();
	// Write phase: Immediately check that the correct value was stored
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++)
	{
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value)
		{
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			write_errors++;
		}
	}
	// Retrieval phase: Check that no values were changed during or after the write phase
	srand(seed);
	// reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < ext_ram_size; i++)
	{
		uint8_t some_value = rand();
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value)
		{
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			retrieval_errors++;
		}
	}
	printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
}

void testPrintf(int i)
{
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

void enable_Latch()
{

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

void test_controller(uint8_t x_joy, uint8_t y_joy, uint8_t left_slider_binary, uint8_t right_slider_binary)
{
	printf("(x,y) = (%i, %i)", x_joy, y_joy);

	joystick_vector vec = get_vector_direction_from_binary(x_joy, y_joy);

	uint8_t myDir = get_dir_from_joystick_vector(vec, 10);
	printf("Joystick (x, y, dir) = (%i, %i, %i)\n", vec.x, vec.y, myDir);

	printf("Slider (left, right) = (%i, %i)\n", slider_binary_to_decimal(left_slider_binary), slider_binary_to_decimal(right_slider_binary));
	//printf("Slider binary (left,right = (%i, %i)\n", left_slider_binary,right_slider_binary );
}

void test_slider()
{
	volatile char *ext_adc = (char *)0x1400; // Start address for the ADC

	ext_adc[0]; // chip select

	PORTA |= 1 << PA1 | 1 << PA0; // Select datainput

	PORTD &= !(1 << PD6); // Set write low
	_delay_ms(10);

	uint8_t left_slider_binary = ext_adc[0]; // sett riktig verdi
	//printf("Left slider = %i\n", binary_to_decimal_range(0, 100, left_slider_binary, 0xFF));

	printf("Left slider = %i\n", left_slider_binary);
}

void slider_setup_testing()
{
	/*	volatile char *my_pointer = (char *) 0x60;
	
	my_pointer[0] |=   0b00000010;
	my_pointer[0] &= !(0b00000001);
*/
	DDRD |= (1 << PD5); // set pin D5 to output

	// mode set CTC
	// WGMn3	WGMn2	WGMn1	WGMn0
	// 0		1		0		0		CTC		OCRnA	Imediate	Max
	//	TCCR1A = 0b00000000;
	//	TCCR1B = 0b00001010;

	TCCR1B |= (1 << WGM12);
	//prescaler
	TCCR1B |= (1 << CS10);
	TCCR1B &= ~(1 << CS11);
	TCCR1B &= ~(1 << CS12);

	TCCR1A |= (1 << COM1A0);

	OCR1A = 0b00000001;
};

void test_OLED()
{
	//OLED_print("How are you?");
	OLED_print("Jeg er");
	OLED_pos(1, 0);
	OLED_print("ikke her");
	for (int i = 0; i < 5; i++)
		OLED_print_emoji(i);
}

void print_IO()
{
	uint8_t x_joy = get_joystick_x();
	uint8_t y_joy = get_joystick_y();
	uint8_t left_slider = get_slider_left();
	uint8_t right_slider = get_slider_right();

	test_controller(x_joy, y_joy, left_slider, right_slider); // (x,y) (left, right)
	printf("\n");
}

void test_CAN()
{
	can_message msg;
	msg.ID = 255;
	msg.length = 6;
	strcpy(msg.data, "Hello");

	CAN_send_message(msg);
	can_message m;
	CAN_recive_message(&m, 0);

	printf("CAN ID = %d\n", m.ID);
	printf("CAN length = %d\n", m.length);
	printf("CAN message = %s\n", (m.data));
}