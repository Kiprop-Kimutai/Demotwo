/*
 * jsonread.h
 *
 *  Created on: Sep 29, 2014
 *      Author: tarus
 */

#ifndef JSONREAD_H_
#define JSONREAD_H_
/////main menu items
#include "sgfplib.h"
#include "json.h"
#include "../src/utilities/cJSON.h"
#include "jsonread.h"
#include <stdio.h>   /* gets */
#include <stdlib.h>  /* atoi, malloc */
#include <string.h>
#include <stdbool.h>  // if using C99...  for C++ leave this out.
#include <ctype.h>
#include <wnet.h>
#include <pthread.h>
#include <directfb.h>

#include "../src/utilities/lcd.h"
#include "../src/utilities/uthash.h"
#include "../src/utilities/uthash.h"


typedef struct
{
	char * transaction;
	char * fileinfo;
	char * getfile;
	char * login;
	char * uploadfile;
	char * operators;
	char * adminz;
	char * landoperations;
	/*char * ping;*/
}endPoints ;
 endPoints * endpoints;

char transactionfile[3000];
char transactionfile1[3000];
char *params_to_db;

//array holding the characters values
char * jsonResponse;
char** service_code;
char** service_name;
char** service_level;
char** params;
char** charges;
char** subservice_id;
char ** parentid;
char token[100];
char** marktId;
char** parentType;
char** parentId;

char** subservice_name;
char** subservice_name1;
char** transactin_val_name;
char** paramtype;
char** transactin_val;
char** transactin_name;
int number_of_trans;
int please_print;
int service_details_not_found;
int flag_update_menu;
int fag_start_ppp_session;
int flag_getting_z;
int flag_getting_sbp;
int flag_stop_z;
int z_rejected;
int keep_db_after_z;
char * one_tx_for_z;
int reading_params_for_z;
int current_synch_item;
int current_synch_batch;

char currentid[50] , currentPID[50];

char app_number[40];
int online;


char transactionstring[3000];
char receiptlog[100];


//Array holding the key values. These may be needed in printing receipts

char** service_code_key;
char** service_name_key;
char** service_level_key;
char**service_id_key;
char** params_key;
char** subservice_id_key;
char** parentType_key;
char** parentId_key;

char rootServiceName[40];
char rootServiceId[40];
char serviceNameSelected[100];
char serviceNameSelected1[100];
char serviceName32[100];
char newRootServiceNameSelected[100];
char charge[20];
char chargesbp[40];
char response[300];
int z_number_recieved;
char pmode[20];
char payment_ref_code[30];
int rootMenuIdent;
int jcheck(char* mystr);


char * hash_fnc(char * Str);

int logged_offline;

int voidLimit;

int ppp_request_ok;

//Arrays holding the transactions
char mac_address[19];

char mac_address1[19];

int arraylen;

//This shall appear in the menu as the title
char current_menu[];

int start_display_test(void );
int main_menu_count;
//void Start_online_display(void );
char * Start_online_display(char *filename);
//void read_online_jason_file();
void read_online_jason_file(json_object *jobj, char *key);
//void read_online_jason_file(void );
void Printer_Void_receipt(void);
#include <string.h>
///
int funt_get_mac_address(void );
void get_current_time(void );
char * getdate_mine(void );
char *getdate_mine_for_z(void);
char *getdate_mine_for_transaction(char * rcpt_num_entered) ;
char time_for_receipt[30];
char rcpt_date[50];
char z_date[50];
char recptnum[100];
void Printer_Fake_receipt(void);

//
void power_on_modem_device1(void );
int single_biz_permit(void);

void doit(char *text);
int check_if_voidable(void );
void display_image(void) ;

//network variables

	char bname[50];
	char bid[50];
	char bcode[50];
	char bdep[100];
	typedef struct posuser
	{
		char posusername[50];
		char posuserpin[50];
		char posuserlevel[50];
		int status;
		char posuserid[30];
	}
	PosUserFile;
	PosUserFile *myPosUser;
	typedef struct configurations
	{
		char IpAddress[50];
		char portNumber[50];
		char apn_username[50];
		char apn_password[50];
		char ppp_timeout[30];
	}
	ConfigFile;
	ConfigFile *myConfigurations;
typedef struct bendetails
{
	BYTE imageBuffer2[405];
	char firstname[50];
	char lastname[50];
	char dateofbirth[50];
	char gender[50];
	char middlename[30];

	//uint8_t fpbyte;
	char fingerprint[500];
}
BenfFile;
BenfFile myBenf;

//Transaction
typedef struct transaction_details
{
	char transactionType[50];
	float amount;
	char transactiondate[50];
	char onlineOrOffline[50];
}
transaction_details;
transaction_details myTransactions;

typedef struct cost
{
	char IpAddress[50];
	char portNumber[50];
	char apn_username[50];
	char apn_password[50];
	char ppp_timeout[30];
}
costFile;
costFile *myCost;

//configuration functions
int read_config(void );
void save_configarations(void );
void change_configuration(int type );
void set_innitial_configuration(void );


//other utilies
char *trim(char *s);
void design_url(char * parenttype, char * parent_id);
bool is_valid_int(const char *str);
void screen_header(void);
int isValidIpAddress(char *ipAddress);
bool is_valid_positive_int(const char *str);
void load_image_for_screen(IDirectFB *dfb, const char *filename ,IDirectFBSurface *main_surface );
void Display_login_screen(const char * title,const char * line1,const char * line2,const char * line3,const char * line4,const char * line5);

//Password functions
void change_passwords(void );

//transaction variables
char transaction[2500];


//transaction
int save_transaction(char *string, char *filename);
void reprint_receipt( int type);
int reprint_receipt_options(void);
void void_receipt(void );

void Printer_Demo(char * type_of_receipt);
void SBP_printer(char * type_of_receipt);

//Reports variables
int x_report_total_txt_amount;
int x_report_total_txt_number;
int x_report_total_txt_amount_offline;
int x_report_total_txt_number_offline;
int x_report_total_txt_amount_voided;
int x_report_total_txt_number_voided;
int x_report_total_num_invalid;
int x_report_total_val_invalid;


int transaction_count_to_confirm_z ;
int prev_user_txn ;
int user_services_count ;

int x_report_total_num_sbp;
int x_report_total_val_sbp;
int x_report_total_num_invalid_sbp;
int x_report_total_val_invalid_sbp;

int offset_forZ;
int check_previous_z(void);
int check_total_z_txn_is_greator_than_req(void);
int check_if_prev_user_had_txn(void);
int check_if_services_exist(void);
void mark_all(void);
int flag_z_done;

char *z_data;
char  receipts_nos_posted[200];

//Reports functions
int process_main_reports(void);
void x_report(void );
void x_report_old();
void z_report(int offset);
void print_x_report(void);
void print_x_z_report(char * report_type);


//Service variables

char **serviceName;
char **serviceCode;
char main_service_level[3];
int getting_offline_params;
char * my_offline_params;
struct menu_path {
    int id;                    /* key */
    char name[10];
    char current_head_name[40];
    UT_hash_handle hh;         /* makes this structure hashable */
};

//Services functions
void get_offline_menu(char * parenttype, char * parent_id);
int travese_path(int task , char * service_id);
int process_main_menu(int selected);
void prepare_next_menu();


//Sql variables
int increament_read;

/////sqlFunctions
void sqlite_database_create_table(char *sql , char * Db);
int sqlite_database_insert_into_table(char *sql ,char *Db) ;
//int getDataFromServer (char * requestType , cJSON * request ,   int operation ,  char * endpoint);
void read_database(char * sql,char * type);
void create_all_table(void );



//char * z_number_recieved;
int disable_timer;



//Variable that hold last menu
int last_in_path;




//void z_report(void );

int flag_getting_pwd;
int flag_new_line;
int login_successful;
int token_received;
int flag_offline_login;
int update_stopped;
int number_of_z_transactions;
int set_p;


enum curlPostError
{
	CURL_FAILED_POST,
	CURL_SUCCESS,
	CURL_FAILED_SETUP
};

enum serverRequest
{
	REQUEST_POST
};

/*typedef struct
{
	char * transaction;
	char * operators;
	char * fileinfo;
	char * getfile;
	char * login;
	char * uploadfile;
	char * adminz;
	char * landoperations;
	char * ping;
}endPoints ;
 endPoints * endpoints;*/



struct menu_path *s;
char serial_num[20];

char url[100] ;

char * string[5000];

char voided_tx_time[50];
//


char z_number[10];

char * panel_one , *panel_two;
int kb_getStringtwo(uint8_t ucMode,uint8_t ucMode1,  uint8_t ucMinlen, uint8_t ucMaxlen, char *pszOut,char *pszOut1 ,  const char *ch, char *charact, char *charact2 ,char * title , int display_string_below) ;


#endif /* JSONREAD_H_ */
