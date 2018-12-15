/*
 ============================================================================
 Name        : Sqlite_db.c
 Author      : Hillary
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

/*
 * sqlite_db.c
 *
 *  Created on: Nov 9, 2014
 *      Author: hiltarus
 */

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

#include "../src/utilities/lcd.h"
#include "jsonread.h"
#include "receipt.h"
#include "main_old.h"
#include "sql.h"
extern int kb_getString(uint8_t ucMode, uint8_t ucMinlen, uint8_t ucMaxlen,char *pszOut, const char *ch, char *charact);
void clear_arrays(void );
extern int usleep (__useconds_t __useconds);
int create_backup_db();

void read_database_receiptlogs();
/*static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
 int i;

 char str[200];
 for (i = 0; i < argc; i++) {
 printf("Arg C :%d\n", argc);
 if(strcmp(azColName[i],"ID")==0)
 {

 strcpy(str, argv[i] ? argv[i] : "NULL");
 printf("%s",str);
 serviceCode = realloc(serviceCode, (  i + 1 ) * sizeof( char * ));
 serviceCode[i] = malloc( strlen( str) + 1 );
 strcpy( serviceCode[i], str);
 }
 strcpy(str, argv[i] ? argv[i] : "NULL");
 //if(strcmp(str,"NAME")==0)
 //{
 printf("%s",str);
 serviceName = realloc(serviceName, (  i + 1 ) * sizeof( char * ));
 serviceName[i] = malloc( strlen( str) + 1 );
 strcpy( serviceName[i], str);
 //}
 printf("\n");
 printf("\n");
 printf("%s = %s\n",serviceCode[i]);

 }

 return 0;
 }*/
static int callback(void *data, int argc, char **argv, char **azColName) {
	int i;
	//fprintf(stderr, "%s: ", (const char*) data);
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	increament_read++;
	return 0;
}
static int callback_receiptlogs(void *data, int argc, char **argv, char **azColName) {
	printf("Callback called\n");
	int i;
	for(i = 0;i<argc;i++) {
		printf("%s: %s\n",argv[i],argv[i]);
	}
	char rowcontents[50];
	sprintf(rowcontents,"%s :  %s",argv[0],argv[1]);
	printf("Rowcontents::%s\n",rowcontents);
	collectStr = realloc(collectStr, (  row_count + 1 ) * sizeof( char * ));
	collectStr[row_count] = malloc( strlen( rowcontents) + 1 );
	 strcpy(collectStr[row_count],trim(rowcontents));
	 row_count=row_count+1;
	return 0;
}



static int callbackServices(void *data, int argc, char **argv, char **azColName) {
	char str[1000];

	int i;
	//fprintf(stderr, "%s: ", (const char*) data);
	if(getting_offline_params)
	{
		printf("\nThe lenght %d \n",strlen(my_offline_params));
	if(strlen(my_offline_params)>0)
		strcat(my_offline_params,"*");
	printf("\n\n\n\nThe build %s \n",my_offline_params);
	}

	for (i = 0; i < argc; i++) {

		strcpy(str, argv[i] ? argv[i] : "NULL");
		printf("STR  : %s\n", str );
		if(getting_offline_params)
		{
				if (strcmp(azColName[i], "id") == 0)
				{
					printf("\nThe ID %s \n",str);
					my_offline_params = realloc(my_offline_params,strlen(my_offline_params)+strlen(str)+3);
					strcat(my_offline_params,str);
					strcat(my_offline_params,"+");
				}
				if (strcmp(azColName[i], "name") == 0)
				{
					printf("\nThe Name %s \n",str);
					my_offline_params = realloc(my_offline_params,strlen(my_offline_params)+strlen(str)+3);
					strcat(my_offline_params,str);
					strcat(my_offline_params,"+");
				}
				if (strcmp(azColName[i], "paramtype") == 0)
				{
					printf("\nThe Paramtype %s \n",str);
					my_offline_params = realloc(my_offline_params,strlen(my_offline_params)+strlen(str)+2);
					strcat(my_offline_params,str);
				}

		}
		else
		{
		if (strcmp(azColName[i], "serviceName") == 0 || strcmp(azColName[i], "name") == 0)
		{
			printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
			service_name = realloc(service_name,(increament_read + 1) * sizeof(char *));
			service_name[increament_read] = malloc(strlen(str) + 1);
			strcpy(service_name[increament_read], str);
		}
		if (strcmp(azColName[i], "name") == 0)
		{
			printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
			subservice_name = realloc(subservice_name,(increament_read + 1) * sizeof(char *));
			subservice_name[increament_read] = malloc(strlen(str) + 1);
			strcpy(subservice_name[increament_read], str);
		}
		if (strcmp(azColName[i], "level") == 0)
		{
			service_level = realloc(service_level,(increament_read + 1) * sizeof(char *));
			service_level[increament_read] = malloc(strlen(str) + 1);
			strcpy(service_level[increament_read], str);
		}
		if (strcmp(azColName[i], "charge") == 0)
		{
			strcpy(charge, str);
		}
		if (strcmp(azColName[i], "params") == 0)
		{
			params = realloc(params,(increament_read + 1) * sizeof(char *));
			params[increament_read] = malloc(strlen(str) + 1);
			strcpy(params[increament_read], str);
		}
		if (strcmp(azColName[i], "id") == 0)
		{
			printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
			subservice_id = realloc(subservice_id,(increament_read + 1) * sizeof(char *));
			subservice_id[increament_read] = malloc(strlen(str) + 1);
			strcpy(subservice_id[increament_read], str);
		}
		if (strcmp(azColName[i], "parentid") == 0)
		{
			printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
			parentid = realloc(parentid,(increament_read + 1) * sizeof(char *));
			parentid[increament_read] = malloc(strlen(str) + 1);
			strcpy(parentid[increament_read], str);
		}
		if (strcmp(azColName[i], "serviceCode") == 0)
		{
			service_code = realloc(service_code,(increament_read + 1) * sizeof(char *));
			service_code[increament_read] = malloc(strlen(str) + 1);
			strcpy(service_code[increament_read], str);
		}

/*		"userid  		TEXT  PRIMARY KEY NOT NULL,"
		"username   	TEXT ,"
		"password   	TEXT ,"
		"paybill		TEXT ,"
		"Currency		TEXT ,"
		"Status			TEXT  );";*/

		if (strcmp(azColName[i], "login_userid") == 0)
			CurrentUser.userid = atoi( str);
		if (strcmp(azColName[i], "login_posVersion") == 0)
			strcpy(CurrentUser.latest_version_number, str);
		if (strcmp(azColName[i], "login_voidLimit") == 0)
			CurrentUser.voidLimit = atoi( str);
		if (strcmp(azColName[i], "login_voidTimer") == 0)
			CurrentUser.voidTimer = atoi( str);
		if (strcmp(azColName[i], "login_name") == 0)
			strcpy(CurrentUser.name, str);
		if (strcmp(azColName[i], "login_paybill") == 0)
			strcpy(CurrentUser.paybill, str);
		if (strcmp(azColName[i], "login_MarketID") == 0)
			strcpy(CurrentUser.marketid, str);
		if (strcmp(azColName[i], "login_Market") == 0)
			strcpy(CurrentUser.market, str);
		if (strcmp(azColName[i], "login_Currency") == 0)
			strcpy(CurrentUser.currency, str);
		if (strcmp(azColName[i], "login_watchTimer") == 0)
			CurrentUser.watchTimer = atoi( str);
		if (strcmp(azColName[i], "login_void_count") == 0)
			voidLimit = atoi( str);
		if (strcmp(azColName[i], "void_count") == 0)
			voidLimit = atoi( str);
		if (strcmp(azColName[i], "user_services_count") == 0)
			user_services_count = atoi( str);
		if (strcmp(azColName[i], "login_transactionLimit") == 0)
			CurrentUser.transactionLimit= atoi( str);
	}
	}
	increament_read++;
	arraylen = increament_read;
	return 0;
}



static int callback1(void *data, int argc, char **argv, char **azColName) {
	char str[1000];
//(+billno+:+002282016551-0001+,+transactiontype+:+1+,+transactiondate+:+2014-12-04%2016:26:41+,+serviceid+:+1+,+subserviceid+:+7+,+paymentmethod+:+Cash+,+params+:(+Vehicle%20Reg+:+5+),+total+:+200+,+user+:+6+,+market+:+Nyamira+),(+billno+:+002282016551-0002+,+transactiontype+:+1+,+transactiondate+:+2014-12-04%2016:48:29+,+serviceid+:+1+,+subserviceid+:+7+,+paymentmethod+:+Cash+,+params+:(+Vehicle%20Reg+:+123+),+total+:+200+,+user+:+6+,+market+:+Nyamira+),(+billno+:+002282016551-0003+,+transactiontype+:+1+,+transactiondate+:+2014-12-04%2016:49:16+,+serviceid+:+1+,+subserviceid+:+76+,+paymentmethod+:+Cash+,+params+:(+ID%20NO+:+132+,+Customer%20Name+:+1+,+Customer%20Phone+:+1+,+Vehicle%20Reg+:+1+,+Colour+:+1+,+Animal%20Type+:+1+,+Name%20of%20Buyer+:+1+,+Location%20of%20Seller+:+1+,+Location%20of%20Buyer+:+1+,+:+1+),+total+:+1000+,+user+:+6+,+market+:+Nyamira+),(+billno+:+002282016551-0004+,+transactiontype+:+1+,+transactiondate+:+2014-12-04%2016:50:42+,+serviceid+:+1+,+subserviceid+:+76+,+paymentmethod+:+Cash+,+params+:(+ID%20NO+:+5+,+Customer%20Name+:+6+,+Customer%20Phone+:+6+,+Vehicle%20Reg+:+6+,+Colour+:+6+,+Animal%20Type+:+6+,+Name%20of%20Buyer+:+6+,+Location%20of%20Seller+:+6+,+Location%20of%20Buyer+:+6+,+:+6+),+total+:+1000+,+user+:+6+,+market+:+Nyamira+),(+billno+:+002282016551-0005+,+transactiontype+:+1+,+transactiondate+:+2014-12-04%2016:56:22+,+serviceid+:+2+,+subserviceid+:+26+,+paymentmethod+:+Cash+,+params+:(+ID%20NO+:+2+,+Customer%20Name+:+2+),+total+:+20+,+user+:+6+,+market+:+Nyamira+),(+billno+:+002282016551-0007+,+transactiontype+:+1+,+transactiondate+:+2014-12-04%2021:46:26+,+serviceid+:+1+,+subserviceid+:+7+,+paymentmethod+:+Cash+,+params+:(+Vehicle%20Reg+:+5+),+total+:+200+,+user+:+6+,+market+:+Nyamira+),(+billno+:+002282016551-0013+,+transactiontype+:+1+,+transactiondate+:+2014-12-04%2022:12:52+,+serviceid+:+1+,+subserviceid+:+7+,+paymentmethod+:+Cash+,+params+:(+Vehicle%20Reg+:+555+),+total+:+200+,+user+:+6+,+market+:+Nyamira+),(+billno+:+002282016551-0015+,+transactiontype+:+1+,+transactiondate+:+2014-12-04%2022:13:21+,+serviceid+:+1+,+subserviceid+:+7+,+paymentmethod+:+Cash+,+params+:(+Vehicle%20Reg+:+558+),+total+:+200+,+user+:+6+,+market+:+Nyamira+),(+billno+:+002282016551-0017+,+transactiontype+:+1+,+transactiondate+:+2014-12-04%2022:13:46+,+serviceid+:+1+,+subserviceid+:+7+,+paymentmethod+:+Cash+,+params+:(+Vehicle%20Reg+:+78+),+total+:+200+,+user+:+6+,+market+:+Nyamira+),(+billno+:+002282016551-0018+,+transactiontype+:+1+,+transactiondate+:+2014-12-04%2022:14:29+,+serviceid+:+2+,+subserviceid+:+19+,+paymentmethod+:+Cash+,+params+:(+ID%20NO+:+4+,+Customer%20Name+:+4+),+total+:+500+,+user+:+6+,+market+:+Nyamira+)&$$
	//[{"id":76,"charge":"1000","param":"ID NO+NUMERIC*Customer Name+ALPHA*Customer Phone+NUMERIC*Vehicle Reg+ALPHA*Colour+ALPHA*Animal Type+ALPHA*Name of Buyer+ALPHA*Location of Seller+ALPHA*Location of Buyer+ALPHA*Location of Customer+ALPHA*ID NO of Seller+ALPHA*Name of Seller+ALPHA*Location+ALPHA*Sub-Location+ALPHA*Picture+ALPHA*qwertyuiop12+NUMERIC*Qwerty+NUMERIC*Port+ALPHA"}]



	int i;
	//fprintf(stderr, "%s: ", (const char*) data);



	if(flag_getting_z  && !reading_params_for_z)
	{
		strcpy(pmode, "");
		one_tx_for_z = malloc(2);
		strcpy(one_tx_for_z,"");
		//z_data =  realloc(z_data,strlen(z_data)+1);
		if(number_of_z_transactions!=0){

			//strcat(z_data,",");

		}

		strcat(one_tx_for_z,"{");

		//number_of_z_transactions++;

	}
	if(reading_params_for_z)
	{
		if(reading_params_for_z==1)
		{
			one_tx_for_z =  realloc(one_tx_for_z,strlen(one_tx_for_z)+14);
			strcat(one_tx_for_z,",\"params\":{");
		}

		reading_params_for_z++;

	}
	for (i = 0; i < argc; i++) {
		strcpy(str, argv[i] ? argv[i] : "NULL");

		//printf("Flag getting Z : %d\n\n\n", flag_getting_z);
		if(flag_getting_z)
		{
			if(!reading_params_for_z){
				one_tx_for_z =  realloc(one_tx_for_z,strlen(one_tx_for_z)+strlen(azColName[i])+strlen(str)+7);
				if(i!=0)
					strcat(one_tx_for_z,",");
				strcat(one_tx_for_z,"\"");
				strcat(one_tx_for_z,azColName[i]);
				strcat(one_tx_for_z,"\":\"");
				strcat(one_tx_for_z,str);
				strcat(one_tx_for_z,"\"");
				if (strcmp(azColName[i], "billno") == 0 || strcmp(azColName[i], "receipt") == 0 ){
					strcpy(recptnum, str);
				}
			}
			else
			{
				one_tx_for_z =  realloc(one_tx_for_z,strlen(one_tx_for_z)+strlen(str)+7);

				if (strcmp(azColName[i], "key") == 0)
				{
					if(reading_params_for_z>2)
						strcat(one_tx_for_z,",");
					strcat(one_tx_for_z,"\"");
					strcat(one_tx_for_z,str);
					strcat(one_tx_for_z,"\":\"");

				}
				if (strcmp(azColName[i], "Value") == 0)
				{
					strcat(one_tx_for_z,str);
					strcat(one_tx_for_z,"\"");

				}


			}



			//}







			//z_data =  realloc(z_data,strlen(z_data)+strlen(str)+10);
			//strcat(z_data,str);




		}
		else
		{
			//printf("The str : %s %s\n",azColName[i],str );
			if (strcmp(azColName[i], "NAME") == 0)
					{
						serviceName = realloc(serviceName,(increament_read + 1) * sizeof(char *));
						serviceName[increament_read] = malloc(strlen(str) + 1);
						strcpy(serviceName[increament_read], str);
					}
					if (strcmp(azColName[i], "x_report_total_txt_amount") == 0) {
						x_report_total_txt_amount = atoi(str);
						printf("x_report_total_txt_number :%s", str);
					}
					if (strcmp(azColName[i], "x_report_total_txt_number") == 0) {
						printf("x_report_total_txt_amount :%s", str);
						x_report_total_txt_number = atoi(str);
					}
					if (strcmp(azColName[i], "x_report_total_txt_amount_offline") == 0) {
						x_report_total_txt_amount_offline = atoi(str);
						printf("x_report_total_txt_number_offline :%s", str);
					}
					if (strcmp(azColName[i], "x_report_total_txt_number_offline") == 0) {
						printf("x_report_total_txt_amount_offline :%s", str);
						x_report_total_txt_number_offline = atoi(str);
					}
					if (strcmp(azColName[i], "x_report_total_txt_amount_voided") == 0) {
						x_report_total_txt_amount_voided = atoi(str);
						printf("x_report_total_txt_number :%s", str);
					}
					if (strcmp(azColName[i], "x_report_total_txt_number_voided") == 0) {
						printf("x_report_total_txt_amount :%s", str);
						x_report_total_txt_number_voided = atoi(str);

					}


					if (strcmp(azColName[i], "transaction_count_to_confirm_z") == 0) {
						transaction_count_to_confirm_z = atoi(str);

					}
					if (strcmp(azColName[i], "prev_user_txn") == 0) {
						prev_user_txn = atoi(str);

					}

					if (strcmp(azColName[i], "billno_acknowledge") == 0) {
						if(increament_read == 0 )
						{
							//{"request":"UPDATE_FLAGS", "serialNo":"sdfsf", "bills":[{"billNo":val},{"billNo":val}]}
							strcpy(transactionfile, "");
							strcat(transactionfile,"{\"request\":\"UPDATE_PRINT_FLAGS\",  \"serialNo\":\"");
							strcat(transactionfile,mac_address);
							strcat(transactionfile,"\", \"bills\":[{");
							strcat(transactionfile,"\"billNo\": \"");
							strcat(transactionfile,str);
							strcat(transactionfile,"\"}");
						}
						else
						{

							strcat(transactionfile,",{\"billNo\": \"");
							strcat(transactionfile,str);
							strcat(transactionfile,"\"}");
						}

						}


					if (strcmp(azColName[i], "x_report_total_num_invalid") == 0) {
						x_report_total_num_invalid = atoi(str);
						printf("x_report_total_num_invalid :%s", str);
					}
					if (strcmp(azColName[i], "x_report_total_val_invalid") == 0) {
						printf("x_report_total_val_invalid :%s", str);
						x_report_total_val_invalid = atoi(str);
					}
					if (strcmp(azColName[i], "x_report_total_num_sbp") == 0) {
						x_report_total_num_sbp = atoi(str);
						printf("x_report_total_num_sbp :%s", str);
					}
					if (strcmp(azColName[i], "x_report_total_val_sbp") == 0) {
						printf("x_report_total_val_sbp :%s", str);
						x_report_total_val_sbp = atoi(str);

					}
					if (strcmp(azColName[i], "x_report_total_num_invalid_sbp") == 0) {
						x_report_total_num_invalid_sbp = atoi(str);
						printf("x_report_total_num_invalid_sbp :%s", str);
					}
					if (strcmp(azColName[i], "x_report_total_val_invalid_sbp") == 0) {
						printf("x_report_total_val_invalid_sbp :%s", str);
						x_report_total_val_invalid_sbp = atoi(str);

					}
					if (strcmp(azColName[i], "Z_report") == 0) {
							if(number_of_z_transactions!=0){
								strcat(z_data,",");
							}
							z_data =  realloc(z_data,strlen(z_data)+strlen(str)+10);
							strcat(z_data,str);

							number_of_z_transactions++;
					}


					//For reprint receipt and void receipt:
					if (strcmp(azColName[i], "transactiondate") == 0) {
						strcpy(voided_tx_time, str);
					}
					if (strcmp(azColName[i], "servicename") == 0) {
						strcpy(serviceNameSelected, str);
					}
					if (strcmp(azColName[i], "rootservice") == 0) {
						strcpy(rootServiceName, str);
					}
					if (strcmp(azColName[i], "total") == 0) {
						strcpy(charge, str);
					}
					if (strcmp(azColName[i], "paymentCode") == 0) {
						strcpy(payment_ref_code, str);
					}
					if (strcmp(azColName[i], "billno_for_reprint") == 0) {
						strcpy(recptnum, str);
					}
					if (strcmp(azColName[i], "paymentmethod") == 0) {
						strcpy(pmode, str);
					}/*
					if (strcmp(azColName[i], "param_dtls") == 0) {
						//strcpy(charge, str);
						transactin_val_name = realloc(transactin_val_name,(number_of_trans + 1) * sizeof(char *));
						transactin_val_name[number_of_trans] = malloc(strlen(str) + 1);
						strcpy(transactin_val_name[number_of_trans], str);
						number_of_trans++;
					}*/
					if (strcmp(azColName[i], "param_dtls_val") == 0) {
						//strcpy(charge, str);
						transactin_val = realloc(transactin_val,(number_of_trans + 1) * sizeof(char *));
						transactin_val[number_of_trans] = malloc(strlen(str) + 1);
						strcpy(transactin_val[number_of_trans], str);
						number_of_trans++;
					}
					if (strcmp(azColName[i], "param_dtls_name") == 0)
					{
						//strcpy(charge, str);
						strcat(str," : ");
						transactin_name = realloc(transactin_name,(number_of_trans + 1) * sizeof(char *));
						transactin_name[number_of_trans] = malloc(strlen(str) + 1);
						strcpy(transactin_name[number_of_trans], str);
						//number_of_trans--;
					}

					if (strcmp(azColName[i], "Z_done") == 0) {
						strcpy(z_date , str);
						flag_z_done = atoi(str);
						printf("Z_done : %s \n", z_date );
					}


		}
		// printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");



	}
/*	if(flag_getting_z  && reading_params_for_z)
	{
		z_data =  realloc(z_data,strlen(z_data)+2);
		strcat(z_data,"}");

	}*/
	if(flag_getting_z  && !reading_params_for_z)
	{
		one_tx_for_z =  realloc(one_tx_for_z,strlen(one_tx_for_z)+2);

		sprintf(str,"select id as key , Value as Value from params where billno = '%s';",recptnum);
		//printf("Querry : %s\n",str);
		reading_params_for_z = 1;
		read_database(str , "transaction");
		if(reading_params_for_z>1){
			one_tx_for_z =  realloc(one_tx_for_z,strlen(one_tx_for_z)+2);
			strcat(one_tx_for_z,"}");

		}
		strcat(one_tx_for_z,"}");
		printf("The tx is : %s\n",one_tx_for_z);

		reading_params_for_z = 0;

		if(jcheck(one_tx_for_z))

		{
			if(number_of_z_transactions!=0 )
				strcat(z_data,",");
			else
			{

				if(!flag_getting_sbp)
				{
					z_data =  realloc(z_data,strlen(z_data)+15);
					//strcat(z_data,"{\"REPORTS\":[");
				}
			}
			if(flag_getting_sbp && increament_read==0 && set_p ==number_of_z_transactions)
			{

				z_data =  realloc(z_data,strlen(z_data)+15);
				//strcat(z_data,"{\"SBP\":[");
			}
			z_data =  realloc(z_data,strlen(z_data)+strlen(one_tx_for_z)+3);
			strcat(z_data , one_tx_for_z);
			free(one_tx_for_z);

			number_of_z_transactions++;
		}

//{"SBP":[{"billno":"009782016551-0001","transactiontype":"SBP","transactiondate":"2014-12-30 05:35:59","app_number":"APP14090002","paymentmethod":"Cash","paymentCode":"","total":"30.0","user":"6","voided":"0","posted":"0","receiptprinted":"0"},{"billno":"009782016551-0005","transactiontype":"SBP","transactiondate":"2014-12-30 07:13:28","app_number":"APP14090002","paymentmethod":"Cash","paymentCode":"","total":"30.0","user":"6","voided":"0","posted":"0","receiptprinted":"0"}]}

	}
	increament_read++;
	return 0;
}
void  sqlite_database_create_table(char *sql ,char *Db) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	//const char* data = "Callback function called";

	/* Open database */
	if(strcmp(Db , "services")==0)
		rc = sqlite3_open("services.db", &db);
	else if(strcmp(Db , "transaction")==0)
		rc = sqlite3_open("vihiga.db", &db);
	else if(strcmp(Db , "users_config")==0)
		rc = sqlite3_open("users_config.db", &db);
	else if(strcmp(Db , "operator")==0)
			rc = sqlite3_open("operator.db", &db);
	else
		return;

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);

	}

	/* Create SQL statement */

	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "Records created successfully\n");
	}
	sqlite3_close(db);
}

int sqlite_database_insert_into_table(char *sql ,char *Db)  {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	const char* data = "";

	//const char* data = "Callback function called";

	/* Open database */
	if(strcmp(Db , "services")==0)
		rc = sqlite3_open("services.db", &db);
	else if(strcmp(Db , "transaction")==0)
		rc = sqlite3_open("vihiga.db", &db);
	else if(strcmp(Db , "users_config")==0)
		rc = sqlite3_open("users_config.db", &db);
	else
		return 1;

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		return 0;
	}

	/* Execute SQL statement */
	if(strcmp(Db , "transaction")==0)
	rc = sqlite3_exec(db, sql, callback1, (void*) data, &zErrMsg);
	if(strcmp(Db , "services")==0 || strcmp(Db , "users_config")==0)
	rc = sqlite3_exec(db, sql, callbackServices, (void*) data, &zErrMsg);
	usleep(100000);



	/*rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);*/
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		//fprintf(stdout, "Records created successfully\n");
	}
	sqlite3_close(db);
	//read_database("Select * from TRANSACTIONS" ,"transaction");
	return 0;
}

//A function that reads the database.
void read_database(char * sql,char * Db) {
	{
		sqlite3 *db;
		char *zErrMsg = 0;
		int rc;
		const char* data = "Callback function called";
		printf("\nsql : %s\n\n", sql);

		/* Open database */
		if(strcmp(Db , "services")==0)
			rc = sqlite3_open("services.db", &db);
		else if(strcmp(Db , "transaction")==0)
			rc = sqlite3_open("vihiga.db", &db);
		else if(strcmp(Db , "users_config")==0)
			rc = sqlite3_open("users_config.db", &db);
		else if(strcmp(Db , "operator")==0)
				rc = sqlite3_open("operator.db", &db);


		if (rc) {
			fprintf(stderr, "Can't open database: %s\nCreating one \n",
					sqlite3_errmsg(db));
			exit(0);
		} else {
			//fprintf(stderr, "Opened database successfully\n");
		}
		clear_arrays();
		increament_read = 0;
		if(strcmp(Db , "operator")==0)
				rc = sqlite3_exec(db, sql, callback, (void*) data, &zErrMsg);
		if(strcmp(Db , "transaction")==0)
		rc = sqlite3_exec(db, sql, callback1, (void*) data, &zErrMsg);
		if(strcmp(Db , "services")==0 || strcmp(Db , "users_config")==0)
		rc = sqlite3_exec(db, sql, callbackServices, (void*) data, &zErrMsg);
		usleep(100000);


		if (rc != SQLITE_OK)
		{
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else
		{
			//fprintf(stdout, "Operation done successfully\n");
		}
		sqlite3_close(db);
		//read_database("select * from TRANSACTIONS;");
	}
}
void read_database_receiptlogs(char * sql,char * Db) {
	{
		sqlite3 *db;
		char *zErrMsg = 0;
		int rc;
		const char* data = "Callback function called";
		printf("\nsql : %s\n\n", sql);
		/* Open database */
			rc = sqlite3_open("vihiga.db", &db);
		if (rc) {
			fprintf(stderr, "Can't open database: %s\nCreating one \n",
					sqlite3_errmsg(db));
			exit(0);
		} else {
			//fprintf(stderr, "Opened database successfully\n");
		}
		clear_arrays();
		increament_read = 0;

		rc = sqlite3_exec(db,sql,callback_receiptlogs, (void*) data, &zErrMsg);
		usleep(100000);


		if (rc != SQLITE_OK)
		{
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else
		{
			printf("Operation done successfully\n");
		}
		sqlite3_close(db);
		//read_database("select * from TRANSACTIONS;");
	}
}
void clear_arrays() {
	if (subservice_id != 0) {
		free(serviceName);
		serviceName = 0;
	}
	if (serviceCode != 0) {
		free(serviceCode);
		serviceCode = 0;
	}
}
/*void create_all_table() {
	//(mac_address,username,billno,transactiontype,transactiondate,serviceid,paymentmethod,total,market)

	char * transactiontable;
	char * sbp_table;
	char * paramtable;
	char * Z_table;


	//Services
	char * paramservices;
	char * table_servicedtls;
	char * log_path;
	char * table_services;
	char * table_subservices;
	char * users_table;
	char * voids_count;


	transactiontable = "CREATE TABLE TRANSACTIONS("
			"mac_address  		CHAR(50)    NOT NULL,"
			"username     		CHAR(50)    NOT NULL,"
			"billno 			CHAR(50) 	PRIMARY KEY     NOT NULL,"
			"transactiontype   	INTEGER,"
			"transactiondate   	TEXT ,"
			"serviceid		   	TEXT ,"
			"subserviceid		TEXT ,"
			"rootservice		TEXT ,"
			"servicename		TEXT ,"
			"paymentmethod 	    TEXT,"
			"paymentCode 	    TEXT," //change to mpesa code
			"total  			REAL    NOT NULL,"
			"user_id  			TEXT    NOT NULL,"	//z_data = (char *) malloc(100+1);
			"market  			TEXT    NOT NULL,"
			"voided_col  		TEXT    NOT NULL,"
			"posted     		TEXT    NOT NULL,"
			"Z_done  			TEXT    NOT NULL,"
			"receipt_printed	TEXT 	NOT NULL"
			");";
	sqlite_database_create_table(transactiontable ,"transaction");

	transactiontable = "ALTER TABLE transactions ADD COLUMN z_posted REAL  DEFAULT 0;";

	sqlite_database_create_table(transactiontable ,"transaction");

	transactiontable = "ALTER TABLE transactions ADD COLUMN reprintCount REAL  DEFAULT 0;";

	sqlite_database_create_table(transactiontable ,"transaction");

	//strcpy(transactiontable,"");

	sbp_table = "CREATE TABLE SBP_TX("
			"mac_address  		CHAR(50)    NOT NULL,"
			"username     		CHAR(50)    NOT NULL,"
			"billno 			CHAR(50) 	PRIMARY KEY     NOT NULL,"
			"transactiontype   	INTEGER,"
			"transactiondate   	TEXT ,"
			"bname		   	TEXT ,"
			"bcode		TEXT ,"
			"bid		TEXT ,"
			"bdep		TEXT ,"
			"app_number		TEXT ,"
			"paymentmethod 	    TEXT,"
			"paymentCode 	    TEXT,"
			"total  			REAL    NOT NULL,"
			"user_id  			TEXT    NOT NULL,"	//z_data = (char *) malloc(100+1);
			"market  			TEXT    NOT NULL,"
			"voided_col  		TEXT    NOT NULL,"
			"posted     		TEXT    NOT NULL,"
			"Z_done  			TEXT    NOT NULL,"
			"receipt_printed	TEXT 	NOT NULL"
			");";
	sqlite_database_create_table(sbp_table ,"transaction");

	voids_count = "CREATE TABLE voids_count("
			"voidtransactiondate TEXT PRIMARY KEY NOT NULL,"
			"voids   		REAL ,"
			"reprintCount   		REAL "
			");";
	sqlite_database_create_table(voids_count,"users_config");

	voids_count = "CREATE TABLE reprint_count("
			"voidtransactiondate TEXT PRIMARY KEY NOT NULL,"
			"voids   		REAL );";
	sqlite_database_create_table(voids_count,"users_config");


	paramtable = "CREATE TABLE PARAMS("
			"billno 		CHAR(50)  NOT NULL,"
			"name   		TEXT ,"
			"Value		   	TEXT ,"
			"ID		   	TEXT ,"
			"PRIMARY KEY (billno,ID));";
	sqlite_database_create_table(paramtable,"transaction");


	Z_table = "CREATE TABLE Z_REPORTS("
			"billno 		CHAR(50) 	PRIMARY KEY     NOT NULL,"
			"Z_report   	TEXT ,"
			"Printed		TEXT ,"
			"Z_done		   	TEXT );";
	sqlite_database_create_table(Z_table,"transaction");

	log_path = "CREATE TABLE log_path("
			"id 			CHAR(50) 	PRIMARY KEY     NOT NULL,"
			"service_id  REAL); .mode column; .headers on;";
	sqlite_database_create_table(log_path,"transaction");


//[{"id":1,"serviceCode":"PAR","serviceName":"Parking"},{"id":2,"serviceCode":"MAR","serviceName":"Market"},{"id":3,"serviceCode":"EPARK","serviceName":"Entry Park"},{"id":4,"serviceCode":"TEST","serviceName":"Test Folder"},{"id":5,"serviceCode":"TEST2","serviceName":"Test Folder 2"}]

	///Offline
	paramservices = "CREATE TABLE IF NOT EXISTS PARAMS("
			"id 			TEXT  PRIMARY KEY NOT NULL,"
			"name   		TEXT ,"
			"paramtype		TEXT );";
	sqlite_database_create_table(paramservices,"services");

	table_services = "CREATE TABLE IF NOT EXISTS SERVICES("
			"id 				TEXT   PRIMARY KEY NOT NULL,"
			"serviceCode   		TEXT ,"
			"serviceName		TEXT );";
	sqlite_database_create_table(table_services,"services");


	////{"id":273,"charge":"50.00","param":"4"
	table_servicedtls = "CREATE TABLE IF NOT EXISTS SERVICEDTLS("
			"id 				TEXT   PRIMARY KEY NOT NULL,"
			"charge 	   		TEXT ,"
			"params		 		TEXT );";
	sqlite_database_create_table(table_servicedtls,"services");

	//{"id":7,"parentType":0,"parentId":1,"level":-1,"name":"Truck"}
	table_subservices = "CREATE TABLE IF NOT EXISTS SUBSERVICES("
			"id 			TEXT   PRIMARY KEY NOT NULL,"
			"parentType   	TEXT ,"
			"parentId   	TEXT ,"
			"level   		TEXT ,"
			"name			TEXT );";
	sqlite_database_create_table(table_subservices,"services");

	table_subservices = "CREATE TABLE IF NOT EXISTS messages("
			"id 			TEXT   PRIMARY KEY NOT NULL,"
			"number_from   	TEXT ,"
			"message   		TEXT ,"
			"Date_recieved   TEXT ,"
			"message_read   Real ,"
			"messsage_rep   Real );";
	sqlite_database_create_table(table_subservices,"users_config");

	users_table = "CREATE TABLE IF NOT EXISTS users("
			"benfid  		TEXT  PRIMARY KEY NOT NULL,"
			"firstname   	TEXT ,"
			"lastname   	TEXT ,"
			"middlename  	TEXT ,"
			"dateofbirth	TEXT ,"
			"gender		TEXT ,"
			"Address			TEXT ,"
			");";
	sqlite_database_create_table(users_table,"users_config");
	*
	 * chmod is a linux command for change mode
	 * changes mode in which files are accessed
	 * below command gives users,group and others permission to read,write and execute all db files

	system("chmod 777 *.db");
}*/
void create_all_table() {
	//(mac_address,username,billno,transactiontype,transactiondate,serviceid,paymentmethod,total,market)

	char * transactiontable;
	char * sbp_table;
	char * paramtable;
	char * Z_table;


	//Services
	char * parentservices;
	char * subservices;
	char * paramservices;
	char * table_servicedtls;
	char * log_path;
	char * table_services;
	char * table_subservices;
	char * users_table;
	char * voids_count;
	char * receipt_logs;

	//configs

		char * operatortable;
		char * netconf;
		//Services


		operatortable = "CREATE TABLE IF NOT EXISTS operator("
						"username 	TEXT  PRIMARY KEY NOT NULL,"
						"pin	   TEXT,"
						"idnumber  TEXT ,"
						"agentid   TEXT );";
				sqlite_database_create_table(operatortable,"operator");

				netconf = "CREATE TABLE IF NOT EXISTS netconf("
					"ip 			TEXT   PRIMARY KEY NOT NULL,"
					"port   	TEXT ,"
					"protocol   		TEXT ,"
					"timeout   TEXT ,"
					"apnname   TEXT ,"
					"password   TEXT );";
			sqlite_database_create_table(netconf,"operator");

	transactiontable = "CREATE TABLE TRANSACTIONS("
			"mac_address  		CHAR(50)    NOT NULL,"
			"username     		CHAR(50)    NOT NULL,"
			"billno 			CHAR(50) 	PRIMARY KEY     NOT NULL,"
			"transactiontype   	INTEGER,"
			"transactiondate   	TEXT ,"
			"serviceid		   	TEXT ,"
			"subserviceid		TEXT ,"
			"rootservice		TEXT ,"
			"servicename		TEXT ,"
			"paymentmethod 	    TEXT,"
			"paymentCode 	    TEXT,"
			"total  			REAL    NOT NULL,"
			"user_id  			TEXT    NOT NULL,"	//z_data = (char *) malloc(100+1);
			"market  			TEXT    NOT NULL,"
			"voided_col  		TEXT    NOT NULL,"
			"posted     		TEXT    NOT NULL,"
			"Z_done  			TEXT    NOT NULL,"
			"receipt_printed	TEXT 	NOT NULL"

			");";
	sqlite_database_create_table(transactiontable ,"transaction");

	//strcpy(transactiontable,"");

	/*sbp_table = "CREATE TABLE SBP_TX("
			"mac_address  		CHAR(50)    NOT NULL,"
			"username     		CHAR(50)    NOT NULL,"
			"billno 			CHAR(50) 	PRIMARY KEY     NOT NULL,"
			"transactiontype   	INTEGER,"
			"transactiondate   	TEXT ,"
			"bname		   	TEXT ,"
			"bcode		TEXT ,"
			"bid		TEXT ,"
			"bdep		TEXT ,"
			"app_number		TEXT ,"
			"paymentmethod 	    TEXT,"
			"paymentCode 	    TEXT,"
			"total  			REAL    NOT NULL,"
			"user_id  			TEXT    NOT NULL,"	//z_data = (char *) malloc(100+1);
			"market  			TEXT    NOT NULL,"
			"voided_col  		TEXT    NOT NULL,"
			"posted     		TEXT    NOT NULL,"
			"Z_done  			TEXT    NOT NULL,"
			"receipt_printed	TEXT 	NOT NULL"
			");";
	sqlite_database_create_table(sbp_table ,"transaction");*/

	/*voids_count = "CREATE TABLE voids_count("
			"voidtransactiondate TEXT PRIMARY KEY NOT NULL,"
			"voids   		REAL );";
	sqlite_database_create_table(voids_count,"users_config");*/


	/*paramtable = "CREATE TABLE PARAMS("
			"billno 		CHAR(50)  NOT NULL,"
			"name   		TEXT ,"
			"Value		   	TEXT ,"
			"ID		   	TEXT ,"
			"PRIMARY KEY (billno,ID));";
	sqlite_database_create_table(paramtable,"transaction");*/


/*	Z_table = "CREATE TABLE Z_REPORTS("
			"billno 		CHAR(50) 	PRIMARY KEY     NOT NULL,"
			"Z_report   	TEXT ,"
			"Printed		TEXT ,"
			"Z_done		   	TEXT );";
	sqlite_database_create_table(Z_table,"transaction");*/

	/*log_path = "CREATE TABLE log_path("
			"id 			CHAR(50) 	PRIMARY KEY     NOT NULL,"
			"service_id  REAL); .mode column; .headers on;";
	sqlite_database_create_table(log_path,"transaction");*/


//[{"id":1,"serviceCode":"PAR","serviceName":"Parking"},{"id":2,"serviceCode":"MAR","serviceName":"Market"},{"id":3,"serviceCode":"EPARK","serviceName":"Entry Park"},{"id":4,"serviceCode":"TEST","serviceName":"Test Folder"},{"id":5,"serviceCode":"TEST2","serviceName":"Test Folder 2"}]

	///Offline
/*	paramservices = "CREATE TABLE IF NOT EXISTS PARAMS("
			"id 			TEXT  PRIMARY KEY NOT NULL,"
			"name   		TEXT ,"
			"paramtype		TEXT );";
	sqlite_database_create_table(paramservices,"services");*/

	/*table_services = "CREATE TABLE IF NOT EXISTS SERVICES("
			"id 				TEXT   PRIMARY KEY NOT NULL,"
			"serviceCode   		TEXT ,"
			"serviceName		TEXT );";
	sqlite_database_create_table(table_services,"services");*/


	////{"id":273,"charge":"50.00","param":"4"
	/*table_servicedtls = "CREATE TABLE IF NOT EXISTS SERVICEDTLS("
			"id 				TEXT   PRIMARY KEY NOT NULL,"
			"charge 	   		TEXT ,"
			"params		 		TEXT );";
	sqlite_database_create_table(table_servicedtls,"services");*/

	//{"id":7,"parentType":0,"parentId":1,"level":-1,"name":"Truck"}
/*	table_subservices = "CREATE TABLE IF NOT EXISTS SUBSERVICES("
			"id 			TEXT   PRIMARY KEY NOT NULL,"
			"parentType   	TEXT ,"
			"parentId   	TEXT ,"
			"level   		TEXT ,"
			"name			TEXT );";
	sqlite_database_create_table(table_subservices,"services");*/
//read_database
	users_table = "CREATE TABLE IF NOT EXISTS users("
			"benfid  		TEXT  PRIMARY KEY NOT NULL,"
			"firstname   	TEXT ,"
			"lastname   	TEXT ,"
			"middlename   	TEXT ,"
			"dateofbirth			TEXT ,"
			"gender		TEXT   );";
	sqlite_database_create_table(users_table,"users_config");

	/*receipt_logs = "CREATE TABLE IF NOT EXISTS receipt_logs("
			"receipt_no 			TEXT   PRIMARY KEY NOT NULL,"
			"amount  	TEXT ,"
			"date   	TEXT );";
	sqlite_database_create_table(receipt_logs,"transaction");*/
	system("chmod 777 *.db");
}

void x_report() {
	char * tottal_num =
			"SELECT count(*) as x_report_total_txt_number FROM TRANSACTIONS where posted = 1 ;";
	char * total_val =
			"SELECT sum(total) AS x_report_total_txt_amount FROM TRANSACTIONS where posted = 1 ;";
	char * tottal_num_offline =
			"SELECT count(*) as x_report_total_txt_number_offline FROM TRANSACTIONS where posted = 0 ;";
	char * total_val_offline =
			"SELECT sum(total) AS x_report_total_txt_amount_offline FROM TRANSACTIONS where posted = 0;";
	char * tottal_num_voided =
			"SELECT count(*) as x_report_total_txt_number_voided FROM TRANSACTIONS where voided_col = 1  and  receipt_printed > 0;";
	char * total_val_voided =
			"SELECT sum(total) AS x_report_total_txt_amount_voided FROM TRANSACTIONS where voided_col = 1  and  receipt_printed > 0;";
	char * total_num_invalid =
			       "SELECT count(*) as x_report_total_num_invalid FROM TRANSACTIONS where receipt_printed = 0;";
				//"SELECT count(*) as x_report_total_num_invalid FROM TRANSACTIONS where receipt_printed = 0;";
	char * total_val_invalid =
				//"SELECT sum(total) AS x_report_total_val_invalid FROM TRANSACTIONS where receipt_printed = 0;";
			   "SELECT count(*) as x_report_total_num_invalid FROM TRANSACTIONS where receipt_printed = 0;";


	char * total_num_sbp =
			"SELECT count(*) as x_report_total_num_sbp FROM SBP_TX;";
	char * total_val_sbp =
			"SELECT sum(total) AS x_report_total_val_sbp FROM SBP_TX;";
	char * total_num_invalid_sbp =
				"SELECT count(*) as x_report_total_num_invalid_sbp FROM SBP_TX where receipt_printed = 0;";
	char * total_val_invalid_sbp =
				"SELECT sum(total) AS x_report_total_val_invalid_sbp FROM SBP_TX where receipt_printed = 0;";

	read_database(tottal_num,"transaction");
	read_database(total_val,"transaction");
	read_database(tottal_num_offline,"transaction");
	read_database(total_val_offline,"transaction");
	read_database(tottal_num_voided,"transaction");
	read_database(total_val_voided,"transaction");
	read_database(total_num_invalid,"transaction");
	read_database(total_val_invalid,"transaction");

	read_database(total_num_sbp,"transaction");
	read_database(total_val_sbp,"transaction");
	read_database(total_num_invalid_sbp,"transaction");
	read_database(total_val_invalid_sbp,"transaction");
}

void x_report_old() {
	char tottal_num[400];
	sprintf(tottal_num,"SELECT count(*) as x_report_total_txt_number FROM TRANSACTIONS where z_done = %d;",flag_z_done)
			;
	char  total_val [400];
	sprintf(total_val,	"SELECT sum(total) AS x_report_total_txt_amount FROM TRANSACTIONS where z_done = %d;",flag_z_done);
	char tottal_num_voided [400];
	sprintf(tottal_num_voided,"SELECT count(*) as x_report_total_txt_number_voided FROM TRANSACTIONS where voided_col = 1 and  z_done = %d;",flag_z_done);
	char total_val_voided [400];
	sprintf(total_val_voided,"SELECT sum(total) AS x_report_total_txt_amount_voided FROM TRANSACTIONS where voided_col = 1 and  z_done = %d;",flag_z_done);
	char total_num_invalid [400];
	sprintf(total_num_invalid,"SELECT count(*) as x_report_total_num_invalid FROM TRANSACTIONS where receipt_printed = 0 and  z_done = %d;",flag_z_done);
	char total_val_invalid [400];
	sprintf(total_val_invalid,"SELECT sum(total) AS x_report_total_val_invalid FROM TRANSACTIONS where receipt_printed = 0 and  z_done = %d;",flag_z_done);


	read_database(tottal_num,"transaction");
	read_database(total_val,"transaction");
	read_database(tottal_num_voided,"transaction");
	read_database(total_val_voided,"transaction");
	read_database(total_num_invalid,"transaction");
	read_database(total_val_invalid,"transaction");

}


void z_report(int offset)
{

	char more_z_data[300];
	//(+billno+:+004182016551-0001+,+transactiontype+:+1+,+transactiondate+:+2014-12-23%2011:01:20+,+serviceid+:+1+,+subserviceid+:+7+,+paymentmethod+:+Cash+,+params+:(+ID%20NO+:+25+),+total+:+200+,+user+:+6+,+market+:+Kesses+)
				//mac_address|username|billno|transactiontype|transactiondate|serviceid|rootservice|servicename|paymentmethod|paymentCode|total|user_id|market|voided_col|posted|Z_done|receipt_printed

	//{"REPORTS":[{"billno":"009782016551-0002","transactiontype":"1","transactiondate":"2014-12-30 05:37:06","serviceid":"1","paymentmethod":"Cash","paymentCode":"","total":"50.0","user":"6","voided":"0","posted":"1","receiptprinted":"1","params":{"ID NO":"254"}},{"billno":"009782016551-0003","transactiontype":"1","transactiondate":"2014-12-30 05:40:41","serviceid":"1","paymentmethod":"Cash","paymentCode":"","total":"50.0","user":"6","voided":"0","posted":"1","receiptprinted":"1","params":{"ID NO":"55"}},{"billno":"009782016551-0004","transactiontype":"1","transactiondate":"2014-12-30 06:27:21","serviceid":"2","paymentmethod":"Cash","paymentCode":"","total":"25.0","user":"6","voided":"0","posted":"1","receiptprinted":"1"}],{"SBP":[{"billno":"009782016551-0001","transactiontype":"SBP","transactiondate":"2014-12-30 05:35:59","app_number":"APP14090002","paymentmethod":"Cash","paymentCode":"","total":"30.0","user":"6","voided":"0","posted":"0","receiptprinted":"0"}]}

	char str[300];
	char test[50];

	char * tx_select ="SELECT billno ,transactiontype,transactiondate,serviceid , subserviceid, paymentmethod ,paymentCode,total,user_id as user, market,voided_col as voided,posted , receipt_printed as receiptprinted from TRANSACTIONS LIMIT 10 OFFSET ";
	char * sbp_select ="SELECT billno as receipt,transactiontype as type,transactiondate,app_number as appno,paymentmethod ,paymentCode,total,user_id as user,voided_col as voided,posted , receipt_printed as receiptprinted from SBP_TX;";
//App_number
	char tx_old[400];
	sprintf(tx_old,"SELECT billno ,transactiontype,transactiondate,serviceid , subserviceid, paymentmethod ,paymentCode,total,user_id as user, market,voided_col as voided,posted , receipt_printed as receiptprinted from TRANSACTIONS where z_done = '%s'  LIMIT 10 OFFSET %d  ", z_date,  offset_forZ);
	sprintf(str,"%s %d;",tx_select, offset);
	printf(str);

	number_of_z_transactions = 0;
	set_p=0;
	z_data = (char *) malloc(200+1);
	strcpy(z_data,"");
	strcpy(z_data, "$$");
	strcat(z_data, token);
	strcat(z_data, "@");
	strcat(z_data, mac_address);
	strcat(z_data, "@");
	strcat(z_data, CurrentUser.username);
	strcat(z_data, "@");
	strcat(z_data, "REPORT");
	strcat(z_data, "@&");
	//kb_getkey();
/*	if(z_number_recieved>0)
	{*/
		//sprintf(test,"z_number:%d*",z_number_recieved);
		sprintf(test,"z_number:%s*",z_date);
		strcat(z_data, test);
	///}
	strcat(z_data, "[");
	number_of_z_transactions = 0;

	reading_params_for_z = 0;
	flag_getting_sbp = 0;
	if(!flag_getting_sbp)
	{
		printf("Flag getting z  : %d \n\n", flag_getting_z) ;
	if(flag_z_done==0)
	read_database(str,"transaction");
	else
		read_database(tx_old,"transaction");
	if(number_of_z_transactions)
	//	strcat(z_data,"]");
	{
		flag_getting_z = 0;
		z_data = (char *) realloc(z_data ,strlen(z_data)+(1000));
		strcat(z_data,"]|");
		x_report();
		{
			sprintf(more_z_data,  "{\"zNumber\": \"%s\",", z_date);
			strcat(z_data,more_z_data);
			sprintf(more_z_data,  "\"market\": \"%s\",", CurrentUser.market);
			strcat(z_data,more_z_data);
			sprintf(more_z_data,  "\"device\": \"%s\",", serial_num);
			strcat(z_data,more_z_data);
			sprintf(more_z_data,  "\"totalTxns\": \"%d\",", x_report_total_txt_number);
			strcat(z_data,more_z_data);
			sprintf(more_z_data,  "\"totalAmt\": \"%d\",", x_report_total_txt_amount);
			strcat(z_data,more_z_data);
			sprintf(more_z_data,  "\"totalOfflineTxns\": \"%d\",", x_report_total_txt_number_offline);
			strcat(z_data,more_z_data);
			sprintf(more_z_data,  "\"totalOfflineAmnt\": \"%d\",", x_report_total_txt_amount_offline);
			strcat(z_data,more_z_data);
			sprintf(more_z_data,  "\"totalFailed\": \"%d\",", x_report_total_num_invalid);
			strcat(z_data,more_z_data);
			sprintf(more_z_data,  "\"failedAmt\": \"%d\",", x_report_total_val_invalid);
			strcat(z_data,more_z_data);
			sprintf(more_z_data,  "\"totalVoid\": \"%d\",", x_report_total_txt_number_voided);
			strcat(z_data,more_z_data);
			sprintf(more_z_data,  "\"voidAmt\": \"%d\",", x_report_total_txt_amount_voided);
			strcat(z_data,more_z_data);
			sprintf(more_z_data,  "\"totalNetTxns\": \"%d\",", x_report_total_txt_number+x_report_total_txt_number_offline-x_report_total_txt_number_voided- x_report_total_num_invalid);
			strcat(z_data,more_z_data);
			sprintf(more_z_data,  "\"netAmt\": \"%d\",",x_report_total_txt_amount+ x_report_total_txt_amount_offline- x_report_total_txt_amount_voided - x_report_total_val_invalid);
			strcat(z_data,more_z_data);
			sprintf(more_z_data,  "\"submittedBy\": \"\",");
			strcat(z_data,more_z_data);
			sprintf(more_z_data,  "\"receivedBy\": \"%d\",", CurrentUser.userid);
			strcat(z_data,more_z_data);
			sprintf(more_z_data,  "\"receivedAt\": \"\",");
			strcat(z_data,more_z_data);
			sprintf(more_z_data,  "\"createdAt\": \"\",");
			strcat(z_data,more_z_data);
			sprintf(more_z_data,  "\"updatedAt\": \"\",");
			strcat(z_data,more_z_data);
			sprintf(more_z_data,  "\"deletedAt\": \"\"}");
			strcat(z_data,more_z_data);
		}
		printf("%s\n", z_data);
		flag_getting_z = 1;
	}
	}
//{"REPORTS":[{"billno":"009782016551-0031","transactiontype":"1","transactiondate":"2014-12-30 20:16:40","serviceid":"1","paymentmethod":"Cash","paymentCode":"","total":"50.0","user":"6","voided":"0","posted":"1","receiptprinted":"1","params":{"ID NO":"54"}}]}
	else{
}
	if(number_of_z_transactions == 0 && offset ==0 && flag_getting_sbp == 0)
	{
		//if(download_complete)
		keep_db_after_z = 1;
		lcd_clean();
		screen_header();
		lcd_printf(ALG_CENTER,"There are no service");
		lcd_printf(ALG_CENTER,"transactions to post");
		lcd_printf(ALG_CENTER,"Press any Key to continue");
		lcd_flip();
		//free(z_data);
		//z_data=0;
		kb_getkey();
	}
/*	else if(number_of_z_transactions == 0 && offset ==0 && flag_getting_sbp == 1)
	{
		lcd_clean();
		screen_header();
		lcd_printf(ALG_CENTER,"There are no SBP ");
		lcd_printf(ALG_CENTER,"transactions to post");
		lcd_printf(ALG_CENTER,"Press any Key to continue");
		lcd_flip();
		//free(z_data);
		//z_data=0;
		kb_getkey();
	}*/
	else if(number_of_z_transactions == 0 && offset>0){

		offset_forZ = 0;
	}
	else
	{

		design_url("REPORT","1");
	}


}

void void_receipt()
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char sql[300];
	//char receipt[100];
	//char pword[40];
	//strcpy(pword, "Please enter Receipt No");
	const char* data = "Callback function called";


	/* Open database */

	rc = sqlite3_open("vihiga.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	} else {
		fprintf(stderr, "Opened database successfully\n");
	}

	/* Create merged SQL statement */
	//kb_getString(PASS_IN, 1, 16, receipt, NULL, pword);
	sprintf(sql,
			"UPDATE TRANSACTIONS set voided_col= 1 where billno = '%s';", recptnum);
	printf("%s", sql);
	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, (void*) data, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);

	}
	else {
		//fprintf(stdout, "Operation done successfully\n");



	}
	//Printer_Demo("VOIDED RECEIPT");
	sqlite3_close(db);
}

void reprint_receipt( int type)
{
	//get_last_receipt();
	char str_cmp[200];
	getdate_mine();
	char name[50];
	char gt_data[50] , gt_data1[50];
	int ret ,  recpt_voided;

	getdate_mine();


	if(type == 0 )
	{


		sprintf(str_cmp ,"SELECT MAX(billno) as billno_for_reprint FROM transactions where receipt_printed ='1';");
		read_database(str_cmp ,"transaction");

		sprintf(str_cmp ,"SELECT * from TRANSACTIONS where billno = '%s' and receipt_printed = 1 and  voided_col = 1;",recptnum);
		read_database(str_cmp ,"transaction");

		if(increament_read)
			recpt_voided = 1;
		else
			recpt_voided = 0;

		sprintf(str_cmp ,"SELECT * from TRANSACTIONS where billno = '%s' and receipt_printed = 1;",recptnum);
	//	sprintf(str_cmp ,"SELECT * FROM transactions WHERE billno = (SELECT MAX(billno) FROM transactions where posted  = 1);");

	}
	else if (type == 1 )
	{
		strcpy(name ,"Please enter the Receipt No");
		panel_two= "";
		strcpy(recptnum,"");

/*		kb_getString(ALPHA_IN, 1, 16,gt_data, NULL, name);
		if(strcmp(gt_data,"CANCEL_PRESSED")==0)
		{
			return;
		}*/
		ret = kb_getStringtwo(NUM_IN ,NUM_IN ,  6, 6, gt_data,gt_data1, NULL, name, panel_two,"Getting Receipt To reprint", 0);

		if(ret ==-1)
		{
			return;
		}
		//strcpy(recptnum,gt_data);
		//sprintf(recptnum,"%s%s-%s",z_number,serial_num,gt_data);
		getdate_mine_for_transaction(gt_data);
		sprintf(str_cmp ,"SELECT * from TRANSACTIONS where billno = '%s' and receipt_printed = 1 and  voided_col = 1;",recptnum);
		read_database(str_cmp ,"transaction");

		if(increament_read)
			recpt_voided = 1;
		else
			recpt_voided = 0;

		sprintf(str_cmp ,"SELECT * from TRANSACTIONS where billno = '%s' and  receipt_printed=1;",recptnum);

	}

		increament_read = 0;
		printf("SQL : %s",str_cmp);
						read_database(str_cmp ,"transaction");

						if(increament_read>0)
						{

						}
						else
						{
							lcd_clean();
							screen_header();
							lcd_printf(ALG_CENTER,"Invalid receipt Number");
							lcd_printf(ALG_CENTER,"OR Receipt printed successfully");
							lcd_printf(ALG_CENTER,"Press any Key to continue");
							lcd_flip();
							kb_getkey();
							return;
						}

						if (transactin_val_name != 0)
						{
							free(transactin_val_name);
							transactin_val_name = 0;
						}
						if (paramtype != 0)
						{
							free(paramtype);
							paramtype = 0;
						}
						if (transactin_name != 0)
						{
							free(transactin_name);
							transactin_name = 0;
						}
						if (transactin_val != 0)
						{
							free(transactin_val);
							transactin_val = 0;
						}
						number_of_trans=0;
						sprintf(str_cmp ,"SELECT name  as param_dtls_name , Value as param_dtls_val from PARAMS where billno = '%s'",recptnum);
						read_database(str_cmp,"transaction");

						//void_receipt();
						if(!recpt_voided){
						Printer_Demo("REPRINTED RECEIPT");
						sprintf(transactionfile , "update TRANSACTIONS set receipt_printed = '2'where billno = '%s'", recptnum);
						sqlite_database_insert_into_table(transactionfile,"transaction");
						}
						else
							Printer_Demo("VOIDED RECEIPT");



}
