/*
 * global_params.h
 *
 *  Created on: Feb 16, 2019
 *      Author: linux
 */

#ifndef SRC_GLOBAL_PARAMS_H_
#define SRC_GLOBAL_PARAMS_H_

typedef struct  endpoints endpoints;

struct  endpoints
{
	char * beneficiary_link_card;
	char * beneficary_transaction;
	char * store_transaction;
	char * beneficairy_balance_request;
	char * store_get_balance;
	char * beneficairy_statement_request;
	char * store_statement_request;

};

endpoints * my_endpoint;

typedef struct  transAuthModes{
	char * PIN;
	char * fingerprint;
}authmodes;

authmodes * my_authmodes;

typedef struct merchantTransOperations {

	char * WFP_ROLL_UP ;
	char * ORGANIZATION_ROLL_UP;

} merchant_trans_operations;

merchant_trans_operations *merchant_operations;

typedef struct cutomerTransOperations {

	char *ORGANIZATION_ACCOUNT_BUY_GOODS;
	char *ORGANIZATION_ACCOUNT_CASH_OUT;
	char *BUY_GOODS_WFP_ACCOUNT;

} customer_trans_operations;

customer_trans_operations *customer_operations;




void initialize_params(void) ;
#endif /* SRC_GLOBAL_PARAMS_H_ */
