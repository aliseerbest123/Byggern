/*
 * menu.c
 *
 * Created: 05.10.2021 15:15:49
 *  Author: alises
 */

#include <stddef.h>
#include "menu.h"
#include "pong_driver.h"
#include "play_ping.h"

struct node *start = NULL;
struct node *current;
struct node *head;
uint8_t arrow_line = 1;

struct node *new_menu(struct node *menu, char name[], struct node *parent, struct node *next, struct node *prev, struct node *child, int16_t ID)
{
	strcpy(menu->name, name);
	//	menu->name[strlen(name)]=name;
	menu->parent = parent;
	menu->next = next;
	menu->prev = prev;
	menu->child = child;
	menu->ID = ID;
	return menu;
}

void menu_init()
{
	struct node *play_ping, *high_scores, *options /*, *set_difficulty*/, *randome, *easy, *medium, *hard, *play_pong, *invert, *easy_pong, *medium_pong, *hard_pong, *High_Score_pong;

	play_ping = (struct node *)malloc(sizeof(struct node));
	play_pong = (struct node *)malloc(sizeof(struct node));
	options = (struct node *)malloc(sizeof(struct node));
	high_scores = (struct node *)malloc(sizeof(struct node));
	//set_difficulty= (struct node*)malloc(sizeof(struct node));
	easy = (struct node *)malloc(sizeof(struct node));
	medium = (struct node *)malloc(sizeof(struct node));
	hard = (struct node *)malloc(sizeof(struct node));
	randome = (struct node *)malloc(sizeof(struct node));
	invert = (struct node *)malloc(sizeof(struct node));
	easy_pong = (struct node *)malloc(sizeof(struct node));
	medium_pong = (struct node *)malloc(sizeof(struct node));
	hard_pong = (struct node *)malloc(sizeof(struct node));
	High_Score_pong = (struct node *)malloc(sizeof(struct node));

	play_ping = new_menu(play_ping, "Play Ping", NULL, play_pong, NULL, easy, 1);

	play_pong = new_menu(play_pong, "Play Pong", NULL, options, play_ping, easy_pong, 2);

	options = new_menu(options, "Options", NULL, NULL, play_pong, invert, 3);

	high_scores = new_menu(high_scores, "High Scores", play_ping, NULL, randome, NULL, 4);

	//set_difficulty= new_menu(set_difficulty, "Difficulty", NULL, NULL, high_scores, easy);

	easy = new_menu(easy, "Easy", play_ping, medium, NULL, NULL, 5);

	medium = new_menu(medium, "Medium", play_ping, hard, easy, NULL, 6);

	hard = new_menu(hard, "Hard", play_ping, randome, medium, NULL, 7);

	randome = new_menu(randome, "Random", play_ping, high_scores, hard, NULL, 8);

	invert = new_menu(invert, "Invert Screen", play_ping, NULL, NULL, NULL, 9);

	easy_pong = new_menu(easy_pong, "Easy", play_ping, medium_pong, NULL, NULL, 10);
	medium_pong = new_menu(medium_pong, "Medium", play_ping, hard_pong, easy_pong, NULL, 11);
	hard_pong = new_menu(hard_pong, "Hard", play_ping, High_Score_pong, medium_pong, NULL, 12);
	High_Score_pong = new_menu(High_Score_pong, "High Scores", play_ping, NULL, hard_pong, NULL, 13);

	head = play_ping;
	current = play_ping;
}

struct node *return_to_head()
{
	return head;
}

void display()
{
	//OLED_reset();
	OLED_reset_sram();
	uint8_t current_line = 0;
	//printf("%i", arrow_line);
	//OLED_pos(arrow_line, 8);
	//_delay_ms(100);
	//OLED_pos(current_line, 16);
	struct node *node;
	node = head;
	while (node != NULL)
	{
		++current_line;
		//OLED_pos(++current_line, 16);
		_delay_ms(400);
		//OLED_print(node->name);
		//OLED_print("Georg");
		OLED_print_to_sram(node->name, current_line, 3);
		//printf("%d",node->name);
		OLED_draw_from_sram();
		//OLED_goto_col(16);
		//OLED_print("HI");
		node = node->next;

		//OLED_goto_col(8*3);
		//_delay_ms(100);
	}
	OLED_print_emoji_sram(0, arrow_line, 10);
	OLED_draw_from_sram();
	return;
}

void update_display_next()
{
	if (current->next == NULL)
	{
		return;
	}
	update_current_next();
	//OLED_reset();
	OLED_reset_sram();
	//OLED_clear_line(0);
	//OLED_clear_line(1);
	//OLED_clear_line(2);
	//OLED_clear_line(3);
	//printf("%i", arrow_line);
	arrow_line++;
	//printf("%i", arrow_line);
	uint8_t current_line = 0;
	//printf("%i", arrow_line);
	//OLED_pos(current_line, 20);
	struct node *node;
	node = head;
	while (node != NULL)
	{
		++current_line;
		OLED_print_to_sram(node->name, current_line, 3);
		OLED_draw_from_sram();
		node = node->next;
		//OLED_pos(++current_line, 30);
		_delay_us(1);
		//OLED_goto_col(8*3);
		//_delay_ms(100);
	}
	//OLED_pos(arrow_line, 10);
	OLED_print_emoji_sram(0, arrow_line, 10);
	OLED_draw_from_sram();
	return;
}

void update_display_prev()
{
	if (current->prev == NULL)
	{
		return;
	}
	update_current_prev();
	//OLED_clear_line(0);
	//OLED_clear_line(1);
	//OLED_clear_line(2);
	//OLED_clear_line(3);
	//OLED_reset();
	OLED_reset_sram();
	//printf("%i", arrow_line);
	--arrow_line;
	//printf("%i", arrow_line);
	uint8_t current_line = 0;
	//printf("%i", arrow_line);
	//OLED_pos(current_line, 20);
	struct node *node;
	node = head;
	while (node != NULL)
	{
		++current_line;
		OLED_print_to_sram(node->name, current_line, 3);
		OLED_draw_from_sram();
		node = node->next;
		//OLED_pos(++current_line, 30);
		_delay_us(100);
		//OLED_goto_col(8*3);
		//_delay_us(100);
	}
	//OLED_pos(arrow_line, 10);
	OLED_print_emoji_sram(0, arrow_line, 10);
	OLED_draw_from_sram();
	return;
}

void update_current_next()
{
	current = current->next;
}

void update_current_prev()
{
	current = current->prev;
}

void check_child()
{
	if (current->child != NULL)
	{
		head = current->child;
		current = current->child;
		arrow_line = 1;
		OLED_reset_sram();
		//OLED_clear_line(0);
		//OLED_clear_line(1);
		//OLED_clear_line(2);
		//OLED_clear_line(3);
		_delay_ms(10);
		display();
	}
	else
	{
		printf(current->name);
		if (current->ID == 5)
			play_ping(10);
		else if (current->ID == 6)
			play_ping(11);
		else if (current->ID == 7)
			play_ping(12);
		else if (current->ID == 10)
			play_pong(1);
		else if (current->ID == 11)
			play_pong(3);
		else if (current->ID == 12)
			play_pong(4);
		//play_pong();
		// go_back();
	}
}

void go_back()
{
	if (current->parent != NULL)
	{
		head = current->parent;
		current = current->parent;
		arrow_line = 1;
		OLED_reset_sram();
		_delay_ms(10);
		display();
	}
}