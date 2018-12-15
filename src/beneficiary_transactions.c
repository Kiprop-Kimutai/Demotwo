/*
 * beneficiary_transactions.c
 *
 *  Created on: Dec 5, 2018
 *      Author: linux
 */

#include <stdlib.h>
#include "common_includes.h"

#include "../src/utilities/lcd.h"
#include "utilities/keyboart.h"
#include "utilities/cJSON.h"
#include "fingerprint_reader.h"
#include "../src1/desfire_test.h"
#include "print_receipt.h"
//#include "utilities/sql_functions.h"
//#include "database.h"
#include "beneficiary_transactions.h"

#include "utilities/common_functions.h"
#include "utilities/sql_functions.h"
#include "mutwol.h"
/*
 * Function to  register beneficiary
 */
void register_beneficiary(void);

/*
 * Does transaction on the card
 */
void do_beneficiary_transaction( void);

void beneficiary_transactions(void )
{
	int selected = 0;
	const char  beneficiary_menu[][100] = {"Beneficiary Transaction","Beneficiary Registration","Edit/Update Beneficiary  Details", "Balance Inquiry/Update","Mini-Statement" , "Exit"};
	while(1)
	{
		switch(lcd_menu("Beneficiary Transactions", beneficiary_menu, 5 ,selected))
		{
		case 0:
			do_beneficiary_transaction();
			break;
		case 1:
			register_beneficiary();
			break;
		case 2:
			//printf("Balance Inquiry/Update\n");
			break;
		case 3:
			printf("Balance Inquiry/Update\n");
			break;
		case 4:
			printf("Mini-Statement");
			break;
		case 5 :
			return;
			break;
		}
	}
}

//int argc, char *argv[]


void register_beneficiary(void){
	int ret;
	int change_made =0;
	int x= 0;

	cJSON * txToPosted=cJSON_CreateObject();
	//cJSON * Biodata = NULL;
	char getCharacters[40];
	char getCharacters1[40];
	char name1[30];
	char name[100];
	const  char details[10][100];
	BYTE * imageBuffer2[2];
	BYTE  imageBuffer1 []={0x80, 0xE0, 0x00, 0x00, 0x11, 0x85, 0x0F, 0x17, 0x70, 0x10, 0xF1,
			0x01, 0x00, 0x36, 0x33, 0x33, 0x01, 0x04, 0x00, 0x00, 0x00, 0xFA, 0x00};
	/*
	const char gender[][100] = {
			"Male",
			"Female"
	};
	 */

	//BYTE fp23;
	int selected = 0;
	int cur_row = 0;

	char  ** data;
	char** tokens = 0;
	int  i;
	char  options[100];
	//Get  KYC from Table

	sqlite_database_read_write_operation(read_kycs_sql , "database.db");
	//data = get_collumn_data(sql_data,sql_data_count,sql_collumn_count,0);
	/*	for(i=1; i<sql_row_count;i++)
		strcpy( details[i-1],data[i]);*/
	//selected= lcd_menu("Test", details,sql_row_count-1 , selected);

	//Read card // replace with Kims Functions


	strcpy(name1, "");



	//Get  Beneficiary details
	lcd_clean();

	for( x=0; x<sql_row_count-1;x++)
	{

		int w = 0;
		cur_row = sql_collumn_count + cur_row;
		change_made =0;
		selected  = 0;
		printf("Here : %d  : %s \n",(cur_row +1) , sql_data[cur_row+1]);

		//Get Gender
		if(strcmp(sql_data[cur_row +1], "OPT") == 0)
		{
			printf("Options goes here %d\n", selected);
			/*	kb_getkey();*/

			sprintf( name , "Please select %s" ,sql_data [cur_row ]);
			printf("%s", name);
			strcpy(options  , sql_data[cur_row+3]);
			printf("String : %s\n" ,sql_data[cur_row+3] );
			printf("String : %s\n" ,options );
			if(strlen(options)>0)
				tokens = str_split(options, ',');

			if (tokens)
			{

				for (w = 0; *(tokens + w); w++)
				{
					//printf("month=[%s]\n", *(tokens + w));
					//if(strlen(*(tokens + w))>0)
					strcpy(details[w], *(tokens + w));
					free(*(tokens + w));
				}
				printf("Ended\n");
				free(tokens);
				printf("Ended 1\n");
			}

			/*		for(i=1; i<sql_row_count;i++)
					strcpy( details[i-1],data[i]);*/

			while (selected>=0){



				printf("Selected on %d\n", selected);
				selected= lcd_menu(name, details, w, selected);

				printf("Selected on %d\n", selected);
				if(selected > -1 && selected < w )
				{
					//strcpy(myBenf.gender, "Male" );
					cJSON_AddStringToObject(txToPosted,sql_data[cur_row+4],details[selected]);
					break;
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

		//Get Document Type
		else if(strcmp(sql_data[cur_row+1], "STR")==0 || strcmp(sql_data[cur_row+1], "INT")==0  )
		{
			sprintf( name , "Please enter  %s" ,sql_data [cur_row ]);
			if(strcmp(sql_data[cur_row+1], "STR")==0)
			{
				if(strcmp(sql_data[cur_row+2]  , "1") == 0)
					ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  1, 16, getCharacters,getCharacters1, NULL, name, "","Register Beneficiary", 0);
				else
					ret = kb_getStringtwo(ALPHA_IN ,ALPHA_IN ,  0, 16, getCharacters,getCharacters1, NULL, name, "","Register Beneficiary", 0);

			}
			else
			{
				if(strcmp(sql_data[cur_row+2]  , "1") == 0)
					ret = kb_getStringtwo(NUM_IN ,NUM_IN ,  1,16, getCharacters,getCharacters1, NULL, name, "","Register Beneficiary", 0);
				else
					ret = kb_getStringtwo(NUM_IN ,NUM_IN ,  0, 16, getCharacters,getCharacters1, NULL, name, "","Register Beneficiary", 0);

			}
			printf("Ret on %d", ret);
			if( strlen(getCharacters)>0 && ret!=-1)
			{
				change_made =1;
				cJSON_AddStringToObject(txToPosted,sql_data[cur_row+4],getCharacters);

			}

			else if(ret==-1  )
			{

				//printf("Ret on ")
				return;
				//&& strlen(getCharacters)>0
			}

		}
	}
	printf("================================================================\n" );
	printf("Data to  be written\n" );
	printf(">>>>>>>JSON %s\n", cJSON_Print(txToPosted));
	//Get  Beneficiary fingerprint
	printf("Sample finger print\n" );
	for(x = 0 ; x<10 ; x++)
		printf("%02x",imageBuffer1[x]);
	personalizecard(cJSON_Print(txToPosted),imageBuffer1);

	kb_getkey();

	printf("================================================================\n" );
	printf("Data Read\n" );
	char * read_data[2];
	BYTE * fingerprint[2];
	cardoperations(1,"",  read_data ,fingerprint);

	printf("Read sample fingerprint\n" );
	for(x = 0 ; x<10 ; x++)
		printf("%02x",imageBuffer2[0][x]);

	printf(">>>>>>>Read P_details %s\n", *(read_data+0));
	printf("End Read\n" );
	printf("================================================================\n" );

/*	if(fplib_test(0 , NULL  , txToPosted ))
	{*/
		printf("You know what here it is\n " );
		printf(">>>>>>>JSON %s\n", cJSON_Print(txToPosted));

		int w=0;
		while(w<400)
		{
			/*			printf( "%02X",myBenf.fingerprint[w]);
			strcpy(m[w],"%02X",imageBuffer1[w]);*/
			w++;
		}

		if(createApplicationAndFileAndWriteData(cJSON_Print(txToPosted),"myBenf.fingerprint"))
		{

			//Validate Beneficiary online
			message_display_function(1,"","Registration Successful", "Please note that  your card has been successfully created. Thank you and welcome again", (char *)NULL);
			kb_getkey();
			//Write Beneficiary to  card card
			//end
		}
		else
		{
			message_display_function(1,"","EMV card Error ", "Error reading card, please check your card reader and place your at POS proximity  and try  again", (char *)NULL);
			kb_getkey();
		}
/*
	}
	else
	{
		return;
	}
*/



}



void do_beneficiary_transaction( void){
	int key, ret ,  selected;
	char getCharacters[30], getCharacters1[30];

	//Transation JSON definations
	cJSON * txToPosted;
	cJSON * final_transaction ;
	cJSON * json_beneficary_details ;
	cJSON *  json_array;
	cJSON *  json_beneficiary_data;
	cJSON * balance_information;
	cJSON * balance_array;
	cJSON * transaction_json;
	BYTE *  stored_fingerprints[3];
	char * card_number;




	char * txnDate, *transactionID ,  transID[100];
	char * existing_transaction;
	char sql_transaction[800];
	char new_transaction[800];
	/*	message_display_function(1, "","Beneficiary Registration" ,  "Please tap the NFC card and press enter key");
	key=kb_getkey();
	if(key==DIKS_ENTER){*/
	//function to  get  the card details from the card
	//char

	// To  be removed
	char  * transaction_file = "{\"balances\":[{\"wallet\":\"WFP\",\"walletId\":\"100\",\"walletbalance\":3000},{\"wallet\":\"IOM\",\"walletId\":\"200\",\"walletbalance\":1000}],\"transactions\":{\"benTxn\":\"BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101\"}}";

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



	//read card for data
	if(1){
		//Convert  strings to  Jsons and define Jsons

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

		if(fplib_test(1 , stored_fingerprints ,  json_beneficiary_data))
		{
			//if fingerprint verification successful
			//Start
			char wallet_name[10][50];
			char  wallet_id[10][50];
			double  wallet_amount[10];
			cJSON *  new_wallet = cJSON_CreateArray();
			cJSON * new_transaction_json = cJSON_CreateObject();
			cJSON * new_txs = cJSON_CreateObject();
			int i= 0 ,  w=0;

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
				strcpy( wallet_id[i], get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "walletId"))));
				wallet_amount[i] = atof(cJSON_Print(cJSON_GetObjectItem(balance_array , "walletbalance")));


				printf("Got in %.2f\n" , wallet_amount[i]);
				//i = i + collums_in_rs;
				//x++;
			}
			//printf("%s\n" , cJSON_Print(balance_information));
			//forcJSON_GetArraySize(balance_information)
			while (selected>=0){
				selected= lcd_menu("Please select the program", wallet_name, i, selected);
				printf("Selected on %d\n", selected);
				if(selected > -1 && selected < i )
				{
					ret = kb_getStringtwo(NUM_IN ,NUM_IN ,  1, 16, getCharacters,getCharacters1, NULL, "Please enter the transaction amount", "","Beneficiary Transaction", 0);
					if( strlen(getCharacters)>0 && ret!=-1)
					{
						if(wallet_amount[selected]>atof(getCharacters)){
							int printflag ,print_complete;
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
							txToPosted=cJSON_CreateObject();
							card_number = get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(json_beneficiary_data , "iccid")));

							cJSON_AddItemToArray(json_array , txToPosted);
							cJSON_AddItemToObject(final_transaction , "benTxn" , json_array);
							get_date_and_receipt (0,  &txnDate , &transactionID);
							strcpy(transID ,transactionID);
							cJSON_AddStringToObject(txToPosted,"amount",getCharacters);
							cJSON_AddStringToObject(txToPosted,"transOperation","111");
							cJSON_AddStringToObject(txToPosted,"debiticcid",card_number);
							cJSON_AddStringToObject(txToPosted,"crediticcid","Merchant_ID");
							cJSON_AddStringToObject(txToPosted,"walletId",wallet_id[selected]);
							cJSON_AddStringToObject(txToPosted,"walletName",wallet_name[selected]);
							cJSON_AddNumberToObject(txToPosted,"walletBalance",(wallet_amount[selected]-atof(getCharacters)));
							wallet_amount[selected] =wallet_amount[selected]-atof(getCharacters);
							cJSON_AddStringToObject(txToPosted,"transId",transID);
							cJSON_AddStringToObject(txToPosted,"terminalId",pos_serial_number);
							cJSON_AddStringToObject(txToPosted,"date",txnDate);
							cJSON_AddStringToObject(txToPosted,"authMode","101");
							printf("The Json :  %s\n", cJSON_Print(final_transaction));
							printf("sql_transaction : %s\n" , transID);
							sprintf(sql_transaction , "Insert  into  transaction_table "
									"(transId ,transOperation ,debiticcid,crediticcid , walletId ,amount , terminalId ,  date , authMode ) "
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
								cJSON_AddStringToObject(new_wallet_item, "walletId",wallet_id[w]);
								cJSON_AddNumberToObject(new_wallet_item, "walletbalance",wallet_amount[w]);
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
									print_receipt("TRANSACTION RECEIPT" , txToPosted ,  json_beneficiary_data , &printflag ,   &print_complete) ;
								}
								//}
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

	//register_ben();
	/*	}
	else
		return;*/
}




void update_beneficiary_balances( void){
	int key, ret ,  selected;
	char getCharacters[30], getCharacters1[30];

	BYTE  * fingerprint[3];
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




	char * txnDate, *transactionID ,  transID[100];
	char * existing_transaction;
	char sql_transaction[800];
	char new_transaction[800];
	/*	message_display_function(1, "","Beneficiary Registration" ,  "Please tap the NFC card and press enter key");
	key=kb_getkey();
	if(key==DIKS_ENTER){*/
	//function to  get  the card details from the card
	//char
	char  * transaction_file = "{\"balances\":[{\"wallet\":\"WFP\",\"walletId\":\"100\",\"walletbalance\":3000},{\"wallet\":\"IOM\",\"walletId\":\"200\",\"walletbalance\":1000}],\"transactions\":{\"benTxn\":\"BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101#BT-122865212*82671220*111*098765789876*72355289383*6000*123432567*101\"}}";

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

	if(fplib_test(1 ,fingerprint , cJSON_GetObjectItem(json_beneficary_details , "BioData")))
	{
		//if fingerprint verification successful

		//Start
		char wallet_name[10][50];
		char  wallet_id[10][50];
		double  wallet_amount[10];
		cJSON *  new_wallet = cJSON_CreateArray();
		cJSON * new_transaction_json = cJSON_CreateObject();
		cJSON * new_txs = cJSON_CreateObject();
		int i= 0 ,  w=0;

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
			strcpy( wallet_id[i], get_string_from_jason_object(cJSON_Print(cJSON_GetObjectItem(balance_array , "walletId"))));
			wallet_amount[i] = atof(cJSON_Print(cJSON_GetObjectItem(balance_array , "walletbalance")));


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
		int printflag ,print_complete;
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
		get_date_and_receipt (0,  &txnDate , &transactionID);
		strcpy(transID ,transactionID);
		cJSON_AddStringToObject(txToPosted,"amount",getCharacters);
		cJSON_AddStringToObject(txToPosted,"transOperation","111");
		cJSON_AddStringToObject(txToPosted,"debiticcid",card_number);
		cJSON_AddStringToObject(txToPosted,"crediticcid","Merchant_ID");
		cJSON_AddStringToObject(txToPosted,"walletId",wallet_id[selected]);
		cJSON_AddStringToObject(txToPosted,"walletName",wallet_name[selected]);
		cJSON_AddNumberToObject(txToPosted,"walletBalance",(wallet_amount[selected]-atof(getCharacters)));
		wallet_amount[selected] =wallet_amount[selected]-atof(getCharacters);
		cJSON_AddStringToObject(txToPosted,"transId",transID);
		cJSON_AddStringToObject(txToPosted,"terminalId",pos_serial_number);
		cJSON_AddStringToObject(txToPosted,"date",txnDate);
		cJSON_AddStringToObject(txToPosted,"authMode","101");
		printf("The Json :  %s\n", cJSON_Print(final_transaction));
		printf("sql_transaction : %s\n" , transID);
		sprintf(sql_transaction , "Insert  into  transaction_table "
				"(transId ,transOperation ,debiticcid,crediticcid , walletId ,amount , terminalId ,  date , authMode ) "
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
			cJSON_AddStringToObject(new_wallet_item, "walletId",wallet_id[w]);
			cJSON_AddNumberToObject(new_wallet_item, "walletbalance",wallet_amount[w]);
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
				print_receipt("TRANSACTION RECEIPT" , txToPosted ,  json_beneficiary_data , &printflag ,   &print_complete) ;
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
