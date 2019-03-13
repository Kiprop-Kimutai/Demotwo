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

char datam[100];
void printfConfigs(void);

void display_gprs_start_option(int feedback);

#endif /* MAIN_H_ */
