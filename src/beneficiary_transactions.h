/*
 * beneficiary_transactions.h
 *
 *  Created on: Dec 5, 2018
 *      Author: linux
 */

#ifndef SRC_BENEFICIARY_TRANSACTIONS_H_
#define SRC_BENEFICIARY_TRANSACTIONS_H_

/*
 * Function used to  manage all beneficiary  related transactions
 */
void beneficiary_transactions(void );
int  post_transaction_file(char  * transaction_source, char * transaction  , cJSON ** response_balance  , int display_messages  ,    int *  transaction_count);
int post_pos_offline_transactions (void);

#endif /* SRC_BENEFICIARY_TRANSACTIONS_H_ */
