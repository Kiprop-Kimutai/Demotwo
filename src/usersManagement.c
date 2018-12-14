/*
 * usersManagement.c
 *
 *  Created on: Oct 28, 2018
 *      Author: linux
 */


#include <stdlib.h>
#include "common_includes.h"
#include "usersManagement.h"
#include "unistd.h"
#include "../src/utilities/inputs.h"
#include "../src/utilities/keyboart.h"
#include "../src/utilities/send_online_request.h"
#include "../src1/jsonread.h"
#include "../src1/sql.h"
#include "main.h"


const char operatorfieldsa[][100] = {
		"User Name",
		"PIN",
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
char insertoperaterstring[2000];
void manage_users_menu(void )
{
	int selected = 0;
	char user_managent_menu[][100] = {"Request POS Users",  "Create POS User" ,  "Edit POS User" ,  "Manage User Privilages" ,"Change User Status",  "Exit"};
	//const char  user_managent_menu[][100] = {"POS Device Configuration","POS User Account Operation", "Privileges Setup", "Exit"};
	while(1)
	{
		switch(lcd_menu("POS User Management", user_managent_menu, 6,selected))
		{
		case 0:
			request_operators();
			break;
		case 1:
			create_pos_users();
			break;
		case 2:
			edit_user_details();
			break;
		case 3:
			manage_user_privilages();
			break;
		case 4:
			edit_user_status();
			break;
		case 5:
			return;
			break;
		}
	}

}
int request_operators(){

	/*
	 * check flag for online
	 * prompt for two menus:::enquire land balance && 2. Make payment
	 */
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
			strcpy(plotNo,"69");

			cJSON * landratejson;
			landratejson = cJSON_CreateObject();
			printf("####Serial Number %s\n",serial_num );
			cJSON_AddStringToObject(landratejson,"mac_address",serial_num);

			printf("%s initial json\n",cJSON_Print(landratejson));
			getDataFromServer("networkrequest",landratejson,0,"/api/usersrequest/fetchusers");

			//heloo



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

	if(logged_offline) {

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


	if (flag_online)
	{
		update_stopped = 0;
		flag_update_menu= 1;
		//copy files from services.db to services.db.bak
		system("mv operator.db operator.db_bak");
		create_all_table();
		printf("\n after create all tables1 \n");
		current_synch_batch = 0;
		current_synch_item =0;

		while(flag_update_menu){

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
		}
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




void create_pos_users() {
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

		strcpy(name, "Enter POS Username");
		strcpy(name1, "");

		ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"User Management", 1);


		printf("#### VAlue of ret: %d\n",ret);

		if(ret !=-1){
			change_made =1;
			strcpy(myPosUser->posusername, getCharacters);

		}else if(ret==-1 && strlen(myPosUser->posusername)>0 )
		{
			break;
		}
		else if(ret==-1 && strlen(myPosUser->posusername) == 0 )
		{
			message_display_function(1,"","POS user config", "POS Username must be set . Please try again", (char *)NULL);
			kb_getkey();
		}
		else
		{

			message_display_function(1,"","POS user config", "Invalid POS username. Please check username and try again", (char *)NULL);
			kb_getkey();

		}
	}while(strlen(myPosUser->posusername)==0 );

	//Setting POS User PIN
	do
	{
		strcpy(name, "Enter POS  User PIN");
		strcpy(name1, "");
		ret = kb_getStringtwo(NUM_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"User Management", 1);
		if(ret !=-1){
			change_made =1;
			strcpy(myPosUser->posuserpin, getCharacters);

		}else if(ret==-1 && strlen(myPosUser->posuserpin)>0 )
		{
			break;
		}
		else if(ret==-1 && strlen(myPosUser->posuserpin) == 0 )
		{
			message_display_function(1,"","USER PIN Config", "PIN must be set . Please try again. ", (char *)NULL);
			kb_getkey();

		}
		else
		{
			message_display_function(1,"","USER PIN Config", "Invalid PIN . Please enter a numeric value.", (char *)NULL);
			kb_getkey();

		}
	}while(strlen(myPosUser->posuserpin)==0 );

	//Setting APN username
	do
	{
		strcpy(name, "Enter user ID");
		strcpy(name1, "");
		ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"User Management", 1);
		if(ret !=-1)
		{
			change_made =1;
			strcpy(myPosUser->posuserid, getCharacters);
		}

		else if(ret==-1 && strlen(myPosUser->posuserid)>0 )
		{
			break;
		}
		else if(ret==-1 && strlen(myPosUser->posuserid) == 0 )
		{
			message_display_function(1,"","USER ID Config", "User ID must be set. Please enter a numeric value.", (char *)NULL);
			kb_getkey();

		}

	}while(strlen(myPosUser->posuserid)==0 );

	printf("after posuserid\n");

	//setting PP timeout

	strcpy(name, "User Level");
	strcpy(name1, "");
	do
	{
		ret = kb_getStringtwo(NUM_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, name1,"User Management", 1);


		if(ret !=-1){
			change_made =1;
			strcpy(myPosUser->posuserlevel, getCharacters);
		}else if(ret==-1 && strlen(myPosUser->posuserlevel)>0 )
		{
			break;
		}
		else if(ret==-1 && strlen(myPosUser->posuserlevel) == 0 )
		{
			message_display_function(1,"","USER Level", "User Level must be set. Please enter a numeric value.", (char *)NULL);
			kb_getkey();
		}
		else
		{
			message_display_function(1,"","USER Level", "Invalid User Level value. Please enter a numeric value.", (char *)NULL);
			kb_getkey();

		}
	}while(strlen(myPosUser->posuserlevel)==0 );


	if(change_made){
		//save_configarations();

		sprintf(deleteoprators,"delete from operator");
		printf("\n");
		sqlite_insert_operator_into_table(deleteoprators, "operator");

		printf("######POS username is: %s\n", myPosUser->posusername);
		printf("\n");
		printf("######POS pin is: %s\n", myPosUser->posuserpin);

		printf("\n");
		printf("######POS username is: %s\n", myPosUser->posuserid);
		printf("\n");
		printf("######POS username is: %s\n", myPosUser->posuserlevel);

		sprintf(insertoperaterstring,"insert into operator (firstname,pin,idnumber,operaterlevel) values ('%s','%s','%s',0)",myPosUser->posusername,myPosUser->posuserpin, myPosUser->posuserid);
		printf("\n");



		sqlite_insert_operator_into_table(insertoperaterstring, "operator");

	}

	else
	{
		message_display_function(1,"","Configuration Alert", "No configuration have \n been changed.", (char *)NULL);
		kb_getkey();

	}


}
void select_user_from_db(char * password){
	char * sqlstmt;
	char hashedpassword[40];

	//flag_online = 0;
	sqlstmt = malloc(700);
	//password received by this function is pword,obtained from user input when prompted by the POS to log in
	strcpy(hashedpassword,hash_fnc(password));
	/**
	 * Currentuser.username is obtained from username user enters when prompted by the POS
	 */

	sprintf(sqlstmt , "select idnumber as login_userid  from operator where firstname =  '%s' and pin = '%s';", CurrentOperator.username, password);

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

	sprintf(sqlstmt , "select username as login_firstname  from operator where idnumber =  '%s';", idnumber);
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
		flag_offline_login = 1;

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
	 * prompt for two menus:::enquire land balance && 2. Make payment
	 */
	int selected;
	int ret;
	int key;
	int i;
	char posid[30];
	char holder[30];
	user_found_true=0;
	while(1){
		select_from_db();

		while (user_found_true) {
			flag_update_menu= 0;
			selected = lcd_menu("EDIT POS USER", operatorfieldsa, sizeof(operatorfieldsa) / 100,selected);
			switch (selected) {
			case 0:

				update_operator_firstname();

				break;
			case 1:
				update_operator_pin();
				break;
			case 2:
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

	return;
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
