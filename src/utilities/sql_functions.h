/*
 * sql_functions.h
 *
 *  Created on: Dec 6, 2018
 *      Author: linux
 */

#ifndef SRC_UTILITIES_SQL_FUNCTIONS_H_
#define SRC_UTILITIES_SQL_FUNCTIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

char  **  sql_data;
char  ** sql_data_header;

int sql_row_count , sql_collumn_count , sql_data_count;

int increament_read;

int read_database(char * sql,char * Db);

void  sqlite_database_create_table_function(char *sql ,char *Db);
int sqlite_database_read_write_operation(char *sql ,char *Db );


#endif /* SRC_UTILITIES_SQL_FUNCTIONS_H_ */
