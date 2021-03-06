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

struct node *start = NULL; //first menu item
struct node *current; //Current menu item
struct node *head; //Head menu item
uint8_t arrow_line = 1; //global variable determining which line the arrow is on 
int INVERT_FLAG=1; // inverted screen(0), non-inverted screen(1)

/**
 * \brief creates a new menu node
 *
 * \param menu struct containg the menu varibles
 * \param name the name diplayed on the sreen
 * \param parent the parent node
 * \param next the next node on the list
 * \param prev the previous node on the list
 * \param child the child of the node
 * \param ID the id of the node (every node has its own ID)
 *
 * \retval menu finished filling inn all the values.
 */
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

/**
 * \brief initilizes our menu
 *
 * \param void
 *
 * \retval void.
 */
void menu_init()
{
	struct node *play_ping, *high_scores, *options, *easy, *medium, *hard, *play_pong, *invert, *easy_pong, *medium_pong, *hard_pong, *High_Score_pong;

	play_ping = (struct node *)malloc(sizeof(struct node));
	play_pong = (struct node *)malloc(sizeof(struct node));
	options = (struct node *)malloc(sizeof(struct node));
	high_scores = (struct node *)malloc(sizeof(struct node));
	easy = (struct node *)malloc(sizeof(struct node));
	medium = (struct node *)malloc(sizeof(struct node));
	hard = (struct node *)malloc(sizeof(struct node));
	invert = (struct node *)malloc(sizeof(struct node));
	easy_pong = (struct node *)malloc(sizeof(struct node));
	medium_pong = (struct node *)malloc(sizeof(struct node));
	hard_pong = (struct node *)malloc(sizeof(struct node));
	High_Score_pong = (struct node *)malloc(sizeof(struct node));

	play_ping = new_menu(play_ping, "Play Ping", NULL, play_pong, NULL, easy, 1);

	play_pong = new_menu(play_pong, "Play Pong", NULL, options, play_ping, easy_pong, 2);

	options = new_menu(options, "Options", NULL, NULL, play_pong, invert, 3);

	high_scores = new_menu(high_scores, "High Scores", play_ping, NULL, hard, NULL, 4);

	easy = new_menu(easy, "Easy", play_ping, medium, NULL, NULL, 5);

	medium = new_menu(medium, "Medium", play_ping, hard, easy, NULL, 6);

	hard = new_menu(hard, "Hard", play_ping, high_scores, medium, NULL, 7);

	invert = new_menu(invert, "Invert Screen", play_ping, NULL, NULL, NULL, 9);

	easy_pong = new_menu(easy_pong, "Easy", play_ping, medium_pong, NULL, NULL, 10);
	
	medium_pong = new_menu(medium_pong, "Medium", play_ping, hard_pong, easy_pong, NULL, 11);
	
	hard_pong = new_menu(hard_pong, "Hard", play_ping, High_Score_pong, medium_pong, NULL, 12);
	
	High_Score_pong = new_menu(High_Score_pong, "High Scores", play_ping, NULL, hard_pong, NULL, 13);

	head = play_ping;
	current = play_ping;
}

/*
 * \brief displays the menu on the OLED
 *
 * \param void
 *
 * \retval void.
 */
void display()
{
	OLED_reset_sram();
	uint8_t current_line = 0;
	struct node *node;
	node = head;
	while (node != NULL)
	{
		++current_line;
		_delay_ms(400);
		OLED_print_to_sram(node->name, current_line, 3);
		OLED_draw_from_sram(INVERT_FLAG);
		node = node->next;
	}
	OLED_print_emoji_sram(0, arrow_line, 10);
	OLED_draw_from_sram(INVERT_FLAG);
	return;
}

/**
 * \brief updates the display so that the arrow is pointed at the next node
 *
 * \param void
 *
 * \retval void.
 */
void update_display_next()
{
	if (current->next == NULL)
	{
		return;
	}
	update_current_next();
	OLED_reset_sram();
	arrow_line++;
	uint8_t current_line = 0;
	struct node *node;
	node = head;
	while (node != NULL)
	{
		++current_line;
		OLED_print_to_sram(node->name, current_line, 3);
		OLED_draw_from_sram(INVERT_FLAG);
		node = node->next;
		_delay_us(1);
	}
	OLED_print_emoji_sram(0, arrow_line, 10);
	OLED_draw_from_sram(INVERT_FLAG);
	return;
}

/**
 * \brief updates the display so that the arrow is pointed at the previous node
 *
 * \param void
 *
 * \retval void.
 */
void update_display_prev()
{
	if (current->prev == NULL)
	{
		return;
	}
	update_current_prev();
	OLED_reset_sram();
	--arrow_line;
	uint8_t current_line = 0;
	struct node *node;
	node = head;
	while (node != NULL)
	{
		++current_line;
		OLED_print_to_sram(node->name, current_line, 3);
		OLED_draw_from_sram(INVERT_FLAG);
		node = node->next;
		_delay_us(100);
	}
	OLED_print_emoji_sram(0, arrow_line, 10);
	OLED_draw_from_sram(INVERT_FLAG);
	return;
}

/**
 * \brief updates current node to the next node
 *
 * \param void
 *
 * \retval void.
 */
void update_current_next()
{
	current = current->next;
}

/**
 * \brief updates current node to the previous node
 *
 * \param void
 *
 * \retval void.
 */
void update_current_prev()
{
	current = current->prev;
}

/**
 * \brief inverts the screen
 *
 * \param void
 *
 * \retval void.
 */
void invert_screen(){
	if (INVERT_FLAG)
	{
		INVERT_FLAG=0;
	}
	else{
		INVERT_FLAG=1;
	}
}

/**
 * \brief checks if a function has a child, if we have a child the display is updated and the new list is displayed. Else it performs the task related to the menu choice
 *
 * \param void
 *
 * \retval void.
 */
void check_child()
{
	if (current->child != NULL)
	{
		head = current->child;
		current = current->child;
		arrow_line = 1;
		OLED_reset_sram();
		_delay_ms(10);
		display();
	}
	else
	{
		printf(current->name);
		if (current->ID == 5){
			play_ping(10);
			go_back();}
		else if (current->ID == 6){
			play_ping(11);
			go_back();}
		else if (current->ID == 7){
			play_ping(12);
			go_back();}
		else if (current->ID == 10)
			play_pong(1);
		else if (current->ID == 11)
			play_pong(3);
		else if (current->ID == 12)
			play_pong(4);
		else if (current->ID == 9)
			invert_screen();
		else if (current->ID == 13)
			print_score_history();
	}
}

/**
 * \brief if there is a parent node the display is updated with the new list
 *
 * \param void
 *
 * \retval void.
 */
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