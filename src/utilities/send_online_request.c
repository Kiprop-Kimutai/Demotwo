/*
 * send_online_request.c
 *
 *  Created on: Dec 9, 2018
 *      Author: linux
 */
#include "keyboart.h"
#include <ppp.h>
#include "lcd.h"
#include "send_online_request.h"
#include "cJSON.h"
#include <posapi.h>
#include <wnet.h>
#include <ppp.h>
#include "../../src/main.h"
#include <sys/time.h>
#include <curl/curl.h>
#include <time.h>
#include "../../src1/jsonread.h"


#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */

void error(const char *msg) { perror(msg); exit(0); }
char gprs_chat_file[] = { "ABORT          'NO CARRIER'" "\n"
		"ABORT          'NO DIALTONE'" "\n"
		"ABORT          'ERROR'" "\n"
		"ABORT          'NO ANSWER'" "\n"
		"ABORT          'BUSY'" "\n"
		"ABORT          'Invalid Login'" "\n"
		"ABORT          'Login incorrect'" "\n"
		"TIMEOUT        '60'" "\n"
		"''             'ATZ'" "\n"
		"'OK'           'AT+CGDCONT=1,\"IP\",\"4.2.2.2\"'" "\n"
		"'OK'           'ATDT*99***1#'" "\n"
		"'CONNECT'      ''" "\n" };

char * send_gprs_request(char * requestType, cJSON * requestjson, char * url ){

	int ret_val;

char  string1[100];

	if(/*!logged_offline &&*/ /*flag_online  && */login_successful)
	{
		disable_timer = 1;


		if(flag_online == 0)
		{
			message_display_function(1,"","Network Mode Config ", "Please wait as the POS switches GPRS configuration", (char *)NULL);

			ret_val  = power_on_modem_device(myConfigurations->apn_username , myConfigurations->apn_password, atoi(myConfigurations->ppp_timeout));
			if(ret_val == GPRS_SUCCESS)
			{
				screen_header();
				lcd_printf(ALG_CENTER, "");
				lcd_printf(ALG_CENTER, "Please wait ..");
				lcd_printf(ALG_CENTER, "Sending Request");
				lcd_flip();


				printf("%s initial json\n",cJSON_Print(requestjson));
			strcpy(	string1,getDataFromServer(requestType,requestjson,0,url));


				return string1;

			}
			else
			{
				message_display_function(1,"","Online Posting Error", "The POS is operating in offline mode. Please turn on GPRS to post your transactions.", (char *)NULL);
				kb_getkey();
				return "network error";
			}


		}


	}
	else
	{
		message_display_function(1,"","Online Posting Error", " Please confirm if you have Internet connectivity and you have been logged in online to do a Z report", (char *)NULL);
		kb_getkey();
		return "Error";
	}
	return string1;

}


void start_modem(void){

	ret_val = ppp_check("/var/mux1");
	if (0 == ret_val) {
/*				flag_online = 1;
		flag_offline_login = 0;
		login_successful = 0 ;
		logged_offline = 0;*/
		if(login_successful && logged_offline)
		{
			logged_offline = 0;
			cJSON * cjson_login_params = cJSON_CreateObject();
			//cJSON_AddStringToObject(cjson_login_params , "password", CurrentUser.password);
			//getDataFromServer("LOGIN", cjson_login_params ,REQUEST_POST, endpoints->transaction);

			cJSON_Delete(cjson_login_params);
		}
		flag_offline_login = 0;
		logged_offline = 0;
		flag_online = 1;

		message_display_function(1,"","Network Mode  ", "The POS shall operate in online mode . Please login again", (char *)NULL);
		kb_getkey();
		return ;
	}

/*			fag_start_ppp_session = 1;
	flag_online = 0;
	flag_offline_login = 0;
	login_successful = 0 ;
	logged_offline = 0;*/
	power_on_modem_device(myConfigurations->apn_username,myConfigurations->apn_password,myConfigurations->ppp_timeout);

	if(flag_online)
	{
		if(login_successful && logged_offline)
		{
			logged_offline = 0;
			cJSON * cjson_login_params = cJSON_CreateObject();
			/*cJSON_AddStringToObject(cjson_login_params , "password", CurrentUser.password);
			getDataFromServer("LOGIN", cjson_login_params ,REQUEST_POST, endpoints->transaction);*/

			cJSON_Delete(cjson_login_params);
		}
		flag_offline_login = 0;
		logged_offline = 0;
		flag_online = 1;

		message_display_function(1,"","Network Mode  ", "The POS shall operate in online mode . Please login again", (char *)NULL);
		kb_getkey();


	}
	else
	{
		if(login_successful)
			flag_offline_login = 1;
		else
			flag_offline_login = 0;
		logged_offline = 1;
		flag_online = 0;
		message_display_function(1,"","Network Mode  ", "The POS can not operate in Online mode. Please check your connectivity and try again", (char *)NULL);
		kb_getkey();
	}
	return ;

}

int  power_on_modem_device(char * apn_username , char *  apn_password , int timeout) {
	int retval = 0;

	struct timeval start,cur, diff, end;
	struct timezone tz;
	printf("1\n");
	retval = ppp_check("/var/mux1");
	if (0 == retval)
	{
		return GPRS_SUCCESS;
	}
	printf("before power 1\n");
		retval = wnet_power_on();
		printf("1 afterr power\n");
		if (0 != retval){
			return GPRS_FAILED_TO_START_DEVICE;
		}else {

			retval = wnet_init("/var/mux0");
			printf("after wnet 1\n");
			if (0 != retval){
				return GPRS_FAILED_TO_START_MODEL;
			}else {
				//Started WNET

				sim_power_status = 1;
				gettimeofday(&start, &tz);
				diff.tv_sec = 20;
				diff.tv_usec = 0;
				timeradd(&start, &diff, &end);
				while (1){
					printf("b4 attached 1\n");
					retval = wnet_set_attached(1);
					if (0 != retval){
						gettimeofday(&cur, &tz);

						if (timercmp(&cur, &end, < ))
							usleep(10000);
						else {
							return GPRS_FAILED_TO_ATTACH_TO_NETWORK;
						}
					}else
						break;
				}

				if (0 == retval){
					//retval = ppp_open("/var/mux1", gprs_chat_file, "saf", "data", PPP_ALG_PAP, 30);
					printf("before PPP \n");
					retval = ppp_open("/var/mux1", gprs_chat_file,apn_username, apn_password, PPP_ALG_PAP,timeout);
					printf("PPP_OPEN : %d\n",retval);
					if (0 != retval){

						//sleep(1);
						printf("check \n");
						retval = ppp_check("/var/mux1");
						printf("after check \n");
						if (0 == retval)
						{



							return GPRS_SUCCESS;
						}
						else
						{
							return GPRS_FAILED_TO_START_PPP;
						}
					}else {
						gettimeofday(&start, &tz);
						diff.tv_sec = 20;
						diff.tv_usec = 0;
						timeradd(&start, &diff, &end);
						while (1){
							retval = ppp_check("/var/mux1");
							if (0 == retval){
								return GPRS_SUCCESS;
							}
							else{
								gettimeofday(&cur, &tz);
								if (timercmp(&cur, &end, < ))
									usleep(10000);
								else {

									break;
								}
							}
						}

						//ppp_close("/var/mux1");
					}
				}
			}

			//wnet_power_down();
		}
		sim_power_status = 1;
		return GPRS_SUCCESS;
}
int start_ppp_session(char * requestType, char * request ,  int operation , char * url) {
	CURL *curl;
	CURLcode res;
	char filename[100];
	FILE *out_fd = (FILE *) 0;
	FILE *head_fd = (FILE *) 0;
	struct curl_slist *headers = NULL;


	//if(endpoint == ENDPOINT_TRANSACTION)


	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		sprintf(filename, "%s%s.txt", "", requestType);
		out_fd = fopen(filename, "w");
		head_fd = fopen("header.txt", "w"); //open for read and write
		res = curl_easy_setopt(curl, CURLOPT_FILE, out_fd);
		res = curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);

		printf("%s\n", url);

		res = curl_easy_setopt(curl, CURLOPT_URL, url);
		res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS,request);
		res = curl_easy_setopt(curl, CURLOPT_WRITEHEADER, head_fd);

		token_received = 0 ;
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		disable_timer = 0;
		if(res!= CURLE_OK)
		{
			out_fd = fopen("error.txt", "w");
			fclose(out_fd);
			return CURL_FAILED_POST;
		}
		else
		{
			fclose(out_fd);
			fclose(head_fd);

			printf("CURL RESPONSE:::%d\n",CURL_SUCCESS);
			return CURL_SUCCESS;
		}

	}
	else
		return CURL_FAILED_SETUP;
}
char * getDataFromServer (char * requestType , cJSON * request ,   int operation ,  char * endpoint)
{
	printf("\noffload point atget Data 1\n\n\n");
	char * final_request;
		char filename[100];
		char adminz[100];
char response[5000];
		int resp;

		sprintf(filename , "%s.txt" , requestType);

		sprintf(url , "http://%s:%s%s",myConfigurations->IpAddress,myConfigurations->portNumber,endpoint);
		memset(serial_num,0,sizeof(serial_num)+1);
			sys_get_sn(serial_num,100);
			printf("Serial num : %s\n\n\n\n\n\n\n", serial_num);

		printf("%s\n",cJSON_Print(request));

		resp = start_ppp_session(requestType, request , operation , url);



		if(resp == CURL_SUCCESS)
		{
		strcpy(response, Start_online_display(filename));
			token_received =1;
			return response;
		}
		else if(resp == CURL_FAILED_POST)
		{
			process_response_on_fail(requestType);
			return "";
		}

		return response;
	}
void process_response_on_fail (char * requestType)
{

	token_received =1;

	if( strcmp(requestType,"REPORTS")!=0)
	{
		flag_stop_z = 1;
		flag_online = 0;
	}
	if(strcmp(pmode, "Cash")==0 && strcmp(requestType,"TRANSACTION")==0)
		lcd_printf(ALG_CENTER, " Transaction shall be saved offline.");
	if(strcmp(requestType,"TRANSACTION")==0)
	{
		if(strcmp(pmode, "Cash")==0 )
		{
			//stock_sale_selected_2 = 0;
			//if(stock_sale_selected)
			{
				Printer_Demo("TX RECEIPT");
				//stock_sale_selected_2 = 1;
				message_display_function(1,"", "Stock Sale receipt","Please cut the stock sale receipt and press any key to reprint", (char *)NULL);
				kb_getkey();
			}
			Printer_Demo("TX RECEIPT");
			//stock_sale_selected_2 = 0;
		sprintf(transactionfile , "update TRANSACTIONS set receipt_printed = '1' where billno = '%s'", recptnum);
		sqlite_database_insert_into_table(transactionfile,"transaction");


		usleep(10000);
		please_print = 1;

		sprintf(transactionfile , "update Z_REPORTS set Z_done = '0' , Printed = 1 where billno = '%s'", recptnum);
		sqlite_database_insert_into_table(transactionfile,"transaction");
		}
		else
		{
			//revert_receipt_number();
		}
	}

	if( strcmp(requestType,"LOGIN")==0)
	{
		message_display_function(1,"","Error In Login", "Server connection failed. Please check connection and try again. Proceeding offline", (char *)NULL);
		kb_getkey();
	}


}
char * buildFinalrequest1(char * requestType , cJSON * request )
{
	if(strcmp(requestType,"LAND") == 0){
		printf("output 1\n\n\n");
		printf("%s\n",cJSON_Print(request));
		return cJSON_Print(request);
	}
	cJSON_AddStringToObject(request,"command",requestType);
		cJSON_AddStringToObject(request,"username",CurrentOperator.username);
		cJSON_AddStringToObject(request,"token",token);
		cJSON_AddStringToObject(request,"serialNo",serial_num);
		//cJSON_AddStringToObject(request,"password",CurrentUser.password);
	/*	if(strcmp(requestType,"ZVERIFY")== 0)
		{
			strcpy(ZNUMBER, get_String_value_from_json(request,"znumber"));
		}*/
		printf("********************************************\n");
		printf(cJSON_Print(request));
		printf("************\n");
		return cJSON_Print(request);
}

