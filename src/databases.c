/*
 * database_functions.c
 *
 *  Created on: Dec 6, 2018
 *      Author: linux
 */

#include "../src/utilities/sql_functions.h"
#include "database.h"
/*
 * SQL that  creates KYC tables
 */
char * create_kyc_table =
		" CREATE TABLE KYCS("
				"name   	PRIMARY KEY	TEXT ,"
				"DataType		TEXT ,"
				"Mandatory 	INT ,"
				"Options		TEXT "
				");";
char * insert_default_kyc = 		"INSERT INTO KYCS (name , DataType ,Mandatory , Options) VALUES ('First Name' , 'STR' ,1, ''); "
		"INSERT INTO KYCS (name , DataType , mandatory ,  Options) VALUES ('Last Name' , 'STR' ,1,  ''); "
		"INSERT INTO KYCS (name , DataType , mandatory ,Options) VALUES ('Middle Name' , 'STR' ,0,  ''); "
		"INSERT INTO KYCS (name , DataType , mandatory ,Options) VALUES ('Date Of Birth' , 'OPT' ,1,  ''); "
		"INSERT INTO KYCS (name , DataType , mandatory ,Options) VALUES ('Document Type' , 'OPT' ,1,  'National ID,Passport,Alien ID,NGO Card'); "
		"INSERT INTO KYCS (name , DataType , mandatory ,Options) VALUES ('Document Number' , 'STR' ,1,  ''); "
		"INSERT INTO KYCS (name , DataType , mandatory ,Options) VALUES ('Address Information' , 'STR' ,0,  ''); ";

char  * read_kycs_sql =  "select  * from KYCs";
/*
 * Creates all tables
 *
 */

void create_all_tables(void)
{
	char  * data;
	int  i = 0;
	sqlite_database_create_table_function(create_kyc_table,"database.db");
	sqlite_database_read_write_operation(insert_default_kyc , "database.db" , &data);
	//printf
	sqlite_database_read_write_operation(read_kycs_sql , "database.db" , &data);
	printf("Data count = %d\n Collumn Count = %d\n Row Count = %d\n" , sql_data_count,sql_collumn_count, sql_row_count );
	for(i =0 ; i<sql_data_count; i++)
	printf("%d  : %s\n",  i, sql_data[i]);

}

void read_kycs()
{

}
