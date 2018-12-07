/*
 * services.h
 *
 *  Created on: Mar 24, 2015
 *      Author: tarus
 */

#ifndef SERVICES_H_
#define SERVICES_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <posapi.h>

char * trim(char *s);
char * gettime (char * type);
int user_details(void );
int topup_details(void);
void z_report_post(void);
int retailer_details(void);
char **  increament_pointersize_and_copy(char ** original_pointer ,int i ,char * str);

char **  count_vouchers(char * tablename);
void update_expiry_field(char * tablename);

char * translate_name_by_key(char * key);
int save_innitial_language(char * filename);
void CreateTmStruct_and_set( int Year, int Month, int Day, int Hour, int Minutes, int Seconds );
int translate_menu_list_and_display (char * title  , char menu_list[][100] ,int  count);

char language[20];
int arabic;

#endif /* SERVICES_H_ */
