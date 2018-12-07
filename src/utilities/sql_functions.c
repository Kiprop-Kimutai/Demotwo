/*
 * sql_functions.c
 *
 *  Created on: Dec 5, 2018
 *      Author: linux
 */




#include "sql_functions.h"

extern int usleep (__useconds_t __useconds);
int create_backup_db();


static int callback(void *data, int argc, char **argv, char **azColName) {
	int i;
	char str[100];
	//fprintf(stderr, "------------------------------------------\n", (const char*) data);
	//printf("------------------------------------------\n"  );
	if(sql_row_count == 0)
	{
		//headers();
		for (i = 0; i < argc; i++) {
			//printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
			sql_data = realloc(sql_data,(sql_data_count + 1) * sizeof(char *));
			sql_data[sql_data_count] = malloc(strlen(azColName[i]) + 1);
			strcpy(sql_data[sql_data_count], azColName[i]);
			sql_data_count++;
			sql_collumn_count++;
		}
	}

	for (i = 0; i < argc; i++) {
		strcpy(str, argv[i] ? argv[i] : "NULL");
		//printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		sql_data = realloc(sql_data,(sql_data_count + 1) * sizeof(char *));
		sql_data[sql_data_count] = malloc(strlen(str) + 1);
		strcpy(sql_data[sql_data_count], str);
		sql_data_count++;
	}
	sql_row_count++;
	printf("\n");
	return 0;
}


void  sqlite_database_create_table_function(char *sql ,char *Db) {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;

	//const char* data = "Callback function called";

	/* Open database */
		rc = sqlite3_open(Db, &db);

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

int sqlite_database_read_write_operation(char *sql ,char *Db ,  char **ret_data )  {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	const char* data = "";
	/* Open database */
	if(sql_data)
	{
		free(sql_data );
		sql_data= 0 ;
	}

	sql_row_count = 0 ;
	sql_collumn_count = 0 ;
	sql_data_count= 0;

	rc = sqlite3_open(Db, &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_free(zErrMsg);
		return 0;
	}

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback, (void*) data, &zErrMsg);



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


