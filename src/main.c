/*
 * main.c
 *
 *  Created on: Oct 26, 2018
 *      Author: linux
 */
#include "utilities/inputs.h"
#include <wnet.h>
#include <pthread.h>


#include "mutwol.h"
//Common includes
#include "common_includes.h"
#include <posapi.h>

//User management functions
#include "device_management.h"
#include "usersManagement.h"

#include "main.h"
#include "../src/utilities/keyboart.h"
#include "../src/utilities/send_online_request.h"
//#include "../src1/jsonread.h"


//Beneficiary  management functions
#include "beneficiary_transactions.h"

//Merchant management functions
#include "merchant_transactions.h"

#include "utilities/inputs.h"

//Common includes
#include "common_includes.h"

//User management functions
#include "device_management.h"

//Beneficiary  management functions
#include "beneficiary_transactions.h"

//Merchant management functions
#include "merchant_transactions.h"

#include "../src/utilities/common_functions.h"


/**
 * function to create processes
 *
 */


void* threader(void *arg)
{
	int signal_num=0;
	int i = 0;


	time_t start, current;
	time_t start_for_flag_update, current_for_flag_update;

	int retval= power_on_modem_device(myConfigurations->apn_username, myConfigurations->apn_password, myConfigurations->ppp_timeout);
	if(retval == GPRS_SUCCESS)
		flag_online =1 ;

	start = time(NULL);
	start_for_flag_update = time(NULL);
	do {
		/*		if(i == 0)
			message_reader();*/
		current = time(NULL);
		current_for_flag_update= time(NULL);
		if (difftime(current, start) == 30)
		{
			/*			if(get_SMS_Count())
				message_reader();*/
		}
		if (difftime(current, start) == CurrentOperator.watchTimer) {
			//if (difftime(current, start) == 5) {
			start = time(NULL);
			/*			if(!disable_timer)
			{*/
			wnet_signal(&signal_num);
			if(signal_num<10)
			{
				printf("Low signal strnght\n");
				//login_successful = 0;
				//online = 0;
			}
			else
			{
				//online = 1;
				//login_successful = 1;
				printf("Normal signal strnght\n");
			}
			if( flag_online)
			{
				//getDataFromServer("WATCH",empty_obj,REQUEST_POST , endpoints->transaction);
				//cJSON_Delete(empty_obj);

				post_pos_offline_transactions();
			}


			//}

		}
		if (difftime(current_for_flag_update, start_for_flag_update) == 1800) {
			if(flag_online && login_successful)
			{
				/*			start_for_flag_update = time(NULL);
				mysql = "select billno as billno , receipt_printed as receiptprinted , voided_col as void from transactions;";
				obj = generate_json_from_sql("vihiga.db", mysql,&i);
				printf("Count  : %d\n", i);
				if(i )
					getDataFromServer("UPDATE_PRINT_FLAGS",obj,REQUEST_POST , endpoints->transaction);


				 */

				post_pos_offline_transactions();
			}
		}
		i++;
	} while (1);
	return NULL;
}



int main(int argc, char *argv[])
{
	int selected = 0;
	int retval = 0;
	char  main_menu[][100] = {"Beneficiary Transactions","Merchant  Transactions","POS/User Management","Logout"};


	final_create_all_tables();


	lcd_init(&argc, &argv);
	lcd_set_bk_color(COLOR_WITE);
	lcd_set_font_color(COLOR_BLACK);


	message_display_function(1,"","Initializing ","",(char *)NULL);
	lcd_printf(ALG_LEFT,"");
	lcd_printf(ALG_CENTER,"Loading HIBAH POS ... ");
	lcd_printf(ALG_CENTER,"Version XPOS : 1.0.0");
	lcd_printf(ALG_CENTER,"Release Date : 13TH Mar 2019");
	lcd_flip();
	read_config();
	fag_start_ppp_session = 0;


	final_create_all_tables();
	lcd_init(&argc, &argv);
	lcd_set_bk_color(COLOR_WITE);
	lcd_set_font_color(COLOR_BLACK);

	sys_get_sn(pos_serial_number,40);

	initialize_params();
	//system("echo 'dogoodnotevil' | su -c 'cp *  /media/usb1/files/'");
	while(1)
	{


		LOGIN: login();


		printf("\n login_successful is %d and su_ligin_successful is %d", login_successful, su_login_successful);


		//login_successful = 1;;
		if(login_successful){
			pthread_t tid;
			int err;


			err = pthread_create(&(tid), NULL, &threader, NULL);
			while (login_successful) {

				switch(lcd_menu("HIBAH Value Agency", main_menu,sizeof(main_menu)/100,selected))
				{
				case 0:

					beneficiary_transactions();
					break;
				case 1:

					merchant_transactions();
					break;
				case 2:
					device_management();
					break;
				case 3:
					goto LOGIN;
					break;
				}
			}

		}
		else{
			goto LOGIN;
		}

		//}

	}
	return retval;
}

void printfConfigs(void){
	int i = 0;
	while(true){
		printf("username %s\n",myLoginPosUser->username);
		i++;
		if(i>50){
			break;
		}
	}
}
void display_gprs_start_option(int feedback)
{

	if(feedback == GPRS_SUCCESS)
	{

		message_display_function(1, "", "GPRS Start Message", "GPRS connection established successfully", (char *)NULL);
		flag_online = 1;
	}
	else
		flag_online = 0;

	if(feedback == GPRS_FAILED_TO_ATTACH_TO_NETWORK)
	{
		message_display_function(1, "", "GPRS Start Error", "Failed to attach to GPRS network. Please review your configuration and try again", (char *)NULL);
	}
	if(feedback == GPRS_FAILED_TO_START_DEVICE)
	{
		message_display_function(1, "", "GPRS Start Error", "Failed to start the GPRS Modem device. Please restart POS and try again", (char *)NULL);
	}
	if(feedback == GPRS_FAILED_TO_START_PPP)
	{
		message_display_function(1, "", "GPRS Start Error", "Failed to start PPP. Please select \"Online Mode\" from configuration to restart PPP", (char *)NULL);
	}
	if(feedback == GPRS_FAILED_TO_START_MODEL)
	{
		message_display_function(1, "", "GPRS Start Error", "Failed to start GPRS Model. Please select \"Online Mode\" from configuration to restart PPP", (char *)NULL);
	}
	kb_getkey();
}
