/*
 * global_parameters.c
 *
 *  Created on: Feb 16, 2019
 *      Author: linux
 */
#include "global_params.h"
#include <stdlib.h>
#include <string.h>

void initialize_params(void) {


	char *  linkcard = "/hibah/external/api/v1/POS/customer/register";
	char *  benbalrequest = "/hibah/external/api/v1/POS/customer/balance";
	char *  beneficiarystatementrequest = "/hibah/external/api/v1/POS/customer/statement";
	char *  beneficiarytransaction = "/hibah/external/api/v1/POS/customer/transaction";
	char *  getstorebalance = "/hibah/external/api/v1/POS/organization/balance";
	char *  storestatementrequest = "/hibah/external/api/v1/POS/organization/statement";
	char *  storetransaction = "/hibah/external/api/v1/POS/organization/transaction";


	my_endpoint  =  malloc(sizeof(endpoints));
	my_endpoint->beneficairy_balance_request = malloc(sizeof(my_endpoint->beneficairy_balance_request)*strlen(benbalrequest));
	my_endpoint->beneficairy_statement_request = malloc(sizeof(my_endpoint->beneficairy_statement_request)*strlen(beneficiarystatementrequest));
	my_endpoint->beneficary_transaction = malloc(sizeof(my_endpoint->beneficary_transaction)*strlen(beneficiarytransaction));
	my_endpoint->beneficiary_link_card = malloc(sizeof(my_endpoint->beneficiary_link_card)*strlen(linkcard));
	my_endpoint->store_get_balance = malloc(sizeof(my_endpoint->store_get_balance)*strlen(getstorebalance));
	my_endpoint->store_transaction = malloc(sizeof(my_endpoint->store_transaction)*strlen(storetransaction));
	my_endpoint->store_statement_request = malloc(sizeof(my_endpoint->store_statement_request)*strlen(storestatementrequest));

	strcpy(my_endpoint->beneficairy_balance_request ,benbalrequest );
	strcpy(my_endpoint->beneficairy_statement_request , beneficiarystatementrequest);
	strcpy(my_endpoint->beneficary_transaction ,beneficiarytransaction );
	strcpy(my_endpoint->beneficiary_link_card , linkcard);
	strcpy(my_endpoint->store_get_balance , getstorebalance);
	strcpy(my_endpoint->store_transaction , storetransaction );
	strcpy(my_endpoint->store_statement_request , storestatementrequest);


	customer_operations  =  malloc(sizeof(customer_trans_operations));
	customer_operations->BUY_GOODS_WFP_ACCOUNT = malloc(sizeof(customer_operations->BUY_GOODS_WFP_ACCOUNT)*3);
	strcpy(customer_operations->BUY_GOODS_WFP_ACCOUNT , "300");

	customer_operations->ORGANIZATION_ACCOUNT_BUY_GOODS = malloc(sizeof(customer_operations->ORGANIZATION_ACCOUNT_BUY_GOODS)*3);
	strcpy(customer_operations->ORGANIZATION_ACCOUNT_BUY_GOODS , "100");

	customer_operations->ORGANIZATION_ACCOUNT_CASH_OUT = malloc(sizeof(customer_operations->ORGANIZATION_ACCOUNT_CASH_OUT)*3);
	strcpy(customer_operations->ORGANIZATION_ACCOUNT_CASH_OUT , "200");


	merchant_operations  =  malloc(sizeof(merchant_trans_operations));
	merchant_operations->ORGANIZATION_ROLL_UP = malloc(sizeof(merchant_operations->ORGANIZATION_ROLL_UP)*3);
	strcpy(merchant_operations->ORGANIZATION_ROLL_UP , "100");

	merchant_operations->WFP_ROLL_UP = malloc(sizeof(merchant_operations->WFP_ROLL_UP)*3);
	strcpy(merchant_operations->WFP_ROLL_UP , "200");

	char *  PIN = "100";
		char * fingerprint  = "101";
		my_authmodes  =  malloc(sizeof(authmodes));
		my_authmodes->PIN = malloc(sizeof(my_authmodes->PIN)*2);
		strcpy(my_authmodes->PIN  ,PIN);

		my_authmodes->fingerprint= malloc(sizeof(my_authmodes->fingerprint)*2);
		strcpy(my_authmodes->fingerprint ,fingerprint);



	return;
}
