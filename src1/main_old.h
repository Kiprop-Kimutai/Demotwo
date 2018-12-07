/*
 * main.h
 *
 *  Created on: Oct 26, 2014
 *      Author: tarus
 */

#ifndef MAIN_H_
#define MAIN_H_
typedef struct user_details
{
int userid;
char username[50];
char name[100];

char marketid[100];
const char market[50];
char subcounty[50];

char password[50];

char paybill[20];
char currency[20];
int watchTimer;
int voidTimer;
int voidLimit;
int transactionLimit;
char update_url[300];
char latest_version_number[30];
char version_number[30];

}userDetails;

userDetails CurrentUser;


int printflag;
int print_complete;
void login(void );
int peformCardOperations(void);
#endif /* MAIN_H_ */
