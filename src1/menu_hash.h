/*
 * menu_hash.h
 *
 *  Created on: Sep 27, 2014
 *      Author: tarus
 */

#ifndef MENU_HASH_H_
#define MENU_HASH_H_

#include "jsonread.h"

struct menu_structure {
    char serviceId[30];                    /* key */
    char serviceName[30];					/*Menu name eg parking*/
    char serviceCode[40];				/*PER102*/
    int has_next;					/*true/false*/
    int  number_of_next_menu;
    UT_hash_handle hh;         /* makes this structure hashable */
};


char listed_menu[10][100];
//Functions and Methos in this class:
void add_menu(char *menu_level, char *name);
void delete_menu(struct menu_structure *user);
void delete_all(void);
void print_menus(void);
void add_code(char *menu_level, char *code);
void add_has_next(char *menu_level, int Next);
struct menu_structure *find_menu(char *menu_id);
void menu_array (char* current_menu,int  i);
char* one_level_deep(char *s1,int  i);
char* one_level_up(char *s1, int i);

#endif /* MENU_HASH_H_ */
