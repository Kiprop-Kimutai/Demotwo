/*
 * device_management.c
 *
 *  Created on: Dec 5, 2018
 *      Author: linux
 */


#include <stdlib.h>
#include <ppp.h>
#include "common_includes.h"
#include "device_management.h"
#include "../src/utilities/send_online_request.h"
#include "usersManagement.h"
#include "../src/utilities/lcd.h"
#include "../src/utilities/keyboart.h"
#include "../src1/jsonread.h"
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
	const char  device_managment_menu[][100] = {"POS Device Configuration","POS User Account Operation", "Exit"};
	while(1)
	{
		switch(lcd_menu("POS User Management", device_managment_menu, 3 ,selected))
		{
		case 0:
			pos_device_configuration(-1);
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

void pos_device_configuration(int type){
    int selected , ret;
    int change_made =0;
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
                flag_offline_login = 0;
                login_successful = 0 ;
                logged_offline = 0;
                message_display_function(1,"","Network Mode  ", "The POS shall operate in online mode . Please login again", (char *)NULL);
                kb_getkey();
                return ;
            }

            fag_start_ppp_session = 1;
            flag_online = 0;
            flag_offline_login = 0;
            login_successful = 0 ;
            logged_offline = 0;
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
            flag_offline_login = 0;
            login_successful = 0 ;
            logged_offline = 0;
            flag_online = 0;
            return ;
        }
    }
    break;
    case 6:
        request_operators();
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
