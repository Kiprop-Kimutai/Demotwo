/*
 * database_functions.c
 *
 *  Created on: Dec 6, 2018
 *      Author: linux
 */

#include "../src/utilities/sql_functions.h"
#include "database.h"
#include <unistd.h>
/*
 * SQL that  creates KYC tables
 */
char * create_kyc_table =
		" CREATE TABLE KYCS("
				"name   	TEXT PRIMARY KEY	 ,"
				"DataType		TEXT ,"
				"Mandatory 	INT ,"
				"Options		TEXT, "
				"tag		TEXT "
				");";


/*char * operatortable;
char * netconf;*/
//Services





/*
{
	"benTxn":	[{
			"amount":	"105",
			"transOperation":	"111",
			"debiticcid":	"8cg4382017583718",
			"crediticcid":	"Merchant_ID",
			"walletId":	"100",
			"transId":	"BT-82461843-181212224826",
			"terminalId":	"82461843",
			"date":	"2018-12-12 22:48:26",
			"authMode":	"101"
		}]
}
*/


/*transID,
pos_serial_number,
card_number,
myMerchantUser->agentid,
getCharacters,transaction amount
txnDate,
wallet_id[selected] ,
(wallet_amount[selected]+atof(getCharacters)), Card balance Before
wallet_amount[selected] , card balance after
wallet_currency[selected], Currency
myLoginPosUser->username,*/

char * create_transactions_table =
		" CREATE TABLE transaction_table("
				"transId   	TEXT PRIMARY KEY	 ,"
				"transOperation		TEXT ,"
				"debiticcid 	TEXT ,"
				"crediticcid		TEXT, "
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
				"cardUpdated		INTEGER"
				");";
/*firstname
middlename
lastname
dob
gender
docType
docNumber
address
iccid
products
transId
terminalId
date
operator*/


//Services




char  * operatortable = "CREATE TABLE IF NOT EXISTS operator("
    "username  TEXT  PRIMARY KEY NOT NULL,"
	"name  TEXT,"
    "pin    TEXT,"
    "idnumber  TEXT ,"
	"active   TEXT,"
    "userlevel TEXT );";



char  * merchantsTable = "CREATE TABLE IF NOT EXISTS agentsTables("
    "merchantid  TEXT NOT NULL,"
	"agentid  TEXT NOT NULL,"
	"agentName  TEXT,"
    "merchantName  TEXT ,"
	"active   INTEGER);";

  //sqlite_database_create_table(operatortable,"operator");

 char  * netconf = "CREATE TABLE IF NOT EXISTS netconf("
   "ip    TEXT   PRIMARY KEY NOT NULL,"
   "port    TEXT ,"
   "protocol     TEXT ,"
   "timeout   TEXT ,"
   "apnname   TEXT ,"
   "password   TEXT );";
 //sqlite_database_create_table(netconf,"operator");

char * insert_default_kyc =
		"INSERT INTO KYCS (name , DataType , mandatory ,Options ,  tag) VALUES ('First Name'   , 'STR' ,1, '',  'firstname'); "
		"INSERT INTO KYCS (name , DataType , mandatory ,Options ,  tag) VALUES ('Last Name'    , 'STR' ,1,  '' ,  'lastname'); "
		"INSERT INTO KYCS (name , DataType , mandatory ,Options ,  tag) VALUES ('Middle Name'  , 'STR' ,0,  '' , 'middlename'); "
		"INSERT INTO KYCS (name , DataType , mandatory ,Options ,  tag) VALUES ('Date Of Birth', 'INT' ,1,  '', 'dob'); "
		"INSERT INTO KYCS (name , DataType , mandatory ,Options ,  tag) VALUES ('Document Type', 'OPT' ,1,  'National ID,Passport,Alien ID,NGO Card','docType'); "
		"INSERT INTO KYCS (name , DataType , mandatory ,Options ,  tag) VALUES ('Document Number' , 'STR' ,1,  '' , 'docNumber'); "
		"INSERT INTO KYCS (name , DataType , mandatory ,Options ,  tag) VALUES ('Address Information' , 'STR' ,0,  '' , 'address'); ";


/*
 * Creates all tables
 *
 */

void final_create_all_tables(void)
{
	//char  * data;
	int  i = 0;
	read_kycs_sql =  "select  * from KYCs";
	sqlite_database_create_table_function(create_kyc_table,"database.db");
	sqlite_database_read_write_operation(insert_default_kyc , "database.db" );
	sqlite_database_read_write_operation(create_transactions_table , "database.db" );
	sqlite_database_create_table_function(operatortable,"operator.db");
	sqlite_database_create_table_function(merchantsTable,"operator.db");
	sqlite_database_create_table_function(netconf,"operator.db");
	//printf
	//sqlite_database_read_write_operation(read_kycs_sql , "database.db" );
/*	printf("Data count = %d\n Collumn Count = %d\n Row Count = %d\n" , sql_data_count,sql_collumn_count, sql_row_count );
	for(i =0 ; i<sql_data_count; i++)
		printf("%d  : %s\n",  i, sql_data[i]);*/

}

char  **  get_collumn_data(char  ** data , int data_c ,  int column_c , int col_number)
{
	char  ** collumn = 0;
	int  i = 0;
	int cols = 0;
	int cur_col = col_number;
	printf("Count %d\n",  data_c);
	for (i  = 0 ; i<(data_c/column_c) ; i++)
	{
		printf("Start of %d C\n", i);
		//printf("End of 1 C\n");
		collumn = realloc(collumn,(cols + 1) * sizeof(char *));
		//printf("End of 2 C\n");
		collumn[i] = malloc(strlen(data[cur_col]) + 1);
		//printf("End of 3 C\n");
		strcpy(collumn[i], data[cur_col]);
		printf("End of %d C\n", i);
		cur_col = cur_col+column_c;
		cols++;
	}
	printf("End of C\n");
//	sleep(100);
	return collumn;
}

/*void read_kycs()
{

}*/
