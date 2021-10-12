/*
 * menu.c
 *
 * Created: 05.10.2021 15:15:49
 *  Author: alises
 */ 


#include <stddef.h>
#include "menu.h"


struct node* start = NULL;
struct node* current;
struct node* head;
uint8_t arrow_line = 1;

struct node* new_menu(struct node* menu, char name[], struct node* parent, struct node* next, struct node* prev, struct node* child){
	strcpy(menu->name, name);
//	menu->name[strlen(name)]=name;
	menu->parent=parent;
	menu->next=next;
	menu->prev=prev;
	menu->child=child;
	return menu;
}

void menu_init(){
	struct node *play_game, *high_scores, *options, *set_difficulty, *randome, *easy, *medium, *hard;

	play_game = (struct node*)malloc(sizeof(struct node));
	options= (struct node*)malloc(sizeof(struct node));
	high_scores= (struct node*)malloc(sizeof(struct node));
	set_difficulty= (struct node*)malloc(sizeof(struct node));
	easy= (struct node*)malloc(sizeof(struct node));
	medium= (struct node*)malloc(sizeof(struct node));
	hard= (struct node*)malloc(sizeof(struct node));
	randome= (struct node*)malloc(sizeof(struct node));

	play_game= new_menu(play_game, "Play Game", NULL, options, NULL, NULL);

	options= new_menu(options, " Options", NULL, high_scores, play_game, NULL);

	high_scores= new_menu(high_scores, "High Scores", NULL, set_difficulty, options, NULL);

	set_difficulty= new_menu(set_difficulty, "Difficulty", NULL, NULL, high_scores, easy);
	
	easy= new_menu(easy, "Easy", set_difficulty, medium, NULL, NULL);

	medium= new_menu(medium, "Medium", set_difficulty, hard, medium, NULL);

	hard= new_menu(hard, "Hard", set_difficulty, randome, hard, NULL);

	randome= new_menu(randome, "Random" , set_difficulty, NULL, hard, NULL);

	head=play_game;
	current=play_game;
}

struct node* return_to_head(){
	return head;
}

void display()
{
	OLED_reset();
	uint8_t current_line = 0;
	//printf("%i", arrow_line);
	OLED_pos(arrow_line, 8);
	//_delay_ms(100);
	OLED_print_emoji(0);
	//OLED_pos(current_line, 16);
	struct node* node;
	node = head;
	while (node != NULL) {
		OLED_pos(++current_line, 16);
		_delay_ms(400);
		//OLED_print(node->name);
		OLED_print("Georg");
		OLED_goto_col(16);
		OLED_print("HI");
		node = node->next;
		
		//OLED_goto_col(8*3);
		//_delay_ms(100);
	}
	return;
}

void update_display_next(){
	update_current_next();
	OLED_reset();
	//OLED_clear_line(0);
	//OLED_clear_line(1);
	//OLED_clear_line(2);
	//OLED_clear_line(3);
	//printf("%i", arrow_line);
	arrow_line++;
	//printf("%i", arrow_line);
	uint8_t current_line = 0;
	//printf("%i", arrow_line);
	OLED_pos(current_line, 20);
	struct node* node;
	node = head;
	while (node != NULL) {
		OLED_print(node->name);
		node = node->next;
		OLED_pos(++current_line, 30);
		_delay_us(1);
		//OLED_goto_col(8*3);
		//_delay_ms(100);
	}
	OLED_pos(arrow_line, 10);
	OLED_print_emoji(0);
	return;
}

void update_display_prev(){
		update_current_prev();
		//OLED_clear_line(0);
		//OLED_clear_line(1);
		//OLED_clear_line(2);
		//OLED_clear_line(3);
		OLED_reset();
		//printf("%i", arrow_line);
		--arrow_line;
		//printf("%i", arrow_line);
		uint8_t current_line = 0;
		printf("%i", arrow_line);
		OLED_pos(current_line, 20);
		struct node* node;
		node = head;
		while (node != NULL) {
			OLED_print(node->name);
			node = node->next;
			OLED_pos(++current_line, 30);
			_delay_us(100);
			//OLED_goto_col(8*3);
			//_delay_us(100);
		}
		OLED_pos(arrow_line, 10);
		OLED_print_emoji(0);
		return;
}

void update_current_next(){
	current=current->next;
}

void update_current_prev(){
	current=current->prev;
}

void check_child(){
	if (current->child!=NULL)
	{
		head=current->child;
		current=current->child;
		arrow_line=0;
		OLED_clear_line(0);
		OLED_clear_line(1);
		OLED_clear_line(2);
		OLED_clear_line(3);
		display();
	}
}