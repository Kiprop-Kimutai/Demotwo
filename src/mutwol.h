/*
 * mutwol.h
 *
 *  Created on: Dec 9, 2018
 *      Author: pos
 */

#ifndef SRC1_MUTWOL_H_
#define SRC1_MUTWOL_H_
#include "../src/utilities/sgfplib.h"
int personalizecard(char *personaldetails,unsigned char *fp ,  char  * balances , char * card_number);
int cardoperations(int operation,char *data ,char ** personal_details ,char ** transaction_file, BYTE * fingerprint[], char  *  input_personal_details , char  * input_transaction_data , char * card_number ,  char ** card_uid_to_be_obtained);
char * readcarduid(void);
char *processcarduid(char *uid);
int testnfc(void);
char  gbl_str[2000];
#endif /* SRC1_MUTWOL_H_ */
