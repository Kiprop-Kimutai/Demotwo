/*
 * device_management.c
 *
 *  Created on: Dec 5, 2018
 *      Author: linux
 */


#include <stdlib.h>
#include "common_includes.h"
#include "device_management.h"
#include "usersManagement.h"
#include "../src/utilities/lcd.h"
#include "../src/utilities/inputs.h"

/*
 * Function that  manages devie configuration
 */
void pos_device_configuration(void);

/*
 * Main Function that manages devices related configuration and users.
 */
void device_management(void )
{
	int selected = 0;
	//char user_managent_menu[][100] = {"Request POS Users",  "Change Password" ,  "Block User" ,  "Delete User" ,  "Exit"};
	const char  device_managment_menu[][100] = {"POS Device Configuration","POS User Account Operation", "Exit"};
	while(1)
	{
		switch(lcd_menu("POS User Management", device_managment_menu, 3 ,selected))
		{
		case 0:
			pos_device_configuration();
			break;
		case 1:
			manage_users_menu();
			break;
		case 2 :
			return;
			break;
		}
	}

}

void pos_device_configuration(void){

	int selected = 0;
	char user_managent_menu[][100] = {"Server IP",  "Server Port" ,  "Server Protocol" ,  "Device Date/Time" ,"APN Name","APN Password", "Device Timeout" , "Exit"};
	while(1)
	{
		switch(lcd_menu("POS User Management", user_managent_menu, 8,selected))
		{
		case 1:
			//pos_device_configuration();

			break;
		case 2:
			printf("");
			break;
		case 3:
			printf("");
			break;
		case 4:
			printf("");
			break;
		case 5:
			printf("");
			break;
		case 6:
			printf("");
			break;
		case 7 :
			return;
			break;
		}
	}

}
