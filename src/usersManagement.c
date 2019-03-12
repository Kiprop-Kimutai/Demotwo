/*
 * usersManagement.c
 *
 *  Created on: Oct 28, 2018
 *      Author: linux
 */


#include <stdlib.h>
#include "common_includes.h"
#include "usersManagement.h"
#include "utilities/common_functions.h"
#include "unistd.h"
#include "../src/utilities/inputs.h"
#include "../src/utilities/keyboart.h"
#include "../src/utilities/send_online_request.h"
#include "utilities/sql_functions.h"
#include "device_management.h"
#include "main.h"


void update_operator_firstname(void);
void update_operator_pin(void);
const char operatorfieldsa[][100] = {
		"User Name",
		"PIN",
		"User Status",
		"Exit"
};


//Local use functions from the backend. The is to  be sent to  merchant for authemtication
/*
 * Get  POS user
 */
int request_pos_users(void);
/*
 * Used to  create the offline POS users by the merchant
 */
void create_pos_user(void);
/*
 * Create and manages the various user privileges
 */
void manage_user_privilages(void);
/*
 * User to  make a POS user disabled , enabled or even to  delete a POS user.
 */
void edit_user_details(void);

/*
 * User to  make a POS user disabled , enabled or even to  delete a POS user.
 */
void edit_user_status(void);
char deleteoprators[100];
char insertsql[2000];
void manage_users_menu(void )
{
	int selected = 0;
	char user_managent_menu[][100] = { "Create POS User" ,  "Edit POS User"  /* , "Manage User Privilages" ,"Change User Status" */ };
	//const char  user_managent_menu[][100] = {"POS Device Configuration","POS User Account Operation", "Privileges Setup", "Exit"};
	while(1)
	{
		switch(lcd_menu("POS User Management", user_managent_menu, sizeof(user_managent_menu)/100,selected))
		{
		/*case 0:
			request_operators();
			break;*/
		case 0:
			create_pos_users();
			break;
		case 1:
			edit_user_details();
			break;
		case 2:
			manage_user_privilages();
			break;
		case 3:
			edit_user_status();
			break;
		case -1:
			return;
			break;
		}
	}

}
int request_operators(){
	cJSON *request_json = NULL;
	/*
	 * check flag for online
	 * prompt for two menus:::enquire land balance && 2. Make payment
	 */
	char * sql;
	int ret;
	char plotNo[30];
	if(flag_online == 0)
	{
		message_display_function(1,"","Network Mode Config ", "Please wait as the POS switches GPRS configuration", (char *)NULL);

		ret_val  = power_on_modem_device(myConfigurations->apn_username , myConfigurations->apn_password, atoi(myConfigurations->ppp_timeout));
		if(ret_val == GPRS_SUCCESS){

			/*	if(!flag_online){
	message_display_function(1,"","Offline network", "power on connection to proceed", (char *)NULL);

}*/
			char plotNo[30];
			char holder[30];
			char  * returned;
			strcpy(plotNo,"69");

			cJSON * requestoperator;
			requestoperator = cJSON_CreateObject();
			printf("####Serial Number %s\n",serial_num );
			cJSON_AddStringToObject(requestoperator,"mac_address",serial_num);

			printf("%s initial json\n",cJSON_Print(requestoperator));

			//int  i =  send_gprs_request("OPERATOR",requestoperator ,"/api/usersrequest/fetchusers" ,&returned  );
			int  i;
			if(i)//{
				printf("returned %d :  %s\n",i,  returned );
			if(jcheck(returned)){
				printf("\nresponse is a json");
				request_json = cJSON_Parse(returned);


				//system("rm operator.db");
				//create_all_table();

				printf("Line 2\n");
				cJSON * message =cJSON_GetObjectItem(request_json,"message" );
				printf("Line 3\n");

				char *  pin    = remove_quotes(cJSON_Print(cJSON_GetObjectItem(message, "pin")));
				char * merchantid = remove_quotes(cJSON_Print(cJSON_GetObjectItem(message, "merchantid")));
				char *  username    = remove_quotes(cJSON_Print(cJSON_GetObjectItem(message, "username")));
				char *  agentid    = remove_quotes(cJSON_Print(cJSON_GetObjectItem(message, "agentid")));
				char *  idnumber    = remove_quotes(cJSON_Print(cJSON_GetObjectItem(message, "idnumber")));

				printf("Line 4\n");


				sql = malloc(1000);
				sprintf(sql , "insert into operator (username,pin,idnumber,agentid , merchantid) values ( LOWER('%s'),'%s','%s', '%s', '%s')" , username, pin, idnumber, agentid, merchantid );
				printf("%s\n",sql);

				sqlite_database_read_write_operation(sql ,"operator.db");
				//read_database(sql,"operator");
				free(sql);
				printf("Line 5\n");



			}
			/*
		if(jcheck(returned)){
		printf("\nresponse is a json");
		}

		}*/

		}else
		{
			message_display_function(1,"","Online Posting Error", "The POS is operating in offline mode. Please turn on GPRS to post your transactions.", (char *)NULL);
			kb_getkey();
			return 0;
		}

	}
	return 0;
}

int request_pos_users(void){

	cJSON * synch_params = NULL;
	int retval = 0;
	int ret_val;

	/*
//if(logged_offline)
if(1){

	message_display_function(1,"","Network Mode Config ", "Please wait as the POS switches GPRS configuration", (char *)NULL);

	ret_val  = power_on_modem_device(myConfigurations->apn_username , myConfigurations->apn_password, atoi(myConfigurations->ppp_timeout));
	if(ret_val == GPRS_SUCCESS)
	{
		//cJSON * ping = cJSON_CreateNull();
		flag_online  = 1;
		//getDataFromServer("PING",ping ,REQUEST_POST , endpoints->login);

	}
	else
	{
		message_display_function(1,"","Error getting services", "The POS is operating in offline mode. Please turn on GPRS to update services.", (char *)NULL);
		kb_getkey();
		return 0;
	}


}
	 */


	if (flag_online)
	{
		//copy files from services.db to services.db.bak
		system("mv operator.db operator.db_bak");
		final_create_all_tables();
		printf("\n after create all tables1 \n");

		/*while(flag_update_menu){

		if(current_synch_item ==0)
		{
			printf("\n after create all tables1 \n");
			screen_header();
			lcd_printf(ALG_CENTER, "");
			lcd_printf(ALG_CENTER, "Please wait ..");
			lcd_printf(ALG_CENTER, "Synchronizing operators");
			lcd_flip();
		}
		synch_params  = cJSON_CreateObject();
		printf("\n after create all tables1 \n");
		cJSON_AddStringToObject(synch_params,"hello","world");


		getDataFromServer("OPERATOR",synch_params,REQUEST_POST, endpoints->operators);
		if(update_stopped || current_synch_item == 4 )
			break;
	}
	if(update_stopped)
	{
		//copy files from sercices.db bak (back-up) to services.db
		system("cp operators.db_bak operators.db");
		lcd_clean();
		screen_header();
		lcd_printf(ALG_CENTER, "Error occured while updating operators");
		lcd_printf(ALG_CENTER, "Please try again");
		lcd_flip();
		kb_getkey();
		return 0;
	}
	else
	{
		lcd_clean();
		screen_header();
		lcd_printf(ALG_CENTER, "operators update was successful. ");
		lcd_printf(ALG_CENTER, "Press any key to proceed");
		lcd_flip();
		donnot_get_key = 0;
		sleep(2);
		kb_getkey();
		donnot_get_key = 1;
		///kb_getkey();
	}*/
	}
	//change made here

	else
	{
		message_display_function(1,"","Update Menu Error", "Please confirm if you have internet connectivity and you have been logged in online to update operators", (char *)NULL);
		kb_getkey();
		return 0;
	}
	return 1;

}
int create_agent_details()
{
	int  ret;
	char getCharacters[40];
	char getCharacters1[40];
	char name1[130];
	char name[130];
	int change_made  = 0;
	//Setting user ID
	do
	{
		strcpy(name, "Agent account Id");
		strcpy(name1, "");
		ret = kb_getStringtwo(NUM_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"User Management", 1);
		if(ret !=-1)
		{
			change_made =1;
			strcpy(myMerchantUser->agentid, getCharacters);
		}

		else if(ret==-1 && strlen(myMerchantUser->agentid)>0 )
		{
			break;
		}
		else if(ret==-1 && strlen(myMerchantUser->agentid) == 0 )
		{
			message_display_function(1,"","Agent ID Error", "Agent ID must be set.", (char *)NULL);
			kb_getkey();

		}

	}while(strlen(myMerchantUser->agentid)==0 );

	do
	{

		strcpy(name, "Merchant account Id");
		strcpy(name1, "");
		ret = kb_getStringtwo(NUM_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"User Management", 1);


		if(ret !=-1){
			change_made =1;
			strcpy(myMerchantUser->merchantid, getCharacters);
		}else if(ret==-1 && strlen(myMerchantUser->merchantid)>0 )
		{
			break;
		}
		else if(ret==-1 && strlen(myMerchantUser->merchantid) == 0 )
		{
			message_display_function(1,"","Merchant ID Error", "Merchant ID must be set.", (char *)NULL);
			kb_getkey();
		}
		else
		{
			message_display_function(1,"","Merchant ID Error", "Merchant ID must be set.", (char *)NULL);
			kb_getkey();

		}
	}while(strlen(myMerchantUser->merchantid)==0 );

	do
	{

		strcpy(name, "Enter Merchant Name");
		strcpy(name1, "");
		ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"User Management", 1);


		if(ret !=-1){
			change_made =1;
			strcpy(myMerchantUser->MerchantName, getCharacters);
		}else if(ret==-1 && strlen(myMerchantUser->MerchantName)>0 )
		{
			break;
		}
		else if(ret==-1 && strlen(myMerchantUser->MerchantName) == 0 )
		{
			message_display_function(1,"","Merchant Name Error", "Merchant name should be name", (char *)NULL);
			kb_getkey();
		}
		else
		{
			message_display_function(1,"","Merchant Name Error", "Merchant name should be name", (char *)NULL);
			kb_getkey();

		}
	}while(strlen(myMerchantUser->MerchantName)==0 );


	do
	{

		strcpy(name, "Enter Agent Name");
		strcpy(name1, "");
		ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"User Management", 1);


		if(ret !=-1){
			change_made =1;
			strcpy(myMerchantUser->agentName, getCharacters);
		}else if(ret==-1 && strlen(myMerchantUser->agentName)>0 )
		{
			break;
		}
		else if(ret==-1 && strlen(myMerchantUser->agentName) == 0 )
		{
			message_display_function(1,"","Agent Name", "Agent name must be set.", (char *)NULL);
			kb_getkey();
		}
		else
		{
			message_display_function(1,"","Agent Name", "Agent name must be set.", (char *)NULL);
			kb_getkey();

		}
	}while(strlen(myMerchantUser->agentName)==0 );

	if(change_made)
	{
		/*
	"merchantid  TEXT NOT NULL,"
	"agentid  TEXT NOT NULL,"
	"agentName  TEXT,"
	"merchantName  TEXT ,"
	"active   INTEGER"
	"
		 */
		sprintf(insertsql,"insert into agentsTables (merchantid,agentid ,agentName, merchantName,  active) values ('%s', '%s', '%s', '%s',1 );", myMerchantUser->merchantid, myMerchantUser->agentid,myMerchantUser->agentName, myMerchantUser->MerchantName);
		if(read_database(insertsql, "operator"))
			return 1;
	}
	return  0;


}

int create_pos_users() {
	int  ret;
	int change_made =0;
	myPosUser = (PosUserFile *) malloc(sizeof(PosUserFile));
	char getCharacters[40];
	char getCharacters1[40];
	char name1[130];
	char name[130];
	char sql[100];

	strcpy(name1, "");


	//Setting POS Username
	do
	{
		lcd_clean();
		strcpy(name, "Enter POS Username");
		strcpy(name1, "");
		strcpy(getCharacters , "");
		strcpy(getCharacters1 , "");
		strcpy(myPosUser->username,"");
		printf("#### VAlue of myPosUser->username: %s\n",myPosUser->username);
		ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"User Management", 1);


		if(ret !=-1){
			change_made =1;
			strcpy(myPosUser->username, getCharacters);

		}else if(ret==-1 && strlen(myPosUser->username)>0 )
		{
			break;
		}
		else if(ret==-1 && strlen(myPosUser->username) == 0 )
		{
			message_display_function(1,"","POS user config", "POS Username must be set . Please try again", (char *)NULL);
			kb_getkey();
		}
		else
		{

			message_display_function(1,"","POS user config", "Invalid POS username. Please check username and try again", (char *)NULL);
			kb_getkey();

		}


	}while(strlen(myPosUser->username)==0 );


	//Creates POS user's name
	do
	{
		lcd_clean();
		strcpy(name, "Enter POS User Full Name");
		strcpy(name1, "");

		ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"User Management", 1);


		printf("#### VAlue of ret: %d\n",ret);

		if(ret !=-1){
			change_made =1;
			strcpy(myPosUser->name, getCharacters);

		}else if(ret==-1 && strlen(myPosUser->name)>0 )
		{
			break;
		}
		else if(ret==-1 && strlen(myPosUser->name) == 0 )
		{
			message_display_function(1,"","POS user config", "POS Username must be set . Please try again", (char *)NULL);
			kb_getkey();
		}
		else
		{

			message_display_function(1,"","POS user config", "Invalid POS username. Please check username and try again", (char *)NULL);
			kb_getkey();

		}

		/*if(kb_getkey() == DIKS_ESCAPE)
		{
			return 0;
		}*/
	}while(strlen(myPosUser->name)==0 );
	printf("after posusername\n");
	//Setting POS User PIN
	do
	{
		strcpy(name, "Enter POS  User PIN");
		strcpy(name1, "");
		ret = kb_getStringtwo(PASS_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"User Management", 1);
		if(ret !=-1){
			change_made =1;
			strcpy(myPosUser->pin, getCharacters);

		}else if(ret==-1 && strlen(myPosUser->pin)>0 )
		{
			break;
		}
		else if(ret==-1 && strlen(myPosUser->pin) == 0 )
		{
			message_display_function(1,"","USER PIN Config", "PIN must be set . Please try again. ", (char *)NULL);
			kb_getkey();

		}
		else
		{
			message_display_function(1,"","USER PIN Config", "Invalid PIN . Please enter a numeric value.", (char *)NULL);
			kb_getkey();

		}

	}while(strlen(myPosUser->pin)==0 );


	printf("after posuserid\n");


	do
	{
		strcpy(name, "Agent id number");
		strcpy(name1, "");
		ret = kb_getStringtwo(NUM_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"User Management", 1);


		if(ret !=-1){
			change_made =1;
			strcpy(myPosUser->idnumber, getCharacters);
		}else if(ret==-1 && strlen(myPosUser->idnumber)>0 )
		{
			break;
		}
		else if(ret==-1 && strlen(myPosUser->idnumber) == 0 )
		{
			message_display_function(1,"","User  ID Number Error", "User ID Number must  be set", (char *)NULL);
			kb_getkey();
		}
		else
		{
			message_display_function(1,"","User  ID Number Error", "User ID Number must  be set", (char *)NULL);
			kb_getkey();

		}

		/*if(kb_getkey() == DIKS_ESCAPE)
		{
			return 1;
		}*/
	}while(strlen(myPosUser->idnumber)==0 );



	if(change_made){
		//save_configarations();
		strcpy(myPosUser->userlevel,"1");
		sprintf(insertsql,"insert into operator (username,name ,pin, idnumber, userlevel  , active) values ('%s', '%s', '%s', '%s','%s',  1 );", myPosUser->username, myPosUser->name,myPosUser->pin, myPosUser->idnumber,  myPosUser->userlevel);
		printf("\n");


		if(read_database(insertsql, "operator"))
		{

			sprintf(insertsql,"delete from operator  where userlevel  =  '-1';");
			if(read_database(insertsql, "operator"))
			{
				//Deletes default user.
				return 1;

			}
			printf("\n");

		}
		else
		{
			message_display_function(1,"","Configuration Alert", "Invalid user  details provided. Please check and user  details and retry", (char *)NULL);
			kb_getkey();

		}

	}

	else
	{
		message_display_function(1,"","Configuration Alert", "No configuration have \n been changed.", (char *)NULL);
		kb_getkey();

	}

	return 0;
}
void select_user_from_db(char * password){
	char * sqlstmt;
	char hashedpassword[40];

	//flag_online = 0;
	sqlstmt = malloc(700);
	//password received by this function is pword,obtained from user input when prompted by the POS to log in
	//strcpy(hashedpassword,password);
	/**
	 * Currentuser.username is obtained from username user enters when prompted by the POS
	 */

	sprintf(sqlstmt , "select idnumber as login_userid  from operator where firstname =  '%s' and pin = '%s';", CurrentOperator.username, password);

	//sprintf(sqlstmt , "select userid as login_userid ,username as login_username ,password as login_password,watchTimer as login_watchTimer ,name as login_name	, MarketID as login_MarketID	, Market as login_Market	, paybill as login_paybill	 ,Currency as login_Currency	, voidTime as login_voidTime ,voidLimit  as login_voidLimit , transactionLimit  , reprint , reprintLimit as login_transactionLimit , systemName as login_systemName , posVersion as login_posVersion from users where username =  LOWER('%s') and password = '%s';", CurrentUser.username, hashedpassword);
	printf("%s\n",sqlstmt);
	increament_read = 0;
	read_database(sqlstmt,"operator.db");
	free(sqlstmt);
	/**
	 * if results are found,it means users exist,set login_successful to 1
	 */
	if(increament_read > 0)
	{

		login_successful  = 1;

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


void select_from_db() {
	int ret;

	char idNumber[30];
	char holder[30];
	strcpy(idNumber,"69");

	ret = kb_getStringtwo(NUM_IN ,NUM_IN ,  1, 20, idNumber,holder, NULL, "Enter ID No", "","EDIT POSUSER", 0);



	printf("Entered ID Number is %s\n", idNumber);
	char * sqlstmt;
	char idnumber[40];

	sqlstmt = malloc(700);
	strcpy(idnumber,(idNumber));

	sprintf(sqlstmt , "select username as login_firstname  from operator where userlevel ='%s'  and idnumber = '%s';", "1", idnumber);
	printf("%s\n",sqlstmt);
	increament_read = 0;
	read_database(sqlstmt,"operator");
	free(sqlstmt);
	/**
	 * if results are found,it means users exist,set login_successful to 1
	 */
	if(increament_read > 0)
	{

		user_found_true  = 1;


	}
	/**
	 * if increament_read = 0,it means no results were found,
	 * this is a case of invalid login credentials
	 */
	else
	{
		message_display_function(1,"","Invalid Operator ID", "Please check the Operator ID and Try again !", (char *)NULL);
		kb_getkey();
		user_found_true  = 0;
	}
	//set increament_read to 0 for the next login
	increament_read = 0;


}


void edit_user_details(void){

	/*
	 * check flag for online
	 */
	int selected;
	/*int ret;
	int key;
	int i;
	char posid[30];
	char holder[30];*/
	user_found_true=0;
	while(1){
		select_from_db();

		while (user_found_true) {
			//flag_update_menu= 0;
			selected = lcd_menu("EDIT POS USER", operatorfieldsa, sizeof(operatorfieldsa) / 100,selected);
			switch (selected) {
			case 0:

				update_operator_firstname();

				break;
			case 1:
				update_operator_pin();
				break;
			case 2:
				edit_user_status();
				break;
			case 3:
				return;
				break;

			}

		}
	}

	//key=kb_getkey();





}

void manage_user_privilages(void){

	return;
}

void edit_user_status(void){

	/*
	 * check flag for online
	 */

	char * sqlstmt;

	sqlstmt = malloc(700);
	int ret;

	char request_name[30];
	char holder[30];

	//ret = kb_getStringtwo(ALPHA_IN ,NUM_IN ,  1, 6, plotNo,"", NULL, "L/R No", "","",0);
	ret = kb_getStringtwo(NUM_IN ,NUM_IN ,  1, 20, request_name,holder, NULL, "Enter New User status", "","UPDATE OPERATOR STATUS", 0);


	if(strlen(request_name)>0){

		sprintf(sqlstmt , "update operator set userlevel = '%s';",  request_name);
		printf("%s\n",sqlstmt);
		increament_read = 0;
		read_database(sqlstmt,"operator");
		free(sqlstmt);}

	message_display_function(1,"","Update Operation", "Status changed successfully", (char *)NULL);
	kb_getkey();

}

void initialize_user_params(void){

	//active_pos_user = 0;


	return;

}
void update_operator_firstname(){

	/*
	 * check flag for online
	 * prompt for two menus:::enquire land balance && 2. Make payment
	 */

	char * sqlstmt;

	sqlstmt = malloc(700);
	int ret;
	/*if(!flag_online){
	//message_display_function(0,0,"Offline Mode Error." ,"Pos cannot retrieve records when in offline mode.Please turn it online");
	message_display_function(1,"","Offline network", "power on connection to proceed", (char *)NULL);

}*/
	char request_name[30];
	char holder[30];
	strcpy(request_name,"69");
	//ret = kb_getStringtwo(ALPHA_IN ,NUM_IN ,  1, 6, plotNo,"", NULL, "L/R No", "","",0);
	ret = kb_getStringtwo(ALPHA_IN ,NUM_IN ,  1, 20, request_name,holder, NULL, "Enter User Name", "","UPDATE USERNAME", 0);

	if(strlen(request_name)>0)
	{

		sprintf(sqlstmt , "update operator set username = '%s';",  request_name);
		printf("%s\n",sqlstmt);
		increament_read = 0;
		read_database(sqlstmt,"operator");
		free(sqlstmt);
		message_display_function(1,"","Update Operation", "USERNAME change successful", (char *)NULL);
		kb_getkey();
	}else{
		message_display_function(1,"","Update Operation", "USERNAME update failed", (char *)NULL);
		kb_getkey();
	}




}

void update_operator_pin(){

	/*
	 * check flag for online
	 * prompt for two menus:::enquire land balance && 2. Make payment
	 */

	char * sqlstmt;

	sqlstmt = malloc(700);
	int ret;

	char request_name[30];
	char holder[30];
	strcpy(request_name,"69");
	//ret = kb_getStringtwo(ALPHA_IN ,NUM_IN ,  1, 6, plotNo,"", NULL, "L/R No", "","",0);
	ret = kb_getStringtwo(ALPHA_IN ,NUM_IN ,  1, 20, request_name,holder, NULL, "Enter New PIN", "","UPDATE OPERATOR PIN", 0);

	if(strlen(request_name)>0){

		sprintf(sqlstmt , "update operator set pin = '%s';",  request_name);
		printf("%s\n",sqlstmt);
		increament_read = 0;
		read_database(sqlstmt,"operator");
		free(sqlstmt);}

	message_display_function(1,"","Update Operation", "PIN changed successfully", (char *)NULL);
	kb_getkey();

}





void offline_login(char * password){
	char * sqlstmt;
	char hashedpassword[40];
	printf("\n Userlevel at offline_login is %s\n", myLoginPosUser->userlevel);
	//flag_online = 0;
	sqlstmt = malloc(700);
	//password received by this function is pword,obtained from user input when prompted by the POS to log in
	//strcpy(hashedpassword,hash_fnc(password));
	/**
	 * Currentuser.username is obtained from username user enters when prompted by the POS
	 */

	sprintf(sqlstmt , "select idnumber as login_userid  from operator where username =  '%s' and pin = '%s' and userlevel = '%s';", myLoginPosUser->username, password, myLoginPosUser->userlevel);

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
		if(strcmp(myLoginPosUser->userlevel, "1")==0){
			login_successful  = 1;
		}else if(strcmp(myLoginPosUser->userlevel, "0")==0){

			su_login_successful  = 1;

		}
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
		su_login_successful=0;
	}
	//set increament_read to 0 for the next login
	increament_read = 0;

}



void login(void) {
	char password[40];
	char uname[100];
	char pword[100];
	//myPosUser = (PosUserFile *) malloc(sizeof(PosUserFile));
login_successful = 0;
	char level[20];
	char  * sqlstmt;
	int ret ;
	int default_user_exist = 0 ;


	myLoginPosUser = (LoginUserFile *) malloc(sizeof(LoginUserFile));
	myMerchantUser = (MerchantFile *) malloc(sizeof(MerchantFile));


	//Jack
	sqlstmt = malloc(700);
	sprintf(sqlstmt , "select *   from operator where userlevel =  '-1';");
	printf("%s\n",sqlstmt);
	read_database(sqlstmt,"operator");
	printf("sql_data_count  =  %d\n", sql_data_count);
	if(sql_data_count)
		default_user_exist = 1;
	free(sqlstmt);

	//Jack
	//printf("\n Userlevel around here is %s", myLoginPosUser->userlevel);

	lcd_clean();
	strcpy(myLoginPosUser->username,"");

	strcpy(password,"");
	label_one = "Please enter username";
	label_two = "Please enter Password";
	ret = kb_getStringtwo(ALPHA_IN ,PASS_IN ,  1, 16, uname, pword,NULL , label_one, label_two , "Getting Login Credentials", 0);

	if(ret == -1)
	{
		default_user_exist = 0;
		login();
	}
	else
	{
		sqlstmt = malloc(700);
		sprintf(sqlstmt , "select *   from operator where upper(username) =  upper('%s') and pin = '%s';" ,  uname,  pword);
		read_database(sqlstmt,"operator");
		free(sqlstmt);
		if(sql_data_count)
		{
			if(!default_user_exist)
			{
				int d;


				printf("\n **************Login success****************\n");
				strcpy(myLoginPosUser->username , uname);
				strcpy(password, pword);
				strcpy(myLoginPosUser->pin, password);
				printf("sql_collumn_count : %d\n", sql_collumn_count);
				for(d =0 ;  d< sql_collumn_count;  d++)
				{
					printf("D1 : %s\n", sql_data[d]);
					if(strcmp(sql_data_header[d] , "userlevel") ==0)
						strcpy(myLoginPosUser->userlevel, sql_data[d]);
					if(strcmp(sql_data_header[d] , "userlevel") ==0)
						strcpy(myLoginPosUser->name, sql_data[d]);
					if(strcmp(sql_data_header[d] , "userlevel") ==0)
						strcpy(myLoginPosUser->status, sql_data[d]);
				}


				sqlstmt = malloc(700);
				sprintf(sqlstmt , "select *   from agentsTables;");
				read_database(sqlstmt,"operator");
				free(sqlstmt);


				/*				char  * merchantsTable = "CREATE TABLE IF NOT EXISTS agentsTables("
				    "merchantid  TEXT NOT NULL,"
					"agentid  TEXT NOT NULL,"
					"agentName  TEXT,"
				    "merchantName  TEXT ,"
					"active   INTEGER);";
				 */
				printf("sql_collumn_count : %d\n", sql_collumn_count);
				for(d =0 ;  d< sql_collumn_count;  d++)
				{
					printf("D1 : %s\n", sql_data[d]);
					if(strcmp(sql_data_header[d] , "merchantid") ==0)
						strcpy(myMerchantUser->merchantid, sql_data[d]);
					if(strcmp(sql_data_header[d] , "agentid") ==0)
						strcpy(myMerchantUser->agentid, sql_data[d]);
					if(strcmp(sql_data_header[d] , "agentName") ==0)
						strcpy(myMerchantUser->agentName, sql_data[d]);
					if(strcmp(sql_data_header[d] , "merchantName") ==0)
						strcpy(myMerchantUser->MerchantName, sql_data[d]);

				}
				lcd_clean();
				login_successful = 1;

			}
			else
			{
				sqlstmt = malloc(700);
				sprintf(sqlstmt , "select *   from agentsTables;");
				read_database(sqlstmt,"operator");
				free(sqlstmt);
				if(!sql_data_count)
				{
					message_display_function(1,  "","Agent Details Creation" , "Please press any key  to  create merchant's details." ,  (char  *)NULL);
					kb_getkey();
					if(create_agent_details())
					{

					}
					else
					{
						message_display_function(1,  "","Agent Details Creation Error" , "Please note that  the POS shall not operate without the agent and merchants details." ,  (char  *)NULL);
						kb_getkey();
					}

				}
				message_display_function(1,  "","POS User Creation" , "Please create super user for the merchant." ,  (char  *)NULL);
				kb_getkey();
				if(create_pos_users())
				{
					message_display_function(1,  "","POS User Creation" , "POS admin user  created successfully. Press any key  to  login" ,  (char  *)NULL);
					kb_getkey();
					//default_user_exist = 1;
				}

			}

		}
		else
		{
			message_display_function(1, "","Login Error" ,  "Invalid username or password , please try again", (char *)NULL);
			kb_getkey();
		}

	}
}
