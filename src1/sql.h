/*
 * sql.h
 *
 *  Created on: Mar 3, 2017
 *      Author: pos
 */

#ifndef SRC1_SQL_H_
#define SRC1_SQL_H_
int row_count;
char ** collectStr;
static int callbackop(void *NotUsed, int argc, char **argv, char **azColName);
int sqlite_insert_operator_into_table(char * sql, char *Db);


#endif /* SRC1_SQL_H_ */
