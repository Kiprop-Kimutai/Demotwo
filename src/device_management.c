/*
 * device_management.c
 *
 *  Created on: Dec 5, 2018
 *      Author: linux
 */


#include <stdlib.h>
#include <ppp.h>
#include "common_includes.h"
#include "utilities/common_functions.h"
#include "device_management.h"
#include "../src/utilities/send_online_request.h"
#include "usersManagement.h"
#include "../src/utilities/lcd.h"
#include "../src/utilities/keyboart.h"
//#include "../src1/jsonread.h"
#include "../src/utilities/inputs.h"


/*
 * Function that  manages devie configuration
 */
void pos_device_configuration(int type);

/*
 * Main Function that manages devices related configuration and users.
 */
void device_management(void )
{
	int selected = 0;
	//char user_managent_menu[][100] = {"Request POS Users",  "Change Password" ,  "Block User" ,  "Delete User" ,  "Exit"};
	char  device_managment_menu[][100] = {"POS Device Configuration","POS User Account Operation"};
	while(1)
	{
		switch(lcd_menu("POS User Management", device_managment_menu, sizeof(device_managment_menu)/100 ,selected))
		{
		case 0:
			pos_device_configuration(-1);
			break;
		case 1:
			manage_users_menu();
			break;
		case -1 :
			return;
			break;
		}
	}

}

void pos_device_configuration(int type){
	int selected , ret;
	int change_made =0;
	int ret_val;
	const char menu[][100] = { "Server IP Address   ", "Server port number  ",
			"Network APN username", "Network APN password",
			"Network ppp Timeout " , "Turn Online On/Off", "About"  };
	const char menu_on_off[][100] = { "Online Mode  ", "Offline Mode " };
	char getCharacters[40],  getCharacters1[40];
	char name[100],  name1[100];
	if(type == -1)
		selected = lcd_menu("Network Settings", menu, sizeof(menu) / 100, selected);
	else
		selected = type;
	switch (selected) {
	case 0:
		do
		{
			strcpy(name, "Enter IP address");
			strcpy(name1, "Current Ip address");
			strcpy(getCharacters1,myConfigurations->IpAddress);
			ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);

			if( isValidIpAddress((getCharacters)))
			{
				change_made =1;
				strcpy(myConfigurations->IpAddress, (getCharacters));
			}

			else if(ret==-1 && strlen((myConfigurations->IpAddress))>0 )
				break;
			else if(ret==-1 && strlen((myConfigurations->IpAddress)) == 0 )
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "IP address must be set");
				lcd_printf(ALG_CENTER, "Please try again");
				lcd_flip();
				kb_getkey();
			}
			else
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "Invalid IP address");
				lcd_printf(ALG_CENTER, "Please check IP\n and try again");
				lcd_flip();
				kb_getkey();
			}
		}while(strlen((myConfigurations->IpAddress))==0 );

		break;
	case 1: {
		do
		{
			strcpy(name, "Enter port number");
			strcpy(name1, "Current port number");
			strcpy(getCharacters1,myConfigurations->portNumber);
			ret = kb_getStringtwo(NUM_IN ,NUM_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);
			if( is_valid_int(getCharacters))
			{
				change_made =1;
				strcpy(myConfigurations->portNumber, getCharacters);
			}
			else if(ret==-1 &&  strlen(myConfigurations->portNumber)>0 )
			{
				break;
			}
			else if(ret==-1 && strlen(myConfigurations->portNumber) == 0 )
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "Port number must be set");
				lcd_printf(ALG_CENTER, "Please try again");
				lcd_flip();
				kb_getkey();
			}
			else
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "Invalid port number");
				lcd_printf(ALG_CENTER, "Please enter a numeric value");
				lcd_flip();
				kb_getkey();
			}
		}while(strlen(myConfigurations->portNumber)==0 );
	}
	break;

	case 2: {

		do
		{
			strcpy(name, "Enter APN username");
			strcpy(name1, "Current APN username");
			strcpy(getCharacters1,myConfigurations->apn_username);
			ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);
			if(ret !=-1 )
			{
				change_made =1;
				strcpy(myConfigurations->apn_username, getCharacters);
			}

			if(ret==-1 &&strlen(myConfigurations->apn_username)>0 )
			{
				break;
			}
			else if(ret==-1 &&strlen(myConfigurations->apn_username) == 0 )
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "APN username must be set");
				lcd_printf(ALG_CENTER, "Please try again");
				lcd_flip();
				kb_getkey();
			}

		}while(strlen(myConfigurations->apn_username)==0 );

	}
	break;

	case 3: {

		do
		{
			strcpy(name, "Enter APN password");
			strcpy(name1, "Current APN password");
			strcpy(getCharacters1,myConfigurations->apn_password);
			ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);

			if(ret != -1)
			{
				if(strlen((getCharacters)) == 0)
					continue;
				else{
					change_made =1;
					strcpy(myConfigurations->apn_password, getCharacters);
					break;
				}

			}

			else if(ret==-1 && strlen(myConfigurations->apn_password)>0 )
			{
				break;
			}
			else if(ret==-1 && strlen(myConfigurations->apn_password) == 0 )
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "APN password must be set");
				lcd_printf(ALG_CENTER, "Please try again");
				lcd_flip();
				kb_getkey();
			}

		}while(strlen(myConfigurations->apn_password)==0 );
	}
	break;

	case 4: {

		do
		{
			strcpy(name, "Enter IP PPP time out");
			strcpy(name1, "Current PPP time out");
			strcpy(getCharacters1,myConfigurations->ppp_timeout);
			ret = kb_getStringtwo(NUM_IN ,NUM_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);

			if( is_valid_int(getCharacters))
			{
				change_made =1;
				strcpy(myConfigurations->ppp_timeout, getCharacters);
			}
			else if(ret==-1 && strlen(myConfigurations->ppp_timeout)>0 )
			{
				break;
			}
			else if(ret==-1 && strlen(myConfigurations->ppp_timeout) == 0 )
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "PPP timeout number must be set");
				lcd_printf(ALG_CENTER, "Please try again");
				lcd_flip();
				kb_getkey();
			}
			else
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "Invalid PPP timeout value");
				lcd_printf(ALG_CENTER, "Please enter a numeric value");
				lcd_flip();
				kb_getkey();
			}
		}while(strlen(myConfigurations->ppp_timeout)==0 );
	}

	break;
	case 5: {
		selected = lcd_menu("Network Operation Mode", menu_on_off, sizeof(menu_on_off) / 100, selected);
		if(selected == -1)
		{
			return;
		}
		else if(selected == 0)
		{
			ret_val = ppp_check("/var/mux1");
			if (0 == ret_val) {
				flag_online = 1;
				//login_successful = 0 ;
				message_display_function(1,"","Network Mode  ", "The POS shall operate in online mode . Please login again", (char *)NULL);
				kb_getkey();
				return ;
			}

			fag_start_ppp_session = 1;
			flag_online = 0;
			//login_successful = 0 ;
			message_display_function(1,"","Network Configuration  ", "GPRS service starting .  Please wait ...", (char *)NULL);

			power_on_modem_device(myConfigurations->apn_username,myConfigurations->apn_password,atoi(myConfigurations->ppp_timeout));
			ret_val = ppp_check("/var/mux1");
			if (0 == ret_val) {
				flag_online = 1;
				return ;
			}
			if(flag_online)
			{
				message_display_function(1,"","Network Mode  ", "The POS shall operate in online mode . Please login again", (char *)NULL);
				kb_getkey();


			}
			else
			{
				message_display_function(1,"","Network Mode  ", "The POS  can not operate in Online mode. Please check your connectivity and try again", (char *)NULL);
				kb_getkey();
			}
			return ;
		}
		else if(selected == 1)
		{
			message_display_function(1,"","Network Mode  ", "The POS shall operate in offline mode .", (char *)NULL);
			kb_getkey();
			//login_successful = 0 ;
			flag_online = 0;
			return ;
		}
	}
	break;
	case 6:
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "HIBAH Value Agency");
			lcd_printf(ALG_CENTER, "Version 1.0.0\n Realese date : 13th March, 2018");
			lcd_flip();
			kb_getkey();
			type = 2;
			break;
	default:
		break;

	}
	if(type == -1)
	{
		if(change_made)
			save_configarations();
		else
		{
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "No configuration have \n been changed");

			lcd_flip();
			kb_getkey();
		}
	}

}


void save_configarations() {
	FILE *f;
	system("mv config.cfg config.cfg.bak");
	f = fopen("config.cfg", "w");
	if (f) {
		fprintf(f, "IpAddress=%s\n", myConfigurations->IpAddress);
		fprintf(f, "apn_username=%s\n", myConfigurations->apn_username);
		fprintf(f, "apn_password=%s\n", myConfigurations->apn_password);
		fprintf(f, "portNumber=%s\n", myConfigurations->portNumber);
		fprintf(f, "ppp_timeout=%s\n", myConfigurations->ppp_timeout);
		fprintf(f, "user_level=%s\n", myConfigurations->user_level);

		fclose(f);
		/*		strcpy(url ,"");
		strcpy(url ,"http://" );
		strcat(url , myConfigurations->IpAddress);
		strcat(url ,":");
		strcat(url , myConfigurations->portNumber);

		strcat(url , "/safaricom/service/");*/
		lcd_clean();
		screen_header();
		lcd_printf(ALG_CENTER,"Configurations change effected successfully. \nPress any key to continue");

		lcd_flip();
		kb_getkey();
	}
}

void change_configuration(int type) {
	int selected , ret;

	int ret_val;
	const char menu[][100] = { "Server IP Address   ", "Server port number  ",
			"Network APN username", "Network APN password",
			"Network ppp Timeout " , "Turn Online On/Off" , "Get Operator Details"};
	const char menu_on_off[][100] = { "Online Mode  ", "Offline Mode "};
	char getCharacters[40],  getCharacters1[40];
	char name[100],  name1[100];
	if(type == -1)
		selected = lcd_menu("Network Settings", menu, sizeof(menu) / 100, selected);
	else
		selected = type;
	switch (selected) {
	case 0:
		do
		{
			strcpy(name, "Enter IP address");
			strcpy(name1, "Current Ip address");

			strcpy(getCharacters1,myConfigurations->IpAddress);
			ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);

			if( isValidIpAddress((getCharacters)))
			{
				change_made =1;
				strcpy(myConfigurations->IpAddress, (getCharacters));
			}

			else if(ret==-1 && strlen((myConfigurations->IpAddress))>0 )
				break;
			else if(ret==-1 && strlen((myConfigurations->IpAddress)) == 0 )
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "IP address must be set");
				lcd_printf(ALG_CENTER, "Please try again");
				lcd_flip();
				kb_getkey();
			}
			else
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "Invalid IP address");
				lcd_printf(ALG_CENTER, "Please check IP\n and try again");
				lcd_flip();
				kb_getkey();
			}
		}while(strlen((myConfigurations->IpAddress))==0 );

		break;
	case 1: {
		do
		{
			strcpy(name, "Enter port number");
			strcpy(name1, "Current port number");
			strcpy(getCharacters1,myConfigurations->portNumber);
			ret = kb_getStringtwo(NUM_IN ,NUM_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);
			if( is_valid_int(getCharacters))
			{
				change_made =1;
				strcpy(myConfigurations->portNumber, getCharacters);
			}
			else if(ret==-1 &&  strlen(myConfigurations->portNumber)>0 )
			{
				break;
			}
			else if(ret==-1 && strlen(myConfigurations->portNumber) == 0 )
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "Port number must be set");
				lcd_printf(ALG_CENTER, "Please try again");
				lcd_flip();
				kb_getkey();
			}
			else
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "Invalid port number");
				lcd_printf(ALG_CENTER, "Please enter a numeric value");
				lcd_flip();
				kb_getkey();
			}
		}while(strlen(myConfigurations->portNumber)==0 );
	}
	break;

	case 2: {

		do
		{
			strcpy(name, "Enter APN username");
			strcpy(name1, "Current APN username");
			strcpy(getCharacters1,myConfigurations->apn_username);
			ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);
			if(ret !=-1 )
			{
				change_made =1;
				strcpy(myConfigurations->apn_username, getCharacters);
			}

			if(ret==-1 &&strlen(myConfigurations->apn_username)>0 )
			{
				break;
			}
			else if(ret==-1 &&strlen(myConfigurations->apn_username) == 0 )
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "APN username must be set");
				lcd_printf(ALG_CENTER, "Please try again");
				lcd_flip();
				kb_getkey();
			}

		}while(strlen(myConfigurations->apn_username)==0 );

	}
	break;

	case 3: {

		do
		{
			strcpy(name, "Enter APN password");
			strcpy(name1, "Current APN password");
			strcpy(getCharacters1,myConfigurations->apn_password);
			ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);

			if(ret != -1)
			{
				if(strlen((getCharacters)) == 0)
					continue;
				else{
					change_made =1;
					strcpy(myConfigurations->apn_password, getCharacters);
					break;
				}

			}

			else if(ret==-1 && strlen(myConfigurations->apn_password)>0 )
			{
				break;
			}
			else if(ret==-1 && strlen(myConfigurations->apn_password) == 0 )
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "APN password must be set");
				lcd_printf(ALG_CENTER, "Please try again");
				lcd_flip();
				kb_getkey();
			}

		}while(strlen(myConfigurations->apn_password)==0 );
	}
	break;

	case 4: {

		do
		{
			strcpy(name, "Enter IP PPP time out");
			strcpy(name1, "Current IP PPP time out");
			strcpy(getCharacters1,myConfigurations->ppp_timeout);
			ret = kb_getStringtwo(NUM_IN ,NUM_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);

			if( is_valid_int(getCharacters))
			{
				change_made =1;
				strcpy(myConfigurations->ppp_timeout, getCharacters);
			}
			else if(ret==-1 && strlen(myConfigurations->ppp_timeout)>0 )
			{
				break;
			}
			else if(ret==-1 && strlen(myConfigurations->ppp_timeout) == 0 )
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "PPP timeout number must be set");
				lcd_printf(ALG_CENTER, "Please try again");
				lcd_flip();
				kb_getkey();
			}
			else
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "Invalid PPP timeout value");
				lcd_printf(ALG_CENTER, "Please enter a numeric value");
				lcd_flip();
				kb_getkey();
			}
		}while(strlen(myConfigurations->ppp_timeout)==0 );
	}

	break;
	case 5: {
		selected = lcd_menu("Network Operation Mode", menu_on_off, sizeof(menu_on_off) / 100, selected);
		if(selected == -1)
		{
			return;
		}
		else if(selected == 0)
		{
			ret_val = ppp_check("/var/mux1");
			if (0 == ret_val) {
				flag_online = 1;
				login_successful = 0 ;
				message_display_function(1,"","Network Mode  ", "The POS shall operate in online mode . Please login again", (char *)NULL);
				kb_getkey();
				return ;
			}

			fag_start_ppp_session = 1;
			flag_online = 0;
			login_successful = 0 ;
			power_on_modem_device(myConfigurations->apn_username,myConfigurations->apn_password,myConfigurations->ppp_timeout);
			if(flag_online)
			{
				message_display_function(1,"","Network Mode  ", "The POS shall operate in online mode . Please login again", (char *)NULL);
				kb_getkey();


			}
			else
			{
				message_display_function(1,"","Network Mode  ", "The POS  can not operate in Online mode. Please check your connectivity and try again", (char *)NULL);
				kb_getkey();
			}
			return ;
		}
		else if(selected == 1)
		{
			message_display_function(1,"","Network Mode  ", "The POS shall operate in offline mode .", (char *)NULL);
			kb_getkey();
			login_successful = 0 ;
			flag_online = 0;
			return ;
		}
	}
	break;
	case 6:
		create_master_operator();
		//request_operators();
		break;
	default:
		break;

	}
	if(type == -1)
	{
		if(change_made)
			save_configarations();
		else
		{
			lcd_clean();
			screen_header();
			lcd_printf(ALG_CENTER, "No configuration have \n been changed");

			lcd_flip();
			kb_getkey();
		}
	}


}

void create_master_operator(){
	printf("\nhere1\n");
	char *sql;
	myMerchantUser = (MerchantFile *) malloc(sizeof(MerchantFile));
	printf("\nhere2\n");
	lcd_clean();
	screen_header();
	lcd_printf(ALG_CENTER,"Create Master user. \nPress any key to continue");

	lcd_flip();
	kb_getkey();

	sql=malloc(200);
	/*
	strcpy(myMerchantUser->name, "");
	strcpy(myMerchantUser->pin, "1");
	strcpy(myMerchantUser->idnumber, "24720604");	//username|pin|merchantid|idnumber|agentid|userlevel
	strcpy(myMerchantUser->merchantid, "10");
	strcpy(myMerchantUser->agentid, "20");
	strcpy(myMerchantUser->userlevel, "-1");
	 */

	//username|pin|merchantid|idnumber|agentid|userlevel
	sprintf(sql , "insert into operator (username,name, pin, idnumber,active, userlevel ) values ('%s', 'Default User','%s', '%s', '1', '%s' );","a","1","","-1");
	printf("\nhere3\n");


	printf("%s\n",sql);
	printf("\nhere4\n");
	//sqlite_database_read_write_operation(sql ,"operator.db");
	read_database(sql,"operator");
	free(sql);
	printf("\nhere5\n");
	change_made=1;
	printf("Line 5\n");

}


void set_innitial_configuration() {
	int  ret;
	int change_made =0;

	char getCharacters[40];
	char getCharacters1[40];
	char name1[130];
	char name[100];

	strcpy(name1, "");


	//Setting IP address
	do
	{

		strcpy(name, "Enter IP address");
		strcpy(name1, "Current IP address");

		ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);


		if( isValidIpAddress(getCharacters))
		{
			change_made =1;
			strcpy(myConfigurations->IpAddress, getCharacters);
		}

		else if(ret==-1 && strlen(myConfigurations->IpAddress)>0 )
		{
			break;
		}
		else if(ret==-1 && strlen(myConfigurations->IpAddress) == 0 )
		{
			message_display_function(1,"","IP Address Config", "Ip address must be set . Please try again", (char *)NULL);
			kb_getkey();
		}
		else
		{

			message_display_function(1,"","IP Address Config", "Invalid IP address. Please check IP and try again", (char *)NULL);
			kb_getkey();

		}
	}while(strlen(myConfigurations->IpAddress)==0 );

	//Setting port number
	do
	{
		strcpy(name, "Enter port number");
		strcpy(name1, "Current port number");

		ret = kb_getStringtwo(NUM_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);
		if( is_valid_int(getCharacters))
		{
			change_made =1;
			strcpy(myConfigurations->portNumber, getCharacters);
		}
		else if(ret==-1 && strlen(myConfigurations->portNumber)>0 )
		{
			break;
		}
		else if(ret==-1 && strlen(myConfigurations->portNumber) == 0 )
		{
			message_display_function(1,"","Port number Config", "Port number must be set . Please try again. ", (char *)NULL);
			kb_getkey();

		}
		else
		{
			message_display_function(1,"","Port number Config", "Invalid port number . Please enter a numeric value.", (char *)NULL);
			kb_getkey();

		}
	}while(strlen(myConfigurations->portNumber)==0 );

	//Setting APN username
	do
	{
		strcpy(name, "Enter APN username");
		strcpy(name1, "Current APN username");
		ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);
		if(ret !=-1)
		{
			change_made =1;
			strcpy(myConfigurations->apn_username, getCharacters);
		}

		if(ret==-1 && strlen(myConfigurations->apn_username)>0 )
		{
			break;
		}
		else if(ret==-1 && strlen(myConfigurations->apn_username) == 0 )
		{
			message_display_function(1,"","APN username Config", "APN Username must be set. Please enter a numeric value.", (char *)NULL);
			kb_getkey();

		}

	}while(strlen(myConfigurations->apn_username)==0 );

	//Setting  APN password
	strcpy(name, "Enter APN password");
	strcpy(name1, "Current APN password");
	do
	{
		printf("in password\n");
		ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);
		if(ret != -1)
		{
			change_made =1;
			strcpy(myConfigurations->apn_password, getCharacters);
		}

		if(ret==-1 && strlen(myConfigurations->apn_password)>0 )
		{
			break;
		}
		else if(ret==-1 && strlen(myConfigurations->apn_password) == 0 )
		{
			message_display_function(1,"","APN password Config", "APN password must be set. Please enter a numeric value.", (char *)NULL);
			kb_getkey();

		}

	}while(strlen(myConfigurations->apn_password)==0 );
	printf("after username\n");

	//setting PP timeout

	strcpy(name, "Enter IP PPP time out");
	strcpy(name1, "Current IP PPP time out");
	do
	{
		ret = kb_getStringtwo(NUM_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"Configurations", 1);
		if( is_valid_int(getCharacters))
		{
			change_made =1;
			strcpy(myConfigurations->ppp_timeout, getCharacters);
		}
		else if(ret==-1 && strlen(myConfigurations->ppp_timeout)>0 )
		{
			break;
		}
		else if(ret==-1 && strlen(myConfigurations->ppp_timeout) == 0 )
		{
			message_display_function(1,"","PPP timeout Config", "PPP timeout number must be set. Please enter a numeric value.", (char *)NULL);
			kb_getkey();
		}
		else
		{
			message_display_function(1,"","PPP timeout Config", "Invalid PPP timeout value. Please enter a numeric value.", (char *)NULL);
			kb_getkey();

		}
	}while(strlen(myConfigurations->ppp_timeout)==0 );


	if(change_made){
		save_configarations();
		create_master_operator();
	}else
	{
		message_display_function(1,"","Configuration Alert", "No configuration have \n been changed.", (char *)NULL);
		kb_getkey();

	}


}

int read_config() {
	FILE* config_fp;
	char line[1024 + 1];
	char *s = 0;
	int num = 0;
	char name[1024 - 1], value[1024 - 1];
	int counter;
	char buffer_int[100];

	/* FILE f;
 //system("mv config.cfg config.cfg.bak");
 f = fopen("config.cfg", "w");*/
	myConfigurations = (ConfigFile *) malloc(sizeof(ConfigFile));

	if (!myConfigurations)
		return -1;
	config_fp = fopen("config.cfg", "r");

	if (config_fp == NULL)
	{
		printf("Error Openning the file\n");
		set_innitial_configuration();

		return 1;
	}

	counter = 0;
	while (fgets(line, 1024, config_fp) != NULL) {
		num++;
		printf("line : %d :%s\n", num, line);
		//memset(name, 0, sizeof(name));
		//memset(value, 0, sizeof(value));
		if (line[0] == '\n' || line[0] == '#')
			if (line[0] == '#')
				continue;
		s = strtok(line, "=");
		if (s == NULL)
			continue;
		else
			strcpy(name, s);
		//trim(name);

		s = strtok(NULL, "=");
		if (s == NULL)
			continue;
		else
			strcpy(value, s);
		//trim(value);

		if (num < 100)
			printf("%d %s\n %s\n", num, name, value);
		strcpy(value,trim(value));

		if (strcmp(name, "IpAddress") == 0)
		{

			if(strlen((value))==0)
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "IP address must be set");
				lcd_printf(ALG_CENTER, "Please enter IP address");
				lcd_flip();
				kb_getkey();
				change_configuration(0);
				change_made =1;
			}
			else if(!isValidIpAddress(value))
			{

				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "Invalid IP address");
				lcd_printf(ALG_CENTER, "Please change IP address");
				lcd_flip();
				kb_getkey();
				change_configuration(0);
				change_made =1;
			}
			else
				strcpy(myConfigurations->IpAddress, value);

		}

		else if (strcmp(name, "portNumber") == 0)
			if(strlen((value))==0)
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "Port number must be set");
				lcd_printf(ALG_CENTER, "Please enter IP address");
				lcd_flip();
				kb_getkey();
				change_configuration(1);
				change_made =1;
			}
			else if(!is_valid_int((value)))
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "Invalid port number");
				lcd_printf(ALG_CENTER, "Please change port number");
				lcd_flip();
				kb_getkey();
				change_configuration(1);
				change_made =1;
			}
			else
				strcpy(myConfigurations->portNumber, value);

		else if (strcmp(name, "apn_username") == 0)
		{
			if(strlen((value))==0)
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "APN username must be set");
				lcd_printf(ALG_CENTER, "Please configure APN username");
				lcd_flip();
				kb_getkey();
				change_configuration(2);
				change_made =1;
			}

			else
				strcpy(myConfigurations->apn_username, value);
		}

		else if (strcmp(name, "apn_password") == 0)
		{
			/*   if(strlen((value))==0)
   {
    lcd_clean();
    screen_header();
    lcd_printf(ALG_CENTER, "APN password must be set");
    lcd_printf(ALG_CENTER, "Please configure APN password");
    lcd_flip();
    kb_getkey();
    change_configuration(3);
    change_made =1;
   }
   else
   strcpy(myConfigurations->apn_password, value);*/

			strcpy(myConfigurations->apn_password, value);
		}

		else if (strcmp(name, "ppp_timeout") == 0)
		{
			if(strlen((value))==0)
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "Port number must be set");
				lcd_printf(ALG_CENTER, "Please enter IP address");
				lcd_flip();
				kb_getkey();
				change_configuration(4);
				change_made =1;
			}
			else if(!is_valid_int((value)))
			{
				lcd_clean();
				screen_header();
				lcd_printf(ALG_CENTER, "Invalid port number");
				lcd_printf(ALG_CENTER, "Please change port number");
				lcd_flip();
				kb_getkey();
				change_configuration(4);
				change_made =1;
			}//888888888888888888888

			else
				strcpy(myConfigurations->ppp_timeout, value);
		}
		else if (strcmp(name, "user_level") == 0){

		}

		else
			printf("WARNING: %s/%s: Unknown name/value pair!\n", name, value);
	}

	if(change_made){
		save_configarations();
	}
	/*
 strcpy(url ,"http://" );
 snprintf(buffer_int,strlen(myConfigurations->IpAddress)+1 ,myConfigurations->IpAddress );
 strcat(url , buffer_int);
 snprintf(buffer_int,strlen(myConfigurations->portNumber)+1 ,myConfigurations->portNumber );
 strcat(url ,":");
 strcat(url , buffer_int);

 strcat(url , "/safaricom/service/");
 printf("The URL : %s \n",url);*/
	return 0;
}
