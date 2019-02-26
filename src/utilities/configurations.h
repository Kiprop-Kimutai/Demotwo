/*
 * configurations.h
 *
 *  Created on: Feb 28, 2015
 *      Author: tarus
 */

#ifndef CONFIGURATIONS_H_
#define CONFIGURATIONS_H_
#include <posapi.h>
#include <sys/socket.h>
#include <time.h>


//Variables
/*typedef struct configurations
{
	//Network
	char IpAddress[50];
	char portNumber[50];
	char apn_username[50];
	char apn_password[50];
	char ppp_timeout[30];
	char network_access_mode[20];
	char purchase_mode[40];
	char pos_timeout[30];
	char card_access_mode[30];
	char batch_size[40];

	//County Details
	char county_address[100];
    char county_name[100];

    //SIM config
    char balance_code[40];
    char recharge_code[40];
    char data_balance_code[40];
    char data_recharge_code[40];
    char sim_name[40];
    char phone_number[20];
   // char
	//int ppp_timeout;
}
ConfigFile;
ConfigFile myConfigurations;*/


//configuration functions and methods
int save_configurations(char * config , char * value , char * name) ;
void change_network_configuration(int type );
void change_sim_configuration(int type );
void change_system_configuration(void);
void set_date(void);
void set_innitial_configuration(void );
int Read_users_config(void);
void synch_service(void);
void change_admin_details(int type);
void admin_functions(void);
void view_config(int x);
int sys_software_update(const char *filename);

//To be checked.
void save_admin_configarations(void);
void verify_z_report(void);
void set_network(void);




char const config_menus_array[20][100];

#include "lcd.h"

#include "keyboart.h"






/*extern int kb_getString(uint8_t ucMode, uint8_t ucMinlen, uint8_t ucMaxlen,
		char *pszOut, uint8_t ucTimeOut, const char *ch);
extern int kb_getStringtwo(uint8_t ucMode,uint8_t ucMode1,  uint8_t ucMinlen, uint8_t ucMaxlen,
		char *pszOut,char *pszOut1 ,  const char *ch, char *charact, char *charact2);*/




#endif /* CONFIGURATIONS_H_ */
