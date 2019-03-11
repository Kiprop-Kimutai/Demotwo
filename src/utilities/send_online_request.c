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
#include "common_functions.h"
#include "../device_management.h"
#include "cJSON.h"
#include "JSON_checker.h"
#include <posapi.h>
#include <wnet.h>
#include <ppp.h>
#include "../../src/main.h"
#include <sys/time.h>
#include <curl/curl.h>
#include <time.h>


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


/*
 * if ret =  0 - Failure
 * else if ret  = 1  -  Success
 */
int send_gprs_request(char * requestType, cJSON * requestjson, char * endpoint ,  cJSON ** response_json , int display_errors_messages ){

	char filename[100];
	char  url[200];
	int resp;
	char  * response;
//	char *  file_string;

	//char * reponse;

	//char  string1[100];

	printf("initial json\n");
	if(flag_online == 0)
	{
		if(display_errors_messages)
		message_display_function(1,"","Network Mode Config ", "Please wait as the POS switches GPRS configuration", (char *)NULL);

		ret_val  = power_on_modem_device(myConfigurations->apn_username , myConfigurations->apn_password, atoi(myConfigurations->ppp_timeout));
		if(ret_val == GPRS_SUCCESS){
	/*	if(1)
		{*/
		}
		else
		{
			if(display_errors_messages){
				message_display_function(1,"","Online Posting Error", "The POS is operating in offline mode. Please turn on GPRS to post your transactions.", (char *)NULL);
				kb_getkey();}
			return 0;
		}
		/*		else
		{
			return 0;
		}*/
	}

	//}


if(display_errors_messages)
	message_display_function(1,"", "Posting Transaction","Please wait .." ,"Sending Request" ,( char *) NULL  );
/*	screen_header();
	lcd_printf(ALG_CENTER, "");
	lcd_printf(ALG_CENTER, "Please wait ..");
	lcd_printf(ALG_CENTER, "Sending Request");
	lcd_flip();*/


	printf("%s initial json\n",cJSON_Print(requestjson));
	sprintf(filename , "%s.txt" , requestType);
	sprintf(url , "https://%s:%s%s",myConfigurations->IpAddress,myConfigurations->portNumber,endpoint);
	resp = start_ppp_session(requestType, cJSON_Print(requestjson) , 0 , url ,&response);

	if(resp == CURL_SUCCESS){

		//char filelast[100];
/*		FILE* body_file ;
		body_file = fopen(filename, "r"); //open for read and write
		printf("FILE NAME: %s\n",filename);
		if (body_file == NULL)
		{
			if(display_errors_messages)
			{
				message_display_function(1,"","Network Error", "No data received from server. Please check connection and try again", (char *)NULL);
				kb_getkey();
			}
			return 0;

		}
		file_string  = malloc(10000);
		//memset(file_string,0,sizeof(file_string));
		if (fgets(file_string, 10000, body_file) != NULL)
		{
			printf("RETURNED : \n %d : \n%s\n\n", strlen(file_string) , file_string);
			fclose(body_file);
			//sprintf(filelast,"rm %s",filename);
			system(filelast);*/
			if(strlen(response))
			{
				printf("Returned from  ppp :  %s\n" ,  response);
				if(jcheck(response))
				{
					*response_json = cJSON_Parse(response);
					//free(file_string);
					return 1;
				}
				else
				{
					//free(file_string);
					if(display_errors_messages){
						message_display_function(1,"","Network Error", "Invalid received from server. Please check connection and try again", (char *)NULL);
						kb_getkey();
					}
				}
			}else
			{

			//free(file_string);
			if(display_errors_messages){
			message_display_function(1,"","Server Error", "No response from server. Contact administrator.\n Press any key to continue", (char *)NULL);
			kb_getkey();
			}


			}
		}

	/*}*/
	else if(resp == CURL_FAILED_POST){
		//process_response_on_fail(requestType);
		if(display_errors_messages){
			message_display_function(1,"","Network Error", "Server connection failed. Please check connection and try again", (char *)NULL);
			kb_getkey();
		}
	}

	return 0;


}


void start_modem(void){

	ret_val = ppp_check("/var/mux1");
	if (0 == ret_val) {
		/*				flag_online = 1;
		flag_offline_login = 0;
		login_successful = 0 ;
		logged_offline = 0;*/
		if(login_successful)
		{
			cJSON * cjson_login_params = cJSON_CreateObject();
			//cJSON_AddStringToObject(cjson_login_params , "password", CurrentUser.password);
			//getDataFromServer("LOGIN", cjson_login_params ,REQUEST_POST, endpoints->transaction);

			cJSON_Delete(cjson_login_params);
		}
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
		if(login_successful)
		{
			cJSON * cjson_login_params = cJSON_CreateObject();
			/*cJSON_AddStringToObject(cjson_login_params , "password", CurrentUser.password);
			getDataFromServer("LOGIN", cjson_login_params ,REQUEST_POST, endpoints->transaction);*/

			cJSON_Delete(cjson_login_params);
		}
		flag_online = 1;

		message_display_function(1,"","Network Mode  ", "The POS shall operate in online mode . Please login again", (char *)NULL);
		kb_getkey();


	}
	else
	{
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


struct string {
  char *ptr;
  size_t len;
};

void init_string(struct string *s) {
  s->len = 0;
  //s->ptr = malloc(s->len+1);
  s->ptr = malloc(20*sizeof(char));
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}


int start_ppp_session(char * requestType, char * request ,  int operation , char * url , char **response) {
	CURL *curl;
	CURLcode res;
	char  * resp_str;
	//char filename[100];
	//FILE *out_fd = (FILE *) 0;
	//FILE *head_fd = (FILE *) 0;
/*	struct curl_slist *headers = NULL;


	//if(endpoint == ENDPOINT_TRANSACTION)
	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");*/

	curl = curl_easy_init();
	if (curl) {
	   struct curl_slist *chunk = NULL;

	    struct string s;
	    init_string(&s);
		/* Add a custom header */
		chunk = curl_slist_append(chunk, "X-InstaConnect-API-Key: 36b7d76a-7020-4b4a-a369-0bdc9c63eb52");
		chunk = curl_slist_append(chunk, "Accept: application/json");
		chunk = curl_slist_append(chunk, "Content-Type: application/json");
		/* set our custom set of headers */
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

		//sprintf(filename, "%s%s.txt", "", requestType);
		//out_fd = fopen(filename, "w");
		//head_fd = fopen("header.txt", "w"); //open for read and write
		//res = curl_easy_setopt(curl, CURLOPT_FILE, out_fd);
	    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
	    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

		//printf("Thus :  %s\n" , &res);
		res = curl_easy_setopt(curl, CURLOPT_TIMEOUT, 40L);

		res  = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		res = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		printf("URL : %s\n", url);

		res = curl_easy_setopt(curl, CURLOPT_URL, url);
		res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS,request);
	//	res = curl_easy_setopt(curl, CURLOPT_WRITEHEADER, head_fd);


		//token_received = 0 ;
		res = curl_easy_perform(curl);
		printf("%d\n",res);

	/*	   printf("Returned Ster : %s\n", s.ptr);
		   resp_str =  malloc(strlen(s.ptr) +5);
		   strcpy(resp_str , s.ptr);
		   * response = resp_str;
		    free(s.ptr);*/

		curl_easy_cleanup(curl);
		//disable_timer = 0;
		if(res == CURLE_OK)
		{
/*			out_fd = fopen("error.txt", "w");
			fclose(out_fd);*/
			 printf("Returned Ster : %s\n", s.ptr);
			 resp_str =  malloc(strlen(s.ptr) +5);
			 strcpy(resp_str , s.ptr);
			 *response = resp_str;
			 free(s.ptr);
			printf("CURL RESPONSE:::%d\n",CURL_SUCCESS);
			return CURL_SUCCESS;



		}
		else
		{
	/*		fclose(out_fd);
			fclose(head_fd);*/

			return CURL_FAILED_POST;

		}

	}
	else
		return CURL_FAILED_SETUP;
}
//Response
