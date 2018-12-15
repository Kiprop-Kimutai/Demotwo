/*
 * database.h
 *
 *  Created on: Dec 7, 2018
 *      Author: linux
 */

#ifndef SRC_DATABASE_H_
#define SRC_DATABASE_H_

char  * read_kycs_sql ;

void final_create_all_tables(void);
char  **  get_collumn_data(char  ** data , int data_c ,  int column_c , int col_number);

#endif /* SRC_DATABASE_H_ */
