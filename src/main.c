/*
 * main.c
 *
 *  Created on: Oct 26, 2018
 *      Author: linux
 */
#include "utilities/inputs.h"



//Common includes
#include "common_includes.h"
#include <posapi.h>

//User management functions
#include "device_management.h"
#include "usersManagement.h"

#include "main.h"
#include "../src/utilities/keyboart.h"
#include "../src/utilities/send_online_request.h"
#include "../src1/jsonread.h"


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
 */


void* threader(void *arg)
{
	time_t start, current;
	time_t start_for_flag_update, current_for_flag_update;
	cJSON * obj ;
	char * mysql;
	int signal_num=0;
	int i = 0;


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
			if(!disable_timer)
			{
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
				if(!logged_offline && flag_online)
				{
					//getDataFromServer("WATCH",empty_obj,REQUEST_POST , endpoints->transaction);
					//cJSON_Delete(empty_obj);
				}


			}

		}
		if (difftime(current_for_flag_update, start_for_flag_update) == 1800) {
			if(!logged_offline && flag_online && login_successful)
			{
				/*			start_for_flag_update = time(NULL);
				mysql = "select billno as billno , receipt_printed as receiptprinted , voided_col as void from transactions;";
				obj = generate_json_from_sql("vihiga.db", mysql,&i);
				printf("Count  : %d\n", i);
				if(i )
					getDataFromServer("UPDATE_PRINT_FLAGS",obj,REQUEST_POST , endpoints->transaction);


				 */
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
	const char  main_menu[][100] = {"Beneficiary Transactions","Merchant  Transactions","POS/User Management" , "Exit"};
	char * date_t;
	char * receipt_t;

	final_create_all_tables();
	create_all_table();
	lcd_init(&argc, &argv);
	lcd_set_bk_color(COLOR_WITE);
	lcd_set_font_color(COLOR_BLACK);
	read_config();
	sys_get_sn(serial_num,100);
	printf("Serial_Number = %s\n", serial_num);
	fag_start_ppp_session = 0;


	lcd_init(&argc, &argv);
	lcd_set_bk_color(COLOR_WITE);
	lcd_set_font_color(COLOR_BLACK);

	printf("%s %s\n",date_t, receipt_t  );
	sys_get_sn(pos_serial_number,40);


	while(1)
	{


		LOGIN: login();
		//login_successful = 1;;
		if(login_successful){
			pthread_t tid;
			int err;


			err = pthread_create(&(tid), NULL, &threader, NULL);
			while (login_successful) {

				switch(lcd_menu("Safaricom Value Agency", main_menu, 4,selected))
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

		}else{
			goto LOGIN;
		}

	}
	return retval;
}
