/*
 * beneficiary_transactions.c
 *
 *  Created on: Dec 5, 2018
 *      Author: linux
 */

#include <stdlib.h>
#include "common_includes.h"
#include "merchant_transactions.h"
#include "../src/utilities/send_online_request.h"
#include "../src/utilities/lcd.h"
#include "utilities/keyboart.h"
#include "utilities/cJSON.h"
#include "fingerprint_reader.h"
#include "utilities/desfire_test.h"
#include "print_receipt.h"
//#include "utilities/sql_functions.h"
//#include "database.h"
#include "beneficiary_transactions.h"

#include "utilities/common_functions.h"
#include "utilities/sql_functions.h"
#include "usersManagement.h"
#include "mutwol.h"
#include "global_params.h"

int verify_card_and_read_data(  cJSON ** transaction_file , cJSON ** beneficiary_details);



void register_beneficiary(void);


/*
 * Does transaction on the card
 */
void do_beneficiary_transaction( int option);
void beneficiary_card_operation_functions(void );
void edit_beneficiary_details(void);
void fetch_beneficary_balance( void);
void fetch_beneficary_balances( void);
void fetch_beneficary_mini_statement( void);
void print_beneficairy_card_balances(void);
int card_mini_statement(char *  read_card_number   ,  char * wallet);

void beneficiary_transactions(void )
{
	int selected = 0;
	char  beneficiary_menu[][100] = {"Pay Merchant", "Cash  Out","Beneficiary  Account Operation", "Card Issuance", "Exit"};

	while(1)
	{


		switch( lcd_menu("Beneficiary Transactions", beneficiary_menu, 5 ,selected))
		{

		case 0:
			do_beneficiary_transaction(0);
			break;
		case 1:
			do_beneficiary_transaction(1);
			break;
		case 2:
			beneficiary_card_operation_functions();
			break;
		case 3:
			register_beneficiary();
			break;
		case 4:
			return;
			break;

		case -1:
			return;
			break;
		}
	}
}



void beneficiary_card_operation_functions(void )
{
	int selected = 0;
	char  beneficiary_card_operation[][100] = {"Balance Inquiry","Update Programmes","Mini Statement", "Exit"};

	while(1)
	{
		switch(lcd_menu("Beneficiary Card Operation", beneficiary_card_operation, 4 ,selected))
		{
		case 0:
			print_beneficairy_card_balances();
			break;
		case 1:
			fetch_beneficary_balances();
			break;
		case 2:
			fetch_beneficary_mini_statement();
			//Mini statement
			break;
		case 3:
		case -1:
			return;
			break;
		}
	}
}


/**
 * Function void register_beneficiary
 * Function carries our beneficiary  card linkage
 * @param void -  No input
 * @return void -  No  Output
 *
 */
void register_beneficiary(void){
	int ret;
	int change_made =0;
	int x= 0;

	cJSON * txToPosted=cJSON_CreateObject();
	cJSON * personalDetailsJson=cJSON_CreateObject();
	cJSON * request=cJSON_CreateObject();
	int w = 0;
	char getCharacters[40];
	char getCharacters1[40];
	char name1[30];
	char name[100];
	cJSON * response;
	unsigned char  *fingerprint[800];
	char * string ;
	char  * date , * receipt_no ,  *unformatedDate ,  returned_request_id;
	//Get  KYC from Table

	sqlite_database_read_write_operation(read_kycs_sql , "database.db");
	cJSON_AddItemToObject(request , "benRegistration" , txToPosted);

	strcpy(name1, "");
	//Get  Beneficiary details
	lcd_clean();

	while(w<400)
	{
		fp_response[w] = 0;
		w++;
	}







	int returned;
	char  * personal_details;
	char * transaction_file;
	char * new_read_card_number;
	char read_card_number[100];



	returned  = cardoperations(3,"", &personal_details ,&transaction_file,fingerprint ,  "" ,  "" ,"" , &new_read_card_number);

	if(returned){
		strcpy(read_card_number , new_read_card_number );
		printf("NNread_card_number :  %s\n ", read_card_number);

		strcpy( name , "Please enter Beneficairy Card Identifier" );


		ret = kb_getStringtwo(ALPHA_IN ,NUM_IN ,  0, 16, getCharacters,getCharacters1, NULL, name, "","Register Beneficiary", 0);
		if( strlen(getCharacters)>0 && ret!=-1)
		{}
		else
			return ;


		//ret =  cardoperations(1 , )
		get_date_and_receipt(0 , &date ,&unformatedDate , &receipt_no);
		//get_request_id(0 , &returned_request_id);
		cJSON_AddStringToObject(txToPosted,"iccid",read_card_number);
		cJSON_AddStringToObject(txToPosted,"transId",receipt_no);
		//cJSON_AddStringToObject(txToPosted,"requestId",returned_request_id);
		cJSON_AddStringToObject(txToPosted,"requestId","6482482");
		cJSON_AddStringToObject(txToPosted,"date",unformatedDate );
		cJSON_AddStringToObject(txToPosted,"terminalId",pos_serial_number);
		cJSON_AddStringToObject(txToPosted,"authMode","112");
		cJSON_AddStringToObject(txToPosted,"cardIdentifier",getCharacters);
		cJSON_AddStringToObject(txToPosted,"userId",myLoginPosUser->username);

		//Personal  File
		cJSON_AddStringToObject(personalDetailsJson,"iccid",read_card_number);
		cJSON_AddStringToObject(personalDetailsJson,"date",unformatedDate);


		string  = cJSON_Print(personalDetailsJson);
		printf(cJSON_Print(request));

		if(send_gprs_request("RegisterBen", request ,my_endpoint->beneficiary_link_card ,&response , 1)){
			/*	 char  * print;
		get_icCard(&print);*/
			printf("Response:  %s\n" , cJSON_Print(response));

			if(strcmp(cJSON_Print(cJSON_GetObjectItem(response,"status")) , "true")==0){
				//cJSON * balances  = cJSON_GetObjectItem(response , "customerResultDesc" );
				cJSON * balances  = cJSON_GetObjectItem(response , "resultDesc" );
				cJSON * create_new_obj  = cJSON_CreateObject();
				cJSON_AddItemToObject(create_new_obj , "balances" ,balances );
				cJSON_AddStringToObject(create_new_obj , "transactions" ,"" );
				printf("Value  to  card:  %s\n" , cJSON_Print(create_new_obj));

				if(fplib_test(0  ))
				{

					char * transact_string;
					transact_string   = cJSON_Print(create_new_obj);
					cJSON_Minify(transact_string);
					cJSON_Minify(string);
					printf(">>>>>>>JSON %s\n", string);
					personalizecard(string,fp_response ,  transact_string , read_card_number );
				}
				/*			else
			{
				return;
			}*/
			}
			else
			{

				message_display_function(1, "","Card Issuance Failed",get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(response,"resultDesc"))) , " Please try  again" ,  (char *)NULL );
				kb_getkey();
			}
		}
	}
}



void do_beneficiary_transaction( int option){
	int ret ,  selected = 0 , returned;
	char getCharacters[30], getCharacters1[30];

	//Transation JSON definations

	cJSON * final_transaction  =  cJSON_CreateObject();

	cJSON *  json_beneficiary_data;
	cJSON * balance_information;
	cJSON * balance_array;
	cJSON * transaction_json;
	cJSON * returned_balances;
	char * card_number;
	char  * new_read_card_number;
	char   read_card_number[20];
	customer_trans_operations *transops;


	char  txnDate[50],*transactionDate , *unformatedDate, *transactionID ,  transID[100] , receieved_unformatedDate[20];
	char * existing_transaction;
	char sql_transaction[800];
	char new_transaction[800];

	//char  * transaction_file = "{\"balances\":[{\"wallet\":\"WFP\",\"wallet\":\"100\",\"balance\":3000},{\"wallet\":\"IOM\",\"wallet\":\"200\",\"balance\":1000}],\"transactions\":{\"benTxn\":\"BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101\"}}";

	/*	char * beneficiary_details = "{"
			"\"BenRegistration\":{"
			"\"firstname\":	\"Kiptoo\","
			"\"lastname\":	\"Alex\","
			"\"iccid\":\"8cg4382017583718\","
			"\"middlename\":	\"K\","
			"\"dob\":	\"12134652\","
			"\"docType\":	\"National ID\","
			"\"docNumber\":	\"123465\""
			"}"
			"}";*/

	//Convert  strings to  Jsons and define Jsons

	char * personal_details ;
	char * personal_details_string ;
	char * transaction_file;
	char  temp[50];

	unsigned char  *fingerprint[800];


	int w = 0;
	while(w<400)
	{
		fp_response[w] = 0;
		w++;
	}


	returned  = cardoperations(1,"", &personal_details ,&transaction_file,fingerprint ,  "" ,  "" ,"" , &new_read_card_number);

	strcpy(read_card_number , new_read_card_number );
	printf("New C Number    %s\n"  ,  read_card_number);
	if(returned > 0){
		//char wallet_name[10][100];
		char  wallet_currency[10][50];
		char  wallet_id[10][100];
		double  wallet_amount[10];
		cJSON *  new_wallet = cJSON_CreateArray();

		//cJSON * new_txs = cJSON_CreateObject();
		int i= 0 ,  w=0;

		personal_details_string =  malloc(strlen(personal_details)+ 2);
		strcpy(personal_details_string  ,personal_details );
		//Create the Beneficiary  details and transaction Jsons.

		if(!jcheck(personal_details_string))
		{
			message_display_function(1, "","Transaction  Error","The user details file is incorrectly formated or the card contact  period is insufficient. Please try  again" ,  (char *)NULL );
			kb_getkey();
			return ;
		}
		if(!jcheck(transaction_file))
		{
			message_display_function(1, "","Transaction  Error","The card transaction file is incorrectly formated or the card contact  period is insufficient. Please try  again" ,  (char *)NULL );
			kb_getkey();
			return ;
		}
		json_beneficiary_data =  cJSON_Parse(personal_details_string);
		transaction_json  = cJSON_Parse(transaction_file);

		free(personal_details_string);
		//json_beneficiary_data  = cJSON_GetObjectItem(json_beneficary_details , "BenRegistration");
		printf("Read Beneficiary  Details   :  %s\n" , cJSON_Print(json_beneficiary_data));
		printf("Read stored   transactions  :  %s\n" , cJSON_Print(transaction_json));


		if(fplib_test(1 ))
		{
			//if fingerprint verification successful
			//Start


			balance_information  = cJSON_GetObjectItem(transaction_json , "balances");

			if(cJSON_GetArraySize(balance_information) == 0){
				message_display_function(1, "","Beneficiary Transaction error" ,  "The card has no available balances. Please get update card online and try  again ");
				kb_getkey();
				return;
			}

			//Get  vouchers
			balance_information  = cJSON_GetObjectItem(transaction_json , "balances");
			printf("%d\n" , cJSON_GetArraySize(balance_information));

			if(cJSON_GetArraySize(balance_information) == 0)
			{
				message_display_function(1, "","Beneficiary Transaction error" ,  "The card has no available balances. Please get update card online and try  again ");
				kb_getkey();
				return;
			}
			for (i  = 0; i<cJSON_GetArraySize(balance_information); i++){
				balance_array = cJSON_GetArrayItem(balance_information,i);
				//printf("Got in\n");

				//printf("",  cJSON_Print(cJSON_GetObject(balance_array , "wallet"))));
				//printf("Got in\n");

				strcpy(wallet_id[i],"");
				//strcpy( wallet_name[i], get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "wallet"))));
				strcpy( wallet_currency[i], get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "currency"))));
				strcpy( wallet_id[i], get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "wallet"))));
				wallet_amount[i] = atof(cJSON_Print(cJSON_GetObjectItem(balance_array , "balance")));
				printf("wallet_name  :  %s\n" , wallet_id[i]);

			}
			printf("%s\n" , cJSON_Print(balance_information));
			//forcJSON_GetArraySize(balance_information)
			while (selected>=0){

				selected= lcd_menu("Please select the program", wallet_id, i, selected);
				printf("optioned on %d\n", option);
				printf("Selected on %d\n", selected);
				if(selected > -1 && selected < i )
				{
					strcpy(getCharacters ,"");
					strcpy(getCharacters1 ,"");
					ret = kb_getStringtwo(NUM_IN ,NUM_IN ,  1, 16, getCharacters,getCharacters1, NULL, "Please enter the transaction amount", "","Beneficiary Transaction", 0);
					if( strlen(getCharacters)>0 && ret!=-1)
					{
						cJSON * txToPosted;
						cJSON *  json_array;
						if(wallet_amount[selected]>=atof(getCharacters)){
							int printflag ,print_complete;
							int resp;
							char transType[10];
							txToPosted=cJSON_CreateObject();
							json_array  =  cJSON_CreateArray();
							card_number = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(json_beneficiary_data , "iccid")));

							cJSON_AddItemToArray(json_array , txToPosted);
							cJSON_AddItemToObject(final_transaction , "benTxn" , json_array);
							get_date_and_receipt (0,  &transactionDate ,&unformatedDate,  &transactionID);
							strcpy(txnDate ,transactionDate);
							strcpy(transID ,transactionID);
							strcpy(receieved_unformatedDate ,unformatedDate);
							cJSON_AddStringToObject(txToPosted,"amount",getCharacters);
							switch(option){
							case 0:
								//switch(wallet_id[selected]){
								if(strcmp(wallet_id[selected],"M-PESA Account")== 0){
									cJSON_AddStringToObject(txToPosted,"transOperation",customer_operations->ORGANIZATION_ACCOUNT_BUY_GOODS);
									strcpy(transType ,customer_operations->ORGANIZATION_ACCOUNT_BUY_GOODS);
								}
								else if(strcmp(wallet_id[selected],"WFP Beneficiary Account") == 0){
									cJSON_AddStringToObject(txToPosted,"transOperation",customer_operations->BUY_GOODS_WFP_ACCOUNT);
									strcpy(transType ,customer_operations->BUY_GOODS_WFP_ACCOUNT );
								}
								else{
									//do nothinf
								}
								break;
							case 1:
								//switch(wallet_id[selected]){
								if(strcmp(wallet_id[selected],"M-PESA Account") == 0){
									cJSON_AddStringToObject(txToPosted,"transOperation",customer_operations->ORGANIZATION_ACCOUNT_CASH_OUT);
									strcpy(transType ,customer_operations->ORGANIZATION_ACCOUNT_CASH_OUT);
								}

								else if(strcmp(wallet_id[selected],"WFP Beneficiary Account") == 0){
									/*cJSON_AddStringToObject(txToPosted,"transOperation",customer_operations->ORGANIZATION_ACCOUNT_CASH_OUT);
								strcpy(transType ,customer_operations->ORGANIZATION_ACCOUNT_CASH_OUT);*/
									//cJSON_AddStringToObject(txToPosted,"transOperation",NULL);
									message_display_function(1, "", "Operation Error" , "This transaction type is not permitted on the selected wallet.", (char *)NULL);
									kb_getkey();
									return;
								}
								break;
							default:
								cJSON_AddStringToObject(txToPosted,"transOperation",NULL);
								message_display_function(1, "", "Transaction Error" , "This transaction type is not permitted on the selected wallet. Please select  the correct  wallet and try  again", (char *)NULL);
								kb_getkey();
								return;
							}
							/*******************************************************************************************************/
							/*if(option == 0 && selected ==0)
						{
							cJSON_AddStringToObject(txToPosted,"transOperation",customer_operations->BUY_GOODS_WFP_ACCOUNT);
							strcpy(transType ,customer_operations->BUY_GOODS_WFP_ACCOUNT );
						}
						else if(option == 0 && selected == 1)
						{
							cJSON_AddStringToObject(txToPosted,"transOperation",customer_operations->ORGANIZATION_ACCOUNT_BUY_GOODS);
							strcpy(transType ,customer_operations->ORGANIZATION_ACCOUNT_BUY_GOODS);
						}
						if(option == 1 && selected ==0)
						{
							cJSON_AddStringToObject(txToPosted,"transOperation",customer_operations->ORGANIZATION_ACCOUNT_CASH_OUT);
							strcpy(transType ,customer_operations->ORGANIZATION_ACCOUNT_CASH_OUT);
						}*/

							/*******************************************************************************************************/
							if(option == 1 && selected == 1)
							{
								cJSON_AddStringToObject(txToPosted,"transOperation",NULL);
								message_display_function(1, "", "Transaction Error" , "This transaction type is not permitted on the selected wallet. Please select  the correct  wallet and try  again", (char *)NULL);
								kb_getkey();
								return;
							}
							cJSON_AddStringToObject(txToPosted,"debitAccountNo",card_number);
							cJSON_AddStringToObject(txToPosted,"creditAccountNo",myMerchantUser->agentid);
							//cJSON_AddStringToObject(txToPosted,"wallet",wallet_id[selected]);
							cJSON_AddStringToObject(txToPosted,"walletName",wallet_id[selected]);
							cJSON_AddStringToObject(txToPosted,"currency",wallet_currency[selected]);
							//cJSON_AddNumberToObject(txToPosted,"balance",(wallet_amount[selected]-atof(getCharacters)));
							wallet_amount[selected] =wallet_amount[selected]-atof(getCharacters);
							cJSON_AddNumberToObject(txToPosted,"walletBalanceBefore",(wallet_amount[selected]+atof(getCharacters)));
							cJSON_AddNumberToObject(txToPosted,"walletBalanceAfter",(wallet_amount[selected]));
							cJSON_AddStringToObject(txToPosted,"transId",transID);
							cJSON_AddStringToObject(txToPosted,"terminalId",pos_serial_number);
							cJSON_AddStringToObject(txToPosted,"date",receieved_unformatedDate);
							cJSON_AddStringToObject(txToPosted,"authMode","101");
							cJSON_AddStringToObject(txToPosted,"userId",myLoginPosUser->username);
							printf("The Json :  %s\n", cJSON_Print(final_transaction));
							printf("sql_transaction : %s\n" , transID);

							/*						" CREATE TABLE transaction_table("
								"transId   	TEXT PRIMARY KEY	 ,"
								"transOperation		TEXT ,"
								"debitAccountNo 	TEXT ,"
								"creditAccountNo		TEXT, "
								"wallet		TEXT, "
								"amount		REAL, "
								"balanceBefore		REAL, "
								"balanceAfter		REAL, "
								"currency		REAL, "
								"terminalId		TEXT, "
								"date		TEXT, "
								"authMode		TEXT ,"
								"userId		TEXT ,"
								"posted		INTEGER	, "
								"receiptPrinted		INTEGER, "
								"cardUpdated		INTEGER"*/
							sprintf(sql_transaction , "Insert  into  transaction_table "
									"(transId ,transOperation ,debiticcid,crediticcid , wallet ,amount ,balanceBefore ,balanceAfter, currency, terminalId ,  date , authMode ,userId, posted  ,receiptPrinted , cardUpdated) "
									"VALUES('%s' , '%s', '%s' ,  '%s', '%s', %s,%0.2f, %0.2f ,'%s' ,'%s' , '%s' , '%s' ,  '%s' , 0 , 0 , 0);" ,
									transID ,transType ,card_number , myMerchantUser->agentid, wallet_id[selected],getCharacters ,(wallet_amount[selected]+atof(getCharacters)) ,wallet_amount[selected] ,wallet_currency[selected] ,pos_serial_number ,receieved_unformatedDate,"101" ,myLoginPosUser->username  );
							//sqlite_database_read_write_operation(sql_transaction ,"database.db");
							printf("sql_transaction : %s\n" , sql_transaction);
							sqlite_database_read_write_operation(sql_transaction ,"database.db");

							//111*transId
							//#%s*%s*transOperation*debiticcid*crediticcid*amount*date*authModeexisting_transaction  existing_transaction
							existing_transaction = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(transaction_json , "transactions")));


							sprintf(new_transaction , "%s*%s*%s*%s*%s*%s*%s*%s*%.2f*%.2f*%s*%s#%s" ,transID,pos_serial_number,transType , card_number,myMerchantUser->agentid, getCharacters,receieved_unformatedDate,wallet_id[selected] ,(wallet_amount[selected]+atof(getCharacters)),wallet_amount[selected] ,wallet_currency[selected],myLoginPosUser->username, existing_transaction);
							printf("New Transaction : %s\n",  new_transaction);



							for(w = 0 ; w<i;w++)
							{
								cJSON * new_wallet_item = cJSON_CreateObject();
								cJSON_AddStringToObject(new_wallet_item, "wallet",wallet_id[w]);
								//cJSON_AddStringToObject(new_wallet_item, "wallet",wallet_id[w]);
								cJSON_AddStringToObject(new_wallet_item, "currency",wallet_currency[w]);
								cJSON_AddNumberToObject(new_wallet_item, "balance",wallet_amount[w]);
								cJSON_AddItemToArray(new_wallet ,new_wallet_item );
							}
							//cJSON_AddStringToObject(new_txs, "benTxn",new_transaction );


							printf("Existing Transaction : %s\n",  existing_transaction);
							//printf("New Balance : %s\n",  cJSON_Print(new_transaction_json));

							//if Post and get_feedback is ok
							resp = post_transaction_file("card",new_transaction, &returned_balances ,1  );
							if(resp){
								cJSON * new_transaction_json = cJSON_CreateObject();
								char  * data_to_be_written;
								char * final_data_to_be_written;
								char  * tx;
								char * returned_balances_string;
								cJSON * balances_object;
								cJSON * responded_json_balances;
								char  update_sql[200];
								if(resp ==2)
								{
									//save offline and on card
									cJSON_AddItemToObject(new_transaction_json ,"balances" ,new_wallet);

									cJSON_AddStringToObject(new_transaction_json ,"transactions" ,new_transaction);
									//sqlite_database_read_write_operation(sql_transaction ,"database.db");

								}
								else
								{
									sprintf(update_sql , "update  transaction_table set posted = 1  where transId = '%s' ;" ,transID );
									sqlite_database_read_write_operation(update_sql ,"database.db");

									strcpy(update_sql ,"" );
									responded_json_balances = returned_balances;
									printf("Before add : %s\n" , cJSON_Print(new_transaction_json));

									//printf("Returned Balances : %s\n" , cJSON_Print(responded_json_balances));
									returned_balances_string = cJSON_Print(returned_balances);
									printf("Ret String  :  %s\n" , returned_balances_string);
									balances_object =  cJSON_GetObjectItem(cJSON_Parse(returned_balances_string),"balances");
									printf("Ret Json  :  %s\n" , cJSON_Print(balances_object));
									//are you here?
									//so after posting transactions, we want to write back balances to card right?
									cJSON_AddItemToObject(new_transaction_json ,"balances" ,balances_object);

									cJSON_AddStringToObject(new_transaction_json ,"transactions" ,"");
									cJSON_DeleteItemFromObject(new_transaction_json ,"benTxnResult");
									printf("1. I am writting  :  %s\n", cJSON_Print(new_transaction_json));
								}
								data_to_be_written = cJSON_Print(new_transaction_json);
								cJSON_Minify(data_to_be_written);
								printf("I am writting  :  %s\n", data_to_be_written);
								final_data_to_be_written = malloc(strlen(data_to_be_written) + 2);
								strcpy(final_data_to_be_written , data_to_be_written);
								strcat(final_data_to_be_written , "\n");
								//If write card is ok
								//writefile(int fd,uint8_t keyno,char MF[3],char APP[3],char authkey,char transactiondata,char* personaldetails)
								returned  = cardoperations(2,"", &tx ,&tx,fingerprint , "" ,  final_data_to_be_written , read_card_number , &new_read_card_number);

								if(returned){
									sprintf(update_sql , "update  transaction_table set cardUpdated = 1  where transId = '%s' ;" ,transID );
									sqlite_database_read_write_operation(update_sql ,"database.db");

									printf("\n card update success\n");
									/*if(1)
							{*/
									//update db that write was successful
									//
									//sqlite_database_read_write_operation(sql_transaction ,"database.db");
									print_receipt("TRANSACTION RECEIPT" , txToPosted ,   &printflag ,   &print_complete) ;
									print_complete = 1;
									if(print_complete)
									{
										sprintf(update_sql , "update  transaction_table set receiptPrinted = 1  where transId = '%s' ;" ,transID );
										sqlite_database_read_write_operation(update_sql ,"database.db");
										if(resp == 1)
										{

											//Delete the multiple posted transactions

											sprintf(update_sql , "delete from transaction_table where transId = '%s' ;" ,transID );
											sqlite_database_read_write_operation(update_sql ,"database.db");
											printf("++++++++++++++++++++++++++++++++++++++++\n");

										}
									}
								}
								break;
							}

						}
						else{
							//clear all params
							message_display_function(1, "","Beneficiary Transaction error" ,  "The requested transaction amount is more than the card balance available");
							kb_getkey();

						}
					}

					else if(ret==-1  )
					{
						return;
					}
				}
				else if(selected == -1)
				{
					return;
				}
				else
				{
					message_display_function(1,"","Invalid Selection", "Please select  again.", (char *)NULL);
					kb_getkey();
				}
			}

			//cut


		}
	}
	else
	{
		message_display_function(1,"","Card Error", "Error reading card. Please try  again", (char *)NULL);
		kb_getkey();
	}
}




void fetch_beneficary_balance( void){
	cJSON *  json_beneficiary_data;
	cJSON * card_balance_information_json;
	unsigned char  *fingerprint[800];
	cJSON * server_balance_array;
	cJSON * transaction_json;

	cJSON * card_update_request_json;
	char * card_number;
	char * first_card_number;


	//Card update flags
	int balances_found_on_card  = 0;
	int balances_found_on_server  = 0;

	char * existing_transaction;
	cJSON * respJson;
	int w = 0;
	while(w<400)
	{
		fp_response[w] = 0;
		w++;
	}

	if(verify_card_and_read_data( &transaction_json , &json_beneficiary_data)){

		int i= 0 ,  w=0;

		/*
		{
			"benfRequest": {
			    "transOperation": "100|200",
				"iccid": "8cg4382017583718",
				"transId": "BFRQ-10944002",
				"requestId": "123",
				"date": "20181124",
				"terminalId": "82346731",
				"authMode": "112"
			}
		}*/

		cJSON  * response ;
		char  * date,*unformatedDate,  * tx_id;
		cJSON * benf_req= cJSON_CreateObject();
		card_update_request_json =  cJSON_CreateObject();
		//transaction_json = t_json;
		printf("1: %s\n" , cJSON_Print(transaction_json));
		printf("2: %s\n" , cJSON_Print(json_beneficiary_data));
		cJSON_AddItemToObject(card_update_request_json , "benfRequest" ,benf_req );
		cJSON_AddStringToObject(benf_req ,"transOperation" ,"100" );
		cJSON_AddStringToObject(benf_req ,"iccid" ,"100" );
		get_date_and_receipt( 0 ,&date ,&unformatedDate ,&tx_id);
		cJSON_AddStringToObject(benf_req ,"transId" ,tx_id );
		cJSON_AddStringToObject(benf_req ,"requestId" ,"123" );
		cJSON_AddStringToObject(benf_req ,"date" ,date );
		cJSON_AddStringToObject(benf_req ,"terminalId" ,pos_serial_number );
		cJSON_AddStringToObject(benf_req ,"authMode" ,"112" );

		if(send_gprs_request("RegisterBen", card_update_request_json ,"/api/beneficiaryrequest/programmeupdate" ,&response ,  1)){

			printf("\nresponse :  %s\n", cJSON_Print(response));
			printf("\ntransaction_json :  %s\n", cJSON_Print(transaction_json));
			card_balance_information_json  = cJSON_GetObjectItem(transaction_json , "balances");
			existing_transaction = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(transaction_json , "transactions")));

			if(cJSON_GetArraySize(card_balance_information_json) > 0)
			{
				balances_found_on_card = 1;

			}

			respJson = cJSON_GetObjectItem(response ,"responseObj");
			printf("response :  %s\n", cJSON_Print(response));
			//if(cJSON_GetObjectItem(response ,"status")){
			if(strcmp(cJSON_Print(cJSON_GetObjectItem(response ,"status")),  "true") == 0)
			{
				cJSON  *  server_balance_information =  cJSON_GetObjectItem(respJson ,"balances");
				if(cJSON_GetArraySize(server_balance_information) > 0)
				{
					balances_found_on_server = 1;
					if(balances_found_on_card)
					{
						cJSON * new_card_balance_information =  cJSON_CreateArray();
						printf ("new_card_balance_information : %s\n" ,  cJSON_Print(new_card_balance_information));

						printf("\n server_balance_information  - %s\n" ,cJSON_Print(server_balance_information ));
						printf("\n card_balance_information  - %s\n" ,cJSON_Print(card_balance_information_json ));

						for (w  = 0; w<cJSON_GetArraySize(server_balance_information); w++){
							int found = 0;
							char * server_wallet_id;
							server_balance_array = cJSON_GetArrayItem(server_balance_information,w);
							server_wallet_id = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(server_balance_array , "wallet")));
							printf ("%d . server_wallet_id : %s\n" , w,  server_wallet_id);
							for (i  = 0; i<cJSON_GetArraySize(card_balance_information_json); i++)
							{
								cJSON * balance_array = cJSON_GetArrayItem(card_balance_information_json,i);
								char * card_wallet_id = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "wallet")));
								printf ("%d . card_wallet_id : %s\n" , i , card_wallet_id);
								if(strcmp(card_wallet_id ,server_wallet_id ) == 0 )
								{
									double wallet_amount;
									cJSON * other;
									found =1;
									wallet_amount = atof(cJSON_Print(cJSON_GetObjectItem(balance_array , "balance")));
									cJSON_DeleteItemFromObject(balance_array , "balance");
									wallet_amount = wallet_amount  + atof(cJSON_Print(cJSON_GetObjectItem(server_balance_array , "balance")));
									other  = balance_array;
									cJSON_AddNumberToObject(other ,"balance" ,wallet_amount );
									printf("Final :  %s \n" ,  cJSON_Print(card_balance_information_json));
									break;
								}
							}
							if (!found)
							{
								cJSON * new_obj = cJSON_CreateObject();
								cJSON_AddStringToObject(new_obj,"wallet" ,get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(server_balance_array , "wallet")) ));
								cJSON_AddStringToObject(new_obj,"wallet" , get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(server_balance_array , "wallet"))));
								cJSON_AddNumberToObject(new_obj,"balance" , atof(cJSON_Print(cJSON_GetObjectItem(server_balance_array , "balance"))));
								cJSON_AddStringToObject(new_obj,"currency" ,get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(server_balance_array , "currency"))) );


								cJSON_AddItemToArray( card_balance_information_json ,new_obj );
							}
						}
						printf("Final :  %s \n" ,  cJSON_Print(card_balance_information_json));
						//Merge the two
					}
				}
				printf("balances_found_on_server %d\n", balances_found_on_server);
				if(balances_found_on_server)
				{
					char * tx;
					char  * updated_value ;
					cJSON * final_tx  = cJSON_CreateObject();
					cJSON_AddItemToObject(final_tx ,"balances" ,card_balance_information_json );
					cJSON_AddStringToObject(final_tx ,"transactions" ,existing_transaction );
					updated_value  =  cJSON_Print(final_tx);
					cJSON_Minify(updated_value);
					printf("final_tx :  %s\n" , updated_value);
					if(cardoperations(2 ,"",&tx , &tx, fingerprint,"",updated_value,"", &first_card_number))
					{
						printf("Card update successful\n");

					}
				}
				printf(" tHIS IS NOT IT\n");
			}
			else
			{

			}
		}
	}
}




void fetch_beneficary_balances( void){
	cJSON *  json_beneficiary_data;
	cJSON * card_balance_information_json;
	unsigned char  *fingerprint[800];
	cJSON * server_balance_array;
	cJSON * transaction_json;

	cJSON * card_update_request_json;
	char * card_number;
	char * first_card_number;
	int returned;


	char  * new_read_card_number;
	char   read_card_number[20];
	char  * personal_details;
	char * transaction_file;
	//Card update flags


	char * existing_transaction;
	cJSON * respJson;
	int w = 0;
	while(w<400)
	{
		fp_response[w] = 0;
		w++;
	}


	//if(verify_card_and_read_data( &transaction_json , &json_beneficiary_data)){
	returned  = cardoperations(1,"", &personal_details ,&transaction_file,fingerprint ,  "" ,  "" ,"" , &new_read_card_number);



	strcpy(read_card_number , new_read_card_number );
	printf("New C Number    %s\n"  ,  read_card_number);
	if(returned > 0){
		int i= 0 ,  w=0;
		transaction_json = cJSON_Parse(transaction_file);
		cJSON  * response ;
		char  * date,*unformatedDate,  * tx_id;
		cJSON * benf_req= cJSON_CreateObject();
		card_update_request_json =  cJSON_CreateObject();
		json_beneficiary_data = cJSON_Parse(transaction_file);
		existing_transaction = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(transaction_json , "transactions")));


		if(fplib_test(1))
		{
			if(strlen(existing_transaction) == 0)
			{

				char * returned_request_id,req_id[50] , transId[50];
				get_request_id(1, &returned_request_id);
				strcpy(req_id, returned_request_id);
				cJSON_AddItemToObject(card_update_request_json , "benfBalRequest" ,benf_req );
				cJSON_AddStringToObject(benf_req ,"iccid" ,read_card_number );
				get_date_and_receipt( 0 ,&date ,&unformatedDate ,&tx_id);
				strcpy(transId , tx_id);
				cJSON_AddStringToObject(benf_req ,"transId" ,transId );

				cJSON_AddStringToObject(benf_req ,"requestId" ,req_id );
				cJSON_AddStringToObject(benf_req ,"date" ,date );
				cJSON_AddStringToObject(benf_req ,"terminalId" ,pos_serial_number );
				cJSON_AddStringToObject(benf_req ,"authMode" ,"112" );
				cJSON_AddStringToObject(benf_req ,"userId" ,myLoginPosUser->username);


				if(send_gprs_request("RegisterBen", card_update_request_json ,my_endpoint->beneficairy_balance_request ,&response ,  1)){
					printf("\nresponse :  %s\n", cJSON_Print(response));
					//printf("\ntransaction_json :  %s\n", cJSON_Print(transaction_json));

					///*** Start

					char * resp_status ;
					cJSON *  new_transaction_json = cJSON_CreateObject();
					resp_status  = cJSON_Print(cJSON_GetObjectItem(response ,  "status")) ;
					cJSON * resp_balances_result = cJSON_GetObjectItem(response ,  "resultDesc");

					printf("resp_balances_result :  %s\n" ,  cJSON_Print(resp_balances_result));
					printf("resp_balances : %s\n" ,  cJSON_Print(resp_balances_result));
					printf (resp_status);
					printf("\n");
					if(strcmp(resp_status,  "true") == 0)
					{
						char  * tx;
						int print_complete , printflag;
						printf("True\n");

						cJSON_AddItemToObject(new_transaction_json ,"balances" ,resp_balances_result);

						cJSON_AddStringToObject(new_transaction_json ,"transactions" ,"");

						char * data_to_be_written = cJSON_Print(new_transaction_json);
						cJSON_Minify(data_to_be_written);
						printf("I am writting  :  %s\n", data_to_be_written);

						//If write card is ok
						//writefile(int fd,uint8_t keyno,char MF[3],char APP[3],char authkey,char transactiondata,char* personaldetails)
						returned  = cardoperations(2,"", &tx ,&tx,fingerprint , "" ,  data_to_be_written , read_card_number , &new_read_card_number);

						if(returned){

							print_receipt("CARD BALANCE RECEIPT" , resp_balances_result ,   &printflag ,   &print_complete) ;
							print_complete = 1;
							if(print_complete)
							{
								//Print success.
							}
						}
						return ;
					}
					else
					{

						//display response



						errmessage =  cJSON_Print(cJSON_GetObjectItem(response,"message"));


						message_display_function(1,"","Balances Error",errmessage, (char *) NULL);
						kb_getkey();


						return ;
					}
				}

			}
			else {
				//There exist offline transactions

				int resp;
				cJSON * returned_balances;
				//if Post and get_feedback is ok
				resp = post_transaction_file("card",existing_transaction, &returned_balances ,  1);
				if(resp){
					cJSON * new_transaction_json = cJSON_CreateObject();
					char  * data_to_be_written;
					char * final_data_to_be_written;
					char  * tx;
					char * returned_balances_string;
					cJSON * balances_object ,  * new_balances_object;
					cJSON * responded_json_balances;
					char  update_sql[200];
					int returned;
					int print_complete , printflag;
					if(resp ==2)
					{
						message_display_function(1,  "" , "Error Posting Transactions" , "Server could not be reached. Please check  your connectivity  and try  again" ,  (char * ) NULL);
						kb_getkey();
					}
					else
					{

						responded_json_balances = returned_balances;
						printf("Before add : %s\n" , cJSON_Print(new_transaction_json));

						//printf("Returned Balances : %s\n" , cJSON_Print(responded_json_balances));
						returned_balances_string = cJSON_Print(returned_balances);
						printf("Ret String  :  %s\n" , returned_balances_string);
						balances_object =  cJSON_Parse(returned_balances_string);

						new_balances_object = cJSON_GetObjectItem(balances_object , "balances");
						cJSON_AddItemToObject(new_transaction_json ,"balances" ,new_balances_object);
						printf("Ret Json  :  %s\n" , cJSON_Print(new_balances_object));
						cJSON_AddStringToObject(new_transaction_json ,"transactions" ,"");
						cJSON_DeleteItemFromObject(new_transaction_json , "benTxnResult");
						printf("1. I am writting  :  %s\n", cJSON_Print(new_transaction_json));
					}
					data_to_be_written = cJSON_Print(new_transaction_json);
					cJSON_Minify(data_to_be_written);
					printf("I am writting  :  %s\n", data_to_be_written);
					final_data_to_be_written = malloc(strlen(data_to_be_written) + 2);
					strcpy(final_data_to_be_written , data_to_be_written);
					strcat(final_data_to_be_written , "\n");
					//If write card is ok
					//writefile(int fd,uint8_t keyno,char MF[3],char APP[3],char authkey,char transactiondata,char* personaldetails)
					returned  = cardoperations(2,"", &tx ,&tx,fingerprint , "" ,  final_data_to_be_written , read_card_number , &new_read_card_number);

					if(returned){

						print_receipt("CARD BALANCE RECEIPT" , new_balances_object ,   &printflag ,   &print_complete) ;
						print_complete = 1;
						if(print_complete)
						{
							//Print success.
						}
					}
				}

			}
		}
	}
}




void fetch_beneficary_mini_statement( void){
	cJSON *  json_beneficiary_data;
	cJSON * card_balance_information_json;
	unsigned char  *fingerprint[800];
	cJSON * server_balance_array;
	cJSON * transaction_json;

	cJSON * card_update_request_json;
	char * card_number;
	char * first_card_number;
	int returned;


	char  * new_read_card_number;
	char   read_card_number[20];
	char  * personal_details;
	char * transaction_file;
	//Card update flags


	char * existing_transaction;
	cJSON * respJson;
	int w = 0;
	while(w<400)
	{
		fp_response[w] = 0;
		w++;
	}


	//if(verify_card_and_read_data( &transaction_json , &json_beneficiary_data)){
	returned  = cardoperations(1,"", &personal_details ,&transaction_file,fingerprint ,  "" ,  "" ,"" , &new_read_card_number);



	strcpy(read_card_number , new_read_card_number );
	printf("New C Number    %s\n"  ,  read_card_number);
	if(returned > 0){
		int i= 0 ,  w=0;
		int do_mini_statement = 0;
		transaction_json = cJSON_Parse(transaction_file);
		cJSON  * response ;
		char  * date,*unformatedDate,  * tx_id;
		cJSON * benf_req= cJSON_CreateObject();
		card_update_request_json =  cJSON_CreateObject();
		json_beneficiary_data = cJSON_Parse(transaction_file);
		existing_transaction = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(transaction_json , "transactions")));

		if(fplib_test(1)){
			if(strlen(existing_transaction) == 0)
			{
				//Requesting card Mini Statement

				//End
				do_mini_statement = 1;
			}
			else {
				//There exist offline transactions

				int resp;
				cJSON * returned_balances;
				//if Post and get_feedback is ok
				resp = post_transaction_file("card",existing_transaction, &returned_balances , 1);
				if(resp){
					cJSON * new_transaction_json = cJSON_CreateObject();
					char  * data_to_be_written;
					char * final_data_to_be_written;
					char  * tx;
					char * returned_balances_string;
					cJSON * balances_object ,  *new_balance_object;
					cJSON * responded_json_balances;
					char  update_sql[200];
					int returned;
					int print_complete , printflag;
					if(resp ==2)
					{
						message_display_function(1,  "" , "Error Posting Transactions" , "Server could not be reached. Please check  your connectivity  and try  again" ,  (char * ) NULL);
						kb_getkey();
					}
					else
					{

						responded_json_balances = returned_balances;
						printf("Before add : %s\n" , cJSON_Print(new_transaction_json));

						//printf("Returned Balances : %s\n" , cJSON_Print(responded_json_balances));
						returned_balances_string = cJSON_Print(returned_balances);
						printf("Ret String  :  %s\n" , returned_balances_string);
						balances_object =  cJSON_Parse(returned_balances_string);
						printf("Ret Json  :  %s\n" , cJSON_Print(balances_object));
						new_balance_object  =  cJSON_GetObjectItem(balances_object ,"balances");
						cJSON_AddItemToObject(new_transaction_json ,"balances" ,new_balance_object);

						cJSON_AddStringToObject(new_transaction_json ,"transactions" ,"");
						cJSON_DeleteItemFromObject(new_transaction_json ,"benTxnResult");
						printf("1. I am writting  :  %s\n", cJSON_Print(new_transaction_json));
					}
					data_to_be_written = cJSON_Print(new_transaction_json);
					cJSON_Minify(data_to_be_written);
					printf("I am writting  :  %s\n", data_to_be_written);
					final_data_to_be_written = malloc(strlen(data_to_be_written) + 2);
					strcpy(final_data_to_be_written , data_to_be_written);
					strcat(final_data_to_be_written , "\n");
					//If write card is ok
					//writefile(int fd,uint8_t keyno,char MF[3],char APP[3],char authkey,char transactiondata,char* personaldetails)
					returned  = cardoperations(2,"", &tx ,&tx,fingerprint , "" ,  final_data_to_be_written , read_card_number , &new_read_card_number);

					if(returned){

						do_mini_statement = 1;
						/*					print_receipt("CARD BALANCE RECEIPT" , balances_object ,   &printflag ,   &print_complete) ;
					print_complete = 1;
					if(print_complete)
					{
						//Print success.
					}*/
						//request  mini statement
					}
				}

			}

			//
			if(do_mini_statement)
			{
				char wallet_id[10][100];

				int selected;
				cJSON* balance_information  = cJSON_GetObjectItem(transaction_json , "balances");
				printf("%d\n" , cJSON_GetArraySize(balance_information));

				if(cJSON_GetArraySize(balance_information) == 0)
				{
					message_display_function(1, "","Beneficiary Transaction error" ,  "The card has no available wallets. Please get update card online and try  again ");
					kb_getkey();
					return;
				}
				for (i  = 0; i<cJSON_GetArraySize(balance_information); i++){
					cJSON * balance_array = cJSON_GetArrayItem(balance_information,i);
					//printf("Got in\n");

					//printf("",  cJSON_Print(cJSON_GetObject(balance_array , "wallet"))));
					//printf("Got in\n");

					strcpy(wallet_id[i],"");
					//strcpy( wallet_name[i], get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "wallet"))));
					//strcpy( wallet_currency[i], get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "currency"))));
					strcpy( wallet_id[i], get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "wallet"))));
					//wallet_amount[i] = atof(cJSON_Print(cJSON_GetObjectItem(balance_array , "balance")));
					printf("wallet_name  :  %s\n" , wallet_id[i]);

				}
				printf("%s\n" , cJSON_Print(balance_information));
				//forcJSON_GetArraySize(balance_information)
				selected = 0;
				while (selected>=0){

					if(selected > -1 && selected < i )
					{
						selected= lcd_menu("Please select the program", wallet_id, i, selected);
						card_mini_statement(read_card_number ,wallet_id[selected] );
						return;
					}
					else if(selected == -1)
					{
						return;
					}
					else
					{
						message_display_function(1,"","Invalid Selection", "Please select  again.", (char *)NULL);
						kb_getkey();
					}

				}
			}
		}
		//
	}
}


int card_mini_statement(char *  read_card_number   ,  char * wallet){

	cJSON * benf_req , * card_update_request_json , * response;

	char * date, *unformatedDate , *tx_id , transId[40];

	int returned;

	char * returned_request_id,req_id[50];
	get_request_id(1, &returned_request_id);

	strcpy(req_id, returned_request_id);

	printf("req_id : %s\n", req_id);
	card_update_request_json = cJSON_CreateObject();
	benf_req = cJSON_CreateObject();

	/*
		{
		   "benfStmtRequest":{
		      "iccid":"211920000000",
		      "transId":"BFRQ-tfefsfffdr4t1",
		      "requestId":"123",
		      "date":"20181230",
		      "terminalId":"82346731",
		      "authMode":"112",
		      "walletName": "M-PESA Account",
		      "userId": "clement"
		   }
		}*/

	cJSON_AddItemToObject(card_update_request_json , "benfStmtRequest" ,benf_req );
	//cJSON_AddStringToObject(benf_req ,"transOperation" ,"100" );
	cJSON_AddStringToObject(benf_req ,"iccid" ,read_card_number );
	get_date_and_receipt( 0 ,&date ,&unformatedDate ,&tx_id);
	strcpy(transId , tx_id);
	printf("transId : %s\n", transId);
	cJSON_AddStringToObject(benf_req ,"transId" ,transId );
	cJSON_AddStringToObject(benf_req ,"walletName" ,wallet );
	cJSON_AddStringToObject(benf_req ,"requestId" ,req_id );
	cJSON_AddStringToObject(benf_req ,"date" ,unformatedDate );
	cJSON_AddStringToObject(benf_req ,"terminalId" ,pos_serial_number );
	cJSON_AddStringToObject(benf_req ,"authMode" ,"112" );
	cJSON_AddStringToObject(benf_req ,"userId" ,myLoginPosUser->username);

	printf("json : %s\n" , cJSON_Print(benf_req));
	if(send_gprs_request("RegisterBen", card_update_request_json ,my_endpoint->beneficairy_statement_request ,&response ,  1)){
		printf("\nresponse :  %s\n", cJSON_Print(response));
		//printf("\ntransaction_json :  %s\n", cJSON_Print(transaction_json));

		///*** Start

		char * resp_status ;
		cJSON *  new_transaction_json = cJSON_CreateObject();
		resp_status  = cJSON_Print(cJSON_GetObjectItem(response ,  "status")) ;
		cJSON * resp_balances_result = cJSON_GetObjectItem(response ,  "resultDesc");

		printf("resp_balances_result :  %s\n" ,  cJSON_Print(resp_balances_result));
		printf("resp_balances : %s\n" ,  cJSON_Print(resp_balances_result));
		printf (resp_status);
		printf("\n");
		if(strcmp(resp_status,  "true") == 0)
		{
			char  * tx;
			int print_complete , printflag;
			printf("True\n");

			cJSON_AddItemToObject(new_transaction_json ,"balances" ,resp_balances_result);

			cJSON_AddStringToObject(new_transaction_json ,"transactions" ,"");

			char * data_to_be_written = cJSON_Print(new_transaction_json);
			cJSON_Minify(data_to_be_written);
			printf("I am writting  :  %s\n", data_to_be_written);

			print_receipt("CARD MINI STATEMENT" , resp_balances_result ,   &printflag ,   &print_complete) ;
			print_complete = 1;
			if(print_complete)
			{
				//Print success.
			}
			return 1;
		}
		else
		{

			return 0;
		}
	}


}




void print_beneficairy_card_balances( void){
	cJSON *  json_beneficiary_data;
	cJSON * card_balance_information_json;
	unsigned char  *fingerprint[800];
	cJSON * server_balance_array;
	cJSON * transaction_json;

	cJSON * card_update_request_json;


	//Card update flags
	int balances_found_on_card  = 0;
	int balances_found_on_server  = 0;

	int printflag ,   print_complete;
	char * existing_transaction;
	cJSON * respJson;
	int w = 0;
	while(w<400)
	{
		fp_response[w] = 0;
		w++;
	}

	if(verify_card_and_read_data( &transaction_json , &json_beneficiary_data)){

		char  * date, * unformatedDate, * tx_id;
		get_date_and_receipt( 0 , &date ,&unformatedDate,  &tx_id);
		card_balance_information_json  = cJSON_GetObjectItem(transaction_json , "balances");
		existing_transaction = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(transaction_json , "transactions")));

		if(cJSON_GetArraySize(card_balance_information_json) > 0)
		{
			print_receipt("BALANCE ENQUIRY RECEIPT" , card_balance_information_json ,   &printflag ,   &print_complete) ;


			/*			message_display_function(1, "" , "Card Balances" , "Balances found" , (char *)NULL);
			kb_getkey();*/
			balances_found_on_card = 1;

		}
		else
		{
			message_display_function(1, "" , "Card Balances" , "The tapped card has no vouchers. Please update the card online and try  again" , (char *)NULL);
			kb_getkey();
		}


	}
}






void update_beneficiary_balances( void){
	int  selected;
	char getCharacters[30];

	//BYTE  * fingerprint[3];
	//Transation JSON definations
	cJSON * txToPosted;
	cJSON * final_transaction ;
	cJSON * json_beneficary_details ;
	cJSON *  json_array;
	cJSON *  json_beneficiary_data;
	cJSON * balance_information;
	cJSON * balance_array;
	cJSON * transaction_json;
	char * card_number;




	char * txnDate, *transactionID ,*unformatedDate,  transID[100];
	char * existing_transaction;
	char sql_transaction[800];
	char new_transaction[800];
	/*	message_display_function(1, "","Beneficiary Registration" ,  "Please tap the NFC card and press enter key");
	key=kb_getkey();
	if(key==DIKS_ENTER){*/
	//function to  get  the card details from the card
	//char
	char  * transaction_file = "{\"balances\":[{\"wallet\":\"WFP\",\"wallet\":\"100\",\"balance\":3000},{\"wallet\":\"IOM\",\"wallet\":\"200\",\"balance\":1000}],\"transactions\":{\"benTxn\":\"BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101\"}}";

	//	char  * resp  = "{\"productUpdate\":{\"iccid\":\"8cg4382017583718\",\"transId\":\"UB-7584736\",\"terminalId\":\"82346731\",\"date\":\"20181106\"}}";
	char * beneficiary_details = "{"
			"\"BenRegistration\":{"
			"\"firstname\":	\"Kiptoo\","
			"\"lastname\":	\"Alex\","
			"\"iccid\":\"8cg4382017583718\","
			"\"middlename\":	\"K\","
			"\"dob\":	\"12134652\","
			"\"docType\":	\"National ID\","
			"\"docNumber\":	\"123465\""
			"}"
			"}";

	//Convert  strings to  Jsons and define Jsons

	txToPosted=cJSON_CreateObject();
	final_transaction  = cJSON_CreateObject();
	json_beneficary_details =cJSON_CreateObject();
	json_array=  cJSON_CreateArray();
	transaction_json =  cJSON_Parse(transaction_file);


	printf(beneficiary_details);
	printf("\n");
	printf("start :  %s\n",  cJSON_Print(transaction_json));
	json_beneficary_details = cJSON_Parse(beneficiary_details);
	json_beneficiary_data  = cJSON_GetObjectItem(json_beneficary_details , "BenRegistration");
	printf(cJSON_Print(json_beneficary_details));
	printf("End\n");
	printf("\n");
	//  ,cJSON_GetObjectItem(json_beneficary_details , "BioData"
	if(fplib_test(1))
	{
		//if fingerprint verification successful

		//Start
		char wallet_name[10][100];
		char  wallet_id[10][50];
		double  wallet_amount[10];
		cJSON *  new_wallet = cJSON_CreateArray();
		cJSON * new_transaction_json = cJSON_CreateObject();
		cJSON * new_txs = cJSON_CreateObject();
		int i= 0 ,  w=0;
		int printflag ,print_complete;

		//Get  vouchers
		balance_information  = cJSON_GetObjectItem(transaction_json , "balances");
		printf("%d\n" , cJSON_GetArraySize(balance_information));
		for (i  = 0; i<cJSON_GetArraySize(balance_information); i++){
			//printf("Got in\n");
			balance_array = cJSON_GetArrayItem(balance_information,i);
			//printf("Got in\n");

			//printf("",  cJSON_Print(cJSON_GetObject(balance_array , "wallet"))));
			//printf("Got in\n");
			strcpy(wallet_name[i],"");
			strcpy( wallet_name[i], get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "wallet"))));
			strcpy( wallet_id[i], get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "wallet"))));
			wallet_amount[i] = atof(cJSON_Print(cJSON_GetObjectItem(balance_array , "balance")));


			printf("Got in %.2f\n" , wallet_amount[i]);
			//i = i + collums_in_rs;
			//x++;
		}
		//printf("%s\n" , cJSON_Print(balance_information));
		//forcJSON_GetArraySize(balance_information)
		//while (selected>=0){
		selected= lcd_menu("Please select the program", wallet_name, i, selected);
		printf("Selected on %d\n", selected);
		/*		if(selected > -1 && selected < i )
			{*/
		/*				ret = kb_getStringtwo(NUM_IN ,NUM_IN ,  1, 16, getCharacters,getCharacters1, NULL, "Please enter the transaction amount", "","Beneficiary Transaction", 0);
				if( strlen(getCharacters)>0 && ret!=-1)
				{*/
		/*if(wallet_amount[selected]>atof(getCharacters)){*/

		/*					{
					"benTxn":{
					"transOperation":"111",
					"amount":1027,
					"debiticcid":"8cg4382017583718",
					"crediticcid":"8cg4382017583719",
					"transId":"BT-75873937",
					"terminalId":"82346731",
					"date":"20181124",
					"authMode":"100"
					}
					}*/
		//logic to  compare the amount

		/*						{
							"productUpdate": {
								"iccid": "8cg4382017583718",
								"transId": "UB-7584736",
								"terminalId": "82346731",
								"date": "20181106"
							}
						}*/
		card_number = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(json_beneficiary_data , "iccid")));

		cJSON_AddItemToArray(json_array , txToPosted);
		cJSON_AddItemToObject(final_transaction , "benTxn" , json_array);
		get_date_and_receipt (0,  &txnDate ,&unformatedDate, &transactionID);
		strcpy(transID ,transactionID);
		cJSON_AddStringToObject(txToPosted,"amount",getCharacters);
		cJSON_AddStringToObject(txToPosted,"transOperation","111");
		cJSON_AddStringToObject(txToPosted,"debiticcid",card_number);
		cJSON_AddStringToObject(txToPosted,"crediticcid","Merchant_ID");
		cJSON_AddStringToObject(txToPosted,"wallet",wallet_id[selected]);
		cJSON_AddStringToObject(txToPosted,"wallet",wallet_name[selected]);
		cJSON_AddNumberToObject(txToPosted,"balance",(wallet_amount[selected]-atof(getCharacters)));
		wallet_amount[selected] =wallet_amount[selected]-atof(getCharacters);
		cJSON_AddStringToObject(txToPosted,"transId",transID);
		cJSON_AddStringToObject(txToPosted,"terminalId",pos_serial_number);
		cJSON_AddStringToObject(txToPosted,"date",txnDate);
		cJSON_AddStringToObject(txToPosted,"authMode","101");
		printf("The Json :  %s\n", cJSON_Print(final_transaction));
		printf("sql_transaction : %s\n" , transID);
		sprintf(sql_transaction , "Insert  into  transaction_table "
				"(transId ,transOperation ,debiticcid,crediticcid , wallet ,amount , terminalId ,  date , authMode ) "
				"VALUES('%s' , '%s', '%s' ,  '%s', '%s', %s, '%s' , '%s' , '%s');" ,
				transID ,"111" ,card_number , "Merchant_ID", wallet_id[selected],getCharacters ,pos_serial_number ,txnDate,"101"  );
		//sqlite_database_read_write_operation(sql_transaction ,"database.db");
		printf("sql_transaction : %s\n" , sql_transaction);
		//111*transId
		//#%s*%s*transOperation*debiticcid*crediticcid*amount*date*authModeexisting_transaction  existing_transaction
		existing_transaction = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(cJSON_GetObjectItem(transaction_json , "transactions"),"benTxn")));

		sprintf(new_transaction , "%s*%s*111*%s*crediticcid*%s*%s*%s*101#%s" ,transID,pos_serial_number,card_number,getCharacters,txnDate,wallet_id[selected] ,existing_transaction);
		printf("New Transaction : %s\n",  new_transaction);

		cJSON_AddItemToObject(new_transaction_json ,"balances" ,new_wallet);
		cJSON_AddItemToObject(new_transaction_json ,"transactions" ,new_txs);

		for(w = 0 ; w<i;w++)
		{
			cJSON * new_wallet_item = cJSON_CreateObject();
			cJSON_AddStringToObject(new_wallet_item, "wallet",wallet_name[w]);
			cJSON_AddStringToObject(new_wallet_item, "wallet",wallet_id[w]);
			cJSON_AddNumberToObject(new_wallet_item, "balance",wallet_amount[w]);
			cJSON_AddItemToArray(new_wallet ,new_wallet_item );
		}
		cJSON_AddStringToObject(new_txs, "benTxn",new_transaction );


		printf("Existing Transaction : %s\n",  existing_transaction);
		printf("New Balance : %s\n",  cJSON_Print(new_transaction_json));
		//if Post and get_feedback is ok

		if(1){

			//If write card is ok
			if(1)
			{
				//
				sqlite_database_read_write_operation(sql_transaction ,"database.db");
				print_receipt("TRANSACTION RECEIPT" , txToPosted ,  &printflag ,   &print_complete) ;
			}
			//}
		}

		/*					}
					else{
						//clear all params
						message_display_function(1, "","Beneficiary Transaction error" ,  "The requested transaction amount is more than the card balance available");
						kb_getkey();

					}*/
		/*				}

				else if(ret==-1  )
				{
					return;
				}*/
		/*			}
			else if(selected == -1)
			{
				return;
			}
			else
			{
				message_display_function(1,"","Invalid Selection", "Please select  again.", (char *)NULL);
				kb_getkey();
			}*/
		//}

		//cut


	}

	//register_ben();
	/*	}
	else
		return;*/
}


int  post_transaction_file(char  * transaction_source, char * transaction , cJSON ** response_balances  , int display_messages)
{
	//char  ** tokens  = 0 ;
	char * t_file  = transaction;
	char  split_transactions[100][200];
	char  trans[200];
	int w  =  0 ,  x = 0;
	cJSON * response_json ;
	cJSON * final_transaction;
	cJSON * json_array;
	cJSON /** resp_balances , */ * resp_balances_result;

	//No transactions to  post

	//char * str = "BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101";

	//strcat(t_file , "\0");
	char *del = "#";
	char *s;
	if(strlen(transaction)==0)
		return 0;
	s = my_strtok(t_file, del);
	while(s) {
		//printf("%s\n", s);
		strcpy(split_transactions[w], "");
		if(strlen(s)>0)
		{
			strcpy(split_transactions[w], s);
			printf("%d: %s\n", w, split_transactions[w]);
		}
		//s = 0;
		s = my_strtok(NULL, del);
		w++;
	}


	char * returned_request_id,req_id[10];
	get_request_id(1, &returned_request_id);
	strcpy(req_id, returned_request_id);
	final_transaction=cJSON_CreateObject();
	json_array =  cJSON_CreateArray();
	cJSON_AddItemToObject(final_transaction , "benTxn" , json_array);
	cJSON_AddNumberToObject(final_transaction , "count" , w);
	cJSON_AddStringToObject(final_transaction , "source" , transaction_source);
	cJSON_AddStringToObject(final_transaction , "requestId" , req_id);

	//printf("Final_transaction : %s\n" ,  cJSON_Print(final_transaction));
	printf("w : %d \n",w);
	for(x = 0 ;  x<w;x++)
	{
		//printf("%d : %s\n",x,  split_transactions[x]);
		int  y = 0;
		char * delim =  "*";
		cJSON * txToPosted=cJSON_CreateObject();
		//char  *  temp =

		strcpy(trans , split_transactions[x]);
		printf("tx: %s\n" , trans);
		char * data = my_strtok(trans, delim);
		printf("x: %d\n" , x);

		//card_number = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(json_beneficiary_data , "iccid")));

		cJSON_AddItemToArray(json_array , txToPosted);

		/*		{
			"source" : "card",
			"count" : 2,
			"debitAccountNo": "211920000000",
			"requestId": "dsfsdfds",
			"benTxn": [{
				"transOperation": "100",
				"amount": 10,
				"creditAccountNo": "000006",
				"transId": "BT-sdfdfxdfccdfvdcdgfdfggfffdffdsvcddffkjfsf",
				"walletName": "M-PESA Account",
				"walletBalanceBefore": 100,
				"walletBalanceAfter": 100,
				"terminalId": "82346731 ",
				"date": "20181124",
				"authMode": "100",
				"userId": "clement"
			} , {
				"transOperation": "200",
				"amount": 40,
				"creditAccountNo": "000006",
				"transId": "BT-dsfsdffcvddffddcfgdgdfxdfgcfffdsf",
				"walletName": "M-PESA Account",
				"walletBalanceBefore": 100,
				"walletBalanceAfter": 100,
				"terminalId": "82346731 ",
				"date": "20181123",
				"authMode": "100",
				"userId": "clement"
			}]
		}*/


		while(data) {
			//printf("%s\n", s);
			//strcpy(split_transactions[w], data);

			//BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101
			//BT-82247929-190222084508*82247929*111*76114338*123*50.0*20190222*M-PESA Account*5765.0*5715.0*SSP*b



			printf("y :  %d ,  %s\n" , y , data);
			if(y == 0 )
				cJSON_AddStringToObject(txToPosted,"transId",data);
			else if(y == 1 )
				cJSON_AddStringToObject(txToPosted,"terminalId",data);
			else if(y == 2 )
				cJSON_AddStringToObject(txToPosted,"transOperation",data);
			else if(y == 3 )
			{
				if(strcmp(transaction_source , "pos")==0)
					cJSON_AddStringToObject(txToPosted,"debitAccountNo",data);
				if(strcmp(transaction_source , "card")==0  && x == 0 )
				{
					cJSON_AddStringToObject(final_transaction , "debitAccountNo" , data);
				}
			}
			else if(y == 4 )
				cJSON_AddStringToObject(txToPosted,"creditAccountNo",data);

			//,wallet_id[selected]
			else if(y == 5 )
			{
				cJSON_AddNumberToObject(txToPosted,"amount",atof(data));
			}
			else if(y == 6 )
				cJSON_AddStringToObject(txToPosted,"date",data);
			else if(y == 7 )
				cJSON_AddStringToObject(txToPosted,"walletName",data);
			else if(y == 8 )
				cJSON_AddNumberToObject(txToPosted,"walletBalanceBefore",atof(data));
			else if(y == 9)
				cJSON_AddNumberToObject(txToPosted,"walletBalanceAfter",atof(data));
			/*	else if(y == 10)
				cJSON_AddStringToObject(txToPosted,"currency",data);*/
			else if(y == 11)
			{
				cJSON_AddStringToObject(txToPosted,"authMode","101");
				//else if(y == 12)
				cJSON_AddStringToObject(txToPosted,"userId",data);

			}
			//cJSON_AddNumberToObject(txToPosted,"balance",(wallet_amount[selected]-atof(getCharacters)));

			//cJSON_AddStringToObject(txToPosted,"terminalId",pos_serial_number);
			data = my_strtok(NULL, delim);

			y++;
		}
	}
	printf("initial json\n");
	if(send_gprs_request("Transactions" , final_transaction , my_endpoint->beneficary_transaction,  &response_json , display_messages ))
	{

		char * resp_status ;
		cJSON *  from_server;
		from_server  = response_json;
		printf(cJSON_Print(response_json));
		//start
		if(strcmp(transaction_source , "card")== 0)
		{
			//resp_status  = cJSON_Print(cJSON_GetObjectItem(from_server ,  "status"));
			//disable this check once status is included in the returned responses
			if(cJSON_Print(cJSON_GetObjectItem(from_server ,  "status"))!=NULL){
				resp_status  = cJSON_Print(cJSON_GetObjectItem(from_server ,  "status"));
				resp_balances_result = cJSON_GetObjectItem(from_server ,  "resultDesc");
				printf("resp_balances_result :  %s\n" ,  cJSON_Print(resp_balances_result));

				printf (resp_status);
				printf("\n");
				if(strcmp(resp_status,  "true") == 0)
				{
					printf("True\n");
					*response_balances = (cJSON *) NULL;
					*response_balances = resp_balances_result;
					return 1;
				}
				else
				{
					*response_balances =( cJSON * )NULL;
					//errmessage =  cJSON_Print(cJSON_GetObjectItem(resp_balances,"message"));
					if(display_messages)
					{
						message_display_function(1,"","Transactions Error","Error posting transactions.", (char *) NULL);
						kb_getkey();
					}
					return 0;
				}

			}
			else{
				*response_balances =( cJSON * )NULL;
				//errmessage =  cJSON_Print(cJSON_GetObjectItem(resp_balances,"message"));
				if(display_messages)
				{
					message_display_function(1,"","Transactions Error","Error posting transactions.", (char *) NULL);
					kb_getkey();
				}
				return 0;
			}
		}
		else if(strcmp(transaction_source , "pos")== 0)
		{

			*response_balances = from_server;
			return 1;
		}
	}
	*response_balances =( cJSON * )NULL;
	printf("\n+++++++++++++++++++++++End response+++++++++++++++++++++++++\n");
	//Response to  say no network or network connection error
	return 2;
}




/*
 * Fetches data and verifies finger print from a personalized card.
 * @param cJSON ** transaction_file  - Passes back  the transaction value by  pointer
 * @param cJSON ** beneficiary_details -  Passes back  the card number by pointer
 * @return int
 *   0 -  Verification failed
 *   1 -  Verification success
 *
 */
int verify_card_and_read_data(  cJSON ** transaction_file , cJSON ** beneficiary_details){
	int    returned;
	unsigned char  *fingerprint[800];

	char * p_details , * tx_file;


	char  * first_card_number;
	int w = 0;
	while(w<400)
	{
		fp_response[w] = 0;
		w++;
	}

	returned  = cardoperations(1,"", &p_details,  &tx_file ,fingerprint , "", "","" ,  &first_card_number);

	printf("We actually returned\n");
	if(returned == 1){

		char * temp =  malloc(strlen(p_details)+2);
		strcpy(temp ,p_details );
		if(fplib_test(1 ))
		{
			//*cardno = "12335663232";
			//printf("p_details : %s \n" , p_details);
			*beneficiary_details=  cJSON_Parse(temp);
			*transaction_file   = cJSON_Parse(tx_file);
			printf(cJSON_Print(*beneficiary_details));
			return 1;
		}
	}
	return 0;
}


int process_json_response(cJSON *  json_to_be_processed ,  int json_type){

	/*
	{
	    "status": true,
	    "responseCode": "100",
	    "cardStatus": 1,
	    "responseMessage": "Success",
	    "balances": [
	        {
	            "wallet": "WFP",
	            "balance": 3500,
	            "wallet": "100",
	            "currency": "Ksh"
	        },
	        {
	            "wallet": "IOM",
	            "balance": 4000,
	            "wallet": "200",
	            "currency": "Ksh"
	        }
	    ]
	}*/


	return 0;
}

int post_pos_offline_transactions (void)
{
	/*
	BT-82247929-190222085436|111|76114338|123|M-PESA Account|50.0|5765.0|5715.0|SSP|82247929|20190222|101|b|0|0|0
	sprintf(new_transaction , "%s*%s*%s*%s*%s*%s*%s*%s*%.2f*%.2f%s*%s#%s" ,transID,pos_serial_number,transType , card_number,myMerchantUser->agentid, getCharacters,receieved_unformatedDate,wallet_id[selected] ,(wallet_amount[selected]+atof(getCharacters)),wallet_amount[selected] ,wallet_currency[selected],myLoginPosUser->username, existing_transaction);
	 */

	char  * sql = "select transId  || '*' || terminalId || '*' || transOperation || '*' || debiticcid || '*' || crediticcid || '*' || amount || '*' || date  || '*' || wallet || '*' ||  balanceBefore  || '*' ||   balanceAfter || '*' || currency || '*' || userId from  transaction_table where posted = 0 and receiptPrinted = 1 and cardUpdated =1";

	char  * new_transaction = 0;
	cJSON * response;
	int i;
	sqlite_database_read_write_operation(sql  ,  "database.db");
	if(sql_data_count)
	{
		printf("Sql_data_count : %d\n" , sql_data_count);
		for (i = 0 ; i <sql_data_count ; i++)
		{
			if(i == 0)
			{
				new_transaction = malloc(strlen(sql_data[i]) + 1);
				strcpy(new_transaction , sql_data[i]);

			}
			else
			{
				new_transaction = realloc(new_transaction,strlen(new_transaction) + strlen(sql_data[i])+1);
				strcat(new_transaction ,"#");
				strcat(new_transaction , sql_data[i]);
			}
		}
		printf("Obtained :  %s\n" , new_transaction);
		//strcat(new_transaction ,  "\0");
		strcat(new_transaction ,"#");
		printf("Final :  %s\n" , new_transaction);
		if(post_transaction_file("pos" ,new_transaction  , &response  , 0))
		{
			printf("Response :  %s\n" , cJSON_Print(response) );

			//Process the response -  fetch transaction Id and delete it from the local database
			//We need now to  update the database with what Klemo  accepted
			//So  from the Json feedback  below , fetch the originalTransId and delete it from db DB: database.db table transaction_table

			/*			{
			  "benTxnResult": [
			    {
			      "originalTransId": "2002",
			      "status": true,
			      "code": "0",
			      "message": "success/benTxn success",
			      "resultDesc": ""
			    },
			    {
			      "originalTransId": "2001",
			      "status": true,
			      "code": "0",
			      "message": "success/benTxn success",
			      "resultDesc": ""
			    }
			  ]
			}*/
			cJSON *  response_array = cJSON_GetObjectItem(response , "benTxnResult") ;
			int response_array_size = cJSON_GetArraySize(response_array);
			int  i;
			char *  sql_str ;
			int found_transaction = 0;
			if(response_array_size)
			{
				//Innitialize prepared statement
				sql_str =malloc(80);
				strcpy(sql_str, "BEGIN TRANSACTION ; delete from transaction_table where transId IN  (");

			}
			for (i= 0;  i <response_array_size ; i++){
				cJSON * array_item  = cJSON_GetArrayItem(response_array , i);
				int save  = 0;
				if(strcmp(cJSON_Print(cJSON_GetObjectItem(array_item  , "status")) , "true") == 0)
				{
					save = 1;
				}
				else
				{
					if(strcmp(get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(array_item  , "resultDesc"))) , "Duplicate Message Rejected") == 0)
					{
						save = 1;

					}
					else
					{
						//need to  be checked by  admin for reason of failure.

					}
				}

				if(save)
				{


					char str[100];
					char *  txId = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(array_item  , "originalTransId"))) ;
					sql_str = realloc(sql_str , strlen(sql_str)+strlen(txId)+ 5);

					if(found_transaction){
						sprintf(str ,", '%s' " , txId);
						strcat(sql_str ,str );
					}
					else{
						sprintf(str ," '%s' " , txId);
						strcat(sql_str ,str );

					}
					printf("Tx : %s\n" , str);

					found_transaction++;

					save = 0;
				}
			}
			printf("Sql  String :  %s \n" ,  sql_str);
			if(found_transaction)
			{
				sql_str = realloc(sql_str , strlen(sql_str)+30);

				strcat(sql_str,"); END TRANSACTION;");

				sqlite_database_read_write_operation(sql_str,"database.db");
				printf("Finished\n");

			}
			free(sql_str);

		}


	}

	return 0 ;
}
