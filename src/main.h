/*
 * main.h
 *
 *  Created on: Oct 26, 2018
 *      Author: linux
 */

#ifndef MAIN_H_
#define MAIN_H_


typedef struct user_details
{
char userid[30];
char username[30];
char operaterlevel[10];
char pin[50];
int watchTimer;
char firstname[30];
char lastname[30];


}operatorDetails;

operatorDetails CurrentOperator;


void get_current_time(void);
extern void create_all_table();
void* threader(void *arg);
int logged_offline;
void land_rate_operations();
void offline_login(char * password);
void login(void);
extern int read_config();
#endif /* MAIN_H_ */
