/*
 * device_management.h
 *
 *  Created on: Dec 5, 2018
 *      Author: linux
 */

#ifndef SRC_DEVICE_MANAGEMENT_H_
#define SRC_DEVICE_MANAGEMENT_H_


	typedef struct configurations
	    {
	        char IpAddress[50];
	        char portNumber[50];
	        char apn_username[50];
	        char apn_password[50];
	        char ppp_timeout[30];
	        char user_level[10];
	    }
	    ConfigFile;
	    ConfigFile *myConfigurations;
	    int read_config(void) ;
void device_management(void );
void create_master_operator();

void save_configarations(void);
void change_configuration(int type);

int change_made;
enum curlPostError
{
 CURL_FAILED_POST,
 CURL_SUCCESS,
 CURL_FAILED_SETUP
};

#endif /* SRC_DEVICE_MANAGEMENT_H_ */
