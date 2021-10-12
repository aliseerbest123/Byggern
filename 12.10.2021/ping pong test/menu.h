/*
 * menu.h
 *
 * Created: 05.10.2021 15:16:03
 *  Author: alises
 */ 


#define F_CPU 4915200 //clockspeed
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include "OLED_driver.h"

#ifndef MENU_H_
#define MENU_H_

struct node {
	struct node* next;
	struct node* prev;
	struct node* child;
	struct node* parent;
	char name[16];
} node;


void menu_init();
void display();
void update_display_next(); //oppdaterer pil ned en
void update_display_prev(); //Oppdaterer pil opp en
void update_current_next(); //Oppdaterer current node til neste
void update_current_prev(); //Oppdaterer current node til forrige
void check_child(); //Sjekker om vi kan lage en under meny og skriver den ut dersom den finnes.
//struct node* new_menu(struct node* menu, char* name, struct node* parent, struct node* next, struct node* prev, struct node* child)



#endif /* MENU_H_ */