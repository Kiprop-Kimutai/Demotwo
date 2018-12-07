/*
 * usersManagement.c
 *
 *  Created on: Oct 28, 2018
 *      Author: linux
 */


#include <stdlib.h>
#include "common_includes.h"
#include "usersManagement.h"
#include "../src/utilities/inputs.h"


//Local use functions from the backend. The is to  be sent to  merchant for authemtication
/*
 * Get  POS user
 */
void request_pos_users(void);
/*
 * Used to  create the offline POS users by the merchant
 */
void create_pos_user(void);
/*
 * Create and manages the various user privileges
 */
void manage_user_privilages(void);
/*
 * User to  make a POS user disabled , enabled or even to  delete a POS user.
 */
void edit_user_details(void);

/*
 * User to  make a POS user disabled , enabled or even to  delete a POS user.
 */
void edit_user_status(void);


void manage_users_menu(void )
{
	int selected = 0;
	char user_managent_menu[][100] = {"Request POS Users",  "Create POS User" ,  "Edit POS User" ,  "Manage User Privilages" ,"Change User Status",  "Exit"};
	//const char  user_managent_menu[][100] = {"POS Device Configuration","POS User Account Operation", "Privileges Setup", "Exit"};
	while(1)
	{
		switch(lcd_menu("POS User Management", user_managent_menu, 6,selected))
		{
		case 0:
			request_pos_users();
			break;
		case 1:
			create_pos_user();
			break;
		case 2:
			edit_user_details();
			break;
		case 3:
			manage_user_privilages();
			break;
		case 4:
			edit_user_status();
			break;
		case 5:
			return;
			break;
		}
	}

}

void request_pos_users(void){

	//display_info(1,"Requesting users","Please wait ...",1);
	//sleep(10);
	//Users Structure
	//userName#FirstName#pin#userLevel#Active*username#FirstName#password#userLevel#Active*username#FirstName#password#userLevel#Active
	//jackson#Jackson#0000#0#1*alex#Alexandar#1111#1#1*jonah#Jonah#2222#2#1*martin#Martin#3333#3#1

	//User Rights Structure




	return;
}

void create_pos_user(void){


	return;
}

void edit_user_details(void){

}

void manage_user_privilages(void){

	return;
}

void edit_user_status(void){

	return;
}

void initialize_user_params(void){

	//active_pos_user = 0;


	return;

}
