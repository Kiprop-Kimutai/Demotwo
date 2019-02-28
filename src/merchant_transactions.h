/*
 * merchant_transactions.h
 *
 *  Created on: Dec 5, 2018
 *      Author: linux
 */

#ifndef SRC_MERCHANT_TRANSACTIONS_H_
#define SRC_MERCHANT_TRANSACTIONS_H_
/*
 * Main function that  manages the merchant  transactions
 */
int merchant_inquiry(void);
int withdrawal_to_bank(void);
//cJSON * data_to_print;
int store_to_store(void);
int roll_up_transfer(void);
int balance_inquiry(void);
int mini_statement(void);
void merchant_transactions(void );
char * errmessage;
char  * date1;

#endif /* SRC_MERCHANT_TRANSACTIONS_H_ */
