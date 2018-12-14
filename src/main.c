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
#include "main.h"
#include "../src/utilities/keyboart.h"
#include "../src/utilities/send_online_request.h"
#include "../src1/jsonread.h"


//Beneficiary  management functions
#include "beneficiary_transactions.h"

//Merchant management functions
#include "merchant_transactions.h"

cJSON * generate_json_from_sql(char * db, char * sql, int * i);
/**
 * function to create processes
 */


cJSON * generate_json_from_sql(char * db, char * sql , int * ret_val)
{


	cJSON * txJson ;// a json object
	cJSON * arrayJson ; //a json array

	int x , y;
	int transactions_count  = 0;

	read_database(sql,db);
	printf("DB Count %d\n", dbreadCounter);
	*ret_val = 0;
	//dbreadCounter is the number of items returned
	//if values are returned
	if(dbreadCounter)
	{
		*ret_val = dbreadCounter/collums_in_rs;
		y = 0;
		txJson = cJSON_CreateObject();//create new json object
		arrayJson = cJSON_CreateArray();//create new json array
		//transactions_count will return number of rows returned
		transactions_count =  dbreadCounter/collums_in_rs;
		/**
		 * loop through columns and dbreadounter to create a jSON object
		 * create a jSON object as:
		 * 		-txJson {
		 * 		 db_collumn_names[y]: db_values[x];
		 * 		}
		 * when all columns have been looped through, add txJson to arrayJson
		 * set txJson to create a new json object
		 * set y (columns counter to zero)
		 * delete txJson and return arrayJson containing created json
		 */
		for(x = 0 ; x < (dbreadCounter ); x++){
			cJSON_AddStringToObject(txJson,db_collumn_names[y], db_values[x]);
			y++;
			if(y == collums_in_rs)
			{
				cJSON_AddItemReferenceToArray(arrayJson , txJson);
				txJson =cJSON_CreateObject();
				y = 0;
			}
		}
		cJSON_Delete(txJson);
		return arrayJson;
	}
	else
	{
		return cJSON_CreateNull();
	}

}
void* threader(void *arg)
{
	time_t start, current;
	time_t start_for_flag_update, current_for_flag_update;
	//cJSON * empty_obj = cJSON_CreateObject();
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
				start_for_flag_update = time(NULL);
				mysql = "select billno as billno , receipt_printed as receiptprinted , voided_col as void from transactions;";
				obj = generate_json_from_sql("vihiga.db", mysql,&i);
				printf("Count  : %d\n", i);
				if(i )
					getDataFromServer("UPDATE_PRINT_FLAGS",obj,REQUEST_POST , endpoints->transaction);



			}
		}
		i++;
	} while (1);
	return NULL;
}
void offline_login(char * password){
	char * sqlstmt;
	char hashedpassword[40];

	//flag_online = 0;
	sqlstmt = malloc(700);
	//password received by this function is pword,obtained from user input when prompted by the POS to log in
	strcpy(hashedpassword,hash_fnc(password));
	/**
	 * Currentuser.username is obtained from username user enters when prompted by the POS
	 */

	sprintf(sqlstmt , "select idnumber as login_userid  from operator where username =  '%s' and pin = '%s';", CurrentOperator.username, password);

	//sprintf(sqlstmt , "select userid as login_userid ,username as login_username ,password as login_password,watchTimer as login_watchTimer ,name as login_name	, MarketID as login_MarketID	, Market as login_Market	, paybill as login_paybill	 ,Currency as login_Currency	, voidTime as login_voidTime ,voidLimit  as login_voidLimit , transactionLimit  , reprint , reprintLimit as login_transactionLimit , systemName as login_systemName , posVersion as login_posVersion from users where username =  LOWER('%s') and password = '%s';", CurrentUser.username, hashedpassword);
	printf("%s\n",sqlstmt);
	increament_read = 0;
	read_database(sqlstmt,"operator");
	free(sqlstmt);
	/**
	 * if results are found,it means users exist,set login_successful to 1
	 */
	if(increament_read > 0)
	{

		login_successful  = 1;
		flag_offline_login = 1;

	}
	/**
	 * if increament_read = 0,it means no results were found,
	 * this is a case of invalid login credentials
	 */
	else
	{
		message_display_function(1,"","Invalid Offline Login", "Please check the username or password and Try again !", (char *)NULL);
		kb_getkey();
		login_successful  = 0;
	}
	//set increament_read to 0 for the next login
	increament_read = 0;

}



void login() {
	char password[40];
	char uname[100];
	char pword[100];
	int ret ;
	pthread_t tid;
	logged_offline = 1;

	lcd_clean();
	strcpy(CurrentOperator.username,"");
	strcpy(password,"");
	label_one = "Please enter username";
	label_two = "Please enter Password";
	ret = kb_getStringtwo(ALPHA_IN ,PASS_IN ,  1, 16, uname, pword,NULL , label_one, label_two , "Getting Login Credentials", 0);

	if(ret == -1)
				  {
		login();
				  }
	else
	{
		strcpy(CurrentOperator.username , uname);
		strcpy(password, pword);
		strcpy(CurrentOperator.pin, password);
		lcd_clean();

		logged_offline = 1;;
		offline_login(pword);

		}
	}



int main(int argc, char *argv[])
{
	int selected = 0;
	int retval = 0;
	cJSON * requestjson;
	cJSON * getusersjson;
	char  requestType[30];
	char url[50];
	create_all_table();
	char  response1[5000];
	lcd_init(&argc, &argv);
	lcd_set_bk_color(COLOR_WITE);
	lcd_set_font_color(COLOR_BLACK);
	read_config();
	sys_get_sn(serial_num,100);
	printf("Serial_Number = %s\n", serial_num);
	fag_start_ppp_session = 0;

	const char  main_menu[][100] = {"Beneficiary Transactions",
									"Merchant  Transactions",
									"POS/User Management" ,
									"Send Online Request"
									"Exit"
									};

	while(1)
	{


		LOGIN: login();
		//login_successful = 1;;
		if(login_successful){
			pthread_t tid;
					int err;


					err = pthread_create(&(tid), NULL, &threader, NULL);
		while (login_successful) {

		switch(lcd_menu("Safaricom ", main_menu, 4,selected))
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
			strcpy(url,"/api/usersrequest/fetchusers");
			strcpy(requestType,"data");
			getusersjson = cJSON_CreateObject();
			cJSON_AddStringToObject(getusersjson,"mac_address",serial_num);

			strcpy(response1, send_gprs_request(requestType, getusersjson, url));
			printf("RESPONSE %s\n", response1);

			break;
		case 4:
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

