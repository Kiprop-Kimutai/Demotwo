/*
 * device_management.c
 *
 *  Created on: Dec 5, 2018
 *      Author: linux
 */


#include <stdlib.h>
#include <arpa/inet.h>
#include "common_includes.h"
#include "device_management.h"
#include "usersManagement.h"
#include "main.h"
#include "../src/utilities/lcd.h"
#include "../src/utilities/keyboart.h"
#include "../src/utilities/inputs.h"
#include "../src1/jsonread.h"


/*
 * Function that  manages devie configuration
 */
void pos_device_configuration(void);
/*int sys_set_time(const struct tm *tm);*/

/*
 * Main Function that manages devices related configuration and users.
 */

extern int isValidIpAddress(char *ipAddress);
void device_management(void )
{
	//myConfigurations = (ConfigFile *) malloc(sizeof(ConfigFile));
	int selected = 0;
	//char user_managent_menu[][100] = {"Request POS Users",  "Change Password" ,  "Block User" ,  "Delete User" ,  "Exit"};
	const char  device_managment_menu[][100] = {"POS Device Configuration","POS User Account Operation", "Exit"};
	while(1)
	{
		switch(lcd_menu("POS User Management", device_managment_menu, 3 ,selected))
		{
		case 0:
			change_configuration(-1);
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

	int selected , ret;
	char ip[100];
	char getCharacters[40],  getCharacters1[40];
	char name[100],  name1[100];
	char user_managent_menu[][100] = {"Server IP",  "Server Port" ,  "Server Protocol" ,  "Device Date/Time" ,"APN Name","APN Password", "Device Timeout" , "Exit"};
	while(1)
	{
		switch(lcd_menu("Device Configuration", user_managent_menu, 8,selected))
		{
		case 0://Server IP



			strcpy(name, "Enter Enter IP address");
			strcpy(name1, "Current IP address");

			ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);

			if( isValidIpAddress((getCharacters)))
					{
						//change_made =1;
						strcpy(ip, (getCharacters));
						printf("your set ip is %s", ip);
					}

			break;
		case 1://Server Port
			strcpy(name, "Enter Server Port");
			strcpy(name1, "Current Server Port");
			ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);
			break;
		case 2://Server Protocol
			strcpy(name, "Enter Server Protocol");
			strcpy(name1, "Current Server Protocol");
			ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);
			break;
		case 3://Device Date/Time
			strcpy(name, "Enter Device Date/Time");
			strcpy(name1, "Current Device Date/Time");

			ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);
			break;
		case 4://APN Name
			strcpy(name, "Enter APN Name");
			strcpy(name1, "Current APN Name");
			ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);
			break;
		case 5://APN Password
			strcpy(name, "Enter APN Password");
			strcpy(name1, "Current APN Password");
			ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);
			break;
		case 6 ://device timout
			strcpy(name, "Enter Device timeout");
			strcpy(name1, "Current Device timeout");
			ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);
			break;

		case 7 ://exit
					return;
					break;


		}
	}}




