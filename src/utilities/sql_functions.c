/*
 * sql_functions.c
 *
 *  Created on: Dec 5, 2018
 *      Author: linux
 */



#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

extern int usleep (__useconds_t __useconds);
int create_backup_db();


static int callback(void *data, int argc, char **argv, char **azColName) {
	int i;
	//fprintf(stderr, "%s: ", (const char*) data);
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}


void  sqlite_database_create_table1(char *sql ,char *Db) {
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

int sqlite_database_read_write_operation(char *sql ,char *Db)  {
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	const char* data = "";
	/* Open database */

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


