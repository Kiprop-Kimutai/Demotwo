/*
 *
 * merchant_transactions.c
 *
 *  Created on: Dec 5, 2018
 *      Author: linux
 */


#include <cJSON.h>
#include <stdlib.h>
#include "utilities/send_online_request.h"
#include "utilities/keyboart.h"
#include "utilities/common_functions.h"
#include "usersManagement.h"
#include "device_management.h"
#include "global_params.h"
#include "utilities/lcd.h"
#include "print_receipt.h"
#include "main.h"
#include "common_includes.h"
#include "merchant_transactions.h"

void merchant_transactions(void )
{
	int selected = 0;
	const char  merchant_menu[][100] = {"Agent Balance Inquiry", "Mini Statement", "Roll Up Transfer"};
	while(1)
	{
		switch(lcd_menu("Merchant Transactions", merchant_menu, sizeof(merchant_menu)/100 ,selected))
		{
		case 0:

			balance_inquiry();

			break;
		case 1:

			mini_statement();
			break;
		case 2:
			roll_up_transfer();
			break;
		case -1:
			return;
			break;
		}
	}

}


int merchant_inquiry(){

	int selected = 0;
	const char  merchant_enquiry[][100] = {"Balance Inquiry", "Mini Statement", "Exit"};
	while(1)
	{
		switch(lcd_menu("Merchant Inquiry", merchant_enquiry, 3 ,selected))
		{
		case 0:

			balance_inquiry();

			break;
		case 1:

			mini_statement();
			break;

		case 2 :
			return 0;
			break;
		}
	}



}

int mini_statement(){
	int  ret;
	cJSON  * response ;
	char  * date;
	int y = 0;
	char pin[20];
	char * Details;
	int selected = 0;
	char * Amount ;
	char * Currency;
	char * TransactionStatus;
	char * ReceiptNumber;
	int i;
	char holder[30];
	char  main_menu[][100] = {"Organization Account","WFP Organization Account"};
	char walletName[50];
	int print_complete , print_flag;
	char * returned_request_id,req_id[30];
	get_request_id(1, &returned_request_id);

	strcpy(req_id, returned_request_id);

	printf("req_id : %s", req_id);

	ret = kb_getStringtwo(PASS_IN ,NUM_IN ,  0, 20, pin,holder, NULL, "Enter PIN", "","Mini Statement Inquiry", 0);

	if(ret == -1)
		return 0 ;

	if(strcmp(pin, myLoginPosUser->pin)==0){
		switch(lcd_menu("Select Account", main_menu, 2,selected))
		{
		case 0:

			strcpy(walletName , main_menu[0]);

			break;
		case 1:
			strcpy(walletName , main_menu[1]);
			break;


		}
		char  txnDate[50],*transactionDate ,  *transactionID ,  transID[100];
		get_date_and_receipt (1,  &transactionDate ,&date,  &transactionID);
		strcpy(txnDate ,transactionDate);
		strcpy(transID ,transactionID);
		cJSON * mini_statement= cJSON_CreateObject();
		cJSON * mini_statement_obj= cJSON_CreateObject();
		cJSON_AddStringToObject(mini_statement ,"storeAccount" ,myMerchantUser->agentid);
		cJSON_AddStringToObject(mini_statement ,"transId" ,transID);
		cJSON_AddStringToObject(mini_statement ,"requestId" ,req_id);
		cJSON_AddStringToObject(mini_statement ,"date" ,date );
		cJSON_AddStringToObject(mini_statement ,"terminalId" ,pos_serial_number);
		cJSON_AddStringToObject(mini_statement ,"authMode" ,my_authmodes->PIN );
		cJSON_AddStringToObject(mini_statement ,"walletName" ,walletName );
		cJSON_AddStringToObject(mini_statement ,"currency" ,"SSP" );
		cJSON_AddStringToObject(mini_statement ,"userId" ,myLoginPosUser->username);


		cJSON_AddItemToObject(mini_statement_obj, "storeStmtRequest" , mini_statement);
		if(send_gprs_request("storeStmtRequest", mini_statement_obj ,my_endpoint->store_statement_request ,&response ,  1)){

			char  * status   = cJSON_Print(cJSON_GetObjectItem(response,"status"));
			printf("Status : %s\n" , status);

			if(status != (char*) NULL){

				if(strcmp(status , "true")==0){
					cJSON * walletsresp  = cJSON_GetObjectItem(response , "resultDesc");


					if(cJSON_GetArraySize(walletsresp) > 0){
						printf("\n Balance Response JSONArray:  %s\n" , cJSON_Print(walletsresp));
						/*defined_x = 100;
						message_display_function(1,"", "MiniStatement  Details"," ", (char *)NULL);
						 */
						//showministatement("Mini Statement", walletsresp,sizeof(cJSON_GetArraySize(walletsresp)) / 100, selected );
						/*
						for(i =0; i<cJSON_GetArraySize(walletsresp); i++){



							cJSON * ministatement_array = cJSON_GetArrayItem(walletsresp,i);
						    Amount = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(ministatement_array , "Amount")));
							ReceiptNumber = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(ministatement_array , "ReceiptNumber")));
							TransactionStatus = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(ministatement_array , "TransactionStatus")));
							Currency = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(ministatement_array , "Currency")));
							Details = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(ministatement_array , "Details")));

							showministatement("Mini Statement", walletsresp,sizeof(cJSON_GetArraySize(walletsresp)) / 100, selected );


								y = get_y_position();
										lcd_printf(ALG_LEFT ,"%s", "ReceiptNumber");
										lcd_printf_ex(ALG_LEFT_DEF ,y ,": %s",ReceiptNumber);
										y = get_y_position();
										lcd_printf(ALG_LEFT ,"%s", "Details");
										lcd_printf_ex(ALG_LEFT_DEF ,y ,": %s",Details);

							//	message_display_function(1,"","Balance",cJSON_Print(cJSON_GetObjectItem(balance_array , "balance")), (char *) NULL);
						}*/

						//kb_getkey();

					}




					print_receipt("Mini statement", walletsresp,&print_flag, &print_complete);





				}else if(strcmp(status, "false")==0){


					printf("\n Mini Statement Response JSONArray:  %s\n" , cJSON_Print(response));

					errmessage =  cJSON_Print(cJSON_GetObjectItem(response,"resultDesc"));


					message_display_function(1,"","Mini Statement  Error",errmessage, (char *) NULL);
					kb_getkey();



				}
			}

		}

		return 0;
	}else{
		message_display_function(1,"","Error","Wrong pin", (char *) NULL);
		kb_getkey();
		return 0;
	}
}

int balance_inquiry(){
	char * date;
	int y = 0;
	int ret , i ;

	char *transactionDate ,  *transactionID ;
	cJSON  * response ;
	char pin[20];
	char * card_wallet_id;
	char * currency;
	char holder[30];
	char * returned_request_id,req_id[20];
	get_request_id(1, &returned_request_id);
	strcpy(req_id, returned_request_id);
	char  txnDate[50], transID[100];
	int print_flag, print_response;
	cJSON *  confirm_tx_json  =  cJSON_CreateArray();
	cJSON * merchant_inqry= cJSON_CreateObject();
	cJSON * merchant_inqry_obj= cJSON_CreateObject();
	ret = kb_getStringtwo(PASS_IN ,NUM_IN ,  0, 20, pin,holder, NULL, "Enter PIN", "","Balance Inquiry", 0);

	if(ret == -1)
		return 0 ;

	if(strcmp(pin, myLoginPosUser->pin)==0){
		get_date_and_receipt (1,  &transactionDate ,&date,  &transactionID);
		strcpy(txnDate ,transactionDate);
		strcpy(transID ,transactionID);

		cJSON_AddStringToObject(merchant_inqry ,"agentAccountNo" ,myMerchantUser->agentid );
		cJSON_AddStringToObject(merchant_inqry ,"transId" ,transID );
		cJSON_AddStringToObject(merchant_inqry ,"terminalId" ,pos_serial_number);
		cJSON_AddStringToObject(merchant_inqry ,"authMode" ,my_authmodes->PIN );
		cJSON_AddStringToObject(merchant_inqry ,"userId" ,myLoginPosUser->username);
		cJSON_AddStringToObject(merchant_inqry ,"date" ,date );
		cJSON_AddStringToObject(merchant_inqry ,"requestId" ,req_id);
		cJSON_AddItemToObject(merchant_inqry_obj, "storeBalRequest" , merchant_inqry);


		if(send_gprs_request("storeBalRequest", merchant_inqry_obj ,my_endpoint->store_get_balance ,&response ,  1)){

			printf("Response : \n");
			//if(response!=(cJSON *)NULL){
			char  * status   = cJSON_Print(cJSON_GetObjectItem(response,"status"));


			if(status != (char*) NULL){

				printf("Status : %s\n" , status);
				if(strcmp(status, "true")==0){
					cJSON * walletsresp  = cJSON_GetObjectItem(response , "resultDesc" );

					if(cJSON_GetArraySize(walletsresp) > 0){
						printf("\n Balance Response JSONArray:  %s\n" , cJSON_Print(walletsresp));
						defined_x = 100;
						message_display_function(1,"", "Balance Details"," ", (char *)NULL);

						for(i =0; i<cJSON_GetArraySize(walletsresp); i++){


							double balance;
							cJSON * balance_array = cJSON_GetArrayItem(walletsresp,i);
							balance = atof(cJSON_Print(cJSON_GetObjectItem(balance_array , "balance")));
							card_wallet_id = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "wallet")));
							currency = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "currency")));
							char  balance1[50];
							sprintf(balance1,"%.2f" ,balance);


							int  u ;
							for (u  = 0  ;  u  < 2 ;  u ++)
							{
								cJSON * array_item  =  cJSON_CreateObject();

								if(u == 0)
								{
									cJSON_AddStringToObject(array_item ,  "key" , "Account" );
									cJSON_AddStringToObject(array_item ,  "value" , card_wallet_id);
								}
								if ( u ==1)
								{
									cJSON_AddStringToObject(array_item ,  "key" , "Amount" );
									cJSON_AddStringToObject(array_item ,  "value" , balance1 );
								}
								cJSON_AddItemToArray( confirm_tx_json , array_item);
							}
							/*
							y = get_y_position();
							lcd_printf(ALG_LEFT ,"%s", "Account");
							lcd_printf_ex(ALG_LEFT_DEF ,y ,": %s",card_wallet_id);
							y = get_y_position();
							lcd_printf(ALG_LEFT ,"%s", "Balance");
							lcd_printf_ex(ALG_LEFT_DEF ,y ,": %s",balance1);
							lcd_flip();
							//	message_display_function(1,"","Balance",cJSON_Print(cJSON_GetObjectItem(balance_array , "balance")), (char *) NULL);
						}

						kb_getkey();

					}*/}
							if(confirm_screen("Store Balance" , confirm_tx_json))
							{

								print_receipt("Balance", walletsresp,&print_flag, &print_response);

							}


					}

				}else if(strcmp(status , "false")==0){


					printf("\n Balance Response JSONArray:  %s\n" , cJSON_Print(response));

					errmessage =  cJSON_Print(cJSON_GetObjectItem(response,"resultDesc"));


					message_display_function(1,"","Balance Error",errmessage, (char *) NULL);
					kb_getkey();



				}else{

					message_display_function(1,"","Balance Error","Invalid response", (char *) NULL);

				}
			}

		}

	}else{
		message_display_function(1,"","Error","Wrong pin", (char *) NULL);
		kb_getkey();
		return 0;
	}

}

int store_to_store(){

	printf("Store to  Store transfer \n");
	return 0;
}
int roll_up_transfer(){
	//**************************************
	int  ret, selected;
	char  main_menu[][100] = {"Default Account","WFP_Account"};
	int y = 0 , key;

	char  txnDate[50],*transactionDate ,  *transactionID ,  transID[100];
	int print_complete , print_flag;
	cJSON  * response ;
	char pin[30];
	char * date;
	char holder1[20];
	cJSON * roll_up_request= cJSON_CreateObject();
	cJSON * roll_up_request_obj= cJSON_CreateObject();
	char ammount[30];
	char crdacc[20];
	ret = kb_getStringtwo(PASS_IN ,NUM_IN ,  0, 20, pin,holder1, NULL, "Enter PIN", "","Roll Up Request", 0);
	if(ret == -1)
		return 0 ;
	if(strcmp(pin, myLoginPosUser->pin)==0){
		char holder[30];
		char crdacc[20];
		char transoperation[10];
		char * returned_request_id,req_id[10];
		get_request_id(1, &returned_request_id);
		strcpy(req_id, returned_request_id);

		switch(lcd_menu("Select Account", main_menu, 2,selected))
		{
		case 0:

			strcpy(transoperation , merchant_operations->ORGANIZATION_ROLL_UP);

			break;
		case 1:
			strcpy(transoperation , merchant_operations->WFP_ROLL_UP);
			break;

		}
		ret = kb_getStringtwo(NUM_IN ,NUM_IN ,  1, 20, crdacc,holder, NULL, "Merchant Account ID", "","Roll up Transfer", 0);

		ret = kb_getStringtwo(NUM_IN ,NUM_IN ,  1, 20, ammount,holder, NULL, "Amount", "","Roll up Transfer", 0);
		get_date_and_receipt (1,  &transactionDate ,&date,  &transactionID);
		strcpy(txnDate ,transactionDate);
		strcpy(transID ,transactionID);
		cJSON_AddStringToObject(roll_up_request ,"transOperation" ,transoperation);
		cJSON_AddNumberToObject(roll_up_request ,"amount" ,atoi(ammount) );
		cJSON_AddStringToObject(roll_up_request ,"debitAccountNo" ,myMerchantUser->agentid);
		cJSON_AddStringToObject(roll_up_request ,"creditAccountNo" ,crdacc);
		cJSON_AddStringToObject(roll_up_request ,"transId" ,transID );
		cJSON_AddStringToObject(roll_up_request ,"terminalId" ,pos_serial_number);
		cJSON_AddStringToObject(roll_up_request ,"date" ,date);
		cJSON_AddStringToObject(roll_up_request ,"authMode" ,my_authmodes->PIN );
		cJSON_AddStringToObject(roll_up_request ,"userId" ,myLoginPosUser->username);
		cJSON_AddStringToObject(roll_up_request ,"requestId" ,req_id);

		cJSON_AddItemToObject(roll_up_request_obj, "storeTxn" , roll_up_request);
		//printf("\n Roll up request jSON :  %s\n" , cJSON_Print(roll_up_request_obj));

		/*		defined_x = 100;
		message_display_function(1,"", "Transaction Details"," ", (char *)NULL);
		y = get_y_position();
		lcd_printf(ALG_LEFT ,"%s", "Account");
		lcd_printf_ex(ALG_LEFT_DEF ,y ,": %s",crdacc);
		y = get_y_position();
		lcd_printf(ALG_LEFT ,"%s", "Amount");
		lcd_printf_ex(ALG_LEFT_DEF ,y ,": %s",ammount);


		lcd_printf(ALG_LEFT ,"");
		lcd_printf(ALG_CENTER ,"_____________________________________");
		y = get_y_position();
		defined_x = 150;

		lcd_set_font_color(COLOR_BLUE);
		lcd_printf(ALG_CENTER ,"%s" , "Cancel to exit      :     Enter to confirm");

		lcd_flip();
		lcd_set_font_color(COLOR_BLACK);

		key = kb_getkey();*/
		cJSON *  confirm_tx_json  =  cJSON_CreateArray();
		int  u ;
		for (u  = 0  ;  u  < 2 ;  u ++)
		{
			cJSON * array_item  =  cJSON_CreateObject();

			if(u == 0)
			{
				cJSON_AddStringToObject(array_item ,  "key" , "Account" );
				cJSON_AddStringToObject(array_item ,  "value" , crdacc);
			}
			if ( u ==1)
			{
				cJSON_AddStringToObject(array_item ,  "key" , "Amount" );
				cJSON_AddStringToObject(array_item ,  "value" , ammount );
			}
			cJSON_AddItemToArray( confirm_tx_json , array_item);
		}
		if(confirm_screen("Transaction Details" , confirm_tx_json)){



			if(send_gprs_request("storeTxn", roll_up_request_obj ,my_endpoint->store_transaction ,&response ,  1)){


				char  * status   = cJSON_Print(cJSON_GetObjectItem(response,"status"));
				printf("Status : %s\n" , status);
				if(status != (char*) NULL)
				{
					if(strcmp(status , "true")==0){
						cJSON * walletsresp  = cJSON_GetObjectItem(response , "resultDesc" );

						if(cJSON_GetArraySize(walletsresp) > 0){

							//showministatement();
							printf("\n Balance Response JSONArray:  %s\n" , cJSON_Print(walletsresp));
							print_receipt("Roll Up Transfer", walletsresp,&print_flag, &print_complete);

						}
						else
						{
							message_display_function(1,  "", "Roll Up Transaction" , "Transaction successful but invalid result received");
						}


					}else if(strcmp(status , "false")==0){


						printf("\n Roll Up  Response JSONArray:  %s\n" , cJSON_Print(response));

						errmessage =  cJSON_Print(cJSON_GetObjectItem(response,"message"));


						message_display_function(1,"","Roll Up Error",errmessage, (char *) NULL);
						kb_getkey();



					}
				}
			}
		}else if(key==DIKS_CANCEL)
			return 0;
	}else{
		message_display_function(1,"","Error","Wrong pin", (char *) NULL);
		kb_getkey();
		return 0;
	}
}
