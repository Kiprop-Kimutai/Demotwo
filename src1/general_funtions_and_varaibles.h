/*
 * general_funtions_and_varaibles.h
 *
 *  Created on: Mar 26, 2015
 *      Author: tarus
 */

#ifndef GENERAL_FUNTIONS_AND_VARAIBLES_H_
#define GENERAL_FUNTIONS_AND_VARAIBLES_H_

#include <time.h>

char ** db_collumn_names;
char ** db_collumn_names_languages;
char ** db_values;
char ** db_values_languages;
int dbreadCounter , dbreadCounter_languages;

int collums_in_rs , collums_in_rs_languages;
int update_smart_card;
int Linearfile_counter;
int exit_loop;
int  card_enquiry(char * action);
int check_if_valid_psam_is_present(void);

int curl_socket_test(void);
int main_socket(void);

char card_Number[40];
char original_card_Number[40];
char * personalstring;
char  card_name[100];
char * dateofbirths;
char *  pin_no;
int topup_download;
int downloaded_topup;
int downloaded_topup_value;
int downloaded_topup_items;
int activity_in_progress;

//Timer start
time_t start;

char retailer_name[100];

int in_purchase;
int started_program ;
int sam_verification;

//int fplib_test(int option);


#endif /* GENERAL_FUNTIONS_AND_VARAIBLES_H_ */
