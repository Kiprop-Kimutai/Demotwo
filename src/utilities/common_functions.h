/*
 * split_string.h
 *
 *  Created on: Dec 8, 2018
 *      Author: linux
 */

#ifndef SRC_UTILITIES_COMMON_FUNCTIONS_H_
#define SRC_UTILITIES_COMMON_FUNCTIONS_H_
#include <posapi.h>
#include <posapi.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <net/if.h>   //ifreq
#include <arpa/inet.h>
#include <netif.h>
//#include "jsonread.h"

char * hash_fnc(char * Str);
void power_off_options(void);

//Validation methods
bool is_valid_positive_int(const char *str);
bool is_valid_int(const char *str);
int isValidIpAddress(char *ipAddress);
int jcheck(char* mystr);
char *trim(char *s);
//'Device services
//int funt_get_mac_address(void );
void get_current_time(void );
char * getdate_mine(void );
void remove_all_chars(char* str, char c);

char** str_split(char* a_str, const char a_delim);
char ** get_pos_network_data(char * command , int * x);

void get_date_and_receipt (int required_output,  char ** returned_date , char ** unformatted_date,  char ** returned_value) ;
void get_request_id (int required_output,  char ** returned_request_id);
char macaddress[30];
char pos_serial_number[40];

char * get_string_from_jason_object(char * string);
char * remove_quotes(char * string);
char *my_strtok(char *str, char *delim);

double getdecimalvalueforhex(char i,int position);
char *hextodecimalconverter(char *hexstring);
void pretty_printf(char  * str ,  int print_length);
int number_length(int number);
const char * get_date_from_string(char * date);

#endif /* SRC_UTILITIES_COMMON_FUNCTIONS_H_ */
