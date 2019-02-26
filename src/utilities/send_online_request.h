/*
 * send_online_request.h
 *
 *  Created on: Dec 9, 2018
 *      Author: linux
 */

#ifndef SRC_UTILITIES_SEND_ONLINE_REQUEST_H_
#define SRC_UTILITIES_SEND_ONLINE_REQUEST_H_

int ret_val;
char serial_num[20];
#include "cJSON.h"
int flag_online;
int fag_start_ppp_session;
void process_response_on_fail (char * requestType);
int send_gprs_request(char * requestType, cJSON * requestjson, char * endpoint ,  cJSON ** response , int display_errors_messages );
int start_ppp_session(char * requestType, char * request ,  int operation , char * url , char **response);
enum modem_error_codes
{
	GPRS_SUCCESS,
	GPRS_FAILED_TO_START_DEVICE,
	GPRS_FAILED_TO_START_MODEL,
	GPRS_FAILED_TO_ATTACH_TO_NETWORK,
	GPRS_FAILED_TO_START_PPP
};

int sim_power_status;
int  power_on_modem_device(char * apn_username , char *  apn_password , int timeout);
int get_SMS_phone(int num, char * message , char * number);
//char * getDataFromServer (char * requestType , cJSON * request ,   int operation ,  char * endpoint);
int get_SMS_Count(void);
int send_SMS_phone(void);
char * buildFinalrequest1(char * requestType , cJSON * request );
//int getDataFromServer (char * requestType , cJSON * request ,   int operation ,  char * endpoint)

void start_modem(void);
extern int usleep (__useconds_t __useconds);
struct timeval start, cur, diff, end;
struct timezone tz;



#endif /* SRC_UTILITIES_SEND_ONLINE_REQUEST_H_ */
